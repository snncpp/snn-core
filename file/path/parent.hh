// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Parent path from string

// Get parent path from string (without touching the filesystem).

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::file::path
{
    // ## Functions

    // ### parent

    // Return the lexical parent path from a string path, `"."` and `".."` have no special meaning.

    template <typename String = str>
    [[nodiscard]] constexpr String parent(const cstrview path) noexcept(
        std::is_nothrow_constructible_v<String, cstrrng>)
    {
        auto rng = path.range();

        // Drop all trailing slashes if there is at least one non-slash character in the range.
        if (rng.has_back('/') && rng.any(fn::is{fn::not_equal_to{}, '/'}))
        {
            rng.pop_back_while(fn::is{fn::equal_to{}, '/'});
        }

        // Drop all trailing non-slashes.
        rng.pop_back_while(fn::is{fn::not_equal_to{}, '/'});

        // Drop all trailing slashes if there is at least one non-slash character in the range.
        if (rng.has_back('/') && rng.any(fn::is{fn::not_equal_to{}, '/'}))
        {
            rng.pop_back_while(fn::is{fn::equal_to{}, '/'});
        }

        return String{rng};
    }
}
