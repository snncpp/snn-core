// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Convert to uppercase

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/chr/common.hh"

namespace snn::ascii
{
    // ## Functions

    // ### upper

    template <typename Buf>
    constexpr void upper(const transient<cstrview> s, strcore<Buf>& append_to)
    {
        // Can overlap.
        const usize offset = append_to.size();
        append_to.append(s.get());
        append_to.view(offset).transform(chr::to_alpha_upper);
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str upper(const transient<cstrview> s)
    {
        Str append_to;
        upper(s, append_to);
        return append_to;
    }
}
