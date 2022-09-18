// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Return `.value()`

// Calls `.value()` on the underlying range's `front()` return value.

// Rvalue references will be dropped. For example if `.value()` returns `int&&` then this range will
// return `int`.

#pragma once

#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### value

    template <input_range Rng>
    class value final
    {
      private:
        Rng rng_;

      public:
        constexpr explicit value(Rng rng) noexcept
            : rng_{std::move(rng)}
        {
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin()
        {
            if constexpr (std::is_copy_constructible_v<value>)
            {
                return iter::forward<value>{*this};
            }
            else
            {
                return iter::forward_reference<value>{*this};
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
            -> remove_cv_rvalue_ref_t<decltype(rng_.front(promise::not_empty).value())>
        {
            return rng_.front(promise::not_empty).value();
        }

        [[nodiscard]] constexpr bool is_empty() const
        {
            return rng_.is_empty();
        }
    };
}

namespace snn::range::v
{
    // ## Classes

    // ### value

    class value final
    {
      public:
        explicit value() = default;

        template <input_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng) noexcept
        {
            return range::view::value<Rng>{std::move(rng)};
        }
    };

    // ## Functions

    // ### operator|

    template <input_range Rng>
    [[nodiscard]] constexpr auto operator|(Rng rng, value f) noexcept
    {
        return f(std::move(rng));
    }
}
