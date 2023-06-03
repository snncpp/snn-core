// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Encode string

#pragma once

#include "snn-core/array.hh"
#include "snn-core/strcore.hh"
#include "snn-core/base64/table/encode/common.hh"

namespace snn::base64
{
    // ## Enums

    // ### encode_padding

    enum encode_padding : u8
    {
        with_padding,
        without_padding,
    };

    // ## Functions

    // ### encode

    namespace detail
    {
        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        [[nodiscard]] constexpr usize encoded_size(const usize size,
                                                   const encode_padding padding_policy) noexcept
        {
            // Every 3 bytes turns into 4 bytes.
            const usize unpadded_block_count = size / 3;
            const usize trailing_byte_count  = size % 3;

            usize size_needed = unpadded_block_count * 4;

            // Trailing bytes (at most 2).
            if (trailing_byte_count)
            {
                const usize encoded_size = trailing_byte_count + 1;
                size_needed += encoded_size;

                if (padding_policy == with_padding)
                {
                    const usize padding_size = 4 - encoded_size;
                    size_needed += padding_size;
                }
            }

            return size_needed;
        }

        template <typename Buf>
        constexpr void encode(const cstrview string, const encode_padding padding_policy,
                              const char pad_char, const array<char, 64>& lookup,
                              strcore<Buf>& append_to)
        {
            if (string.is_empty())
            {
                return;
            }

            // Can't overflow (57-bit-virtual-address-space).
            strview buffer =
                append_to.append_uninitialized(encoded_size(string.size(), padding_policy));

            auto dst            = buffer.begin();
            const auto dst_last = buffer.cend();

            auto cur        = string.cbegin();
            const auto last = string.cend();

            // Blocks of 3 bytes.
            while ((last - cur) >= 3)
            {
                const auto b1 = to_byte(*(cur++));
                const auto b2 = to_byte(*(cur++));
                const auto b3 = to_byte(*(cur++));

                *(dst++) = lookup.at(b1 >> 2u, bounds::mask);
                *(dst++) = lookup.at(((b1 & 0b0000'0011u) << 4u) | (b2 >> 4u), bounds::mask);
                *(dst++) = lookup.at(((b2 & 0b0000'1111u) << 2u) | (b3 >> 6u), bounds::mask);
                *(dst++) = lookup.at(b3 & 0b0011'1111u, bounds::mask);
            }

            // Any bytes left (at most 2)? Will encode as 2 or 3 characters (excluding padding).
            if (cur < last)
            {
                const auto b1 = to_byte(*(cur++));

                // 1st encoded.
                *(dst++) = lookup.at(b1 >> 2u, bounds::mask);

                if (cur < last)
                {
                    const auto b2 = to_byte(*(cur++));

                    // 2nd & 3rd encoded.
                    *(dst++) = lookup.at(((b1 & 0b0000'0011u) << 4u) | (b2 >> 4u), bounds::mask);
                    *(dst++) = lookup.at((b2 & 0b0000'1111u) << 2u, bounds::mask);
                }
                else
                {
                    // 2nd encoded.
                    *(dst++) = lookup.at((b1 & 0b0000'0011u) << 4u, bounds::mask);
                }
            }

            snn_should(cur == last);

            // Padding (at most 2 characters).
            // Don't use a loop because Clang will replace it with a call to `memset`.
            if (dst < dst_last)
            {
                *(dst++) = pad_char;

                if (dst < dst_last)
                {
                    *(dst++) = pad_char;
                }
            }

            snn_should(dst == dst_last);
        }

        SNN_DIAGNOSTIC_POP
    }

    // Encode and return the encoded string.

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str encode(const transient<cstrview> string,
                                       const encode_padding padding_policy = with_padding,
                                       const char pad_char                 = '=',
                                       const array<char, 64>& table = table::encode::standard)
    {
        Str append_to;
        detail::encode(string.get(), padding_policy, pad_char, table, append_to);
        return append_to;
    }

    // Encode and append the encoded string.

    template <typename Buf>
    constexpr void encode(const transient<cstrview> string, strcore<Buf>& append_to,
                          promise::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !string.get().overlaps(append_to));
        detail::encode(string.get(), with_padding, '=', table::encode::standard, append_to);
    }

    // Encode and append the encoded string (extended).

    template <typename Buf>
    constexpr void encode(const transient<cstrview> string, const encode_padding padding_policy,
                          const char pad_char, const array<char, 64>& table,
                          strcore<Buf>& append_to, promise::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !string.get().overlaps(append_to));
        detail::encode(string.get(), padding_policy, pad_char, table, append_to);
    }
}
