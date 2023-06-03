// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Aggregate array (always initialized)

// Value-initialized by default (an `uninitialized_array` type can be added in the future if
// needed).

// With minimal internal dependencies.

#pragma once

#include "snn-core/array_view.fwd.hh"
#include "snn-core/optional.fwd.hh"
#include <algorithm>  // is_sorted, reverse, sort
#include <functional> // less<void>

SNN_DIAGNOSTIC_PUSH
SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

namespace snn
{
    namespace detail
    {
        template <typename Int, usize Count>
        constexpr bool can_overflow_count() noexcept
        {
            return constant::limit<Int>::min < Int{} || constant::limit<Int>::max >= Count;
        }
    }

    // ## Classes

    // ### array

    template <typename T, usize Count>
        requires(std::is_same_v<std::remove_cvref_t<T>, T>)
    struct array final
    {
        // #### Types

        using value_type = T;

        using iterator       = T*;
        using const_iterator = const T*;

        using pointer       = T*;
        using const_pointer = const T*;

        using reference       = T&;
        using const_reference = const T&;

        using trivially_relocatable_type =
            std::conditional_t<is_trivially_relocatable_v<T>, array, void>;

        // #### "Private" buffer

        // Should never be accessed directly.

        T priv_buf_[Count > 0 ? Count : 1]{};

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return !is_empty();
        }

        // #### Iterators

