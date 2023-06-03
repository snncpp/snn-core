// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "snn-core/exception.hh"
#include "snn-core/array_view.hh"
#include "snn-core/generic/error.hh"
#include "snn-core/math/common.hh"
#include "snn-core/mem/trivial_allocator.hh"
#include "snn-core/mem/raw/copy.hh"
#include "snn-core/mem/raw/fill.hh"
#include "snn-core/mem/raw/is_overlapping.hh"
#include "snn-core/mem/raw/move.hh"
#include "snn-core/mem/raw/optimal_size.hh"
#include <new> // nothrow

namespace snn::detail::strcore
{
    SNN_DIAGNOSTIC_PUSH
    SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

    inline constexpr char digit_lookup[201] // 200 digits + '\0'.
        = "000102030405060708091011121314151617181920212223242526272829303132333435363738394041"
          "424344454647484950515253545556575859606162636465666768697071727374757677787980818283"
          "84858687888990919293949596979899";

#if SNN_SHOULD_BOOL
    // 1-byte buffer for empty strings, must not be modified.
    // Not null-terminated.
    // Can help AddressSanitizer catch buffer-overflows.
    class canary final
    {
      public:
        // Non-copyable
        canary(const canary&)            = delete;
        canary& operator=(const canary&) = delete;

        // Non-movable
        canary(canary&&)            = delete;
        canary& operator=(canary&&) = delete;

        ~canary()
        {
            snn_should(buf_[0] == '\xff');
            delete buf_;
        }

        static not_null<char*> writable() noexcept
        {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
            static canary c;
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
            return not_null{c.buf_};
        }

      private:
        char* buf_;

        canary() noexcept
        {
            buf_ = new (std::nothrow) char('\xff');
            snn_should(buf_ != nullptr);
        }
    };
#endif

    class shared
    {
      protected:
        // Maximum size must be less than `1ul << 63` or 8 EiB (SSO uses most significant bit).
        static constexpr usize soft_max_size_ = 0x1000000000000000; // 1 EiB.

        static constexpr usize check_capacity_(const usize capacity)
        {
            if (capacity < soft_max_size_)
            {
                return capacity;
            }
            throw_or_abort(generic::error::size_would_exceed_max_size);
        }

        static constexpr usize recommend_capacity_(const usize current_size) noexcept
        {
            return current_size + (current_size / 2); // 1.5x
        }

        static constexpr void shift_left_(const not_null<char*> buf, const usize buf_size,
                                          const not_zero<usize> pos,
                                          const not_zero<usize> count) noexcept
        {
            snn_should(pos.get() < buf_size && count.get() <= pos.get());

            char* const from = buf.get() + pos.get();
            char* const to   = from - count.get();
            mem::raw::move(not_null<const char*>{from}, not_null{to},
                           byte_size{buf_size - pos.get()});
        }

        static constexpr void shift_right_(const not_null<char*> buf, const usize buf_size,
                                           const usize buf_capacity, const usize pos,
                                           const not_zero<usize> count) noexcept
        {
            snn_should(pos < buf_size && buf_size < buf_capacity);
            snn_should((buf_capacity - buf_size) >= count.get());
            ignore_if_unused(buf_capacity);

            char* const from = buf.get() + pos;
            char* const to   = from + count.get();
            mem::raw::move(not_null<const char*>{from}, not_null{to}, byte_size{buf_size - pos});
        }
    };

    class buf final : public shared
    {
      public:
        using trivially_relocatable_type = buf;

        constexpr buf() noexcept
        {
            init_();
        }

        constexpr buf(const usize capacity)
        {
            if (capacity)
            {
                init_(0, not_zero{capacity});
            }
            else
            {
                init_();
            }
        }

        constexpr buf(const not_null<const char*> data, const usize size)
        {
            if (size)
            {
                init_(size, not_zero{size});
                mem::raw::copy(data, not_null{buf_}, byte_size{size}, promise::no_overlap);
            }
            else
            {
                init_();
            }
        }

        template <character Char, usize Count>
        constexpr buf(const snn::array_view<Char, Count> s)
        {
            if constexpr (Count > 0)
            {
                init_(Count, not_zero{Count});
                mem::raw::copy<Count>(not_null{s.data()}, not_null{buf_}, promise::no_overlap);
            }
            else
            {
                init_();
            }
        }

        template <character Char>
        constexpr buf(const snn::array_view<Char> s)
        {
            if (s)
            {
                init_(s.size(), not_zero{s.size()});
                mem::raw::copy(s.data(), not_null{buf_}, s.byte_size(), promise::no_overlap);
            }
            else
            {
                init_();
            }
        }

        constexpr buf(const usize count, const char c)
        {
            if (count)
            {
                init_(count, not_zero{count});
                mem::raw::fill(not_null{buf_}, byte_size{count}, to_byte(c));
            }
            else
            {
                init_();
            }
        }

        constexpr buf(const char* const first, const char* const last)
        {
            snn_should(first == last || (first != nullptr && last != nullptr && first < last));
            if (first != last)
            {
                const auto size = static_cast<usize>(last - first);
                init_(size, not_zero{size});
                mem::raw::copy(not_null{first}, not_null{buf_}, byte_size{size},
                               promise::no_overlap);
            }
            else
            {
                init_();
            }
        }

