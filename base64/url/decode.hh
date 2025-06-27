// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Decode string with URL table

#pragma once

#include "snn-core/base64/decode.hh"

namespace snn::base64::url
{
    // ## Functions

    // ### decode

    template <any_strcore Str = str>
    [[nodiscard]] constexpr result<Str> decode(const transient<cstrview> string)
    {
        return base64::decode<Str>(string, padding_optional, '=', table::decode::url);
    }

    template <typename Buf>
    [[nodiscard]] constexpr result<void> decode(const transient<cstrview> string,
                                                strcore<Buf>& append_to, assume::no_overlap_t)
    {
        return base64::decode(string, padding_optional, '=', table::decode::url, append_to,
                              assume::no_overlap);
    }
}
