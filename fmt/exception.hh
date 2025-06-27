// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Format `snn::exception` and `std::exception`

#pragma once

#include "snn-core/exception.hh"
#include "snn-core/formatter.hh"
#include "snn-core/strcore.hh"
#include "snn-core/fmt/context.hh"
#include "snn-core/fmt/error_code.hh"

namespace snn::fmt
{
    // ## Functions

    // ### exception

    template <typename Buf>
    void exception(const snn::exception& e, strcore<Buf>& append_to)
    {
        fmt::error_code(e.error_code(), append_to);
    }

    template <typename Buf>
    void exception(const std::exception& e, strcore<Buf>& append_to)
    {
        append_to << cstrview{e.what(), assume::null_terminated};
    }
}

namespace snn
{
    // ## Specializations

    // ### formatter<exception>

    template <>
    struct formatter<exception>
    {
        using type = exception;

        template <typename Buf>
        void format(const exception& e, const cstrview format_string, const fmt::context& ctx,
                    strcore<Buf>& append_to, assume::no_overlap_t)
        {
            formatter_select_t<error_code> f;
            f.format(e.error_code(), format_string, ctx, append_to, assume::no_overlap);
        }
    };

    // ### formatter<std::exception>

    template <>
    struct formatter<std::exception>
    {
        using type = std::exception;

        template <typename Buf>
        void format(const std::exception& e, cstrview, const fmt::context&, strcore<Buf>& append_to,
                    assume::no_overlap_t)
        {
            fmt::exception(e, append_to);
        }
    };
}