        // Non-copyable
        buf(const buf&)            = delete;
        buf& operator=(const buf&) = delete;

        // Move constructor.
        constexpr buf(buf&& other) noexcept
            : buf_{other.buf_},
              size_{other.size_},
              cap_{other.cap_}
        {
            other.init_();
        }

        // Move assignment operator.
        buf& operator=(buf&&) = delete;

        constexpr ~buf()
        {
            if (cap_)
            {
                mem::trivial_allocator<char> alloc;
                alloc.deallocate(buf_, cap_);
            }
        }

        constexpr void append(const not_null<const char*> data, const usize size)
        {
            // Data can point to within current buffer.

            const usize remaining_cap = cap_ - size_;
            if (size <= remaining_cap) [[likely]]
            {
                mem::raw::copy(data, not_null{buf_ + size_}, byte_size{size}, promise::no_overlap);
                size_ += size;
            }
            else
            {
                grow_append_(data, not_zero{size});
            }
        }

        constexpr strview append_uninitialized(const usize size_increase)
        {
            const usize org_size      = size_;
            const usize remaining_cap = cap_ - size_;
            if (size_increase <= remaining_cap) [[likely]]
            {
                size_ += size_increase;
            }
            else
            {
                const auto new_size = math::add_with_saturation(size_, size_increase);
                grow_(not_zero{new_size});
                size_ = new_size;
            }
            return strview{not_null{buf_ + org_size}, size_increase};
        }

        constexpr void assign(const not_null<const char*> data, const usize size)
        {
            // Data can point to within current buffer, but in that case size will be smaller or
            // equal to capacity.

            if (size <= cap_) [[likely]]
            {
                mem::raw::move(data, not_null{buf_}, byte_size{size});
            }
            else
            {
                grow_(not_zero{size});
                mem::raw::copy(data, not_null{buf_}, byte_size{size}, promise::no_overlap);
            }
            size_ = size;
        }

        constexpr usize capacity() const noexcept
        {
            return cap_;
        }

        static constexpr usize default_capacity() noexcept
        {
            return 0;
        }

        constexpr char* begin() noexcept
        {
            return buf_;
        }

        constexpr char* end() noexcept
        {
            return buf_ + size_;
        }

        constexpr const char* cbegin() const noexcept
        {
            return buf_;
        }

        constexpr const char* cend() const noexcept
        {
            return buf_ + size_;
        }

        constexpr void clear() noexcept
        {
            size_ = 0;
        }

        constexpr bool overlaps(const cstrview s) const noexcept
        {
            const char* first = buf_;
            const char* last  = first + cap_;
            return mem::raw::is_overlapping(s.cbegin(), s.cend(), first, last);
        }

        constexpr void drop_back_n(const usize size) noexcept
        {
            size_ = math::subtract_with_saturation(size_, size);
        }

        constexpr void drop_at(const usize pos, const usize size) noexcept
        {
            if (pos < size_)
            {
                if ((size_ - pos) <= size)
                {
                    size_ = pos;
                }
                else if (size > 0)
                {
                    shift_left_(not_null{buf_}, size_, not_zero{pos + size}, not_zero{size});
                    size_ -= size;
                }
            }
        }

        constexpr strview replace_uninitialized(const usize pos, const usize size,
                                                const usize replacement_size)
        {
            if (pos > size_)
            {
                throw_or_abort(generic::error::invalid_position_for_replace_or_insert);
            }

            if ((size_ - pos) > size)
            {
                if (replacement_size == size)
                {
                    // Do nothing.
                }
                else if (replacement_size < size)
                {
                    const usize diff = size - replacement_size;
                    shift_left_(not_null{buf_}, size_, not_zero{pos + size}, not_zero{diff});
                    size_ = size_ - diff;
                }
                else
                {
                    const usize diff    = replacement_size - size;
                    const auto new_size = math::add_with_saturation(size_, diff);
                    if (new_size > cap_)
                    {
                        grow_(not_zero{new_size});
                    }
                    shift_right_(not_null{buf_}, size_, cap_, pos + size, not_zero{diff});
                    size_ = new_size;
                }
            }
            else if ((cap_ - pos) >= replacement_size)
            {
                size_ = pos + replacement_size;
            }
            else
            {
                const auto new_size = math::add_with_saturation(pos, replacement_size);
                grow_(not_zero{new_size});
                size_ = new_size;
            }

            return strview{not_null{buf_ + pos}, replacement_size};
        }

        constexpr void reserve(const usize capacity)
        {
            if (capacity > cap_) [[unlikely]]
            {
                grow_(not_zero{capacity});
            }
        }

        constexpr void reserve_append(const usize size_increase)
        {
            const usize remaining_cap = cap_ - size_;
            if (size_increase > remaining_cap) [[unlikely]]
            {
                grow_(not_zero{math::add_with_saturation(size_, size_increase)});
            }
        }

        constexpr void reset() noexcept
        {
            if (cap_)
            {
                mem::trivial_allocator<char> alloc;
                alloc.deallocate(buf_, cap_);
            }
            init_();
        }

        constexpr void resize(const usize size, const char fill)
        {
            if (size <= size_)
            {
                size_ = size;
            }
            else
            {
                append_uninitialized(size - size_).fill(fill);
            }
        }

        constexpr strview resize_uninitialized(const usize size)
        {
            reserve(size);
            size_ = size;
            return strview{not_null{buf_}, size_};
        }

