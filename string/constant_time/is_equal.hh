// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is equal

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/mem/raw/constant_time/is_equal.hh"

namespace snn::string::constant_time
{
    // ## Functions

    // ### is_equal

    [[nodiscard]] inline bool is_equal(const transient<cstrview> a,
                                       const transient<cstrview> b) noexcept
    {
        if (a.get().size() == b.get().size())
        {
            return mem::raw::constant_time::is_equal(a.get().data(), b.get().data(),
                                                     a.get().byte_size());
        }
        return false;
    }
}
