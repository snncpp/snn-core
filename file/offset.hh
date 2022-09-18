// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Get current file descriptor offset

#pragma once

#include "snn-core/result.hh"
#include "snn-core/system/error.hh"
#include <cerrno>   // errno
#include <unistd.h> // lseek, SEEK_CUR

namespace snn::file
{
    // ## Functions

    // ### offset

    [[nodiscard]] inline result<usize> offset(const int fd) noexcept
    {
        const isize offs = ::lseek(fd, 0, SEEK_CUR);
        if (offs >= 0)
        {
            return to_usize(offs);
        }
        return error_code{errno, system::error_category};
    }
}
