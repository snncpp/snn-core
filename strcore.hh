// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # String (`str[buf]`) and `concat(...)` function

// Owns a contiguous sequence of `char` objects.

// Two strings are currently available (different buffers):

// `str` which is an alias for `strcore<detail::strcore::sso>`:
// * Small string optimization (SSO).
// * Null-terminated.

// `strbuf` which is an alias for `strcore<detail::strcore::buf>`:
// * Without SSO.
// * Not null-terminated.

// `strcore` owns a buffer and is the interface for all strings.

#pragma once

#include "snn-core/array.hh"
#include "snn-core/array_view.hh"
#include "snn-core/contiguous_interface.hh"
#include "snn-core/null_term.hh"
#include "snn-core/strcore.fwd.hh"
#include "snn-core/detail/strcore/common.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/hex/table/common.hh"
#include "snn-core/math/common.hh"
#include "snn-core/mem/raw/copy.hh"
#include "snn-core/mem/raw/move.hh"
#include "snn-core/string/size.hh"
#include "snn-core/range/contiguous.hh"

namespace snn
{
    // ## Classes

    // ### strcore

    SNN_DIAGNOSTIC_PUSH
    SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

    template <typename Buffer>
    class strcore final : public contiguous_interface<strcore<Buffer>>
    {
      public:
        // #### Types

        using value_type = char;

        using iterator       = char*;
        using const_iterator = const char*;

        using pointer       = char*;
        using const_pointer = const char*;

        using reference       = char&;
        using const_reference = const char&;

        using buffer_type = Buffer;

        using trivially_relocatable_type = trivially_relocatable_if_t<strcore, Buffer>;

        // #### Default constructor

        constexpr strcore() noexcept = default;

        // #### Explicit constructors

        template <character Char>
        constexpr explicit strcore(range::contiguous<Char*> rng)
            : buf_{rng.data(), rng.count()}
        {
        }

        constexpr explicit strcore(container::reserve_t, const usize capacity)
            : buf_{capacity}
        {
        }

        template <character Char>
        constexpr explicit strcore(container::fill_t, const usize count, const Char c)
            : buf_{count, c}
        {
        }

        template <character CharA, character CharB>
        explicit strcore(container::fill_t, CharA, CharB) = delete; // Error-prone

        constexpr explicit strcore(const not_null<const_pointer> data, const usize size)
            : buf_{data, size}
        {
        }

        constexpr explicit strcore(const const_pointer s, promise::null_terminated_t)
            : buf_{s, s + string::size(s, promise::null_terminated)}
        {
        }

        constexpr explicit strcore(const not_null<const_pointer> s, promise::null_terminated_t)
            : buf_{s, string::size(s, promise::null_terminated)}
        {
        }

        constexpr explicit strcore(meta::iterators_t, const const_pointer first,
                                   const const_pointer last)
            : buf_{first, last}
        {
        }

        // #### Converting constructors

        template <usize N>
        constexpr strcore(const char (&s)[N])
            : buf_{array_view{s}}
        {
        }

        constexpr strcore(const init_list<char> l)
            : buf_{l.begin(), l.end()}
        {
        }

        strcore(init_list<int>) = delete; // Error-prone

        template <character Char, usize Count>
        constexpr strcore(const array<Char, Count>& a)
            : buf_{a.template view<>()}
        {
        }

        template <character Char, usize Count>
        constexpr strcore(const array_view<Char, Count> s)
            : buf_{s}
        {
        }

        template <typename Buf>
        constexpr strcore(const strcore<Buf>& s)
            : buf_{s.view()}
        {
        }

        // #### Copy constructor/copy assignment operator

        constexpr strcore(const strcore& other)
            : buf_{other.view()}
        {
        }

        constexpr strcore& operator=(const strcore& other)
        {
            if (this != &other)
            {
                const cstrview s = other.view();
                mem::raw::copy(s.data(), buf_.resize_uninitialized(s.size()).writable(),
                               s.byte_size(), promise::no_overlap);
            }
            return *this;
        }

        // #### Move constructor/move assignment operator

        constexpr strcore(strcore&& other) noexcept
            : buf_{std::move(other.buf_)}
        {
        }

