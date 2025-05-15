// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Encode single code point

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/utf8/core.hh"

namespace snn::utf8
{
    // ## Functions

    // ### encode

    [[nodiscard]] constexpr strview encode(const u32 cp, array<char, 4>& buffer,
                                           promise::is_valid_t) noexcept
    {
        snn_should(utf8::is_valid(cp));

        char* const dest = buffer.begin();

        if (cp < codepoint::first_in_2_byte_sequence)
        {
            *dest = static_cast<char>(cp);
            return strview{not_null{dest}, 1};
        }

        if (cp < codepoint::first_in_3_byte_sequence)
        {
            encode_2_byte_sequence(cp, dest, promise::is_valid);
            return strview{not_null{dest}, 2};
        }

        if (cp < codepoint::first_in_4_byte_sequence)
        {
            encode_3_byte_sequence(cp, dest, promise::is_valid);
            return strview{not_null{dest}, 3};
        }

        encode_4_byte_sequence(cp, dest, promise::is_valid);
        return strview{not_null{dest}, 4};
    }

    [[nodiscard]] constexpr strview encode(const u32 cp, array<char, 4>& buffer) noexcept
    {
        return encode(utf8::replace_if_invalid(cp), buffer, promise::is_valid);
    }

    template <typename Buf>
    constexpr strview encode(const u32 cp, strcore<Buf>& append_to, promise::is_valid_t)
    {
        snn_should(utf8::is_valid(cp));

        if (cp < codepoint::first_in_2_byte_sequence)
        {
            char* const dest = append_to.append_for_overwrite(1).begin();
            *dest            = static_cast<char>(cp);
            return strview{not_null{dest}, 1};
        }

        if (cp < codepoint::first_in_3_byte_sequence)
        {
            char* const dest = append_to.append_for_overwrite(2).begin();
            encode_2_byte_sequence(cp, dest, promise::is_valid);
            return strview{not_null{dest}, 2};
        }

        if (cp < codepoint::first_in_4_byte_sequence)
        {
            char* const dest = append_to.append_for_overwrite(3).begin();
            encode_3_byte_sequence(cp, dest, promise::is_valid);
            return strview{not_null{dest}, 3};
        }

        char* const dest = append_to.append_for_overwrite(4).begin();
        encode_4_byte_sequence(cp, dest, promise::is_valid);
        return strview{not_null{dest}, 4};
    }

    template <typename Buf>
    constexpr strview encode(const u32 cp, strcore<Buf>& append_to)
    {
        return encode(utf8::replace_if_invalid(cp), append_to, promise::is_valid);
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str encode(const u32 cp, promise::is_valid_t)
    {
        Str append_to;
        encode(cp, append_to, promise::is_valid);
        return append_to;
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str encode(const u32 cp)
    {
        Str append_to;
        encode(utf8::replace_if_invalid(cp), append_to, promise::is_valid);
        return append_to;
    }
}
