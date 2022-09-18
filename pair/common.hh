// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// This file is generated, DO NOT EDIT MANUALLY.

// # Common pairs

// `first_last`
// `first_second`
// `index_value`
// `key_value`
// `line_column`
// `name_value`
// `value_count`

// Const members tend to break "sane" requirements (`sane` concept) and are disabled.
// Use a wrapper like `semi_const` if const-like behavior is needed.

// Does not propagate const (breaks structured binding declarations).

#pragma once

#include "snn-core/pair/core.hh"

namespace snn::pair
{
    // ## Classes

    // ### first_last

    template <typename A, typename B = A>
        requires(meta::none_v<std::is_const, A, B>)
    struct first_last final
    {
        // #### Data

        A first;
        B last;

        // #### Types

        // For `first_last` only.

        using first_type = A;
        using last_type  = B;

        // For all pairs.

        using a_type = A;
        using b_type = B;

        using trivially_relocatable_type = trivially_relocatable_if_t<first_last, A, B>;

        // #### Default constructor

        // Conditionally explicit.

        // clang-format off
        explicit(explicitly_default_constructible<A> || explicitly_default_constructible<B>)
        constexpr first_last() noexcept(meta::all_v<std::is_nothrow_default_constructible, A, B>)
            requires(meta::all_v<std::is_default_constructible, A, B>)
            : first{},
              last{}
        {
        }
        // clang-format on

        // #### Converting constructors

        constexpr first_last(A a, B b) noexcept
            : first{std::forward<A>(a)},
              last{std::forward<B>(b)}
        {
        }

        // Prevent lvalue references from binding to rvalues.

        // clang-format off
        template <typename T>
        first_last(const std::remove_reference_t<A>&&, T&&)
            requires(std::is_lvalue_reference_v<A>) = delete;

        template <typename T>
        first_last(T&&, const std::remove_reference_t<B>&&)
            requires(std::is_lvalue_reference_v<B>) = delete;
        // clang-format on

