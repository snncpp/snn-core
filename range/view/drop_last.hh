// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Drop N elements from the back

#pragma once

#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### drop_last

    template <bidirectional_range Rng>
    class drop_last final
    {
      public:
        constexpr explicit drop_last(Rng rng, usize count)
            : rng_{std::move(rng)}
        {
            while (count > 0 && rng_)
            {
                rng_.drop_back(assume::not_empty);
                --count;
            }
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin()
        {
            return iter::forward<drop_last>{*this};
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::forward_end{};
        }

        constexpr void drop_back(assume::not_empty_t)
        {
            rng_.drop_back(assume::not_empty);
        }

        constexpr void drop_front(assume::not_empty_t)
        {
            rng_.drop_front(assume::not_empty);
        }

        [[nodiscard]] constexpr decltype(auto) back(assume::not_empty_t)
        {
            return rng_.back(assume::not_empty);
        }

        [[nodiscard]] constexpr decltype(auto) front(assume::not_empty_t)
        {
            return rng_.front(assume::not_empty);
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

    // ### drop_last

    class drop_last final
    {
      public:
        constexpr explicit drop_last(const usize count) noexcept
            : count_{count}
        {
        }

        template <bidirectional_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng)
        {
            return range::view::drop_last{std::move(rng), count_};
        }

      private:
        usize count_;
    };

    // ## Functions

    // ### operator|

    template <bidirectional_range Rng>
    [[nodiscard]] constexpr auto operator|(Rng rng, drop_last f)
    {
        return f(std::move(rng));
    }
}