        constexpr strcore& operator=(strcore&& other) noexcept
        {
            swap(other);
            return *this;
        }

        // #### Destructor

        ~strcore() = default; // "Rule of five".

        // #### Assignment operator

        constexpr strcore& operator=(const same_as<char> auto c)
        {
            buf_.resize_uninitialized(1).begin()[0] = c;
            return *this;
        }

        template <usize N>
        constexpr strcore& operator=(const char (&s)[N])
        {
            constexpr usize Size = N - 1;
            mem::raw::copy<Size>(not_null{s}, buf_.resize_uninitialized(Size).writable(),
                                 promise::no_overlap);
            return *this;
        }

        constexpr strcore& operator=(const init_list<char> l)
        {
            if (l.size() == 0)
            {
                buf_.clear();
            }
            else
            {
                mem::raw::copy(not_null{l.begin()}, buf_.resize_uninitialized(l.size()).writable(),
                               snn::byte_size{l.size()}, promise::no_overlap);
            }
            return *this;
        }

        void operator=(init_list<int>) = delete; // Error-prone

        template <character Char, usize Count>
        constexpr strcore& operator=(const array_view<Char, Count> s)
        {
            if constexpr (Count == 0)
            {
                buf_.clear();
            }
            else
            {
                buf_.assign(not_null{s.data()}, s.size());
            }
            return *this;
        }

        template <typename Buf>
        constexpr strcore& operator=(const strcore<Buf>& s)
        {
            return operator=(s.view());
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return buf_.size() > 0;
        }

        // #### Iterators

        [[nodiscard]] constexpr iterator begin() noexcept
        {
            return buf_.begin();
        }

