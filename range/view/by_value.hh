// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Return by-value

// `front()` will return by value even if the underlying range returns by reference.

#pragma once

#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### by_value

    template <input_range Rng>
    class by_value final
    {
      public:
        constexpr explicit by_value(Rng rng) noexcept
            : rng_{std::move(rng)}
        {
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin()
        {
            if constexpr (std::is_copy_constructible_v<by_value>)
            {
                return iter::forward<by_value>{*this};
            }
            else
            {
                return iter::forward_reference<by_value>{*this};
            }
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::forward_end{};
        }

        constexpr void drop_front(promise::not_empty_t)
        {
            rng_.drop_front(promise::not_empty);
        }

        [[nodiscard]] constexpr auto front(promise::not_empty_t)
        {
            return rng_.front(promise::not_empty);
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

    // ### by_value

    class by_value final
    {
      public:
        explicit by_value() = default;

        template <input_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng) noexcept
        {
            return range::view::by_value<Rng>{std::move(rng)};
        }
    };

    // ## Functions

    // ### operator|

    template <input_range Rng>
    [[nodiscard]] constexpr auto operator|(Rng rng, by_value f) noexcept
    {
        return f(std::move(rng));
    }
}
