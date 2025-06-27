// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Since boot

#pragma once

#include "snn-core/time/duration.hh"
#include <time.h> // clock_gettime, timespec, CLOCK_BOOTTIME

namespace snn::time::steady
{
    // # Functions

    // ## since_boot

    [[nodiscard]] inline time::duration since_boot() noexcept
    {
        struct timespec ts;
        const int ret = ::clock_gettime(CLOCK_BOOTTIME, &ts);
        snn_assert(ret == 0); // Should never fail with valid parameters.
        ignore_if_unused(ret);

        // About the `timespec` struct:
        // "The tv_nsec member is only valid if greater than or equal to zero, and less than the
        // number of nanoseconds in a second (1000 million). The time interval described by this
        // structure is (tv_sec * 1^9 + tv_nsec) nanoseconds."
        // https://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html

        return time::duration{ts.tv_sec, static_cast<u32>(ts.tv_nsec), assume::is_valid};
    }
}