        constexpr void shrink_to_fit() noexcept
        {
            if (size_)
            {
                const auto cap = mem::raw::optimal_size(not_zero{size_});
                if (cap.get() < cap_)
                {
                    mem::trivial_allocator<char> alloc;
                    auto opt = alloc.reallocate(buf_, cap_, cap, size_);
                    if (opt)
                    {
                        buf_ = opt.value(promise::has_value);
                        cap_ = cap.get();
                    }
                }
            }
            else
            {
                reset();
            }
        }

        constexpr usize size() const noexcept
        {
            return size_;
        }

        constexpr void swap(buf& other) noexcept
        {
            // This works even if this == &other.
            std::swap(buf_, other.buf_);
            std::swap(size_, other.size_);
            std::swap(cap_, other.cap_);
        }

        constexpr void truncate(const usize size) noexcept
        {
            if (size < size_)
            {
                size_ = size;
            }
        }

        constexpr auto view() noexcept
        {
            return strview{not_null{buf_}, size_};
        }

        constexpr auto view() const noexcept
        {
            return cstrview{not_null<const char*>{buf_}, size_};
        }

      private:
        char* buf_;
        usize size_;
        usize cap_;

        constexpr void init_() noexcept
        {
            buf_ = strview{}.writable().get();
#if SNN_SHOULD_BOOL
            if (!std::is_constant_evaluated())
            {
                buf_ = canary::writable().get();
            }
#endif
            size_ = 0;
            cap_  = 0;
        }

        constexpr void init_(const usize size, const not_zero<usize> capacity)
        {
            snn_should(size <= capacity.get());
            const auto cap = mem::raw::optimal_size(not_zero{check_capacity_(capacity.get())});
            mem::trivial_allocator<char> alloc;
            buf_  = alloc.allocate(cap).value();
            size_ = size;
            cap_  = cap.get();
        }

        constexpr void grow_(const not_zero<usize> capacity)
        {
            snn_should(size_ <= cap_ && cap_ < capacity.get());
            const usize min_cap = math::max(capacity.get(), recommend_capacity_(size_));
            const auto cap      = mem::raw::optimal_size(not_zero{check_capacity_(min_cap)});
            mem::trivial_allocator<char> alloc;
            if (cap_)
            {
                buf_ = alloc.reallocate(buf_, cap_, cap, size_).value();
            }
            else
            {
                buf_ = alloc.allocate(cap).value();
            }
            cap_ = cap.get();
        }

        constexpr void grow_append_(const not_null<const char*> append_data,
                                    const not_zero<usize> append_size)
        {
            const usize new_size = math::add_with_saturation(size_, append_size.get());
            const usize min_cap  = math::max(new_size, recommend_capacity_(size_));
            const auto cap       = mem::raw::optimal_size(not_zero{check_capacity_(min_cap)});

            mem::trivial_allocator<char> alloc;

            // Always allocate a new buffer.
            char* const buffer = alloc.allocate(cap).value();

            // Copy contents from old buffer (if any).
            mem::raw::copy(not_null<const char*>{buf_}, not_null{buffer}, byte_size{size_},
                           promise::no_overlap);

            // Append new data (can point to old buffer).
            mem::raw::copy(append_data, not_null{buffer + size_}, byte_size{append_size.get()},
                           promise::no_overlap);

            // Free old buffer.
            if (cap_)
            {
                alloc.deallocate(buf_, cap_);
            }

            buf_  = buffer;
            size_ = new_size;
            cap_  = cap.get();
        }
    };

    class sso final : public shared
    {
      public:
        using trivially_relocatable_type = sso;

        constexpr sso() noexcept
        {
            init_();
        }

        constexpr sso(const usize capacity)
        {
            if (std::is_constant_evaluated())
            {
                init_large_(0, capacity);
            }
            else
            {
                if (capacity < small_capacity_incl_zero_)
                    init_small_(0);
                else
                    init_large_(0, capacity);
            }
        }

        constexpr sso(const not_null<const char*> data, const usize size)
        {
            mem::raw::copy(data, init_(size), byte_size{size}, promise::no_overlap);
        }

        template <character Char, usize Count>
        constexpr sso(const snn::array_view<Char, Count> s)
        {
            if constexpr (Count > 0)
            {
                mem::raw::copy<Count>(not_null{s.data()}, init_(Count), promise::no_overlap);
            }
            else
            {
                init_();
            }
        }

        template <character Char>
        constexpr sso(const snn::array_view<Char> s)
        {
            mem::raw::copy(s.data(), init_(s.size()), s.byte_size(), promise::no_overlap);
        }

        constexpr sso(const usize count, const char c)
        {
            mem::raw::fill(init_(count), byte_size{count}, to_byte(c));
        }

        constexpr sso(const char* const first, const char* const last)
        {
            snn_should(first == last || (first != nullptr && last != nullptr && first < last));
            if (first != last)
            {
                const auto size = static_cast<usize>(last - first);
                mem::raw::copy(not_null{first}, init_(size), byte_size{size}, promise::no_overlap);
            }
            else
            {
                init_();
            }
        }

        // Non-copyable
        sso(const sso&)            = delete;
        sso& operator=(const sso&) = delete;

