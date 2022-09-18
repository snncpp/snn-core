// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Create directory

// This is a wrapper around `mkdir(2)`, so permissions are restricted by the `umask(2)` of the
// calling process.

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/file/perm.hh"
#include "snn-core/system/error.hh"
#include <sys/stat.h> // mkdir

namespace snn::file::dir
{
    // ## Functions

    // ### create

    [[nodiscard]] inline result<void> create(const transient<null_term<const char*>> path,
                                             const perm perms = perm::directory_default) noexcept
    {
        if (::mkdir(path.get().get(), static_cast<mode_t>(perms)) != 0)
        {
            return error_code{errno, system::error_category};
        }
        return {};
    }
}
