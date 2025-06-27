// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Return `op(...)`

#pragma once

#include "snn-core/range/iter/forward.hh"

namespace snn::range::view
{
    // ## Classes

    // ### transform

    template <input_range Rng, typename OneArgOp>
    class transform final
    {
      public:
        constexpr explicit transform(Rng rng, OneArgOp op) noexcept
            : rng_{std::move(rng)},
              op_{std::move(op)}
        {
        }

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin()
        {
            if constexpr (std::is_copy_constructible_v<transform>)
            {
                return iter::forward<transform>{*this};
            }
            else
            {
                return iter::forward_reference<transform>{*this};
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
            return op_(rng_.front(assume::not_empty));
        }

        [[nodiscard]] constexpr bool is_empty() const
        {
            return rng_.is_empty();
        }

      private:
        Rng rng_;
        OneArgOp op_;
    };
}

namespace snn::range::v
{
    // ## Classes

    // ### transform

    template <typename OneArgOp>
    class transform final
    {
      public:
        constexpr explicit transform(OneArgOp op) noexcept
            : op_{std::move(op)}
        {
        }

        template <input_range Rng>
        [[nodiscard]] constexpr auto operator()(Rng rng) && noexcept
        {
            return range::view::transform{std::move(rng), std::move(op_)};
        }

      private:
        OneArgOp op_;
    };

    // ## Functions

    // ### operator|

    template <input_range Rng, typename OneArgOp>
    [[nodiscard]] constexpr auto operator|(Rng rng, transform<OneArgOp>&& f) noexcept
    {
        return std::move(f)(std::move(rng));
    }
}