        // Move constructor.
        constexpr sso(sso&& other) noexcept
            : storage_{other.storage_}
        {
            other.init_();
        }

        // Move assignment operator.
        sso& operator=(sso&&) = delete;

        constexpr ~sso()
        {
            if (std::is_constant_evaluated() || !is_small_())
            {
                mem::trivial_allocator<char> alloc;
                alloc.deallocate(storage_.large.data, storage_.large.capacity_incl_zero());
            }
        }

        constexpr void append(const not_null<const char*> data, const usize size)
        {
            // Data can point to within current buffer.

            if (is_small_())
            {
                const usize cur_size          = small_size_();
                const usize rem_cap_incl_zero = small_capacity_incl_zero_ - cur_size;
                if (size < rem_cap_incl_zero) [[likely]]
                {
                    mem::raw::copy(data, not_null{storage_.small + cur_size}, byte_size{size},
                                   promise::no_overlap);
                    set_small_size_and_term_(cur_size + size);
                }
                else
                {
                    grow_small_append_(data, not_zero{size});
                }
            }
            else
            {
                const usize cur_size          = storage_.large.size;
                const usize rem_cap_incl_zero = storage_.large.capacity_incl_zero() - cur_size;
                if (size < rem_cap_incl_zero) [[likely]]
                {
                    mem::raw::copy(data, not_null{storage_.large.data + cur_size}, byte_size{size},
                                   promise::no_overlap);
                    set_large_size_and_term_(cur_size + size);
                }
                else
                {
                    grow_large_append_(data, not_zero{size});
                }
            }
        }

        constexpr strview append_uninitialized(const usize size_increase)
        {
            if (is_small_())
                return append_uninitialized_small_(size_increase);
            else
                return append_uninitialized_large_(size_increase);
        }

        constexpr void assign(const not_null<const char*> data, const usize size)
        {
            // Data can point to within current buffer, but in that case size will be smaller or
            // equal to usable capacity.

            if (is_small_())
            {
                if (size < small_capacity_incl_zero_) [[likely]]
                {
                    mem::raw::move(data, not_null{storage_.small}, byte_size{size});
                    set_small_size_and_term_(size);
                }
                else
                {
                    grow_small_(not_zero{size});
                    mem::raw::copy(data, not_null{storage_.large.data}, byte_size{size},
                                   promise::no_overlap);
                    set_large_size_and_term_(size);
                }
            }
            else
            {
                if (size < storage_.large.capacity_incl_zero()) [[likely]]
                {
                    mem::raw::move(data, not_null{storage_.large.data}, byte_size{size});
                    set_large_size_and_term_(size);
                }
                else
                {
                    grow_large_(not_zero{size});
                    mem::raw::copy(data, not_null{storage_.large.data}, byte_size{size},
                                   promise::no_overlap);
                    set_large_size_and_term_(size);
                }
            }
        }

        constexpr usize capacity() const noexcept
        {
            // Return usable capacity (not including terminating zero).
            if (is_small_())
                return small_max_size_;
            else
                return storage_.large.capacity_incl_zero() - 1;
        }

        static constexpr usize default_capacity() noexcept
        {
            return small_max_size_;
        }

        constexpr char* begin() noexcept
        {
            if (is_small_())
                return storage_.small;
            else
                return storage_.large.data;
        }

        constexpr char* end() noexcept
        {
            if (is_small_())
                return storage_.small + small_size_();
            else
                return storage_.large.data + storage_.large.size;
        }

        constexpr const char* cbegin() const noexcept
        {
            if (is_small_())
                return storage_.small;
            else
                return storage_.large.data;
        }

        constexpr const char* cend() const noexcept
        {
            if (is_small_())
                return storage_.small + small_size_();
            else
                return storage_.large.data + storage_.large.size;
        }

        constexpr void clear() noexcept
        {
            if (is_small_())
                set_small_size_and_term_(0);
            else
                set_large_size_and_term_(0);
        }

        constexpr bool overlaps(const cstrview s) const noexcept
        {
            if (std::is_constant_evaluated())
            {
                const char* first = storage_.large.data;
                const char* last  = first + storage_.large.capacity_incl_zero();
                return mem::raw::is_overlapping(s.cbegin(), s.cend(), first, last);
            }
            else
            {
                // This is typically branchless (using conditional moves).
                const char* first = reinterpret_cast<const char*>(&storage_);
                const char* last  = first + small_capacity_incl_zero_;
                if (!is_small_())
                {
                    first = storage_.large.data;
                    last  = first + storage_.large.capacity_incl_zero();
                }
                return mem::raw::is_overlapping(s.cbegin(), s.cend(), first, last);
            }
        }

        constexpr void drop_back_n(const usize size) noexcept
        {
            if (is_small_())
                set_small_size_and_term_(math::subtract_with_saturation(small_size_(), size));
            else
                set_large_size_and_term_(math::subtract_with_saturation(storage_.large.size, size));
        }

