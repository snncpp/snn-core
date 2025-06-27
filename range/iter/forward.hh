// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Forward iterator

#pragma once

#include "snn-core/core.hh"

namespace snn::range::iter
{
    // ## Classes

    // ### forward_end

    struct forward_end final
    {
        explicit forward_end() = default;
    };

    // ### forward

    template <typename Range>
    class forward final
    {
      public:
        constexpr explicit forward(Range rng) noexcept
            : rng_{std::move(rng)}
        {
        }

        constexpr decltype(auto) operator*()
        {
            return rng_.front(assume::not_empty);
        }

        constexpr forward& operator++()
        {
            rng_.drop_front(assume::not_empty);
            return *this;
        }

        constexpr bool operator!=(iter::forward_end) const
        {
            return !rng_.is_empty();
        }

      private:
        Range rng_;
    };

    // ### forward_reference

    template <typename Range>
    class forward_reference final
    {
      public:
        constexpr explicit forward_reference(Range& rng) noexcept
            : rng_{rng}
        {
            static_assert(!std::is_const_v<Range>);
        }

        forward_reference(const Range&&) = delete;

        constexpr decltype(auto) operator*()
        {
            return rng_.front(assume::not_empty);
        }

        constexpr forward_reference& operator++()
        {
            rng_.drop_front(assume::not_empty);
            return *this;
        }

        constexpr bool operator!=(iter::forward_end) const
        {
            return !rng_.is_empty();
        }

      private:
        Range& rng_;
    };
}
