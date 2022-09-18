// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Stopwatch

#pragma once

#include "snn-core/time/steady/since_boot.hh"

namespace snn::time
{
    // ## Classes

    // ### stopwatch

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
            return nanoseconds() / 1'000;
        }

        [[nodiscard]] i64 milliseconds() const noexcept
        {
            return nanoseconds() / 1'000'000;
        }

        [[nodiscard]] i64 nanoseconds() const noexcept
        {
            const auto d = duration();
            return (d.seconds() * 1'000'000'000) + d.nanoseconds();
        }

        void reset() noexcept
        {
            start_ = time::steady::since_boot();
        }

      private:
        time::duration start_;
    };
}
