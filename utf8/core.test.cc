// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/core.hh"

#include "snn-core/unittest.hh"
#include "snn-core/range/integral.hh"
#include "snn-core/range/step.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(utf8::invalid == to_u32(-1));

            snn_require(utf8::is_valid(0x20));
            snn_require(utf8::is_valid(0x1F343));
            snn_require(!utf8::is_valid(0xD800));
            snn_require(!utf8::is_valid(utf8::invalid));

            snn_require(utf8::replace_if_invalid(0x20) == 0x20);
            snn_require(utf8::replace_if_invalid(0x1F343) == 0x1F343);
            snn_require(utf8::replace_if_invalid(0xDC00) == 0xFFFD);   // Replaced
            snn_require(utf8::replace_if_invalid(0xFFFFFF) == 0xFFFD); // Replaced

            snn_require(utf8::replacement_string<cstrview>() == "\xEF\xBF\xBD"); // U+FFFD

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        // Valid
        static_assert(utf8::replace_if_invalid(0x20) == 0x20);
        static_assert(utf8::replace_if_invalid(0x1F343) == 0x1F343);
        static_assert(utf8::replace_if_invalid(0x10FFFF) == 0x10FFFF);

        // Invalid
        static_assert(utf8::replace_if_invalid(0xDC00) == 0xFFFD);
        static_assert(utf8::replace_if_invalid(0xFFFFFF) == 0xFFFD);
        static_assert(utf8::replace_if_invalid(constant::limit<u32>::max) == 0xFFFD);

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        static_assert(string_size(utf8::replacement_string()) == 3);
        static_assert(utf8::replacement_string()[0] == '\xEF');
        static_assert(utf8::replacement_string()[1] == '\xBF');
        static_assert(utf8::replacement_string()[2] == '\xBD');
        static_assert(utf8::replacement_string()[3] == '\0');

        SNN_DIAGNOSTIC_POP

        static_assert(utf8::replacement_string<cstrview>().size() == 3);
        static_assert(utf8::replacement_string<cstrview>() == "\xEF\xBF\xBD");

        // is_?_byte_sequence, is_continuation
        {
            usize count = 0;
            for (const auto c : range::integral<char>{})
            {
                const auto b = to_byte(c);

                if (b <= 0x7F)
                {
                    snn_require(utf8::is_1_byte_sequence(c));
                    snn_require(!utf8::is_2_byte_sequence(c));
                    snn_require(!utf8::is_3_byte_sequence(c));
                    snn_require(!utf8::is_4_byte_sequence(c));
                    snn_require(!utf8::is_continuation(c));
                }
                else if (b <= 0xBF)
                {
                    snn_require(!utf8::is_1_byte_sequence(c));
                    snn_require(!utf8::is_2_byte_sequence(c));
                    snn_require(!utf8::is_3_byte_sequence(c));
                    snn_require(!utf8::is_4_byte_sequence(c));
                    snn_require(utf8::is_continuation(c));
                }
                else if (b >= 0xC2 && b <= 0xDF)
                {
                    snn_require(!utf8::is_1_byte_sequence(c));
                    snn_require(utf8::is_2_byte_sequence(c));
                    snn_require(!utf8::is_3_byte_sequence(c));
                    snn_require(!utf8::is_4_byte_sequence(c));
                    snn_require(!utf8::is_continuation(c));
                }
                else if (b >= 0xE0 && b <= 0xEF)
                {
                    snn_require(!utf8::is_1_byte_sequence(c));
                    snn_require(!utf8::is_2_byte_sequence(c));
                    snn_require(utf8::is_3_byte_sequence(c));
                    snn_require(!utf8::is_4_byte_sequence(c));
                    snn_require(!utf8::is_continuation(c));
                }
                else if (b >= 0xF0 && b <= 0xF4)
                {
                    snn_require(!utf8::is_1_byte_sequence(c));
                    snn_require(!utf8::is_2_byte_sequence(c));
                    snn_require(!utf8::is_3_byte_sequence(c));
                    snn_require(utf8::is_4_byte_sequence(c));
                    snn_require(!utf8::is_continuation(c));
                }
                else
                {
                    snn_require(!utf8::is_1_byte_sequence(c));
                    snn_require(!utf8::is_2_byte_sequence(c));
                    snn_require(!utf8::is_3_byte_sequence(c));
                    snn_require(!utf8::is_4_byte_sequence(c));
                    snn_require(!utf8::is_continuation(c));
                }

                ++count;
            }
            snn_assert(count == 256);
        }

        // is_valid
        {
            usize count                          = 0;
            constexpr u32 codepoint_max_plus_one = unicode::codepoint::max + 1;
            for (const auto cp : range::step<u32>{0, codepoint_max_plus_one + 1})
            {
                if (cp >= 0xD800 && cp <= 0xDFFF)
                {
                    snn_require(!utf8::is_valid(cp));
                }
                else if (cp <= 0x10FFFF)
                {
                    snn_require(utf8::is_valid(cp));
                }
                else
                {
                    snn_require(cp == codepoint_max_plus_one);
                    snn_require(!utf8::is_valid(cp));
                }

                ++count;
            }
            snn_require(count == 1'114'113);
        }

        // decode_multibyte_sequence

        using utf8::decode_multibyte_sequence;
        using utf8::invalid;
        using vc = pair::value_count<u32>;

        static_assert(decode_multibyte_sequence(cstrrng{""}) == vc{invalid, 0});

        // "COMBINING RING ABOVE" (U+030A)
        static_assert(decode_multibyte_sequence(cstrrng{"\xCC\x8A"}) == vc{0x30A, 2});
        static_assert(decode_multibyte_sequence(cstrrng{"\xCC\x00"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xCC\xFF"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xCC"}) == vc{invalid, 1});

        // "EURO SIGN" (U+20AC)
        static_assert(decode_multibyte_sequence(cstrrng{"\xE2\x82\xAC"}) == vc{0x20AC, 3});
        static_assert(decode_multibyte_sequence(cstrrng{"\xE2\x00\xAC"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xE2\x82\x00"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xE2\xFF\xAC"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xE2\x82\xFF"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xE2\x82"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xE2"}) == vc{invalid, 1});

        // "REPLACEMENT CHARACTER" (U+FFFD)
        static_assert(decode_multibyte_sequence(utf8::replacement_string<cstrview>().range()) ==
                      vc{0xFFFD, 3});

        // "TANGERINE" (U+1F34A)
        static_assert(decode_multibyte_sequence(cstrrng{"\xF0\x9F\x8D\x8A"}) == vc{0x1F34A, 4});
        static_assert(decode_multibyte_sequence(cstrrng{"\xF0\x00\x8D\x8A"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xF0\x9F\x00\x8A"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xF0\x9F\x8D\x00"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xF0\xFF\x8D\x8A"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xF0\x9F\xFF\x8A"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xF0\x9F\x8D\xFF"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xF0\x9F\x8D"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xF0\x9F"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xF0"}) == vc{invalid, 1});

        // U+10FFFF
        static_assert(decode_multibyte_sequence(cstrrng{"\xF4\x8F\xBF\xBF"}) == vc{0x10FFFF, 4});

        // Overlong - "QUESTION MARK" (U+003F)
        static_assert(decode_multibyte_sequence(cstrrng{"\xC0\xBF"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xE0\x80\xBF"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xF0\x80\x80\xBF"}) == vc{invalid, 1});

        // Overlong - "COMBINING RING ABOVE" (U+030A)
        static_assert(decode_multibyte_sequence(cstrrng{"\xE0\x8C\x8A"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xF0\x80\x8C\x8A"}) == vc{invalid, 1});

        // Overlong - "EURO SIGN" (U+20AC)
        static_assert(decode_multibyte_sequence(cstrrng{"\xF0\x82\x82\xAC"}) == vc{invalid, 1});

        // Invalid - greater than U+10FFFF.
        static_assert(decode_multibyte_sequence(cstrrng{"\xF4\x90\x80\x80"}) == vc{invalid, 1});

        // Invalid - surrogates.
        static_assert(decode_multibyte_sequence(cstrrng{"\xED\xA0\x80"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xED\xAF\xBF"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xED\xB0\x80"}) == vc{invalid, 1});
        static_assert(decode_multibyte_sequence(cstrrng{"\xED\xBF\xBF"}) == vc{invalid, 1});

        // Invalid - ascii.
        static_assert(decode_multibyte_sequence(cstrrng{"\x32\x32\x32"}) == vc{invalid, 1});

        // Invalid - other.
        static_assert(decode_multibyte_sequence(cstrrng{"\xF5\xF5\xF5"}) == vc{invalid, 1});

        // encode_up_to_4_bytes

        array<char, 8> buffer;

        // "LATIN CAPITAL LETTER A" (U+0041)
        snn_require(utf8::encode_up_to_4_bytes(0x41, buffer.begin(), promise::is_valid) ==
                    buffer.view(1).begin());
        snn_require(buffer.view() == "\x41\0\0\0\0\0\0\0");

        // "COMBINING RING ABOVE" (U+030A)
        snn_require(utf8::encode_up_to_4_bytes(0x30A, buffer.begin(), promise::is_valid) ==
                    buffer.view(2).begin());
        snn_require(buffer.view() == "\xCC\x8A\0\0\0\0\0\0");

        // "EURO SIGN" (U+20AC)
        snn_require(utf8::encode_up_to_4_bytes(0x20AC, buffer.begin(), promise::is_valid) ==
                    buffer.view(3).begin());
        snn_require(buffer.view() == "\xE2\x82\xAC\0\0\0\0\0");

        // "TANGERINE" (U+1F34A)
        snn_require(utf8::encode_up_to_4_bytes(0x1F34A, buffer.begin(), promise::is_valid) ==
                    buffer.view(4).begin());
        snn_require(buffer.view() == "\xF0\x9F\x8D\x8A\0\0\0\0");
    }
}
