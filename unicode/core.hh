// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Core functionality

#pragma once

#include "snn-core/core.hh"

// Resources:
// https://en.wikipedia.org/wiki/Unicode
// https://en.wikipedia.org/wiki/UTF-16
// https://unicode.org/faq/utf_bom.html
// https://www.unicode.org/faq/private_use.html

namespace snn::unicode
{
    // ## Constants

    namespace codepoint
    {
        // https://unicode.org/faq/utf_bom.html#gen6
        inline constexpr u32 max = 0x10FFFF;

        // https://unicode.org/faq/utf_bom.html#gen8
        inline constexpr u32 replacement = 0xFFFD;

        // https://unicode.org/faq/utf_bom.html#utf16-2
        inline constexpr u32 leading_surrogate_min  = 0xD800;
        inline constexpr u32 leading_surrogate_max  = 0xDBFF;
        inline constexpr u32 trailing_surrogate_min = 0xDC00;
        inline constexpr u32 trailing_surrogate_max = 0xDFFF;
    }

    // ## General functions

    [[nodiscard]] constexpr bool is_ascii(const u32 cp) noexcept
    {
        return cp < 0x80; // "C0 Controls and Basic Latin": U+0000-U+007F
    }

    [[nodiscard]] constexpr bool is_noncharacter(const u32 cp) noexcept
    {
        // Noncharacters are valid but intended for internal use only.
        // 32 + 34 = 66 noncharacters.
        // This matches the following code points:
        // 0xFDD0 - 0xFDEF (inclusive)
        // 0xFFFE,  0xFFFF,  0x1FFFE, 0x1FFFF, 0x2FFFE, 0x2FFFF,  0x3FFFE, 0x3FFFF, 0x4FFFE,
        // 0x4FFFF, 0x5FFFE, 0x5FFFF, 0x6FFFE, 0x6FFFF, 0x7FFFE,  0x7FFFF, 0x8FFFE, 0x8FFFF,
        // 0x9FFFE, 0x9FFFF, 0xAFFFE, 0xAFFFF, 0xBFFFE, 0xBFFFF,  0xCFFFE, 0xCFFFF, 0xDFFFE,
        // 0xDFFFF, 0xEFFFE, 0xEFFFF, 0xFFFFE, 0xFFFFF, 0x10FFFE, 0x10FFFF
        return (cp >= 0xFDD0 && cp <= 0xFDEF) || (cp & 0xFFFFu) >= 0xFFFE;
    }

    [[nodiscard]] constexpr bool is_plane_0(const u32 cp) noexcept
    {
        return cp <= 0xFFFF; // "Basic Multilingual Plane": 0x0000-0xFFFF
    }

    // ## Surrogate functions

    [[nodiscard]] constexpr u32 decode_surrogate_pair(const u32 leading,
                                                      const u32 trailing) noexcept
    {
        return 0x10000u + (((leading & 0x3FFu) << 10u) | (trailing & 0x3FFu));
    }

    [[nodiscard]] constexpr u32 encode_leading_surrogate(const u32 cp) noexcept
    {
        return codepoint::leading_surrogate_min | ((cp - 0x10000u) >> 10u);
    }

    [[nodiscard]] constexpr u32 encode_trailing_surrogate(const u32 cp) noexcept
    {
        return codepoint::trailing_surrogate_min | ((cp - 0x10000u) & 0x3FFu);
    }

    [[nodiscard]] constexpr bool is_leading_surrogate(const u32 cp) noexcept
    {
        return cp >= codepoint::leading_surrogate_min && cp <= codepoint::leading_surrogate_max;
    }

    [[nodiscard]] constexpr bool is_trailing_surrogate(const u32 cp) noexcept
    {
        return cp >= codepoint::trailing_surrogate_min && cp <= codepoint::trailing_surrogate_max;
    }
}
