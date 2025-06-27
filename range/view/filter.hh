// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Filter with predicate

#pragma once

#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### filter

    template <input_range Rng, typename OneArgPred>
    class filter final
    {
      public:
        constexpr explicit filter(Rng rng, OneArgPred is_something)
            : rng_{std::move(rng)},
              is_something_{std::move(is_something)}
        {
            skip_to_next_match_();
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin()
        {
            if constexpr (std::is_copy_constructible_v<filter>)
            {
                return iter::forward<filter>{*this};
            }
            else
            {
                return iter::forward_reference<filter>{*this};
            }
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::forward_end{};
        }

        constexpr void drop_front(assume::not_empty_t)
        {
            rng_.drop_front(assume::not_empty);
            skip_to_next_match_();
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
        OneArgPred is_something_;

        constexpr void skip_to_next_match_()
        {
            while (rng_ && !is_something_(rng_.front(assume::not_empty)))
            {
                rng_.drop_front(assume::not_empty);
            }
        }
    };
}

namespace snn::range::v
{
    // ## Classes

    // ### filter

    template <typename OneArgPred>
    class filter final
    {
      public:
        constexpr explicit filter(OneArgPred is_something) noexcept
            : is_something_{std::move(is_something)}
        {
        }

        template <input_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng) &&
        {
            return range::view::filter{std::move(rng), std::move(is_something_)};
        }

      private:
        OneArgPred is_something_;
    };

    // ## Functions

    // ### operator|

    template <input_range Rng, typename OneArgPred>
    [[nodiscard]] constexpr auto operator|(Rng rng, filter<OneArgPred>&& f)
    {
        return std::move(f)(std::move(rng));
    }
}
