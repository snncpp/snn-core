// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is absolute

#pragma once

#include "snn-core/array_view.hh"

namespace snn::file::path
{
    // ## Functions

    // ### is_absolute

    [[nodiscard]] constexpr bool is_absolute(const transient<cstrview> path) noexcept
    {
        return path.get().has_front('/');
    }
}
