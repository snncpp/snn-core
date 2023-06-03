// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Core constants and functions

// Resources:
// https://en.wikipedia.org/wiki/UTF-8
// https://en.wikipedia.org/wiki/Specials_(Unicode_block)#Replacement_character

#pragma once

#include "snn-core/mem/raw/load.hh"
#include "snn-core/pair/common.hh"
#include "snn-core/unicode/core.hh"

namespace snn::utf8
{
    // ## Constants

    inline constexpr u32 invalid = constant::limit<u32>::max;

    namespace codepoint
    {
        inline constexpr u32 first_in_2_byte_sequence = 0x80;
        inline constexpr u32 first_in_3_byte_sequence = 0x800;
        inline constexpr u32 first_in_4_byte_sequence = 0x10000;
    }

    // ## Validation functions

    // ### is_X_byte_sequence

    [[nodiscard]] constexpr bool is_1_byte_sequence(const char c) noexcept
    {
        return to_byte(c) < 0x80;
    }

    [[nodiscard]] constexpr bool is_2_byte_sequence(const char c) noexcept
    {
        const auto b = to_byte(c);
        return b >= 0xC2 && b <= 0xDF;
    }

    [[nodiscard]] constexpr bool is_3_byte_sequence(const char c) noexcept
    {
        const auto b = to_byte(c);
        return b >= 0xE0 && b <= 0xEF;
    }

    [[nodiscard]] constexpr bool is_4_byte_sequence(const char c) noexcept
    {
        const auto b = to_byte(c);
        return b >= 0xF0 && b <= 0xF4;
    }

    // ### is_continuation

