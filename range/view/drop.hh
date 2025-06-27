// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Drop N elements from the front

#pragma once

#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### drop

    template <input_range Rng>
    class drop final
    {
      public:
        constexpr explicit drop(Rng rng, usize count)
            : rng_{std::move(rng)}
        {
            while (count > 0 && rng_)
            {
                rng_.drop_front(assume::not_empty);
                --count;
            }
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin()
        {
            if constexpr (std::is_copy_constructible_v<drop>)
            {
                return iter::forward<drop>{*this};
            }
            else
            {
                return iter::forward_reference<drop>{*this};
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

    // ### drop

    class drop final
    {
      public:
        constexpr explicit drop(const usize count) noexcept
            : count_{count}
        {
        }

        template <input_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng)
        {
            return range::view::drop{std::move(rng), count_};
        }

      private:
        usize count_;
    };

    // ## Functions

    // ### operator|

    template <input_range Rng>
    [[nodiscard]] constexpr auto operator|(Rng rng, drop f)
    {
        return f(std::move(rng));
    }
}
