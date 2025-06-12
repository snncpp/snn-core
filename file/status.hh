// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Get information about a file path or a file descriptor

// If the file path is a symbolic link this returns information about the file the link references,
// use `file::symlink::status()` to get information about the link.

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/file/info.hh"
#include "snn-core/system/error.hh"
#include <sys/stat.h> // fstat, stat

namespace snn::file
{
    // ## File descriptor

    [[nodiscard]] inline result<file::info> status(const int fd) noexcept
    {
        file::info info{init::do_not_initialize};
        if (::fstat(fd, &info.internal()) == 0)
        {
            return info;
        }
        return error_code{errno, system::error_category};
    }

    [[nodiscard]] inline result<void> status(const int fd, file::info& info) noexcept
    {
        if (::fstat(fd, &info.internal()) == 0)
        {
            return {};
        }
        return error_code{errno, system::error_category};
    }

    // ## Path

    [[nodiscard]] inline result<file::info> status(
        const transient<null_term<const char*>> path) noexcept
    {
        file::info info{init::do_not_initialize};
        if (::stat(path.get().get(), &info.internal()) == 0)
        {
            return info;
        }
        return error_code{errno, system::error_category};
    }

    [[nodiscard]] inline result<void> status(const transient<null_term<const char*>> path,
                                             file::info& info) noexcept
    {
        if (::stat(path.get().get(), &info.internal()) == 0)
        {
            return {};
        }
        return error_code{errno, system::error_category};
    }
}
