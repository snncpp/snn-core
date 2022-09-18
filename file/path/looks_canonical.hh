// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Looks canonical

// Does a path look like it is canonical (without touching the filesystem).

// Note: A single trailing slash is allowed for convenience, but could be considered not canonical.

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/range/contiguous.hh"
#include <climits> // PATH_MAX

namespace snn::file::path
{
    // ## Functions

    // ### looks_canonical

    [[nodiscard]] constexpr bool looks_canonical(const transient<cstrview> path) noexcept
    {
        const cstrview s = path.get();

        // "PATH_MAX - Maximum number of bytes the implementation will store as a pathname in a
        // user-supplied buffer of unspecified size, including the terminating null character."
        // https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/limits.h.html

        if (s && s.size() < PATH_MAX)
        {
            if (s.has_back("/.")) return false;  // Same directory.
            if (s.has_back("/..")) return false; // Parent directory.

            auto rng = s.range();
            while (rng.drop_front('/'))
            {
                if (rng.has_front('/')) return false;   // Double slash.
                if (rng.has_front("./")) return false;  // "/./" component.
                if (rng.has_front("../")) return false; // "/../" component.

                rng.pop_front_while(fn::_not{fn::in_array{'/', '\0'}});
            }
            return rng.is_empty();
        }

        return false;
    }
}
