// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Format `error_code`

#pragma once

#include "snn-core/error_code.hh"
#include "snn-core/formatter.hh"
#include "snn-core/strcore.hh"
#include "snn-core/fmt/context.hh"

namespace snn::fmt
{
    // ## Functions

    // ### error_code

    template <typename Buf>
    constexpr void error_code(const snn::error_code ec, strcore<Buf>& append_to)
    {
        ec.format(append_to);
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str error_code(const snn::error_code ec)
    {
        return ec.format<Str>();
    }
}

namespace snn
{
    // ## Specializations

    // ### formatter<error_code>

    template <>
    struct formatter<error_code>
    {
        using type = error_code;

        template <typename Buf>
        constexpr void format(const error_code ec, cstrview, const fmt::context&,
                              strcore<Buf>& append_to, assume::no_overlap_t)
        {
            fmt::error_code(ec, append_to);
        }
    };
}
