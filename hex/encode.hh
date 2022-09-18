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
        template <typename Buf>
        constexpr void encode(const cstrview s, const array<char, 16>& table,
                              strcore<Buf>& append_to)
        {
            // Every byte turns into 2 bytes. Can't overflow (57-bit-virtual-address-space).
            strview buffer = append_to.append_uninitialized(s.size() * 2);
            char* dest     = buffer.begin();
            for (const char c : s)
            {
                const auto b = to_byte(c);
                *(dest++)    = table.at(b >> 4u, bounds::mask); // High bits.
                *(dest++)    = table.at(b, bounds::mask);       // Low bits.
            }
            snn_should(dest == buffer.end());
        }
    }

    // ## Functions

    // ### encode

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
                          promise::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !s.get().overlaps(append_to));
        detail::encode(s.get(), table::lower, append_to);
    }

    // Encode and append the encoded string (extended).

    template <typename Buf>
    constexpr void encode(const transient<cstrview> s, const array<char, 16>& table,
                          strcore<Buf>& append_to, promise::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !s.get().overlaps(append_to));
        detail::encode(s.get(), table, append_to);
    }
}