        constexpr void drop_at(const usize pos, const usize size) noexcept
        {
            if (is_small_())
            {
                const usize cur_size = small_size_();
                if (pos < cur_size)
                {
                    if ((cur_size - pos) <= size)
                    {
                        set_small_size_and_term_(pos);
                    }
                    else if (size > 0)
                    {
                        shift_left_(not_null{storage_.small}, cur_size, not_zero{pos + size},
                                    not_zero{size});
                        set_small_size_and_term_(cur_size - size);
                    }
                }
            }
            else
            {
                const usize cur_size = storage_.large.size;
                if (pos < cur_size)
                {
                    if ((cur_size - pos) <= size)
                    {
                        set_large_size_and_term_(pos);
                    }
                    else if (size > 0)
                    {
                        shift_left_(not_null{storage_.large.data}, cur_size, not_zero{pos + size},
                                    not_zero{size});
                        set_large_size_and_term_(cur_size - size);
                    }
                }
            }
        }

        constexpr strview replace_uninitialized(const usize pos, const usize size,
                                                const usize replacement_size)
        {
            if (is_small_())
                return replace_uninit_small_(pos, size, replacement_size);
            else
                return replace_uninit_large_(pos, size, replacement_size);
        }

        constexpr void reserve(const usize capacity)
        {
            if (is_small_())
            {
                if (capacity >= small_capacity_incl_zero_) [[unlikely]]
                {
                    grow_small_(not_zero{capacity});
                }
            }
            else
            {
                const usize cur_cap_incl_zero = storage_.large.capacity_incl_zero();
                if (capacity >= cur_cap_incl_zero) [[unlikely]]
                {
                    grow_large_(not_zero{capacity});
                }
            }
        }

        constexpr void reserve_append(const usize size_increase)
        {
            if (is_small_())
            {
                const usize cur_size          = small_size_();
                const usize rem_cap_incl_zero = small_capacity_incl_zero_ - cur_size;
                if (size_increase >= rem_cap_incl_zero) [[unlikely]]
                {
                    grow_small_(not_zero{math::add_with_saturation(cur_size, size_increase)});
                }
            }
            else
            {
                const usize cur_size          = storage_.large.size;
                const usize cur_cap_incl_zero = storage_.large.capacity_incl_zero();
                const usize rem_cap_incl_zero = cur_cap_incl_zero - cur_size;
                if (size_increase >= rem_cap_incl_zero) [[unlikely]]
                {
                    grow_large_(not_zero{math::add_with_saturation(cur_size, size_increase)});
                }
            }
        }

        constexpr void reset() noexcept
        {
            if (is_small_())
            {
                set_small_size_and_term_(0);
            }
            else
            {
                mem::trivial_allocator<char> alloc;
                alloc.deallocate(storage_.large.data, storage_.large.capacity_incl_zero());

                init_();
            }
        }

        constexpr void resize(const usize size, const char fill)
        {
            if (is_small_())
            {
                const usize cur_size = small_size_();
                if (size <= cur_size)
                {
                    set_small_size_and_term_(size);
                }
                else
                {
                    append_uninitialized_small_(size - cur_size).fill(fill);
                }
            }
            else
            {
                const usize cur_size = storage_.large.size;
                if (size <= cur_size)
                {
                    set_large_size_and_term_(size);
                }
                else
                {
                    append_uninitialized_large_(size - cur_size).fill(fill);
                }
            }
        }

        constexpr strview resize_uninitialized(const usize size)
        {
            if (is_small_())
            {
                if (size < small_capacity_incl_zero_) [[likely]]
                {
                    set_small_size_and_term_(size);
                    return strview{not_null{storage_.small}, size};
                }

                grow_small_(not_zero{size});
            }
            else
            {
                const usize cur_cap_incl_zero = storage_.large.capacity_incl_zero();
                if (size >= cur_cap_incl_zero) [[unlikely]]
                {
                    grow_large_(not_zero{size});
                }
            }

            set_large_size_and_term_(size);
            return strview{not_null{storage_.large.data}, size};
        }

        constexpr void shrink_to_fit() noexcept
        {
            if (!is_small_())
            {
                shrink_large_();
            }
        }

        constexpr usize size() const noexcept
        {
            if (is_small_())
                return small_size_();
            else
                return storage_.large.size;
        }

        constexpr void swap(sso& other) noexcept
        {
            // This works even if this == &other.
            std::swap(storage_, other.storage_);
        }

        constexpr void truncate(const usize size) noexcept
        {
            if (is_small_())
            {
                if (size < small_size_())
                {
                    set_small_size_and_term_(size);
                }
            }
            else
            {
                if (size < storage_.large.size)
                {
                    set_large_size_and_term_(size);
                }
            }
        }

        constexpr strview view() noexcept
        {
            if (is_small_())
                return strview{not_null<char*>{storage_.small}, small_size_()};
            else
                return strview{not_null<char*>{storage_.large.data}, storage_.large.size};
        }

        constexpr cstrview view() const noexcept
        {
            if (is_small_())
                return cstrview{not_null<const char*>{storage_.small}, small_size_()};
            else
                return cstrview{not_null<const char*>{storage_.large.data}, storage_.large.size};
        }

      private:
        static constexpr usize capacity_mask_ = 0x8000000000000000;

        struct dynamic_memory final
        {
            char* data;
            usize size;
            usize capacity_raw;

            constexpr usize capacity_incl_zero() const noexcept
            {
                snn_should((capacity_raw & ~capacity_mask_) > 0);
                return capacity_raw & ~capacity_mask_;
            }

            constexpr void set_capacity_incl_zero(const usize capacity) noexcept
            {
                snn_should(capacity > 0);
                capacity_raw = capacity | capacity_mask_;
            }
        };

