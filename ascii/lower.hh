// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Convert to lowercase

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/chr/common.hh"

namespace snn::ascii
{
    // ## Functions

    // ### lower

    template <typename Buf>
    constexpr void lower(const transient<cstrview> s, strcore<Buf>& append_to)
    {
        // Can overlap.
        const usize offset = append_to.size();
        append_to.append(s.get());
        append_to.view(offset).transform(chr::to_alpha_lower);
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str lower(const transient<cstrview> s)
    {
        Str append_to;
        lower(s, append_to);
        return append_to;
    }
}
