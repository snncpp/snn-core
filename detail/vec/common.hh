// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "snn-core/mem/allocator.hh"
#include "snn-core/mem/construct.hh"
#include "snn-core/mem/destruct_n.hh"
#include "snn-core/mem/relocate.hh"
#include "snn-core/mem/raw/optimal_size.hh"

namespace snn::detail::vec
{
    template <typename T>
    inline constexpr usize max_capacity = constant::limit<iptrdiff>::max / sizeof(T);

    SNN_DIAGNOSTIC_PUSH
    SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

    // vec buffer with small capacity, can currently not be `constexpr`.

    template <typename T, usize SmallCapacity>
    class buffer final
    {
      public:
        static_assert(SmallCapacity > 0);

        buffer() noexcept
            : data_{small_ptr_()},
              count_{0},
              capacity_{SmallCapacity}
        {
            snn_should(is_small_());
        }

        // Non-copyable
        buffer(const buffer&)            = delete;
        buffer& operator=(const buffer&) = delete;

        // Move constructor.
        buffer(buffer&& other) noexcept
            : data_{other.data_},
              count_{other.count_},
              capacity_{other.capacity_}
        {
            if (other.is_small_())
            {
                snn_should(capacity_ == SmallCapacity);
                snn_should(count_ <= SmallCapacity);

                data_ = small_ptr_();

                if (count_ > 0)
                {
                    mem::relocate(not_null{other.begin()}, not_null{other.end()},
                                  not_null{begin()});
                }
            }

            // Reset other.
            other.data_     = other.small_ptr_();
            other.count_    = 0;
            other.capacity_ = SmallCapacity;
        }

        // Move assignment operator.
        buffer& operator=(buffer&&) = delete;

        ~buffer()
        {
            clear();

            if (!is_small_())
            {
                snn_should(data_ != nullptr);
                mem::allocator<T> alloc;
                alloc.deallocate(data_, capacity_);
            }
        }

        T* begin() noexcept
        {
            return data_;
        }

        T* end() noexcept
        {
            return data_ + count_;
        }

        const T* cbegin() const noexcept
        {
            return data_;
        }

        const T* cend() const noexcept
        {
            return data_ + count_;
        }

        usize capacity() const noexcept
        {
            return capacity_;
        }

        void clear() noexcept
        {
            mem::destruct_n(data_, count_);
            count_ = 0;
        }

        usize count() const noexcept
        {
            return count_;
        }

        void decrement_count(promise::not_empty_t) noexcept
        {
            snn_should(count_ > 0);
            --count_;
        }

        void grow(const not_zero<usize> capacity, promise::is_valid_t)
        {
            snn_should(capacity.get() > capacity_ && capacity.get() <= vec::max_capacity<T>);

            const auto size = mem::raw::optimal_size(not_zero{capacity.get() * sizeof(T)});
            const auto cap  = not_zero{size.get() / sizeof(T)};

            mem::allocator<T> alloc;

            if (is_small_())
            {
                T* buf = alloc.allocate(cap).value();

                if (count_ > 0)
                {
                    mem::relocate(not_null{begin()}, not_null{end()}, not_null{buf});
                }

                data_ = buf;
            }
            else
            {
                snn_should(data_ != nullptr);
                data_ = alloc.reallocate(data_, capacity_, cap, count_).value();
            }

            // count_ doesn't change.
            capacity_ = cap.get();
        }

        template <typename... Args>
        void grow_append_inplace(const not_zero<usize> capacity, promise::is_valid_t,
                                 Args&&... args)
        {
            snn_should(count_ == capacity_);
            snn_should(capacity.get() > capacity_ && capacity.get() <= vec::max_capacity<T>);

            const auto size = mem::raw::optimal_size(not_zero{capacity.get() * sizeof(T)});
            const auto cap  = not_zero{size.get() / sizeof(T)};

            mem::allocator<T> alloc;

            // Always allocate a new buffer.
            T* buf = alloc.allocate(cap).value();

            try
            {
                mem::construct(not_null{buf + count_}, std::forward<Args>(args)...);
            }
            catch (...)
            {
                alloc.deallocate(buf, cap.get());
                throw;
            }

            if (count_ > 0)
            {
                mem::relocate(not_null{begin()}, not_null{end()}, not_null{buf});
            }

            if (!is_small_())
            {
                snn_should(data_ != nullptr);
                alloc.deallocate(data_, capacity_);
            }

            data_ = buf;
            ++count_;
            capacity_ = cap.get();
        }

        void increment_count(promise::has_capacity_t) noexcept
        {
            snn_should(count_ < capacity_);
            ++count_;
        }

