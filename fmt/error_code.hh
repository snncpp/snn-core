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
        append_to << ec.category().name<cstrview>() << ": " << ec.message<cstrview>() << " ("
                  << as_num(ec.value()) << ")";
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str error_code(const snn::error_code ec)
    {
        Str append_to;
        fmt::error_code(ec, append_to);
        return append_to;
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
                              strcore<Buf>& append_to, promise::no_overlap_t)
        {
            fmt::error_code(ec, append_to);
        }
    };
}
