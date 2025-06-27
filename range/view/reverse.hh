// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Reverse a range

#pragma once

#include "snn-core/range/iter/reverse.hh"

namespace snn::range::view
{
    // ## Classes

    // ### reverse

    template <bidirectional_range Rng>
    class reverse final
    {
      public:
        constexpr explicit reverse(Rng rng) noexcept
            : rng_{std::move(rng)}
        {
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin() const
        {
            return iter::reverse{rng_};
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::reverse_end{};
        }

        constexpr void drop_front(assume::not_empty_t)
        {
            rng_.drop_back(assume::not_empty);
        }

        [[nodiscard]] constexpr decltype(auto) front(assume::not_empty_t)
        {
            return rng_.back(assume::not_empty);
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

    // ### reverse

    struct reverse final
    {
        explicit reverse() = default;

        template <bidirectional_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng) noexcept
        {
            return range::view::reverse{std::move(rng)};
        }
    };

    // ## Functions

    // ### operator|

    template <bidirectional_range Rng>
    [[nodiscard]] constexpr auto operator|(Rng rng, reverse f) noexcept
    {
        return f(std::move(rng));
    }
}
