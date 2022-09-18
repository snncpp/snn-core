// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Set process mask

// Sets the creation permission mask for the process with `umask(2)` and returns the old mask.

#pragma once

#include "snn-core/file/perm.hh"
#include <sys/stat.h> // umask

namespace snn::file::permission
{
    // ## Functions

    // ### mask

    inline file::perm mask(const file::perm perm_mask) noexcept
    {
        return static_cast<file::perm>(::umask(static_cast<mode_t>(perm_mask)));
    }
}
