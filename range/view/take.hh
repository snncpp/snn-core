// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Take N elements from the front

#pragma once

#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### take

    template <input_range Rng>
    class take final
    {
      public:
        constexpr explicit take(Rng rng, const usize count) noexcept
            : rng_{std::move(rng)},
              count_{count}
        {
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin()
        {
            if constexpr (std::is_copy_constructible_v<take>)
            {
                return iter::forward<take>{*this};
            }
            else
            {
                return iter::forward_reference<take>{*this};
            }
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::forward_end{};
        }

        constexpr void drop_front(assume::not_empty_t)
        {
            snn_should(count_ > 0);
            rng_.drop_front(assume::not_empty);
            --count_;
        }

        [[nodiscard]] constexpr decltype(auto) front(assume::not_empty_t)
        {
            snn_should(count_ > 0);
            return rng_.front(assume::not_empty);
        }

        [[nodiscard]] constexpr bool is_empty() const
        {
            return rng_.is_empty() || count_ == 0;
        }

      private:
        Rng rng_;
        usize count_;
    };
}

namespace snn::range::v
{
    // ## Classes

    // ### take

    class take final
    {
      public:
        constexpr explicit take(const usize count) noexcept
            : count_{count}
        {
        }

        template <input_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng) noexcept
        {
            return range::view::take{std::move(rng), count_};
        }

      private:
        usize count_;
    };

    // ## Functions

    // ### operator|

    template <input_range Rng>
    [[nodiscard]] constexpr auto operator|(Rng rng, take f) noexcept
    {
        return f(std::move(rng));
    }
}