        [[nodiscard]] constexpr iterator end() noexcept
        {
            return buf_.end();
        }

        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return buf_.cbegin();
        }

        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return buf_.cend();
        }

        [[nodiscard]] constexpr const_iterator cbegin() const noexcept
        {
            return buf_.cbegin();
        }

        [[nodiscard]] constexpr const_iterator cend() const noexcept
        {
            return buf_.cend();
        }

        // #### Single element access

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) noexcept
        {
            return view().template at<R>(pos);
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) const noexcept
        {
            return view().template at<R>(pos);
        }

        [[nodiscard]] constexpr reference at(const usize pos, promise::within_bounds_t) noexcept
        {
            // Don't use view(), it makes it harder for the optimizer to remove the assert.
            snn_assert(pos < count());
            return begin()[pos];
        }

        [[nodiscard]] constexpr const_reference at(const usize pos,
                                                   promise::within_bounds_t) const noexcept
        {
            // Don't use view(), it makes it harder for the optimizer to remove the assert.
            snn_assert(pos < count());
            return cbegin()[pos];
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> back() noexcept
        {
            return view().template back<R>();
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> back() const noexcept
        {
            return view().template back<R>();
        }

        [[nodiscard]] constexpr reference back(promise::not_empty_t) noexcept
        {
            // Don't use view(), it makes it harder for the optimizer to remove the assert.
            snn_assert(count() > 0);
            return begin()[count() - 1];
        }

        [[nodiscard]] constexpr const_reference back(promise::not_empty_t) const noexcept
        {
            // Don't use view(), it makes it harder for the optimizer to remove the assert.
            snn_assert(count() > 0);
            return cbegin()[count() - 1];
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> front() noexcept
        {
            return view().template front<R>();
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> front() const noexcept
        {
            return view().template front<R>();
        }

        [[nodiscard]] constexpr reference front(promise::not_empty_t) noexcept
        {
            // Don't use view(), it makes it harder for the optimizer to remove the assert.
            snn_assert(count() > 0);
            return begin()[0];
        }

        [[nodiscard]] constexpr const_reference front(promise::not_empty_t) const noexcept
        {
            // Don't use view(), it makes it harder for the optimizer to remove the assert.
            snn_assert(count() > 0);
            return cbegin()[0];
        }

        // #### Append

        constexpr void append(const same_as<char> auto c)
        {
            buf_.append_uninitialized(1).begin()[0] = c;
        }

        template <usize N>
        constexpr void append(const char (&s)[N])
        {
            constexpr usize Size = N - 1;
            mem::raw::copy<Size>(not_null{s}, buf_.append_uninitialized(Size).writable(),
                                 promise::no_overlap);
        }

        constexpr void append(const init_list<char> l)
        {
            if (l.size() > 0)
            {
                mem::raw::copy(not_null{l.begin()}, buf_.append_uninitialized(l.size()).writable(),
                               snn::byte_size{l.size()}, promise::no_overlap);
            }
        }

        void append(init_list<int>) = delete; // Error-prone

        template <character Char, usize Count>
        constexpr void append(const array<Char, Count>& a)
        {
            mem::raw::copy<Count>(a.data(), buf_.append_uninitialized(Count).writable(),
                                  promise::no_overlap);
        }

        template <character Char, usize Count>
        constexpr void append(const array_view<Char, Count> s)
        {
            if constexpr (Count > 0)
            {
                buf_.append(not_null{s.data()}, s.size());
            }
        }

        template <typename Buf>
        constexpr void append(const strcore<Buf>& s)
        {
            append(s.view());
        }

        constexpr void append(const not_null<const_pointer> data, const usize size)
        {
            buf_.append(data, size);
        }

        // #### Append integral

        template <math::base Base = math::base::decimal, strict_unsigned_integral UInt>
        constexpr void append_integral(const UInt num, const usize min_digits = 0)
        {
            const usize size = math::max(math::count_digits<Base>(num).get(), min_digits);
            strview dest     = buf_.append_uninitialized(size);
            append_unsigned_<Base>(dest.end(), dest.begin(), num);
        }

        template <math::base Base = math::base::decimal, strict_signed_integral SInt>
        constexpr void append_integral(const SInt num, const usize min_digits = 0)
        {
            auto n     = force_unsigned(num);
            usize size = 0;

            if (num < 0)
            {
                n = math::negate_with_overflow(n);
                size += sizeof('-');
            }

            size += math::max(math::count_digits<Base>(n).get(), min_digits);
            strview dest = buf_.append_uninitialized(size);
            char* first  = dest.begin();
            char* last   = dest.end();

            if (num < 0)
            {
                *first = '-';
                ++first;
            }

            append_unsigned_<Base>(last, first, n);
        }

        // #### Append uninitialized

        template <usize SizeIncrease>
            requires(SizeIncrease != constant::dynamic_count)
        [[nodiscard]] constexpr auto append_uninitialized()
        {
            return array_view<char, SizeIncrease>{buf_.append_uninitialized(SizeIncrease).begin(),
                                                  promise::has_capacity};
        }

        [[nodiscard]] constexpr strview append_uninitialized(const usize size_increase)
        {
            return buf_.append_uninitialized(size_increase);
        }

        // #### Count/Size

        [[nodiscard]] constexpr snn::byte_size<usize> byte_size() const noexcept
        {
            return snn::byte_size<usize>{buf_.size()};
        }

        [[nodiscard]] constexpr usize count() const noexcept
        {
            return buf_.size();
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return buf_.size() == 0;
        }

        [[nodiscard]] constexpr usize size() const noexcept
        {
            return buf_.size();
        }

        // #### Capacity

        [[nodiscard]] constexpr usize capacity() const noexcept
        {
            return buf_.capacity();
        }

        [[nodiscard]] static constexpr usize default_capacity() noexcept
        {
            return Buffer::default_capacity();
        }

        constexpr void reserve(const usize capacity)
        {
            buf_.reserve(capacity);
        }

        constexpr void reserve_append(const usize size_increase)
        {
            buf_.reserve_append(size_increase);
        }

        constexpr void shrink_to_fit() noexcept
        {
            buf_.shrink_to_fit();
        }

        // #### Null terminated

        [[nodiscard]] static constexpr bool is_null_terminated() noexcept
        {
            return detail::strcore::is_null_terminated_v<Buffer>;
        }

        // Will throw if the buffer contains any null bytes (excluding the trailing null byte).

        [[nodiscard]] constexpr null_term<const char*> null_terminated() const
            requires(detail::strcore::is_null_terminated_v<Buffer>)
        {
            const auto s = view();
            if (string::size(s.data(), promise::null_terminated) == s.size())
            {
                return null_term<const char*>{s.data(), promise::null_terminated};
            }
            throw_or_abort(generic::error::unexpected_null_character);
        }

        // Non-throwing with promise.

        [[nodiscard]] constexpr null_term<const char*> null_terminated(
            promise::is_valid_t) const noexcept
            requires(detail::strcore::is_null_terminated_v<Buffer>)
        {
            const auto s = view();
            snn_should(string::size(s.data(), promise::null_terminated) == s.size());
            return null_term<const char*>{s.data(), promise::null_terminated};
        }

        // #### Raw data access

        [[nodiscard]] constexpr not_null<const_pointer> data() const noexcept
        {
            return not_null<const_pointer>{buf_.cbegin()};
        }

        // constexpr not_null<pointer> data() noexcept
        // {
        //     // Not implemented.
        //     // Use writable(), which is more readable and easier to audit.
        // }

        [[nodiscard]] constexpr not_null<pointer> writable() noexcept
        {
            return not_null<pointer>{buf_.begin()};
        }

        // #### Range

        [[nodiscard]] constexpr strrng range() noexcept
        {
            return buf_.view().range();
        }

        [[nodiscard]] constexpr cstrrng range() const noexcept
        {
            return buf_.view().range();
        }

        // #### View

        [[nodiscard]] constexpr strview view() noexcept
        {
            return buf_.view();
        }

        [[nodiscard]] constexpr cstrview view() const noexcept
        {
            return buf_.view();
        }

        [[nodiscard]] constexpr strview view(const usize pos,
                                             const usize size = constant::npos) noexcept
        {
            return buf_.view().view(pos, size);
        }

        [[nodiscard]] constexpr cstrview view(const usize pos,
                                              const usize size = constant::npos) const noexcept
        {
            return buf_.view().view(pos, size);
        }

        // #### To integral

        template <strict_integral Int, math::base Base = math::base::decimal>
        [[nodiscard]] constexpr optional<Int> to() const noexcept
        {
            return buf_.view().template to<Int, Base>();
        }

        template <strict_integral Int, math::base Base = math::base::decimal, usize MaxDigits = 0>
        [[nodiscard]] constexpr pair::value_count<Int, usize> to_prefix(
            ascii::leading_zeros policy = ascii::leading_zeros::disallow) const noexcept
        {
            return buf_.view().template to_prefix<Int, Base, MaxDigits>(policy);
        }

        // #### Modifiers

        constexpr void clear() noexcept
        {
            buf_.clear();
        }

        constexpr void insert_at(const usize pos, const transient<cstrview> s)
        {
            const cstrview sv = s.get();
            if (!std::is_constant_evaluated() && !buf_.overlaps(sv)) [[likely]]
            {
                mem::raw::copy(sv.data(), buf_.replace_uninitialized(pos, 0, sv.size()).writable(),
                               sv.byte_size(), promise::no_overlap);
            }
            else
            {
                // This could be optimized, but it's currently not worth the complexity.
                Buffer tmp{buf_.view()};
                mem::raw::copy(sv.data(), tmp.replace_uninitialized(pos, 0, sv.size()).writable(),
                               sv.byte_size(), promise::no_overlap);
                buf_.swap(tmp);
            }
        }

        constexpr void insert_at(const usize pos, const transient<cstrview> s,
                                 promise::no_overlap_t)
        {
            const cstrview sv = s.get();
            snn_should(std::is_constant_evaluated() || !buf_.overlaps(sv));
            mem::raw::copy(sv.data(), buf_.replace_uninitialized(pos, 0, sv.size()).writable(),
                           sv.byte_size(), promise::no_overlap);
        }

        template <same_as<const char> ConstChar, usize N>
        constexpr void insert_at(const usize pos, ConstChar (&s)[N])
        {
            constexpr usize Size = N - 1;
            mem::raw::copy<Size>(not_null{s}, buf_.replace_uninitialized(pos, 0, Size).writable(),
                                 promise::no_overlap);
        }

        [[nodiscard]] constexpr strview insert_uninitialized(const usize pos, const usize size)
        {
            return buf_.replace_uninitialized(pos, 0, size);
        }

        constexpr void drop_at(const usize pos, const usize count) noexcept
        {
            buf_.drop_at(pos, count);
        }

        constexpr void drop_back_n(const usize size) noexcept
        {
            buf_.drop_back_n(size);
        }

        constexpr usize remove(const same_as<char> auto needle) noexcept
        {
            using base_type = contiguous_interface<strcore<Buffer>>;
            return base_type::remove_if(fn::is{fn::equal_to{}, char{needle}});
        }

        constexpr usize remove(const transient<cstrview> needle) noexcept
        {
            return replace(needle, "");
        }

        // Replace all occurrences of `needle` with `replacement`.

        constexpr usize replace(const same_as<char> auto needle,
                                const same_as<char> auto replacement,
                                const usize start_pos = 0) noexcept
        {
            strview s           = buf_.view();
            char* const data    = s.writable().get();
            const usize size    = s.size();
            usize replace_count = 0;
            for (usize i = start_pos; i < size; ++i)
            {
                if (data[i] == needle)
                {
                    data[i] = replacement;
                    ++replace_count;
                }
            }
            return replace_count;
        }

        // Replace all occurrences of `needle` with `replacement`.

        constexpr usize replace(const transient<cstrview> needle,
                                const transient<cstrview> replacement, const usize start_pos = 0)
        {
            return replace_(needle.get(), replacement.get(), start_pos);
        }

        constexpr void replace_at(const usize pos, const usize size, const transient<cstrview> s)
        {
            const cstrview replacement = s.get();
            if (!std::is_constant_evaluated() && !buf_.overlaps(replacement)) [[likely]]
            {
                mem::raw::copy(replacement.data(),
                               buf_.replace_uninitialized(pos, size, replacement.size()).writable(),
                               replacement.byte_size(), promise::no_overlap);
            }
            else
            {
                // This could be optimized, but it's currently not worth the complexity.
                Buffer tmp{buf_.view()};
                mem::raw::copy(replacement.data(),
                               tmp.replace_uninitialized(pos, size, replacement.size()).writable(),
                               replacement.byte_size(), promise::no_overlap);
                buf_.swap(tmp);
            }
        }

        constexpr void replace_at(const usize pos, const usize size, const transient<cstrview> s,
                                  promise::no_overlap_t)
        {
            const cstrview replacement = s.get();
            snn_should(std::is_constant_evaluated() || !buf_.overlaps(replacement));
            mem::raw::copy(replacement.data(),
                           buf_.replace_uninitialized(pos, size, replacement.size()).writable(),
                           replacement.byte_size(), promise::no_overlap);
        }

        template <same_as<const char> ConstChar, usize N>
        constexpr void replace_at(const usize pos, const usize size, ConstChar (&replacement)[N])
        {
            constexpr usize Size = N - 1;
            mem::raw::copy<Size>(not_null{replacement},
                                 buf_.replace_uninitialized(pos, size, Size).writable(),
                                 promise::no_overlap);
        }

        [[nodiscard]] constexpr strview replace_uninitialized(const usize pos, const usize size,
                                                              const usize replacement_size)
        {
            return buf_.replace_uninitialized(pos, size, replacement_size);
        }

        constexpr void reset() noexcept
        {
            buf_.reset();
        }

        constexpr void resize(const usize size, const same_as<char> auto fill)
        {
            buf_.resize(size, fill);
        }

        [[nodiscard]] constexpr strview resize_uninitialized(const usize size)
        {
            return buf_.resize_uninitialized(size);
        }

        constexpr void truncate(const usize size) noexcept
        {
            buf_.truncate(size);
        }

        // #### Search

        template <typename V>
        [[nodiscard]] constexpr usize count(V&& value) const noexcept
        {
            return view().count(std::forward<V>(value));
        }

        // #### Swap

        constexpr void swap(strcore& other) noexcept
        {
            // This works even if this == &other.
            buf_.swap(other.buf_);
        }

        // #### Stream append (insertion operator)

        constexpr strcore& operator<<(const same_as<char> auto c)
        {
            append(c);
            return *this;
        }

        template <usize N>
        constexpr strcore& operator<<(const char (&s)[N])
        {
            append(s);
            return *this;
        }

        template <character Char, usize Count>
        constexpr strcore& operator<<(const array<Char, Count>& a)
        {
            append(a);
            return *this;
        }

        template <character Char, usize Count>
        constexpr strcore& operator<<(const array_view<Char, Count> s)
        {
            append(s);
            return *this;
        }

        template <typename Buf>
        constexpr strcore& operator<<(const strcore<Buf>& s)
        {
            append(s);
            return *this;
        }

        template <strict_integral Int>
        constexpr strcore& operator<<(const numeric<Int> n)
        {
            append_integral(n.get());
            return *this;
        }

        // #### Hash

        [[nodiscard]] constexpr usize hash() const noexcept
        {
            return view().hash();
        }

        // #### Comparison

        template <usize N>
        constexpr bool operator==(const char (&s)[N]) const noexcept
        {
            return view() == s;
        }

        template <character Char, usize Count>
        constexpr bool operator==(const array_view<Char, Count> s) const noexcept
        {
            return view() == s;
        }

        template <typename Buf>
        constexpr bool operator==(const strcore<Buf>& s) const noexcept
        {
            return view() == s.view();
        }

        template <usize N>
        constexpr std::strong_ordering operator<=>(const char (&s)[N]) const noexcept
        {
            return view() <=> cstrview{s};
        }

        template <character Char, usize Count>
        constexpr std::strong_ordering operator<=>(const array_view<Char, Count> s) const noexcept
        {
            return view() <=> s;
        }

        template <typename Buf>
        constexpr std::strong_ordering operator<=>(const strcore<Buf>& s) const noexcept
        {
            return view() <=> s.view();
        }

        // #### Overlaps

        [[nodiscard]] constexpr bool overlaps(const cstrview s) const noexcept
        {
            return buf_.overlaps(s);
        }

        // #### Contiguous interface

        // This class inherits from `contiguous_interface`, see
        // [contiguous\_interface.hh](contiguous_interface.hh) for additional functionality.

      private:
        Buffer buf_;

        template <math::base Base, typename UInt>
        constexpr void append_unsigned_(iterator cur, const const_iterator first, UInt num) noexcept
        {
            snn_should(cur > first);

            if constexpr (Base == math::base::decimal)
            {
                // This code is based on the talk:
                // "Three Optimization Tips for C++" by Andrei Alexandrescu.

                while (num >= 100)
                {
                    const usize digit_index = (num % 100) * 2;
                    num /= 100;
                    *(--cur) = detail::strcore::digit_lookup[digit_index + 1];
                    *(--cur) = detail::strcore::digit_lookup[digit_index];
                }

                if (num < 10)
                {
                    *(--cur) = static_cast<char>(num + '0');
                }
                else // Less than 100.
                {
                    const usize digit_index = static_cast<usize>(num) * 2;
                    *(--cur)                = detail::strcore::digit_lookup[digit_index + 1];
                    *(--cur)                = detail::strcore::digit_lookup[digit_index];
                }
            }
            else if constexpr (Base == math::base::hex)
            {
                do
                {
                    *(--cur) = hex::table::lower.at(num, bounds::mask);
                    num >>= 4;
                } while (num);
            }
            else if constexpr (Base == math::base::binary)
            {
                do
                {
                    *(--cur) = static_cast<char>((num & 0b1) + '0');
                    num >>= 1;
                } while (num);
            }
            else if constexpr (Base == math::base::octal)
            {
                do
                {
                    *(--cur) = static_cast<char>((num & 0b111) + '0');
                    num >>= 3;
                } while (num);
            }
            else
            {
                static_assert(meta::always_false<decltype(Base)>, "Unsupported base.");
            }

            // Pad left?
            while (cur > first)
            {
                *(--cur) = '0';
            }

            snn_should(cur == first);
        }

        constexpr usize replace_(const cstrview needle, const cstrview replacement,
                                 const usize start_pos)
        {
            if (needle.is_empty())
            {
                return 0;
            }

            strview subject = view();
            usize pos       = subject.find(needle, start_pos).value_or_npos();
            if (pos == constant::npos)
            {
                return 0;
            }

            usize replace_count = 0;

            if (std::is_constant_evaluated() || replacement.size() > needle.size() ||
                buf_.overlaps(needle) || buf_.overlaps(replacement))
            {
                const usize diff =
                    replacement.size() - math::min(needle.size(), replacement.size());
                // Reserve for one replacement and an overflow here can't do any harm.
                strcore tmp{container::reserve, subject.size() + diff};

                usize last_pos = 0;

                do
                {
                    if (pos > last_pos)
                    {
                        const usize size = pos - last_pos;
                        tmp.append(cstrview{not_null{subject.data().get() + last_pos}, size});
                    }

                    tmp.append(replacement);

                    pos += needle.size();
                    last_pos = pos;
                    pos      = subject.find(needle, pos).value_or_npos();

                    ++replace_count;
                } while (pos != constant::npos);

                if (subject.size() > last_pos)
                {
                    const usize size = subject.size() - last_pos;
                    tmp.append(cstrview{not_null{subject.data().get() + last_pos}, size});
                }

                swap(tmp);
            }
            else if (replacement.size() < needle.size())
            {
                char* const data = subject.writable().get();

                usize last_pos  = 0;
                usize write_pos = 0;

                do
                {
                    if (pos > last_pos)
                    {
                        const usize size = pos - last_pos;
                        if (write_pos < last_pos)
                        {
                            mem::raw::move(not_null<const char*>{data + last_pos},
                                           not_null{data + write_pos}, snn::byte_size{size});
                        }
                        write_pos += size;
                    }

                    if (replacement)
                    {
                        mem::raw::copy(replacement.data(), not_null{data + write_pos},
                                       replacement.byte_size(), promise::no_overlap);
                        write_pos += replacement.size();
                    }

                    pos += needle.size();
                    last_pos = pos;
                    pos      = subject.find(needle, pos).value_or_npos();

                    ++replace_count;
                } while (pos != constant::npos);

                if (subject.size() > last_pos)
                {
                    const usize size = subject.size() - last_pos;
                    snn_should(write_pos < last_pos);
                    mem::raw::move(not_null<const char*>{data + last_pos},
                                   not_null{data + write_pos}, snn::byte_size{size});
                    write_pos += size;
                }

                snn_should(write_pos < subject.size());
                buf_.truncate(write_pos);
            }
            else
            {
                snn_should(replacement.size() == needle.size());
                char* const data = subject.writable().get();
                do
                {
                    mem::raw::copy(replacement.data(), not_null{data + pos},
                                   replacement.byte_size(), promise::no_overlap);
                    pos = subject.find(needle, pos + needle.size()).value_or_npos();

                    ++replace_count;
                } while (pos != constant::npos);
            }

            return replace_count;
        }
    };

    SNN_DIAGNOSTIC_POP

    // ## Functions

    // ### concat

    namespace detail::concat
    {
        constexpr usize size(char) noexcept
        {
            return 1;
        }

        constexpr usize size(const cstrview s) noexcept
        {
            return s.size();
        }

        template <strict_integral Int>
        constexpr usize size(numeric<Int>) noexcept
        {
            return sizeof(Int) * 2; // Should be a reasonable guess.
        }
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str concat() noexcept
    {
        return Str{};
    }

    template <any_strcore Str = str, typename... Args>
    [[nodiscard]] constexpr Str concat(const Args&... args)
    {
        Str s{container::reserve, (detail::concat::size(args) + ...)};
        (s << ... << args);
        return s;
    }

    // ### swap

    template <typename Buf>
    constexpr void swap(strcore<Buf>& a, strcore<Buf>& b) noexcept
    {
        a.swap(b);
    }
}

// ## Specializations

// ### std::hash

template <typename Buf>
struct std::hash<snn::strcore<Buf>>
{
    [[nodiscard]] constexpr std::size_t operator()(const snn::strcore<Buf>& s) const noexcept
    {
        return s.view().hash();
    }
};
