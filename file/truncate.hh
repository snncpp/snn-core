// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Truncate/extend file to a specified size (zero by default)

// The file read/write offset is not changed.

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/system/error.hh"
#include <cerrno>   // errno, E*
#include <unistd.h> // ftruncate, truncate

namespace snn::file
{
    // ## Functions

    // ### truncate

    // Truncate file via path.

    [[nodiscard]] inline result<void> truncate(const transient<null_term<const char*>> path,
                                               const usize size = 0) noexcept
    {
        if (size <= constant::limit<isize>::max)
        {
            if (::truncate(path.get().get(), to_isize(size)) == 0)
            {
                return {};
            }
            return error_code{errno, system::error_category};
        }
        return error_code{EINVAL, system::error_category};
    }

    // Truncate file via descriptor.

    [[nodiscard]] inline result<void> truncate(const int fd, const usize size = 0) noexcept
    {
        if (size <= constant::limit<isize>::max)
        {
            if (::ftruncate(fd, to_isize(size)) == 0)
            {
                return {};
            }
            return error_code{errno, system::error_category};
        }
        return error_code{EINVAL, system::error_category};
    }
}
