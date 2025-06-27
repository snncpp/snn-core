// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Decode string

#pragma once

#include "snn-core/array.hh"
#include "snn-core/result.hh"
#include "snn-core/strcore.hh"
#include "snn-core/base64/error.hh"
#include "snn-core/base64/table/decode/common.hh"

namespace snn::base64
{
    // ## Enums

    // ### decode_padding

    enum decode_padding : u8
    {
        padding_optional,
        padding_required,
    };

    // ## Functions

    // ### decode

    namespace detail
    {
        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        template <typename Buf>
        [[nodiscard]] constexpr result<void> decode(
            const cstrview string, const decode_padding padding_policy, const char pad_char,
            const array<u8, 256>& lookup, strcore<Buf>& append_to)
        {
            if (string.is_empty())
            {
                return {};
            }

            usize unpadded_block_count      = string.size() / 4;
            const usize trailing_byte_count = string.size() % 4;

            if (trailing_byte_count == 0 && string.has_back(pad_char))
            {
                // The string is not empty so we must have at least one block.
                snn_should(unpadded_block_count > 0);
                --unpadded_block_count;
            }
            else if (trailing_byte_count != 0)
            {
                // A single trailing byte is always invalid and any trailing bytes is invalid if
                // padding is required.
                if (trailing_byte_count == 1 || padding_policy == padding_required) [[unlikely]]
                {
                    return error::invalid_size;
                }
            }

            auto cur        = string.cbegin();
            const auto last = string.cend();

            const usize size_restore = append_to.size();

            if (unpadded_block_count > 0)
            {
                // Every 4 byte block decodes to 3 bytes.
                strview buffer = append_to.append_for_overwrite(unpadded_block_count * 3);
                auto dst       = buffer.begin();

                do
                {
                    // Look up Base64 values for each byte in the block.
                    const u8 val1 = lookup.at(to_byte(*(cur++)), assume::within_bounds);
                    const u8 val2 = lookup.at(to_byte(*(cur++)), assume::within_bounds);
                    const u8 val3 = lookup.at(to_byte(*(cur++)), assume::within_bounds);
                    const u8 val4 = lookup.at(to_byte(*(cur++)), assume::within_bounds);

                    // Check if any of the values are invalid.
                    static_assert(base64::table::decode::invalid == 0b1111'1111);
                    const u8 combined = val1 | val2 | val3 | val4;
                    if (combined == base64::table::decode::invalid) [[unlikely]]
                    {
                        append_to.truncate(size_restore);
                        return error::invalid_character;
                    }

                    *(dst++) = static_cast<char>((val1 << 2u) | (val2 >> 4u));
                    *(dst++) = static_cast<char>((val2 << 4u) | (val3 >> 2u));
                    *(dst++) = static_cast<char>((val3 << 6u) | val4);

                    --unpadded_block_count;
                } while (unpadded_block_count > 0);

                snn_should(dst == buffer.end());
            }

            snn_should(cur == last || (cur < last && (last - cur) >= 2));

            // Trailing
            if ((last - cur) >= 2)
            {
                // First 2 bytes can't be padding.

                const u8 val1 = lookup.at(to_byte(*(cur++)), assume::within_bounds);
                const u8 val2 = lookup.at(to_byte(*(cur++)), assume::within_bounds);

                if ((val1 | val2) == base64::table::decode::invalid) [[unlikely]]
                {
                    append_to.truncate(size_restore);
                    return error::invalid_character;
                }

                const auto out1 = static_cast<char>((val1 << 2u) | (val2 >> 4u));

                // Last 2 bytes.

                const auto pad_byte = to_byte(pad_char);

                byte raw3 = pad_byte;
                byte raw4 = pad_byte;

                if (cur < last)
                {
                    raw3 = to_byte(*(cur++));

                    if (cur < last)
                    {
                        raw4 = to_byte(*(cur++));
                    }
                }

                if (raw4 != pad_byte)
                {
                    // Both bytes used.
                    const u8 val3 = lookup.at(raw3, assume::within_bounds);
                    const u8 val4 = lookup.at(raw4, assume::within_bounds);

                    if ((val3 | val4) == base64::table::decode::invalid) [[unlikely]]
                    {
                        append_to.truncate(size_restore);
                        return error::invalid_character;
                    }

                    const auto out2 = static_cast<char>((val2 << 4u) | (val3 >> 2u));
                    const auto out3 = static_cast<char>((val3 << 6u) | val4);
                    append_to.append({out1, out2, out3});
                }
                else if (raw3 != pad_byte)
                {
                    // 3rd byte used.
                    const u8 val3 = lookup.at(raw3, assume::within_bounds);

                    if (val3 == base64::table::decode::invalid) [[unlikely]]
                    {
                        append_to.truncate(size_restore);
                        return error::invalid_character;
                    }

                    const auto out2 = static_cast<char>((val2 << 4u) | (val3 >> 2u));
                    append_to.append({out1, out2});
                }
                else
                {
                    append_to.append(out1);
                }
            }

            snn_should(cur == last);

            return {};
        }

        SNN_DIAGNOSTIC_POP
    }

    // Decode and return the decoded string.

    template <any_strcore Str = str>
    [[nodiscard]] constexpr result<Str> decode(
        const transient<cstrview> string, const decode_padding padding_policy = padding_required,
        const char pad_char = '=', const array<u8, 256>& table = table::decode::standard)
    {
        Str append_to;
        const auto res = detail::decode(string.get(), padding_policy, pad_char, table, append_to);
        if (res)
        {
            return append_to;
        }
        return res.error_code();
    }

    // Decode and append the decoded string.

    template <typename Buf>
    [[nodiscard]] constexpr result<void> decode(const transient<cstrview> string,
                                                strcore<Buf>& append_to, assume::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !string.get().overlaps(append_to));
        return detail::decode(string.get(), padding_required, '=', table::decode::standard,
                              append_to);
    }

    // Decode and append the decoded string (extended).

    template <typename Buf>
    [[nodiscard]] constexpr result<void> decode(const transient<cstrview> string,
                                                const decode_padding padding_policy,
                                                const char pad_char, const array<u8, 256>& table,
                                                strcore<Buf>& append_to, assume::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !string.get().overlaps(append_to));
        return detail::decode(string.get(), padding_policy, pad_char, table, append_to);
    }
}
