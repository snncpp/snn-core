// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Return `std::move(...)`

// Call `std::move(...)` on the underlying range's `front()` return value.

#pragma once

#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### move

    template <input_range Rng>
    class move final
    {
      public:
        constexpr explicit move(Rng rng) noexcept
            : rng_{std::move(rng)}
        {
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin()
        {
            if constexpr (std::is_copy_constructible_v<move>)
            {
                return iter::forward<move>{*this};
            }
            else
            {
                return iter::forward_reference<move>{*this};
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

        [[nodiscard]] constexpr decltype(auto) front(promise::not_empty_t)
        {
            static_assert(std::is_reference_v<decltype(rng_.front(promise::not_empty))>,
                          "Can't return a reference to a temporary.");
            return std::move(rng_.front(promise::not_empty));
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

    // ### move

    class move final
    {
      public:
        explicit move() = default;

        template <input_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng) noexcept
        {
            return range::view::move<Rng>{std::move(rng)};
        }
    };

    // ## Functions

    // ### operator|

    template <input_range Rng>
    [[nodiscard]] constexpr auto operator|(Rng rng, move f) noexcept
    {
        return f(std::move(rng));
    }
}