        // #### get

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() & noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A&>(first);
            else
                return std::forward<B&>(last);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A&>(first);
            else
                return std::forward<const B&>(last);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() && noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A>(first);
            else
                return std::forward<B>(last);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const&& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A>(first);
            else
                return std::forward<const B>(last);
        }
    };

    // #### Specializations

    template <typename A, typename B>
    struct is_any_strict<first_last<A, B>> : public std::true_type
    {
    };

    // ### first_second

    template <typename A, typename B = A>
        requires(meta::none_v<std::is_const, A, B>)
    struct first_second final
    {
        // #### Data

        A first;
        B second;

        // #### Types

        // For `first_second` only.

        using first_type  = A;
        using second_type = B;

        // For all pairs.

        using a_type = A;
        using b_type = B;

        using trivially_relocatable_type = trivially_relocatable_if_t<first_second, A, B>;

        // #### Default constructor

        // Conditionally explicit.

        // clang-format off
        explicit(explicitly_default_constructible<A> || explicitly_default_constructible<B>)
        constexpr first_second() noexcept(meta::all_v<std::is_nothrow_default_constructible, A, B>)
            requires(meta::all_v<std::is_default_constructible, A, B>)
            : first{},
              second{}
        {
        }
        // clang-format on

        // #### Converting constructors

        constexpr first_second(A a, B b) noexcept
            : first{std::forward<A>(a)},
              second{std::forward<B>(b)}
        {
        }

        // Prevent lvalue references from binding to rvalues.

        // clang-format off
        template <typename T>
        first_second(const std::remove_reference_t<A>&&, T&&)
            requires(std::is_lvalue_reference_v<A>) = delete;

        template <typename T>
        first_second(T&&, const std::remove_reference_t<B>&&)
            requires(std::is_lvalue_reference_v<B>) = delete;
        // clang-format on

        // #### get

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() & noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A&>(first);
            else
                return std::forward<B&>(second);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A&>(first);
            else
                return std::forward<const B&>(second);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() && noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A>(first);
            else
                return std::forward<B>(second);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const&& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A>(first);
            else
                return std::forward<const B>(second);
        }
    };

    // #### Specializations

    template <typename A, typename B>
    struct is_any_strict<first_second<A, B>> : public std::true_type
    {
    };

    // ### index_value

    template <typename A, typename B = A>
        requires(meta::none_v<std::is_const, A, B>)
    struct index_value final
    {
        // #### Data

        A index;
        B value;

        // #### Types

        // For `index_value` only.

        using index_type = A;
        using value_type = B;

        // For all pairs.

        using a_type = A;
        using b_type = B;

        using trivially_relocatable_type = trivially_relocatable_if_t<index_value, A, B>;

        // #### Default constructor

        // Conditionally explicit.

        // clang-format off
        explicit(explicitly_default_constructible<A> || explicitly_default_constructible<B>)
        constexpr index_value() noexcept(meta::all_v<std::is_nothrow_default_constructible, A, B>)
            requires(meta::all_v<std::is_default_constructible, A, B>)
            : index{},
              value{}
        {
        }
        // clang-format on

        // #### Converting constructors

        constexpr index_value(A a, B b) noexcept
            : index{std::forward<A>(a)},
              value{std::forward<B>(b)}
        {
        }

        // Prevent lvalue references from binding to rvalues.

        // clang-format off
        template <typename T>
        index_value(const std::remove_reference_t<A>&&, T&&)
            requires(std::is_lvalue_reference_v<A>) = delete;

        template <typename T>
        index_value(T&&, const std::remove_reference_t<B>&&)
            requires(std::is_lvalue_reference_v<B>) = delete;
        // clang-format on

        // #### get

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() & noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A&>(index);
            else
                return std::forward<B&>(value);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A&>(index);
            else
                return std::forward<const B&>(value);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() && noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A>(index);
            else
                return std::forward<B>(value);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const&& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A>(index);
            else
                return std::forward<const B>(value);
        }
    };

    // #### Specializations

    template <typename A, typename B>
    struct is_any_strict<index_value<A, B>> : public std::true_type
    {
    };

    // ### key_value

    template <typename A, typename B = A>
        requires(meta::none_v<std::is_const, A, B>)
    struct key_value final
    {
        // #### Data

        A key;
        B value;

        // #### Types

        // For `key_value` only.

        using key_type   = A;
        using value_type = B;

        // For all pairs.

        using a_type = A;
        using b_type = B;

        using trivially_relocatable_type = trivially_relocatable_if_t<key_value, A, B>;

        // #### Default constructor

        // Conditionally explicit.

        // clang-format off
        explicit(explicitly_default_constructible<A> || explicitly_default_constructible<B>)
        constexpr key_value() noexcept(meta::all_v<std::is_nothrow_default_constructible, A, B>)
            requires(meta::all_v<std::is_default_constructible, A, B>)
            : key{},
              value{}
        {
        }
        // clang-format on

        // #### Converting constructors

        constexpr key_value(A a, B b) noexcept
            : key{std::forward<A>(a)},
              value{std::forward<B>(b)}
        {
        }

        // Prevent lvalue references from binding to rvalues.

        // clang-format off
        template <typename T>
        key_value(const std::remove_reference_t<A>&&, T&&)
            requires(std::is_lvalue_reference_v<A>) = delete;

        template <typename T>
        key_value(T&&, const std::remove_reference_t<B>&&)
            requires(std::is_lvalue_reference_v<B>) = delete;
        // clang-format on

        // #### get

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() & noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A&>(key);
            else
                return std::forward<B&>(value);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A&>(key);
            else
                return std::forward<const B&>(value);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() && noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A>(key);
            else
                return std::forward<B>(value);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const&& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A>(key);
            else
                return std::forward<const B>(value);
        }
    };

    // #### Specializations

    template <typename A, typename B>
    struct is_any_strict<key_value<A, B>> : public std::true_type
    {
    };

    // ### line_column

    template <typename A, typename B = A>
        requires(meta::none_v<std::is_const, A, B>)
    struct line_column final
    {
        // #### Data

        A line;
        B column;

        // #### Types

        // For `line_column` only.

        using line_type   = A;
        using column_type = B;

        // For all pairs.

        using a_type = A;
        using b_type = B;

        using trivially_relocatable_type = trivially_relocatable_if_t<line_column, A, B>;

        // #### Default constructor

        // Conditionally explicit.

        // clang-format off
        explicit(explicitly_default_constructible<A> || explicitly_default_constructible<B>)
        constexpr line_column() noexcept(meta::all_v<std::is_nothrow_default_constructible, A, B>)
            requires(meta::all_v<std::is_default_constructible, A, B>)
            : line{},
              column{}
        {
        }
        // clang-format on

        // #### Converting constructors

        constexpr line_column(A a, B b) noexcept
            : line{std::forward<A>(a)},
              column{std::forward<B>(b)}
        {
        }

        // Prevent lvalue references from binding to rvalues.

        // clang-format off
        template <typename T>
        line_column(const std::remove_reference_t<A>&&, T&&)
            requires(std::is_lvalue_reference_v<A>) = delete;

        template <typename T>
        line_column(T&&, const std::remove_reference_t<B>&&)
            requires(std::is_lvalue_reference_v<B>) = delete;
        // clang-format on

        // #### get

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() & noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A&>(line);
            else
                return std::forward<B&>(column);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A&>(line);
            else
                return std::forward<const B&>(column);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() && noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A>(line);
            else
                return std::forward<B>(column);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const&& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A>(line);
            else
                return std::forward<const B>(column);
        }
    };

    // #### Specializations

    template <typename A, typename B>
    struct is_any_strict<line_column<A, B>> : public std::true_type
    {
    };

    // ### name_value

    template <typename A, typename B = A>
        requires(meta::none_v<std::is_const, A, B>)
    struct name_value final
    {
        // #### Data

        A name;
        B value;

        // #### Types

        // For `name_value` only.

        using name_type  = A;
        using value_type = B;

        // For all pairs.

        using a_type = A;
        using b_type = B;

        using trivially_relocatable_type = trivially_relocatable_if_t<name_value, A, B>;

        // #### Default constructor

        // Conditionally explicit.

        // clang-format off
        explicit(explicitly_default_constructible<A> || explicitly_default_constructible<B>)
        constexpr name_value() noexcept(meta::all_v<std::is_nothrow_default_constructible, A, B>)
            requires(meta::all_v<std::is_default_constructible, A, B>)
            : name{},
              value{}
        {
        }
        // clang-format on

        // #### Converting constructors

        constexpr name_value(A a, B b) noexcept
            : name{std::forward<A>(a)},
              value{std::forward<B>(b)}
        {
        }

        // Prevent lvalue references from binding to rvalues.

        // clang-format off
        template <typename T>
        name_value(const std::remove_reference_t<A>&&, T&&)
            requires(std::is_lvalue_reference_v<A>) = delete;

        template <typename T>
        name_value(T&&, const std::remove_reference_t<B>&&)
            requires(std::is_lvalue_reference_v<B>) = delete;
        // clang-format on

        // #### get

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() & noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A&>(name);
            else
                return std::forward<B&>(value);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A&>(name);
            else
                return std::forward<const B&>(value);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() && noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A>(name);
            else
                return std::forward<B>(value);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const&& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A>(name);
            else
                return std::forward<const B>(value);
        }
    };

    // #### Specializations

    template <typename A, typename B>
    struct is_any_strict<name_value<A, B>> : public std::true_type
    {
    };

    // ### value_count

    template <typename A, typename B = A>
        requires(meta::none_v<std::is_const, A, B>)
    struct value_count final
    {
        // #### Data

        A value;
        B count;

        // #### Types

        // For `value_count` only.

        using value_type = A;
        using count_type = B;

        // For all pairs.

        using a_type = A;
        using b_type = B;

        using trivially_relocatable_type = trivially_relocatable_if_t<value_count, A, B>;

        // #### Default constructor

        // Conditionally explicit.

        // clang-format off
        explicit(explicitly_default_constructible<A> || explicitly_default_constructible<B>)
        constexpr value_count() noexcept(meta::all_v<std::is_nothrow_default_constructible, A, B>)
            requires(meta::all_v<std::is_default_constructible, A, B>)
            : value{},
              count{}
        {
        }
        // clang-format on

        // #### Converting constructors

        constexpr value_count(A a, B b) noexcept
            : value{std::forward<A>(a)},
              count{std::forward<B>(b)}
        {
        }

        // Prevent lvalue references from binding to rvalues.

        // clang-format off
        template <typename T>
        value_count(const std::remove_reference_t<A>&&, T&&)
            requires(std::is_lvalue_reference_v<A>) = delete;

        template <typename T>
        value_count(T&&, const std::remove_reference_t<B>&&)
            requires(std::is_lvalue_reference_v<B>) = delete;
        // clang-format on

        // #### get

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() & noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A&>(value);
            else
                return std::forward<B&>(count);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A&>(value);
            else
                return std::forward<const B&>(count);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() && noexcept
        {
            if constexpr (Index == 0)
                return std::forward<A>(value);
            else
                return std::forward<B>(count);
        }

        template <usize Index>
            requires(Index < 2)
        [[nodiscard]] constexpr decltype(auto) get() const&& noexcept
        {
            if constexpr (Index == 0)
                return std::forward<const A>(value);
            else
                return std::forward<const B>(count);
        }
    };

    // #### Specializations

    template <typename A, typename B>
    struct is_any_strict<value_count<A, B>> : public std::true_type
    {
    };
}
