// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Sleep for a duration

#pragma once

#include "snn-core/result.hh"
#include "snn-core/system/error.hh"
#include "snn-core/time/duration.hh"
#include <cerrno> // EINTR
#include <time.h> // clock_nanosleep, timespec, CLOCK_MONOTONIC

namespace snn::thread
{
    // ## Functions

    // ### sleep_for

    [[nodiscard]] inline result<void> sleep_for(const time::duration d) noexcept
    {
        ::timespec ts{.tv_sec = d.seconds(), .tv_nsec = to_i32(d.nanoseconds())};
        while (true)
        {
            // `clock_nanosleep()` does not use errno, it returns the error (if any) directly.
            // If interrupted, the `timespec` struct has been updated.
            const int ret = ::clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, &ts);
            if (ret == 0)
            {
                break;
            }
            if (ret != EINTR)
            {
                return error_code{ret, system::error_category};
            }
        }
        return {};
    }
}
