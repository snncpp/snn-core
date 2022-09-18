// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Write to file

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/file/option.hh"
#include "snn-core/file/perm.hh"
#include "snn-core/file/reader_writer.hh"
#include "snn-core/file/io/blocking.hh"

namespace snn::file
{
    // ## Functions

    // ### write

    template <typename Io = io::blocking>
    [[nodiscard]] result<void> write(const transient<null_term<const char*>> path,
                                     const transient<cstrview> contents,
                                     const option options   = option::create_or_truncate,
                                     const perm permissions = perm::regular_default,
                                     Io io                  = Io{}) noexcept
    {
        file::reader_writer<Io> rw{std::move(io)};

        if (const auto res = rw.open_for_writing(path, options, permissions); !res)
        {
            return res.error_code();
        }

        if (const auto res = rw.write_all(contents); !res)
        {
            return res.error_code();
        }

        return rw.close();
    }
}
