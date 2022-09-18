// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is directory

#pragma once

#include "snn-core/file/status.hh"

namespace snn::file
{
    // ## Functions

    // ### is_directory

    [[nodiscard]] inline bool is_directory(const transient<null_term<const char*>> path) noexcept
    {
        file::info info{container::do_not_initialize};
        if (file::status(path, info))
        {
            return info.is_directory();
        }
        return false;
    }
}