        void set_count(const usize count, promise::has_capacity_t) noexcept
        {
            snn_should(count <= capacity_);
            count_ = count;
        }

        void swap(buffer&) = delete; // A vec with small capacity can currently not be swapped.

      private:
        alignas(T) byte small_[sizeof(T) * SmallCapacity];
        T* data_;
        usize count_;
        usize capacity_;

        bool is_small_() const noexcept
        {
            return data_ == small_ptr_();
        }

        T* small_ptr_() noexcept
        {
            return std::launder(reinterpret_cast<T*>(&small_[0]));
        }

        const T* small_ptr_() const noexcept
        {
            return std::launder(reinterpret_cast<const T*>(&small_[0]));
        }
    };

    // vec buffer without small capacity, can be `constexpr`.

    template <typename T>
    class buffer<T, 0> final
    {
      public:
        using trivially_relocatable_type = buffer; // Always relocatable.

        constexpr buffer() noexcept
            : data_{nullptr},
              count_{0},
              capacity_{0}
        {
        }

        // Non-copyable
        buffer(const buffer&)            = delete;
        buffer& operator=(const buffer&) = delete;

        // Move constructor.
        constexpr buffer(buffer&& other) noexcept
            : data_{std::exchange(other.data_, nullptr)},
              count_{std::exchange(other.count_, 0)},
              capacity_{std::exchange(other.capacity_, 0)}
        {
        }

        // Move assignment operator.
        buffer& operator=(buffer&&) = delete;

        constexpr ~buffer()
        {
            clear();

            mem::allocator<T> alloc;
            alloc.deallocate(data_, capacity_); // Does nothing if nullptr.
        }

        constexpr T* begin() noexcept
        {
            return data_;
        }

        constexpr T* end() noexcept
        {
            return data_ + count_;
        }

        constexpr const T* cbegin() const noexcept
        {
            return data_;
        }

        constexpr const T* cend() const noexcept
        {
            return data_ + count_;
        }

        constexpr usize capacity() const noexcept
        {
            return capacity_;
        }

        constexpr void clear() noexcept
        {
            mem::destruct_n(data_, count_);
            count_ = 0;
        }

        constexpr usize count() const noexcept
        {
            return count_;
        }

        constexpr void decrement_count(promise::not_empty_t) noexcept
        {
            snn_should(count_ > 0);
            --count_;
        }

        constexpr void grow(const not_zero<usize> capacity, promise::is_valid_t)
        {
            snn_should(capacity.get() > capacity_ && capacity.get() <= vec::max_capacity<T>);

            const auto size = mem::raw::optimal_size(not_zero{capacity.get() * sizeof(T)});
            const auto cap  = not_zero{size.get() / sizeof(T)};

            mem::allocator<T> alloc;
            // data_ can be nullptr here, if so reallocate() behaves as allocate().
            data_ = alloc.reallocate(data_, capacity_, cap, count_).value();

            // count_ doesn't change.
            capacity_ = cap.get();
        }

        template <typename... Args>
        constexpr void grow_append_inplace(const not_zero<usize> capacity, promise::is_valid_t,
                                           Args&&... args)
        {
            snn_should(count_ == capacity_);
            snn_should(capacity.get() > capacity_ && capacity.get() <= vec::max_capacity<T>);

            const auto size = mem::raw::optimal_size(not_zero{capacity.get() * sizeof(T)});
            const auto cap  = not_zero{size.get() / sizeof(T)};

            mem::allocator<T> alloc;

            // Always allocate a new buffer.
            T* buf = alloc.allocate(cap).value();

            try
            {
                mem::construct(not_null{buf + count_}, std::forward<Args>(args)...);
            }
            catch (...)
            {
                alloc.deallocate(buf, cap.get());
                throw;
            }

            if (count_ > 0)
            {
                mem::relocate(not_null{begin()}, not_null{end()}, not_null{buf});
            }

            alloc.deallocate(data_, capacity_); // Does nothing if nullptr.

            data_ = buf;
            ++count_;
            capacity_ = cap.get();
        }

        constexpr void increment_count(promise::has_capacity_t) noexcept
        {
            snn_should(count_ < capacity_);
            ++count_;
        }

        constexpr void set_count(const usize count, promise::has_capacity_t) noexcept
        {
            snn_should(count <= capacity_);
            count_ = count;
        }

        constexpr void swap(buffer& other) noexcept
        {
            // This works even if this == &other.
            std::swap(data_, other.data_);
            std::swap(count_, other.count_);
            std::swap(capacity_, other.capacity_);
        }

      private:
        T* data_;
        usize count_;
        usize capacity_;
    };

    SNN_DIAGNOSTIC_POP
}
