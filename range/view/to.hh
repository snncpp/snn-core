// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Return `.to<To>()` or `To{...}`

// Call `.to<To>()` if available or return `To{...}` if not.

#pragma once

#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### to

    template <typename To, input_range Rng>
    class to final
    {
      public:
        constexpr explicit to(Rng rng) noexcept
            : rng_{std::move(rng)}
        {
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin()
        {
            if constexpr (std::is_copy_constructible_v<to>)
            {
                return iter::forward<to>{*this};
            }
            else
            {
                return iter::forward_reference<to>{*this};
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
            if constexpr (has_to<To, decltype(rng_.front(promise::not_empty))>)
            {
                return rng_.front(promise::not_empty).template to<To>();
            }
            else
            {
                return To{rng_.front(promise::not_empty)};
            }
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

    // ### to

    template <typename To>
    class to final
    {
      public:
        explicit to() = default;

        template <input_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng) noexcept
        {
            return range::view::to<To, Rng>{std::move(rng)};
        }
    };

    // ## Functions

    // ### operator|

    template <input_range Rng, typename To>
    [[nodiscard]] constexpr auto operator|(Rng rng, to<To> f) noexcept
    {
        return f(std::move(rng));
    }
}
