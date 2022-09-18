// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Touch a regular file

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/file/info.hh"
#include "snn-core/file/option.hh"
#include "snn-core/file/perm.hh"
#include "snn-core/file/reader_writer.hh"
#include "snn-core/file/io/blocking.hh"
#include "snn-core/system/error.hh"
#include <cerrno>     // errno, E*
#include <sys/stat.h> // futimens

namespace snn::file
{
    // ## Functions

    // ### touch

    // Touch a regular file (update access and modification times), the file is created by default.

    template <typename Io = io::blocking>
    [[nodiscard]] result<void> touch(const transient<null_term<const char*>> path,
                                     const option options = option::create,
                                     const perm perms     = perm::regular_default,
                                     Io io                = Io{}) noexcept
    {
        file::reader_writer<Io> rw{std::move(io)};

        if (const auto res = rw.open_for_writing(path, options, perms); !res)
        {
            return res.error_code();
        }

        file::info info{container::do_not_initialize};

        if (const auto res = rw.status(info); !res)
        {
            return res.error_code();
        }

        if (!info.is_regular())
        {
            return error_code{EINVAL, system::error_category};
        }

        if (::futimens(rw.descriptor().value_or(-1), nullptr) != 0)
        {
            return error_code{errno, system::error_category};
        }

        return rw.close();
    }
}
