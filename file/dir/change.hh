// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Change working directory

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/system/error.hh"
#include <unistd.h> // chdir

namespace snn::file::dir
{
    // ## Functions

    // ### change

    [[nodiscard]] inline result<void> change(const transient<null_term<const char*>> path) noexcept
    {
        if (::chdir(path.get().get()) != 0)
        {
            return error_code{errno, system::error_category};
        }
        return {};
    }
}
