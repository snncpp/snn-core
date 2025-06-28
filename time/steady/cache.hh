// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Cache

#pragma once

#include "snn-core/time/steady/since_boot.hh"

namespace snn::time::steady
{
    // ## Classes

    // ### `cache`

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
            return d_.to_milliseconds<i64>(assume::not_negative);
        }

        [[nodiscard]] i64 seconds() const noexcept
        {
            return d_.to_seconds<i64>(assume::not_negative);
        }

        void update() noexcept
        {
            d_ = time::steady::since_boot();
        }

      private:
        time::duration d_;
    };
}
