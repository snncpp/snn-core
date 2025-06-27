// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Return `.to_prefix<To>()`

// Call `.to_prefix<To>()` on the underlying range's `front()` return value.

#pragma once

#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### to_prefix

    template <typename To, input_range Rng>
    class to_prefix final
    {
      public:
        constexpr explicit to_prefix(Rng rng) noexcept
            : rng_{std::move(rng)}
        {
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin()
        {
            if constexpr (std::is_copy_constructible_v<to_prefix>)
            {
                return iter::forward<to_prefix>{*this};
            }
            else
            {
                return iter::forward_reference<to_prefix>{*this};
            }
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::forward_end{};
        }

        constexpr void drop_front(assume::not_empty_t)
        {
            rng_.drop_front(assume::not_empty);
        }

        [[nodiscard]] constexpr auto front(assume::not_empty_t)
        {
            return rng_.front(assume::not_empty).template to_prefix<To>();
        }

        [[nodiscard]] constexpr bool is_empty() const
        {
            return rng_.is_empty();
        }

      private:
        Rng rng_;
    };
}

namespace snn::range::v
{
    // ## Classes

    // ### to_prefix

    template <typename To>
    class to_prefix final
    {
      public:
        explicit to_prefix() = default;

        template <input_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng) noexcept
        {
            return range::view::to_prefix<To, Rng>{std::move(rng)};
        }
    };

    // ## Functions

    // ### operator|

    template <input_range Rng, typename To>
    [[nodiscard]] constexpr auto operator|(Rng rng, to_prefix<To> f) noexcept
    {
        return f(std::move(rng));
    }
}
