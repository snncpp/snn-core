// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Page size

#pragma once

#include "snn-core/core.hh"
#include <unistd.h> // _SC_PAGESIZE, sysconf

namespace snn::system
{
    namespace detail
    {
        inline not_zero<usize> page_size_or_trap() noexcept
        {
            // https://man7.org/linux/man-pages/man2/getpagesize.2.html#NOTES
            // "Portable applications should employ `sysconf(_SC_PAGESIZE)` instead of
            // `getpagesize()`."

            const long page_size = ::sysconf(_SC_PAGESIZE); // Returns -1 on error.
            if (page_size > 0)
            {
                return not_zero{static_cast<usize>(page_size)};
            }
            __builtin_trap();
        }
    }

    // ## Functions

    // ### page_size

    [[nodiscard]] inline not_zero<usize> page_size() noexcept
    {
        static const not_zero<usize> i = detail::page_size_or_trap();
        return i;
    }
}
