// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Less-than with directory priority

// Compares two `file::dir::entry` objects and returns `true` if the first compares *less-than*
// the second. A directory compares *less-than* a non-directory.

#pragma once

#include "snn-core/file/dir/entry.hh"

namespace snn::file::dir::fn
{
    // ## Classes

    // ### less_than_directory_first

    struct less_than_directory_first final
    {
        explicit less_than_directory_first() = default;

        [[nodiscard]] constexpr bool operator()(const entry& a, const entry& b) const noexcept
        {
            if (a.is_directory() != b.is_directory())
            {
                return a.is_directory();
            }
            return a < b;
        }
    };
}
