// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Read an entire regular file

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/strcore.hh"
#include "snn-core/file/info.hh"
#include "snn-core/file/option.hh"
#include "snn-core/file/reader_writer.hh"
#include "snn-core/file/io/blocking.hh"
#include "snn-core/system/error.hh"
#include <cerrno> // EINVAL

namespace snn::file
{
    // ## Functions

    // ### read

    // Read a file and append the entire contents to a string.

    template <typename Io = file::io::blocking, typename Buf>
    [[nodiscard]] result<void> read(const transient<null_term<const char*>> path,
                                    strcore<Buf>& append_to, const option options = option::none,
                                    Io io = Io{})
    {
        file::reader_writer<Io> rw{std::move(io)};

        if (const auto res = rw.open_for_reading(path, options); !res)
        {
            return res.error_code();
        }

        file::info info{init::do_not_initialize};
        if (const auto res = rw.status(info); !res)
        {
            return res.error_code();
        }

        if (!info.is_regular())
        {
            return error_code{EINVAL, system::error_category};
        }

        const usize file_size = info.size();
        if (file_size > 0)
        {
            const usize size_before = append_to.size();
            if (const auto res = rw.read_fill(append_to.append_for_overwrite(file_size)); !res)
            {
                append_to.truncate(size_before);
                return res.error_code();
            }
        }

        return rw.close();
    }

    // Read a file and return the entire contents as a string.

    template <any_strcore Str = str, typename Io = file::io::blocking>
    [[nodiscard]] result<Str> read(const transient<null_term<const char*>> path,
                                   const option options = option::none, Io io = Io{})
    {
        Str append_to;
        if (const auto res = read(path, append_to, options, std::move(io)); !res)
        {
            return res.error_code();
        }
        return append_to;
    }
}
