// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is relative

#pragma once

#include "snn-core/file/path/is_absolute.hh"

namespace snn::file::path
{
    // ## Functions

    // ### is_relative

    [[nodiscard]] constexpr bool is_relative(const transient<cstrview> path) noexcept
    {
        return !is_absolute(path);
    }
}
