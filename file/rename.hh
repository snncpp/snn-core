// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Rename directory entry

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/system/error.hh"
#include <cstdio> // rename

namespace snn::file
{
    // ## Functions

    // ### rename

    [[nodiscard]] inline result<void> rename(const transient<null_term<const char*>> from,
                                             const transient<null_term<const char*>> to) noexcept
    {
        if (std::rename(from.get().get(), to.get().get()) != 0)
        {
            return error_code{errno, system::error_category};
        }
        return {};
    }
}