    [[nodiscard]] constexpr bool is_continuation(const char c) noexcept
    {
        return (to_byte(c) & 0b1100'0000u) == 0b1000'0000;
    }

    // ### is_valid

    [[nodiscard]] constexpr bool is_valid(const u32 cp) noexcept
    {
        // 0xD800 to 0xDFFF are reserved for use with UTF-16 as surrogate pairs.
        // Code points greater than 0x10FFFF are not allowed.
        return cp < unicode::codepoint::leading_surrogate_min ||
               (cp > unicode::codepoint::trailing_surrogate_max && cp <= unicode::codepoint::max);
    }

    // ### replace_if_invalid

    [[nodiscard]] constexpr u32 replace_if_invalid(const u32 cp) noexcept
    {
        return is_valid(cp) ? cp : unicode::codepoint::replacement;
    }

    // ## Decode functions

    // ### decode_X_byte_sequence

    SNN_DIAGNOSTIC_PUSH
    SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

    [[nodiscard]] constexpr pair::value_count<u32> decode_2_byte_sequence(
        const char* const data) noexcept
    {
        if (is_continuation(data[1]))
        {
            // 2-byte sequence.
            // Binary UTF-8:        110aaaaa 10bbbbbb
            // Binary code point:   00000aaa aabbbbbb

            // First byte, keep the last 5 bits, shifted 6 steps to the left.
            u32 cp = static_cast<u32>(to_byte(data[0]) & 0b0001'1111u) << 6u;
            // Second byte, add the last 6 bits.
            cp += static_cast<u32>(to_byte(data[1]) & 0b0011'1111u);

            if (cp >= codepoint::first_in_2_byte_sequence)
            {
                return {cp, 2};
            }
        }
        return {utf8::invalid, 1};
    }

    [[nodiscard]] constexpr pair::value_count<u32> decode_3_byte_sequence(
        const char* const data) noexcept
    {
        const auto i = mem::raw::load<u16>(data + 1);
        if ((i & 0xC0C0u) == 0x8080) // 0xC0 == 0b1100'0000, 0x80 == 0b1000'0000
        {
            // 3-byte sequence.
            // Binary UTF-8:        1110aaaa 10bbbbbb 10cccccc
            // Binary code point:            aaaabbbb bbcccccc

            // First byte, keep the last 4 bits, shifted 12 steps to the left.
            u32 cp = static_cast<u32>(to_byte(data[0]) & 0b0000'1111u) << 12u;
            // Second byte, add the last 6 bits, shifted 6 steps to the left.
            cp += static_cast<u32>(to_byte(data[1]) & 0b0011'1111u) << 6u;
            // Third byte, add the last 6 bits.
            cp += static_cast<u32>(to_byte(data[2]) & 0b0011'1111u);

            if (cp >= codepoint::first_in_3_byte_sequence)
            {
                if (cp < unicode::codepoint::leading_surrogate_min ||
                    cp > unicode::codepoint::trailing_surrogate_max)
                {
                    return {cp, 3};
                }
            }
        }
        return {utf8::invalid, 1};
    }

    [[nodiscard]] constexpr pair::value_count<u32> decode_4_byte_sequence(
        const char* const data) noexcept
    {
        const auto i = mem::raw::load<u32>(data);
        if ((i & 0xC0C0C000u) == 0x80808000) // 0xC0 == 0b1100'0000, 0x80 == 0b1000'0000
        {
            // 4-byte sequence.
            // Binary UTF-8:        11110aaa 10bbbbbb 10cccccc 10dddddd
            // Binary code point:            000aaabb bbbbcccc ccdddddd

            // First byte, keep the last 3 bits, shifted 18 steps to the left.
            u32 cp = static_cast<u32>(to_byte(data[0]) & 0b0000'0111u) << 18u;
            // Second byte, add the last 6 bits, shifted 12 steps to the left.
            cp += static_cast<u32>(to_byte(data[1]) & 0b0011'1111u) << 12u;
            // Third byte, add the last 6 bits, shifted 6 steps to the left.
            cp += static_cast<u32>(to_byte(data[2]) & 0b0011'1111u) << 6u;
            // Fourth byte, add the last 6 bits.
            cp += static_cast<u32>(to_byte(data[3]) & 0b0011'1111u);

            if (cp >= codepoint::first_in_4_byte_sequence && cp <= unicode::codepoint::max)
            {
                return {cp, 4};
            }
        }
        return {utf8::invalid, 1};
    }

    SNN_DIAGNOSTIC_POP

    // ### decode_multibyte_sequence

    template <contiguous_range ContiguousRange>
    [[nodiscard]] constexpr pair::value_count<u32> decode_multibyte_sequence(
        const ContiguousRange rng) noexcept
    {
        static_assert(std::is_same_v<front_value_t<const ContiguousRange&>, char>);

        const char* const first = rng.begin();
        const usize range_size  = rng.size();

        if (range_size >= 2)
        {
            const char peek = *first;

            if (is_2_byte_sequence(peek))
            {
                return decode_2_byte_sequence(first);
            }
            else if (is_3_byte_sequence(peek))
            {
                if (range_size >= 3)
                {
                    return decode_3_byte_sequence(first);
                }
            }
            else if (is_4_byte_sequence(peek))
            {
                if (range_size >= 4)
                {
                    return decode_4_byte_sequence(first);
                }
            }
        }

        return {utf8::invalid, u32{range_size > 0}};
    }

    // ## Encode functions

    SNN_DIAGNOSTIC_PUSH
    SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

    // ### encode_X_byte_sequence

    template <typename OutputIt>
    constexpr OutputIt encode_2_byte_sequence(const u32 cp, OutputIt dest,
                                              promise::is_valid_t) noexcept
    {
        snn_should(cp >= codepoint::first_in_2_byte_sequence &&
                   cp < codepoint::first_in_3_byte_sequence);
        // Binary UTF-8:        110aaaaa 10bbbbbb
        // Binary code point:   00000aaa aabbbbbb
        *(dest++) = static_cast<char>((cp >> 6u) | 0b1100'0000u);
        *(dest++) = static_cast<char>((cp & 0b0011'1111u) | 0b1000'0000u);
        return dest;
    }

    template <typename OutputIt>
    constexpr OutputIt encode_3_byte_sequence(const u32 cp, OutputIt dest,
                                              promise::is_valid_t) noexcept
    {
        snn_should(cp >= codepoint::first_in_3_byte_sequence &&
                   cp < codepoint::first_in_4_byte_sequence);
        // Binary UTF-8:        1110aaaa 10bbbbbb 10cccccc
        // Binary code point:            aaaabbbb bbcccccc
        *(dest++) = static_cast<char>((cp >> 12u) | 0b1110'0000u);
        *(dest++) = static_cast<char>(((cp >> 6u) & 0b0011'1111u) | 0b1000'0000u);
        *(dest++) = static_cast<char>((cp & 0b0011'1111u) | 0b1000'0000u);
        return dest;
    }

    template <typename OutputIt>
    constexpr OutputIt encode_4_byte_sequence(const u32 cp, OutputIt dest,
                                              promise::is_valid_t) noexcept
    {
        snn_should(cp >= codepoint::first_in_4_byte_sequence && cp <= unicode::codepoint::max);
        // Binary UTF-8:        11110aaa 10bbbbbb 10cccccc 10dddddd
        // Binary code point:            000aaabb bbbbcccc ccdddddd
        *(dest++) = static_cast<char>((cp >> 18u) | 0b1111'0000u);
        *(dest++) = static_cast<char>(((cp >> 12u) & 0b0011'1111u) | 0b1000'0000u);
        *(dest++) = static_cast<char>(((cp >> 6u) & 0b0011'1111u) | 0b1000'0000u);
        *(dest++) = static_cast<char>((cp & 0b0011'1111u) | 0b1000'0000u);
        return dest;
    }

    // ### encode_up_to_4_bytes

    template <typename OutputIt>
    [[nodiscard]] constexpr OutputIt encode_up_to_4_bytes(const u32 cp, OutputIt dest,
                                                          promise::is_valid_t) noexcept
    {
        snn_should(is_valid(cp));
        if (cp < codepoint::first_in_2_byte_sequence)
        {
            *(dest++) = static_cast<char>(cp);
            return dest;
        }
        if (cp < codepoint::first_in_3_byte_sequence)
        {
            return encode_2_byte_sequence(cp, dest, promise::is_valid);
        }
        if (cp < codepoint::first_in_4_byte_sequence)
        {
            return encode_3_byte_sequence(cp, dest, promise::is_valid);
        }
        return encode_4_byte_sequence(cp, dest, promise::is_valid);
    }

    SNN_DIAGNOSTIC_POP

    // ## Utility functions

    // ### replacement_string

    [[nodiscard]] constexpr auto replacement_string() noexcept -> const char (&)[4]
    {
        return "\xEF\xBF\xBD"; // U+FFFD
    }

    template <typename String>
    [[nodiscard]] constexpr String replacement_string() noexcept(
        std::is_nothrow_copy_constructible_v<String>)
    {
        return String{not_null<const char*>{"\xEF\xBF\xBD"}, usize{3}}; // U+FFFD
    }
}
