// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Append to file

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/file/option.hh"
#include "snn-core/file/perm.hh"
#include "snn-core/file/write.hh"
#include "snn-core/file/io/blocking.hh"

namespace snn::file
{
    // ## Functions

    // ### append

    template <typename Io = io::blocking>

    [[nodiscard]] result<void> append(const transient<null_term<const char*>> path,
                                      const transient<cstrview> contents,
                                      const option options = option::create_or_append,
                                      const perm perms     = perm::regular_default,
                                      Io io                = Io{}) noexcept

    {
        return write(path, contents, options, perms, std::move(io));
    }
}
