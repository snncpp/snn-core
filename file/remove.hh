// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Remove directory entry

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/system/error.hh"
#include <unistd.h> // unlink

namespace snn::file
{
    // ## Functions

    // ### remove

    [[nodiscard]] inline result<void> remove(const transient<null_term<const char*>> path) noexcept
    {
        if (::unlink(path.get().get()) != 0)
        {
            return error_code{errno, system::error_category};
        }
        return {};
    }
}
