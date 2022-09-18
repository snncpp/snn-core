// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Cache

#pragma once

#include "snn-core/time/steady/since_boot.hh"

namespace snn::time::steady
{
    // ## Classes

    // ### cache

    class cache final
    {
      public:
        explicit cache() noexcept
            : d_{time::steady::since_boot()}
        {
        }

        [[nodiscard]] time::duration get() const noexcept
        {
            return d_;
        }

        [[nodiscard]] i64 milliseconds() const noexcept
        {
            // This will not overflow for millions of years.
            return (d_.seconds() * 1'000) + (d_.nanoseconds() / 1'000'000);
        }

        [[nodiscard]] i64 seconds() const noexcept
        {
            return d_.seconds();
        }

        void update() noexcept
        {
            d_ = time::steady::since_boot();
        }

      private:
        time::duration d_;
    };
}
