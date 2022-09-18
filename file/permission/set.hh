// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Set permissions for path

// Sets the permission bits for a path name with `chmod(2)`.

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/file/perm.hh"
#include "snn-core/system/error.hh"
#include <cerrno>     // errno
#include <sys/stat.h> // chmod

namespace snn::file::permission
{
    // ## Functions

    // ### set

    [[nodiscard]] inline result<void> set(const transient<null_term<const char*>> path,
                                          const file::perm perms) noexcept
    {
        if (::chmod(path.get().get(), static_cast<mode_t>(perms)) != 0)
        {
            return error_code{errno, system::error_category};
        }
        return {};
    }
}
