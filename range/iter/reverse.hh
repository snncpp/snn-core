// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Reverse iterator

#pragma once

#include "snn-core/core.hh"

namespace snn::range::iter
{
    // ## Classes

    // ### reverse_end

    struct reverse_end final
    {
        explicit reverse_end() = default;
    };

    // ### reverse

    template <typename Range>
    class reverse final
    {
      public:
        constexpr explicit reverse(Range rng) noexcept
            : rng_{std::move(rng)}
        {
        }

        constexpr decltype(auto) operator*()
        {
            return rng_.back(promise::not_empty);
        }

        constexpr reverse& operator++()
        {
            rng_.drop_back(promise::not_empty);
            return *this;
        }

        constexpr bool operator!=(iter::reverse_end) const
        {
            return !rng_.is_empty();
        }

      private:
        Range rng_;
    };
}