        static constexpr usize small_capacity_incl_zero_ = sizeof(dynamic_memory);
        static constexpr usize small_max_size_           = small_capacity_incl_zero_ - 1;
        static constexpr usize small_size_index_         = small_max_size_;

        static_assert(small_capacity_incl_zero_ == 24); // Incl. terminating zero.
        static_assert(small_max_size_ == 23 && small_size_index_ == 23);

        union
        {
            char small[sizeof(dynamic_memory)];
            dynamic_memory large;
        } storage_;

        // Memory layout inspired by Andrei Alexandrescu small string optimization:
        // https://www.facebook.com/Engineering/videos/10151029396848109

        // If the most significant bit in the last byte of storage is set, the string is large.
        // The last 8 bytes of `storage_.small` or all 8 bytes of `storage_.large.capacity_raw`:
        // ........ ........ ........ ........ ........ ........ ........ 1.......
        //                                                                ^
        //                                                                | Large bit.
        //
        // The small size is stored in the remaining bits of the last byte, it is stored as
        // (MAX - size), so when the size is MAX the last byte is 0 and acts as a terminator.
        // The last 8 bytes of `storage_.small` or all 8 bytes of `storage_.large.capacity_raw`:
        // ........ ........ ........ ........ ........ ........ ........ 0xxxxxxx
        //                                                                 ^
        //                                                                 |
        //                                                                 Small size bits.
        //                                                                ^
        //                                                                | Small bit.
        //
        // `capacity_mask_`:
        // 0x8000000000000000
        // `capacity_mask_` in little-endian:
        // 00000000 00000000 00000000 00000000 00000000 00000000 00000000 10000000

        constexpr not_null<char*> init_() noexcept
        {
            if (std::is_constant_evaluated())
                return init_large_(0, 0);
            else
                return init_small_(0);
        }

        constexpr not_null<char*> init_(const usize size)
        {
            if (std::is_constant_evaluated())
            {
                return init_large_(size, size);
            }
            else
            {
                if (size < small_capacity_incl_zero_)
                    return init_small_(size);
                else
                    return init_large_(size, size);
            }
        }

        constexpr not_null<char*> init_large_(const usize size, const usize capacity)
        {
            snn_should(size <= capacity);

            // Plus one for terminating zero.
            auto cap_incl_zero = mem::raw::optimal_size(not_zero{check_capacity_(capacity) + 1});

            if (std::is_constant_evaluated())
            {
                // Simulate small capacity if possible.
                if (capacity < small_capacity_incl_zero_)
                {
                    cap_incl_zero = not_zero{small_capacity_incl_zero_};
                }
            }

            mem::trivial_allocator<char> alloc;
            return set_large_(alloc.allocate(cap_incl_zero).value(), size, cap_incl_zero);
        }

        not_null<char*> init_small_(const usize size) noexcept
        {
            set_small_size_and_term_(size);
            return not_null{storage_.small};
        }

        constexpr bool is_small_() const noexcept
        {
            if (std::is_constant_evaluated())
                return false;
            else
                return (reinterpret_cast<const byte*>(&storage_)[small_size_index_] & 0x80) == 0;
        }

        usize small_size_() const noexcept
        {
            snn_should(is_small_());
            const auto b = to_byte(storage_.small[small_size_index_]);
            snn_should(b <= small_max_size_);
            return small_max_size_ - b;
        }

        constexpr not_null<char*> set_large_(char* const buffer, const usize size,
                                             const not_zero<usize> capacity_incl_zero) noexcept
        {
            snn_should(buffer != nullptr);
            snn_should(size < capacity_incl_zero.get());
            buffer[size]        = 0; // Terminating zero character.
            storage_.large.data = buffer;
            storage_.large.size = size;
            storage_.large.set_capacity_incl_zero(capacity_incl_zero.get());
            return not_null{buffer};
        }

        constexpr void set_large_(char* const buffer,
                                  const not_zero<usize> capacity_incl_zero) noexcept
        {
            snn_should(buffer != nullptr);
            snn_should(storage_.large.size < capacity_incl_zero.get());
            snn_should(buffer[storage_.large.size] == 0); // Terminating zero character.
            storage_.large.data = buffer;
            storage_.large.set_capacity_incl_zero(capacity_incl_zero.get());
        }

        constexpr void set_large_size_and_term_(const usize size) noexcept
        {
            snn_should(!is_small_());
            snn_should(size < storage_.large.capacity_incl_zero());
            storage_.large.data[size] = 0; // Terminating zero character.
            storage_.large.size       = size;
        }

        void set_small_size_and_term_(const usize size) noexcept
        {
            // Storage might not be initialized here.
            snn_should(size <= small_max_size_);
            storage_.small[size]              = 0; // Terminating zero character.
            storage_.small[small_size_index_] = static_cast<char>(small_max_size_ - size);
            // Storage initialized.
            snn_should(is_small_());
        }

        constexpr void grow_large_(const not_zero<usize> capacity)
        {
            snn_should(!is_small_());
            snn_should(capacity.get() >= storage_.large.capacity_incl_zero());

            const usize cur_size = storage_.large.size;
            const usize min_cap  = math::max(capacity.get(), recommend_capacity_(cur_size));

            // Plus one for terminating zero.
            const auto cap_incl_zero =
                mem::raw::optimal_size(not_zero{check_capacity_(min_cap) + 1});

            mem::trivial_allocator<char> alloc;
            auto buf = alloc.reallocate(storage_.large.data, storage_.large.capacity_incl_zero(),
                                        cap_incl_zero, cur_size + 1); // + 1 for zero.
            set_large_(buf.value(), cap_incl_zero);
        }

