// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is something

// Is something on the file system (successful `lstat(...)`).

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/file/info.hh"
#include <sys/stat.h> // lstat

namespace snn::file
{
    // ## Functions

    // ### is_something

    [[nodiscard]] inline bool is_something(const transient<null_term<const char*>> path) noexcept
    {
        file::info info{container::do_not_initialize};
        return ::lstat(path.get().get(), &info.internal()) == 0;
    }
}