        [[nodiscard]] constexpr iterator begin() noexcept
        {
            return priv_buf_;
        }

        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return priv_buf_;
        }

        [[nodiscard]] constexpr iterator end() noexcept
        {
            return priv_buf_ + Count;
        }

        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return priv_buf_ + Count;
        }

        // #### Count/Size

        [[nodiscard]] static constexpr snn::byte_size byte_size() noexcept
        {
            return snn::byte_size{sizeof(T) * Count};
        }

        [[nodiscard]] static constexpr usize count() noexcept
        {
            return Count;
        }

        [[nodiscard]] static constexpr bool is_empty() noexcept
        {
            return Count == 0;
        }

        [[nodiscard]] static constexpr usize size() noexcept
            requires(sizeof(T) == 1) // Use `count()` or `byte_size()` instead.
        {
            return Count;
        }

        // #### Capacity

        [[nodiscard]] static constexpr usize capacity() noexcept
        {
            return Count;
        }

        // #### Data

        [[nodiscard]] constexpr not_null<const_pointer> data() const noexcept
        {
            return not_null<const_pointer>{priv_buf_};
        }

        [[nodiscard]] constexpr not_null<pointer> writable() noexcept
        {
            return not_null<pointer>{priv_buf_};
        }

        // #### Single element access

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) noexcept(
            std::is_nothrow_copy_constructible_v<R>)
        {
            if (pos < Count)
            {
                return priv_buf_[pos];
            }
            return optional<R>::default_error_code();
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> at(const usize pos) const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if (pos < Count)
            {
                return priv_buf_[pos];
            }
            return optional<R>::default_error_code();
        }

        [[nodiscard]] constexpr reference at(const strict_integral auto pos,
                                             promise::within_bounds_t) noexcept
        {
            if constexpr (detail::can_overflow_count<decltype(pos), Count>())
            {
                snn_assert(pos >= 0 && force_unsigned(pos) < Count);
            }
            return priv_buf_[pos];
        }

        [[nodiscard]] constexpr const_reference at(const strict_integral auto pos,
                                                   promise::within_bounds_t) const noexcept
        {
            if constexpr (detail::can_overflow_count<decltype(pos), Count>())
            {
                snn_assert(pos >= 0 && force_unsigned(pos) < Count);
            }
            return priv_buf_[pos];
        }

        [[nodiscard]] constexpr reference at(const strict_integral auto pos,
                                             bounds::mask_t) noexcept
            requires(power_of_two<Count>)
        {
            return priv_buf_[force_unsigned(pos) & (Count - 1u)];
        }

        [[nodiscard]] constexpr const_reference at(const strict_integral auto pos,
                                                   bounds::mask_t) const noexcept
            requires(power_of_two<Count>)
        {
            return priv_buf_[force_unsigned(pos) & (Count - 1u)];
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> back() noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if constexpr (Count > 0)
            {
                return priv_buf_[Count - 1];
            }
            return optional<R>::default_error_code();
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> back() const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if constexpr (Count > 0)
            {
                return priv_buf_[Count - 1];
            }
            return optional<R>::default_error_code();
        }

        [[nodiscard]] constexpr reference back(promise::not_empty_t) noexcept
            requires(Count > 0)
        {
            return priv_buf_[Count - 1];
        }

        [[nodiscard]] constexpr const_reference back(promise::not_empty_t) const noexcept
            requires(Count > 0)
        {
            return priv_buf_[Count - 1];
        }

        template <value_type_or<reference> R = reference>
        [[nodiscard]] constexpr optional<R> front() noexcept(
            std::is_nothrow_copy_constructible_v<R>)
        {
            if constexpr (Count > 0)
            {
                return priv_buf_[0];
            }
            return optional<R>::default_error_code();
        }

        template <value_type_or<const_reference> R = const_reference>
        [[nodiscard]] constexpr optional<R> front() const
            noexcept(std::is_nothrow_copy_constructible_v<R>)
        {
            if constexpr (Count > 0)
            {
                return priv_buf_[0];
            }
            return optional<R>::default_error_code();
        }

        [[nodiscard]] constexpr reference front(promise::not_empty_t) noexcept
            requires(Count > 0)
        {
            return priv_buf_[0];
        }

        [[nodiscard]] constexpr const_reference front(promise::not_empty_t) const noexcept
            requires(Count > 0)
        {
            return priv_buf_[0];
        }

        template <usize Index>
            requires(Index < Count)
        [[nodiscard]] constexpr reference get() noexcept
        {
            return priv_buf_[Index];
        }

        template <usize Index>
            requires(Index < Count)
        [[nodiscard]] constexpr const_reference get() const noexcept
        {
            return priv_buf_[Index];
        }

        // #### View - Dynamic count

        [[nodiscard]] constexpr auto view() noexcept
        {
            return array_view<T>{not_null{priv_buf_}, Count};
        }

        [[nodiscard]] constexpr auto view() const noexcept
        {
            return array_view<const T>{not_null{priv_buf_}, Count};
        }

        [[nodiscard]] constexpr auto view(const usize pos,
                                          const usize count = constant::npos) noexcept
        {
            return view().view(pos, count);
        }

        [[nodiscard]] constexpr auto view(const usize pos,
                                          const usize count = constant::npos) const noexcept
        {
            return view().view(pos, count);
        }

        // #### View - Static count

        template <usize Pos = 0, usize C = constant::npos>
        [[nodiscard]] constexpr auto view() noexcept
        {
            return array_view<T, Count>{priv_buf_, promise::has_capacity}.template view<Pos, C>();
        }

        template <usize Pos = 0, usize C = constant::npos>
        [[nodiscard]] constexpr auto view() const noexcept
        {
            return array_view<const T, Count>{priv_buf_, promise::has_capacity}
                .template view<Pos, C>();
        }

        // #### View exactly

        template <usize Pos, usize C>
            requires(Pos <= Count && (Count - Pos) >= C)
        [[nodiscard]] constexpr auto view_exactly() noexcept
        {
            return array_view<T, C>{priv_buf_ + Pos, promise::has_capacity};
        }

        template <usize Pos, usize C>
            requires(Pos <= Count && (Count - Pos) >= C)
        [[nodiscard]] constexpr auto view_exactly() const noexcept
        {
            return array_view<const T, C>{priv_buf_ + Pos, promise::has_capacity};
        }

        template <usize Pos, usize C>
        [[nodiscard]] constexpr auto view_exactly(promise::within_bounds_t) noexcept
        {
            static_assert(Pos <= Count && (Count - Pos) >= C);
            return array_view<T, C>{priv_buf_ + Pos, promise::has_capacity};
        }

        template <usize Pos, usize C>
        [[nodiscard]] constexpr auto view_exactly(promise::within_bounds_t) const noexcept
        {
            static_assert(Pos <= Count && (Count - Pos) >= C);
            return array_view<const T, C>{priv_buf_ + Pos, promise::has_capacity};
        }

        // #### View offset - Dynamic count

        [[nodiscard]] constexpr auto view_offset(
            const isize pos, const isize count = constant::limit<isize>::max) noexcept
        {
            return view().view_offset(pos, count);
        }

        [[nodiscard]] constexpr auto view_offset(
            const isize pos, const isize count = constant::limit<isize>::max) const noexcept
        {
            return view().view_offset(pos, count);
        }

        // #### View offset - Static count

        template <isize Pos, isize C = constant::limit<isize>::max>
        [[nodiscard]] constexpr auto view_offset() noexcept
        {
            return array_view<T, Count>{priv_buf_, promise::has_capacity}
                .template view_offset<Pos, C>();
        }

        template <isize Pos, isize C = constant::limit<isize>::max>
        [[nodiscard]] constexpr auto view_offset() const noexcept
        {
            return array_view<const T, Count>{priv_buf_, promise::has_capacity}
                .template view_offset<Pos, C>();
        }

        // #### Range

        [[nodiscard]] constexpr auto range() noexcept
        {
            return view().range();
        }

        [[nodiscard]] constexpr auto range() const noexcept
        {
            return view().range();
        }

        // #### Operations

        template <typename V>
        constexpr void fill(const V& value)
        {
            for (usize i = 0; i < Count; ++i)
            {
                priv_buf_[i] = value;
            }
        }

        template <typename V, usize C>
        constexpr usize fill_front(const array<V, C>& values)
        {
            constexpr usize fill_count = Count < C ? Count : C;
            for (usize i = 0; i < fill_count; ++i)
            {
                priv_buf_[i] = values.priv_buf_[i];
            }
            return fill_count;
        }

        template <typename V, usize C>
        constexpr usize fill_front(const array_view<V, C> values)
        {
            const usize fill_count = Count < values.count() ? Count : values.count();
            for (usize i = 0; i < fill_count; ++i)
            {
                priv_buf_[i] = values.begin()[i];
            }
            return fill_count;
        }

        // #### Search

        template <typename V>
        [[nodiscard]] constexpr bool contains(const V& needle) const
        {
            for (usize i = 0; i < Count; ++i)
            {
                if (priv_buf_[i] == needle)
                {
                    return true;
                }
            }
            return false;
        }

        template <typename V>
        [[nodiscard]] constexpr usize count(const V& needle) const
        {
            usize needle_count = 0;
            for (usize i = 0; i < Count; ++i)
            {
                if (priv_buf_[i] == needle)
                {
                    ++needle_count;
                }
            }
            return needle_count;
        }

        template <typename OneArgPred>
        [[nodiscard]] constexpr usize count_if(OneArgPred p) const
        {
            usize pred_count = 0;
            for (usize i = 0; i < Count; ++i)
            {
                if (p(std::as_const(priv_buf_[i])))
                {
                    ++pred_count;
                }
            }
            return pred_count;
        }

        template <typename V>
        constexpr auto find(V&& value, const usize start_pos = 0) const
        {
            return view<>().find(std::forward<V>(value), start_pos);
        }

        template <typename OneArgPred>
        constexpr auto find_if(OneArgPred&& p, const usize start_pos = 0) const
        {
            return view<>().find_if(std::forward<OneArgPred>(p), start_pos);
        }

        template <typename V>
        constexpr auto find_in_reverse(V&& value, const usize start_pos = constant::npos) const
        {
            return view<>().find_in_reverse(std::forward<V>(value), start_pos);
        }

        template <typename OneArgPred>
        constexpr auto find_in_reverse_if(OneArgPred&& p,
                                          const usize start_pos = constant::npos) const
        {
            return view<>().find_in_reverse_if(std::forward<OneArgPred>(p), start_pos);
        }

        // #### Swap

        constexpr void swap(array& other) noexcept(Count == 0 || std::is_nothrow_swappable_v<T>)
        {
            if (this != &other)
            {
                for (usize i = 0; i < Count; ++i)
                {
                    using std::swap;
                    swap(priv_buf_[i], other.priv_buf_[i]);
                }
            }
        }

        // #### Comparison

        template <typename V>
        [[nodiscard]] constexpr bool has_front(const V& value) const
        {
            return Count > 0 && priv_buf_[0] == value;
        }

        template <typename V>
        [[nodiscard]] constexpr bool has_back(const V& value) const
        {
            return Count > 0 && priv_buf_[Count - 1] == value;
        }

        // #### Algorithm helpers

        template <typename OneArgFn>
        constexpr void each(OneArgFn f)
        {
            for (usize i = 0; i < Count; ++i)
            {
                f(priv_buf_[i]);
            }
        }

        template <typename OneArgFn>
        constexpr void each(OneArgFn f) const
        {
            for (usize i = 0; i < Count; ++i)
            {
                f(std::as_const(priv_buf_[i]));
            }
        }

        template <typename OneArgFn>
        constexpr void each_in_reverse(OneArgFn f)
        {
            usize i = Count;
            while (i > 0)
            {
                --i;
                f(priv_buf_[i]);
            }
        }

        template <typename OneArgFn>
        constexpr void each_in_reverse(OneArgFn f) const
        {
            usize i = Count;
            while (i > 0)
            {
                --i;
                f(std::as_const(priv_buf_[i]));
            }
        }

        template <typename TwoArgPred = std::less<void>>
        [[nodiscard]] constexpr bool is_sorted(TwoArgPred is_less = TwoArgPred{}) const
        {
            return std::is_sorted(begin(), end(), std::move(is_less));
        }

        template <typename U, typename V>
        constexpr usize replace(const U& needle, const V& replacement)
        {
            usize replace_count = 0;
            for (usize i = 0; i < Count; ++i)
            {
                if (priv_buf_[i] == needle)
                {
                    priv_buf_[i] = replacement;
                    ++replace_count;
                }
            }
            return replace_count;
        }

        template <typename OneArgPred, typename V>
        constexpr usize replace_if(OneArgPred p, const V& replacement)
        {
            usize replace_count = 0;
            for (usize i = 0; i < Count; ++i)
            {
                if (p(std::as_const(priv_buf_[i])))
                {
                    priv_buf_[i] = replacement;
                    ++replace_count;
                }
            }
            return replace_count;
        }

        constexpr void reverse()
        {
            std::reverse(begin(), end());
        }

        template <typename TwoArgPred = std::less<void>>
        constexpr void sort(TwoArgPred is_less = TwoArgPred{})
        {
            std::sort(begin(), end(), std::move(is_less));
        }

        template <typename OneArgOp>
        constexpr void transform(OneArgOp op)
        {
            for (usize i = 0; i < Count; ++i)
            {
                priv_buf_[i] = op(std::as_const(priv_buf_[i]));
            }
        }

        template <typename U, typename TwoArgOp>
        constexpr void transform(const array<U, Count>& other, TwoArgOp op)
        {
            for (usize i = 0; i < Count; ++i)
            {
                priv_buf_[i] = op(std::as_const(priv_buf_[i]), other.begin()[i]);
            }
        }

        template <typename U, typename TwoArgOp>
        constexpr void transform(const array_view<U, Count> other, TwoArgOp op)
        {
            for (usize i = 0; i < Count; ++i)
            {
                priv_buf_[i] = op(std::as_const(priv_buf_[i]), other.begin()[i]);
            }
        }

        // #### Validation helpers

        template <typename OneArgPred>
        [[nodiscard]] constexpr bool all(OneArgPred p) const
        {
            for (usize i = 0; i < Count; ++i)
            {
                if (!p(std::as_const(priv_buf_[i])))
                {
                    return false;
                }
            }
            return true;
        }

        template <typename OneArgPred>
        [[nodiscard]] constexpr bool any(OneArgPred p) const
        {
            for (usize i = 0; i < Count; ++i)
            {
                if (p(std::as_const(priv_buf_[i])))
                {
                    return true;
                }
            }
            return false;
        }

        template <typename OneArgPred>
        [[nodiscard]] constexpr bool none(OneArgPred p) const
        {
            for (usize i = 0; i < Count; ++i)
            {
                if (p(std::as_const(priv_buf_[i])))
                {
                    return false;
                }
            }
            return true;
        }
    };

    // ## Deduction guides

    // ### array

    template <typename T, typename... Tn>
    array(T, Tn...) -> array<T, 1 + sizeof...(Tn)>;
    template <usize N, typename... Tn>
    array(const char (&)[N], Tn...) -> array<cstrview, 1 + sizeof...(Tn)>;

    // ## Functions

    // ### Comparison

    template <typename T, usize Count>
    constexpr bool operator==(const array<T, Count>& left, const array<T, Count>& right)
    {
        if (!std::is_constant_evaluated() && is_strict_integral_v<T>)
        {
            return __builtin_memcmp(left.priv_buf_, right.priv_buf_, left.byte_size().get()) == 0;
        }
        else
        {
            for (usize i = 0; i < Count; ++i)
            {
                if (left.priv_buf_[i] != right.priv_buf_[i])
                {
                    return false;
                }
            }
            return true;
        }
    }

    template <typename T, usize Count>
    constexpr auto operator<=>(const array<T, Count>& left, const array<T, Count>& right)
        -> decltype(left.priv_buf_[0] <=> right.priv_buf_[0])
    {
        // char is compared as unsigned char, just like array_view.
        if constexpr (std::is_same_v<T, char>)
        {
            const auto cmp = __builtin_memcmp(left.priv_buf_, right.priv_buf_, Count) <=> 0;
            if (cmp != 0)
            {
                return cmp;
            }
        }
        else
        {
            for (usize i = 0; i < Count; ++i)
            {
                const auto cmp = left.priv_buf_[i] <=> right.priv_buf_[i];
                if (cmp != 0)
                {
                    return cmp;
                }
            }
        }
        return std::strong_ordering::equal;
    }

    // ### get

    template <usize Index, typename T, usize Count>
        requires(Index < Count)
    [[nodiscard]] constexpr T& get(array<T, Count>& a) noexcept
    {
        return a.priv_buf_[Index];
    }

    template <usize Index, typename T, usize Count>
        requires(Index < Count)
    [[nodiscard]] constexpr const T& get(const array<T, Count>& a) noexcept
    {
        return a.priv_buf_[Index];
    }

    template <usize Index, typename T, usize Count>
        requires(Index < Count)
    [[nodiscard]] constexpr T&& get(array<T, Count>&& a) noexcept
    {
        return std::move(a.priv_buf_[Index]);
    }

    template <usize Index, typename T, usize Count>
        requires(Index < Count)
    [[nodiscard]] constexpr const T&& get(const array<T, Count>&& a) noexcept
    {
        return std::move(a.priv_buf_[Index]);
    }

    // ### swap

    template <typename T, usize Count>
    constexpr void swap(array<T, Count>& a,
                        array<T, Count>& b) noexcept(Count == 0 || std::is_nothrow_swappable_v<T>)
    {
        a.swap(b);
    }

    // ### to_array

    namespace detail
    {
        template <usize Count, typename T, usize N, usize... Index>
        constexpr array<std::remove_cv_t<T>, Count> to_array_copy(T (&a)[N],
                                                                  std::index_sequence<Index...>)
        {
            return {{a[Index]...}};
        }

        template <usize Count, typename T, usize N, usize... Index>
        constexpr array<std::remove_cv_t<T>, Count> to_array_move(T (&&a)[N],
                                                                  std::index_sequence<Index...>)
        {
            return {{std::move(a[Index])...}};
        }
    }

    template <typename T, usize Count>
        requires(!std::is_array_v<T> && !std::is_same_v<T, const char>)
    [[nodiscard]] constexpr array<std::remove_cv_t<T>, Count> to_array(T (&a)[Count]) noexcept(
        std::is_nothrow_constructible_v<T, T&>)
    {
        static_assert(std::is_constructible_v<T, T&>);
        return detail::to_array_copy<Count>(a, std::make_index_sequence<Count>{});
    }

    template <typename T, usize Count>
        requires(!std::is_array_v<T> && !std::is_same_v<T, const char>)
    [[nodiscard]] constexpr array<std::remove_cv_t<T>, Count> to_array(T (&&a)[Count]) noexcept(
        std::is_nothrow_move_constructible_v<T>)
    {
        static_assert(std::is_move_constructible_v<T>);
        return detail::to_array_move<Count>(std::move(a), std::make_index_sequence<Count>{});
    }

    template <same_as<const char> ConstChar, usize N>
    [[nodiscard]] constexpr array<char, N - 1> to_array(ConstChar (&s)[N]) noexcept
    {
        return detail::to_array_copy<N - 1>(s, std::make_index_sequence<N - 1>{});
    }
}

SNN_DIAGNOSTIC_POP

// ## Specializations

// ### std::tuple_size

template <typename T, std::size_t Count>
struct std::tuple_size<snn::array<T, Count>> : public std::integral_constant<std::size_t, Count>
{
};

// ### std::tuple_element

template <std::size_t Index, typename T, std::size_t Count>
    requires(Index < Count)
struct std::tuple_element<Index, snn::array<T, Count>>
{
    using type = T;
};
