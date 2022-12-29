// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Decode string

#pragma once

#include "snn-core/result.hh"
#include "snn-core/strcore.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/hex/error.hh"

namespace snn::hex
{
    namespace detail
    {
        template <typename Buf>
        constexpr result<void> decode(const cstrview string, strcore<Buf>& append_to)
        {
            // Make sure the string contains pairs of bytes (or is empty).
            if (string.size() % 2 != 0)
            {
                return error::invalid_size;
            }

            // Every 2 bytes turns into 1 byte. This does nothing if the string is empty.
            const usize size_restore = append_to.size();
            strview buffer           = append_to.append_uninitialized(string.size() / 2);
            auto dest                = buffer.begin();

            auto cur        = string.cbegin();
            const auto last = string.cend();
            while (cur != last)
            {
                const u8 val1 = chr::decode_hex(*(cur++));
                const u8 val2 = chr::decode_hex(*(cur++));

                // Check if any of the values are invalid.
                static_assert(chr::invalid == 0b1111'1111);
                if ((val1 | val2) == chr::invalid) [[unlikely]]
                {
                    append_to.truncate(size_restore);
                    return error::invalid_character;
                }

                *(dest++) = static_cast<char>((val1 << 4u) | val2); // High bits + low bits.
            }
            snn_should(cur == last);
            snn_should(dest == buffer.end());

            return {};
        }
    }

    // ## Functions

    // ### decode

    // Decode and return the decoded string.

    template <typename Str = str>
    [[nodiscard]] constexpr result<Str> decode(const cstrview string)
    {
        Str append_to;
        const auto res = detail::decode(string, append_to);
        if (res)
        {
            return append_to;
        }
        return res.error_code();
    }

    // Decode and append the decoded string.

    template <typename Buf>
    [[nodiscard]] constexpr result<void> decode(const cstrview string, strcore<Buf>& append_to,
                                                promise::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !string.overlaps(append_to));
        return detail::decode(string, append_to);
    }
}
