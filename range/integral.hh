// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Range of all values for an integral type

// Bidirectional range that produces all values for an integral type `Int` from
// `constant::limit<Int>::min` to `constant::limit<Int>::max` (inclusive).

// `Int` must be 32-bit or less.

#pragma once

#include "snn-core/range/iter/forward.hh"

namespace snn::range
{
    // ## Classes

    // ### integral

    namespace detail::integral
    {
        template <typename Int, usize Size = sizeof(Int)>
        struct counter
        {
            // No type member, compilation will fail if this is selected.
        };

        template <typename Int>
        struct counter<Int, 1>
        {
            using type = i16fast;
        };

        template <typename Int>
        struct counter<Int, 2>
        {
            using type = i32fast;
        };

        template <typename Int>
        struct counter<Int, 4>
        {
            using type = i64;
        };
    }

    template <strict_integral Int>
        requires(sizeof(Int) <= sizeof(i32))
    class integral final
    {
      public:
        // #### Types

        using counter_type = typename detail::integral::counter<Int>::type;

        // #### Constructors

        constexpr explicit integral() noexcept
            : front_{constant::limit<Int>::min},
              back_{constant::limit<Int>::max}
        {
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return !is_empty();
        }

        // #### Iterators

        [[nodiscard]] constexpr auto begin() const noexcept
        {
            return iter::forward{*this};
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::forward_end{};
        }

        // #### Single element access

        [[nodiscard]] constexpr Int front(promise::not_empty_t) const noexcept
        {
            snn_should(!is_empty());
            return static_cast<Int>(front_);
        }

        [[nodiscard]] constexpr Int back(promise::not_empty_t) const noexcept
        {
            snn_should(!is_empty());
            return static_cast<Int>(back_);
        }

        // #### Operations

        constexpr void drop_front(promise::not_empty_t) noexcept
        {
            snn_should(!is_empty());
            ++front_;
        }

        constexpr void drop_back(promise::not_empty_t) noexcept
        {
            snn_should(!is_empty());
            --back_;
        }

        // #### Status

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return front_ > back_;
        }

      private:
        counter_type front_;
        counter_type back_;
    };
}
