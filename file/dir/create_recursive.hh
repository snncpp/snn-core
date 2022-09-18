// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Create directory including all intermediaries

// This is a wrapper around `mkdir(2)`, so permissions are restricted by the `umask(2)` of the
// calling process.

// Permissions can be provided for both the leaf (the last component of the path) and for
// intermediate directories.

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/strcore.hh"
#include "snn-core/file/is_something.hh"
#include "snn-core/file/perm.hh"
#include "snn-core/file/dir/create.hh"
#include "snn-core/file/path/parent.hh"
#include "snn-core/system/error.hh"
#include <cerrno> // EEXIST

namespace snn::file::dir
{
    // ## Functions

    // ### create_recursive

    [[nodiscard]] inline result<void> create_recursive(
        const transient<null_term<const char*>> path,
        const perm leaf_perms         = perm::directory_default,
        const perm intermediate_perms = perm::directory_default)
    {
        const auto p      = path.get().to<cstrview>();
        const auto parent = file::path::parent<str>(p);

        if (parent && parent.size() < p.size() && !is_something(parent))
        {
            // Potential race condition, check for `EEXIST` after.
            const auto res = create_recursive(parent, intermediate_perms, intermediate_perms);
            if (!res)
            {
                const auto ec = res.error_code();
                if (ec != error_code{EEXIST, system::error_category})
                {
                    return ec;
                }
            }
        }

        return create(path, leaf_perms);
    }
}
