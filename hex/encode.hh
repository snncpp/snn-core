// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Encode string

#pragma once

#include "snn-core/array.hh"
#include "snn-core/strcore.hh"
#include "snn-core/hex/table/common.hh"

namespace snn::hex
{
    namespace detail
    {
        constexpr array<char, 2> encode(const char c, const array<char, 16>& table)
        {
            array<char, 2> buffer;
            const auto b    = to_byte(c);
            buffer.get<0>() = table.at(b >> 4u, bounds::mask); // High bits.
            buffer.get<1>() = table.at(b, bounds::mask);       // Low bits.
            return buffer;
        }

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        template <typename Buf>
        constexpr void encode(const cstrview s, const array<char, 16>& table,
                              strcore<Buf>& append_to)
        {
            // Every byte turns into 2 bytes. Can't overflow (57-bit-virtual-address-space).
            strview buffer = append_to.append_for_overwrite(s.size() * 2);
            char* dest     = buffer.begin();
            for (const char c : s)
            {
                const auto b = to_byte(c);
                *(dest++)    = table.at(b >> 4u, bounds::mask); // High bits.
                *(dest++)    = table.at(b, bounds::mask);       // Low bits.
            }
            snn_should(dest == buffer.end());
        }

        SNN_DIAGNOSTIC_POP
    }

    // ## Functions

    // ### encode

    // Encode a single character.

    template <character Char>
    [[nodiscard]] constexpr array<char, 2> encode(const Char c)
    {
        return detail::encode(c, table::lower);
    }

    // Encode a single character (extended).

    template <character Char>
    [[nodiscard]] constexpr array<char, 2> encode(const Char c, const array<char, 16>& table)
    {
        return detail::encode(c, table);
    }

    // Encode and return the encoded string.

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str encode(const transient<cstrview> s)
    {
        Str append_to;
        detail::encode(s.get(), table::lower, append_to);
        return append_to;
    }

    // Encode and return the encoded string (extended).

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str encode(const transient<cstrview> s, const array<char, 16>& table)
    {
        Str append_to;
        detail::encode(s.get(), table, append_to);
        return append_to;
    }

    // Encode and append the encoded string.

    template <typename Buf>
    constexpr void encode(const transient<cstrview> s, strcore<Buf>& append_to,
                          assume::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !s.get().overlaps(append_to));
        detail::encode(s.get(), table::lower, append_to);
    }

    // Encode and append the encoded string (extended).

    template <typename Buf>
    constexpr void encode(const transient<cstrview> s, const array<char, 16>& table,
                          strcore<Buf>& append_to, assume::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !s.get().overlaps(append_to));
        detail::encode(s.get(), table, append_to);
    }
}