        constexpr void grow_large_append_(const not_null<const char*> append_data,
                                          const not_zero<usize> append_size)
        {
            const usize cur_size = storage_.large.size;
            const usize new_size = math::add_with_saturation(cur_size, append_size.get());
            const usize min_cap  = math::max(new_size, recommend_capacity_(cur_size));

            // Plus one for terminating zero.
            const auto cap_incl_zero =
                mem::raw::optimal_size(not_zero{check_capacity_(min_cap) + 1});

            mem::trivial_allocator<char> alloc;

            // Always allocate a new buffer.
            char* const buf = alloc.allocate(cap_incl_zero).value();

            // Copy contents from old buffer (if any).
            mem::raw::copy(not_null<const char*>{storage_.large.data}, not_null{buf},
                           byte_size{cur_size}, promise::no_overlap);

            // Append new data.
            mem::raw::copy(append_data, not_null{buf + cur_size}, byte_size{append_size.get()},
                           promise::no_overlap);

            // Free old buffer.
            alloc.deallocate(storage_.large.data, storage_.large.capacity_incl_zero());

            set_large_(buf, new_size, cap_incl_zero);
        }

        constexpr void shrink_large_() noexcept
        {
            char* const cur_data          = storage_.large.data;
            const usize cur_size          = storage_.large.size;
            const usize cur_cap_incl_zero = storage_.large.capacity_incl_zero();

            if (cur_size < small_capacity_incl_zero_)
            {
                if (std::is_constant_evaluated())
                {
                    snn_should(cur_cap_incl_zero >= small_capacity_incl_zero_);
                    if (cur_cap_incl_zero > small_capacity_incl_zero_)
                    {
                        constexpr not_zero<usize> new_cap_incl_zero{small_capacity_incl_zero_};
                        mem::trivial_allocator<char> alloc;
                        auto opt = alloc.reallocate(cur_data, cur_cap_incl_zero, new_cap_incl_zero,
                                                    cur_size + 1); // + 1 for zero.
                        set_large_(opt.value(), new_cap_incl_zero);
                    }
                }
                else
                {
                    mem::raw::copy(not_null<const char*>{cur_data}, not_null{storage_.small},
                                   byte_size{cur_size}, promise::no_overlap);
                    set_small_size_and_term_(cur_size);

                    mem::trivial_allocator<char> alloc;
                    alloc.deallocate(cur_data, cur_cap_incl_zero);
                }
            }
            else
            {
                // Plus one for terminating zero.
                const auto sug_cap_incl_zero = mem::raw::optimal_size(not_zero{cur_size + 1});
                if (sug_cap_incl_zero.get() < cur_cap_incl_zero)
                {
                    mem::trivial_allocator<char> alloc;
                    auto opt = alloc.reallocate(cur_data, cur_cap_incl_zero, sug_cap_incl_zero,
                                                cur_size + 1); // + 1 for zero.
                    if (opt)
                    {
                        set_large_(opt.value(promise::has_value), sug_cap_incl_zero);
                    }
                }
            }
        }

        void grow_small_(const not_zero<usize> capacity)
        {
            snn_should(capacity.get() >= small_capacity_incl_zero_);

            const usize cur_size = small_size_();
            const usize min_cap  = math::max(capacity.get(), recommend_capacity_(cur_size));

            // Plus one for terminating zero.
            const auto cap_incl_zero =
                mem::raw::optimal_size(not_zero{check_capacity_(min_cap) + 1});

            mem::trivial_allocator<char> alloc;
            char* const buf = alloc.allocate(cap_incl_zero).value();

            // Copy contents from local buffer (if any).
            mem::raw::copy(not_null<const char*>{storage_.small}, not_null{buf},
                           byte_size{cur_size}, promise::no_overlap);

            set_large_(buf, cur_size, cap_incl_zero);
        }

        void grow_small_append_(const not_null<const char*> append_data,
                                const not_zero<usize> append_size)
        {
            const usize cur_size = small_size_();
            const usize new_size = math::add_with_saturation(cur_size, append_size.get());
            const usize min_cap  = math::max(new_size, recommend_capacity_(cur_size));

            // Plus one for terminating zero.
            const auto cap_incl_zero =
                mem::raw::optimal_size(not_zero{check_capacity_(min_cap) + 1});

            mem::trivial_allocator<char> alloc;
            char* const buf = alloc.allocate(cap_incl_zero).value();

            // Copy contents from local buffer (if any).
            mem::raw::copy(not_null<const char*>{storage_.small}, not_null{buf},
                           byte_size{cur_size}, promise::no_overlap);

            // Append new data.
            mem::raw::copy(append_data, not_null{buf + cur_size}, byte_size{append_size.get()},
                           promise::no_overlap);

            set_large_(buf, new_size, cap_incl_zero);
        }

