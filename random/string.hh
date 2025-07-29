// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Random string

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/random/detail/source.hh"

namespace snn::random
{
    // ## Functions

    // ### string

    template <typename Buf>
    void string(const usize size, strcore<Buf>& append_to)
    {
        random::detail::source::fill(append_to.append_for_overwrite(size));
    }

    template <any_strcore Str = str>
    [[nodiscard]] Str string(const usize size)
    {
        Str append_to;
        random::string(size, append_to);
        return append_to;
    }
}
