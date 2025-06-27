// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Encode string with URL table

#pragma once

#include "snn-core/base64/encode.hh"

namespace snn::base64::url
{
    // ## Functions

    // ### encode

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str encode(const transient<cstrview> string)
    {
        constexpr char unused_pad_char{};
        return base64::encode<Str>(string, without_padding, unused_pad_char, table::encode::url);
    }

    template <typename Buf>
    constexpr void encode(const transient<cstrview> string, strcore<Buf>& append_to,
                          assume::no_overlap_t)
    {
        constexpr char unused_pad_char{};
        base64::encode(string, without_padding, unused_pad_char, table::encode::url, append_to,
                       assume::no_overlap);
    }
}
