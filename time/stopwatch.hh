// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Stopwatch

#pragma once

#include "snn-core/time/steady/since_boot.hh"

namespace snn::time
{
    // ## Classes

    // ### `stopwatch`

    class stopwatch final
    {
      public:
        explicit stopwatch() noexcept
            : start_{time::steady::since_boot()}
        {
        }

        [[nodiscard]] time::duration duration() const noexcept
        {
            return time::steady::since_boot() - start_;
        }

        [[nodiscard]] i64 microseconds() const noexcept
        {
            // This will not overflow for thousands of years.
            return duration().to_microseconds<i64>(assume::not_negative);
        }

        [[nodiscard]] i64 milliseconds() const noexcept
        {
            // This will not overflow for millions of years.
            return duration().to_milliseconds<i64>(assume::not_negative);
        }

        [[nodiscard]] i64 nanoseconds() const noexcept
        {
            // This will not overflow for hundreds of years.
            return duration().to_nanoseconds<i64>(assume::not_negative);
        }

        void reset() noexcept
        {
            start_ = time::steady::since_boot();
        }

      private:
        time::duration start_;
    };
}
