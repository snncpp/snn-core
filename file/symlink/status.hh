// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Symbolic link or file path status

// If the file path is a symbolic link this returns information about the link,
// use `file::status()` to get information about the file the link references.

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/file/info.hh"
#include "snn-core/system/error.hh"
#include <sys/stat.h> // lstat

namespace snn::file::symlink
{
    // ## Functions

    // ### status

    [[nodiscard]] inline result<file::info> status(
        const transient<null_term<const char*>> path) noexcept
    {
        file::info info{init::do_not_initialize};
        if (::lstat(path.get().get(), &info.internal()) == 0)
        {
            return info;
        }
        return error_code{errno, system::error_category};
    }

    [[nodiscard]] inline result<void> status(const transient<null_term<const char*>> path,
                                             file::info& info) noexcept
    {
        if (::lstat(path.get().get(), &info.internal()) == 0)
        {
            return {};
        }
        return error_code{errno, system::error_category};
    }
}
