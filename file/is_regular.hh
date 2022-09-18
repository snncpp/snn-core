// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is regular file

#pragma once

#include "snn-core/file/status.hh"

namespace snn::file
{
    // ## Functions

    // ### is_regular

    [[nodiscard]] inline bool is_regular(const transient<null_term<const char*>> path) noexcept
    {
        file::info info{container::do_not_initialize};
        if (file::status(path, info))
        {
            return info.is_regular();
        }
        return false;
    }
}
