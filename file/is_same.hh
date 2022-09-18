// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Check if two paths point to the same file

#pragma once

#include "snn-core/file/status.hh"

namespace snn::file
{
    // ## Functions

    // ### is_same

    [[nodiscard]] inline bool is_same(const file::info& info1, const file::info& info2) noexcept
    {
        // From man stat(2):
        // "The st_dev and st_ino fields together identify the file uniquely within the system."
        return info1.internal().st_dev == info2.internal().st_dev &&
               info1.internal().st_ino == info2.internal().st_ino;
    }

    [[nodiscard]] inline bool is_same(const transient<null_term<const char*>> path1,
                                      const transient<null_term<const char*>> path2) noexcept
    {
        file::info info1{container::do_not_initialize};
        file::info info2{container::do_not_initialize};
        if (file::status(path1, info1) && file::status(path2, info2))
        {
            return is_same(info1, info2);
        }
        return false;
    }
}
