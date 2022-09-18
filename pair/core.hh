// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Core functionality

#pragma once

#include "snn-core/core.hh"

namespace snn::pair
{
    // ## Type traits

    // ### is_any

    // `is_any_strict<...>` must be specialized for each pair type.

    template <typename>
    struct is_any_strict : public std::false_type
    {
    };

    template <typename T>
    struct is_any : public is_any_strict<std::remove_cv_t<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_any_v = is_any<T>::value;

    // ## Concepts

    // ### any

    template <typename T>
    concept any = is_any_v<T>;

    // ## Functions

    // ### get

    template <usize Index, typename Pair>
        requires(Index < 2 && any<std::remove_reference_t<Pair>>)
    [[nodiscard]] constexpr decltype(auto) get(Pair&& p) noexcept
    {
        return std::forward<Pair>(p).template get<Index>();
    }

    // ### swap

    template <any Pair>
    constexpr void swap(Pair& x, Pair& y) noexcept(
        meta::all_v<std::is_nothrow_swappable, typename Pair::a_type, typename Pair::b_type>)
    {
        using std::swap;
        swap(x.template get<0>(), y.template get<0>());
        swap(x.template get<1>(), y.template get<1>());
    }

    // ### Comparison operators

    template <any LeftPair, any RightPair>
    constexpr bool operator==(const LeftPair& left, const RightPair& right)
    {
        return left.template get<0>() == right.template get<0>() &&
               left.template get<1>() == right.template get<1>();
    }

    template <any LeftPair, any RightPair>
    constexpr auto operator<=>(const LeftPair& left, const RightPair& right)
        -> std::common_comparison_category_t<
            decltype(left.template get<0>() <=> right.template get<0>()),
            decltype(left.template get<1>() <=> right.template get<1>())>
    {
        if (const auto cmp = left.template get<0>() <=> right.template get<0>(); cmp != 0)
        {
            return cmp;
        }
        return left.template get<1>() <=> right.template get<1>();
    }

    namespace detail
    {
        // For `std::tuple_element`.

        template <usize Index, typename A, typename B>
            requires(Index < 2)
        struct member_type
        {
        };

        template <typename A, typename B>
        struct member_type<0, A, B>
        {
            using type = A;
        };

        template <typename A, typename B>
        struct member_type<1, A, B>
        {
            using type = B;
        };
    }
}

// ## Specializations

// ### std::tuple_element

template <std::size_t Index, snn::pair::any Pair>
struct std::tuple_element<Index, Pair>
    : public snn::pair::detail::member_type<Index, typename Pair::a_type, typename Pair::b_type>
{
};

// ### std::tuple_size

template <snn::pair::any Pair>
struct std::tuple_size<Pair> : public std::integral_constant<std::size_t, 2>
{
};