        strview append_uninitialized_small_(const usize size_increase)
        {
            const usize cur_size          = small_size_();
            const usize rem_cap_incl_zero = small_capacity_incl_zero_ - cur_size;
            if (size_increase < rem_cap_incl_zero) [[likely]]
            {
                set_small_size_and_term_(cur_size + size_increase);
                return strview{not_null{storage_.small + cur_size}, size_increase};
            }
            else
            {
                const usize new_size = math::add_with_saturation(cur_size, size_increase);
                grow_small_(not_zero{new_size});
                set_large_size_and_term_(new_size);
                return strview{not_null{storage_.large.data + cur_size}, size_increase};
            }
        }

        constexpr strview append_uninitialized_large_(const usize size_increase)
        {
            const usize cur_size          = storage_.large.size;
            const usize rem_cap_incl_zero = storage_.large.capacity_incl_zero() - cur_size;
            if (size_increase < rem_cap_incl_zero) [[likely]]
            {
                set_large_size_and_term_(cur_size + size_increase);
            }
            else
            {
                const usize new_size = math::add_with_saturation(cur_size, size_increase);
                grow_large_(not_zero{new_size});
                set_large_size_and_term_(new_size);
            }
            return strview{not_null{storage_.large.data + cur_size}, size_increase};
        }

        strview replace_uninit_small_(const usize pos, const usize size,
                                      const usize replacement_size)
        {
            const usize cur_size          = small_size_();
            const usize cur_cap_excl_zero = small_max_size_;

            if (pos > cur_size)
            {
                throw_or_abort(generic::error::invalid_position_for_replace_or_insert);
            }

            if ((cur_size - pos) > size)
            {
                if (replacement_size == size)
                {
                    // Do nothing.
                }
                else if (replacement_size < size)
                {
                    const usize diff = size - replacement_size;
                    shift_left_(not_null{storage_.small}, cur_size, not_zero{pos + size},
                                not_zero{diff});
                    set_small_size_and_term_(cur_size - diff);
                }
                else
                {
                    const usize diff     = replacement_size - size;
                    const usize new_size = math::add_with_saturation(cur_size, diff);
                    if (new_size <= cur_cap_excl_zero)
                    {
                        shift_right_(not_null{storage_.small}, cur_size, cur_cap_excl_zero,
                                     pos + size, not_zero{diff});
                        set_small_size_and_term_(new_size);
                    }
                    else
                    {
                        grow_small_(not_zero{new_size});
                        const usize cap_after_incl_zero = storage_.large.capacity_incl_zero();
                        shift_right_(not_null{storage_.large.data}, cur_size,
                                     cap_after_incl_zero - 1, pos + size, not_zero{diff});
                        set_large_size_and_term_(new_size);
                        return strview{not_null{storage_.large.data + pos}, replacement_size};
                    }
                }
            }
            else if ((cur_cap_excl_zero - pos) >= replacement_size)
            {
                set_small_size_and_term_(pos + replacement_size);
            }
            else
            {
                const usize new_size = math::add_with_saturation(pos, replacement_size);
                grow_small_(not_zero{new_size});
                set_large_size_and_term_(new_size);
                return strview{not_null{storage_.large.data + pos}, replacement_size};
            }

            return strview{not_null{storage_.small + pos}, replacement_size};
        }

        constexpr strview replace_uninit_large_(const usize pos, const usize size,
                                                const usize replacement_size)
        {
            const usize cur_size          = storage_.large.size;
            const usize cur_cap_excl_zero = storage_.large.capacity_incl_zero() - 1;

            if (pos > cur_size)
            {
                throw_or_abort(generic::error::invalid_position_for_replace_or_insert);
            }

            if ((cur_size - pos) > size)
            {
                if (replacement_size == size)
                {
                    // Do nothing.
                }
                else if (replacement_size < size)
                {
                    const usize diff = size - replacement_size;
                    shift_left_(not_null{storage_.large.data}, cur_size, not_zero{pos + size},
                                not_zero{diff});
                    set_large_size_and_term_(cur_size - diff);
                }
                else
                {
                    const usize diff     = replacement_size - size;
                    const usize new_size = math::add_with_saturation(cur_size, diff);
                    if (new_size <= cur_cap_excl_zero)
                    {
                        shift_right_(not_null{storage_.large.data}, cur_size, cur_cap_excl_zero,
                                     pos + size, not_zero{diff});
                    }
                    else
                    {
                        grow_large_(not_zero{new_size});
                        const usize cap_after_incl_zero = storage_.large.capacity_incl_zero();
                        shift_right_(not_null{storage_.large.data}, cur_size,
                                     cap_after_incl_zero - 1, pos + size, not_zero{diff});
                    }
                    set_large_size_and_term_(new_size);
                }
            }
            else if ((cur_cap_excl_zero - pos) >= replacement_size)
            {
                set_large_size_and_term_(pos + replacement_size);
            }
            else
            {
                const usize new_size = math::add_with_saturation(pos, replacement_size);
                grow_large_(not_zero{new_size});
                set_large_size_and_term_(new_size);
            }

            return strview{not_null{storage_.large.data + pos}, replacement_size};
        }
    };

    SNN_DIAGNOSTIC_POP

    // Type traits.
    template <typename T>
    struct is_null_terminated : public std::false_type
    {
    };

    // Type traits helpers.
    template <typename T>
    inline constexpr bool is_null_terminated_v = is_null_terminated<T>::value;

    template <>
    struct is_null_terminated<sso> : public std::true_type
    {
    };
}
