// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/encode.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // Valid
            snn_require(utf8::encode(0x20) == " ");
            snn_require(utf8::encode(0xE5) == "å");
            snn_require(utf8::encode(0x1F34A) == "\xF0\x9F\x8D\x8A");

            // Invalid
            snn_require(utf8::encode(constant::limit<u32>::max) == "\xEF\xBF\xBD"); // U+FFFD

            return true;
        }

        constexpr bool test_encode()
        {
            {
                array<char, 4> buffer;
                // LATIN CAPITAL LETTER A
                snn_require(utf8::encode(0x41, buffer, assume::is_valid) == "\x41");
                // COMBINING RING ABOVE
                snn_require(utf8::encode(0x30A, buffer, assume::is_valid) == "\xCC\x8A");
                // EURO SIGN
                snn_require(utf8::encode(0x20AC, buffer, assume::is_valid) == "\xE2\x82\xAC");
                // TANGERINE
                snn_require(utf8::encode(0x1F34A, buffer, assume::is_valid) == "\xF0\x9F\x8D\x8A");
            }

            {
                array<char, 4> buffer;
                // COMBINING RING ABOVE
                snn_require(utf8::encode(0x30A, buffer) == "\xCC\x8A");
                // Invalid
                snn_require(utf8::encode(0xFFFFFF, buffer) == "\xEF\xBF\xBD"); // U+FFFD
            }

            {
                // LATIN CAPITAL LETTER A
                snn_require(utf8::encode(0x41, assume::is_valid) == "\x41");
                // COMBINING RING ABOVE
                snn_require(utf8::encode(0x30A, assume::is_valid) == "\xCC\x8A");
                // EURO SIGN
                snn_require(utf8::encode(0x20AC, assume::is_valid) == "\xE2\x82\xAC");
                // TANGERINE
                snn_require(utf8::encode(0x1F34A, assume::is_valid) == "\xF0\x9F\x8D\x8A");
            }

            {
                // COMBINING RING ABOVE
                snn_require(utf8::encode(0x30A) == "\xCC\x8A");
                // Invalid
                snn_require(utf8::encode(0xFFFFFF) == "\xEF\xBF\xBD"); // U+FFFD
            }

            {
                str append_to;
                // LATIN CAPITAL LETTER A
                snn_require(utf8::encode(0x41, append_to, assume::is_valid) == "\x41");
                snn_require(append_to == "\x41");
                // COMBINING RING ABOVE
                snn_require(utf8::encode(0x30A, append_to, assume::is_valid) == "\xCC\x8A");
                snn_require(append_to == "\x41\xCC\x8A");
                // EURO SIGN
                snn_require(utf8::encode(0x20AC, append_to, assume::is_valid) == "\xE2\x82\xAC");
                snn_require(append_to == "\x41\xCC\x8A\xE2\x82\xAC");
                // TANGERINE
                snn_require(utf8::encode(0x1F34A, append_to, assume::is_valid) ==
                            "\xF0\x9F\x8D\x8A");
                snn_require(append_to == "\x41\xCC\x8A\xE2\x82\xAC\xF0\x9F\x8D\x8A");
            }

            {
                str append_to;
                // COMBINING RING ABOVE
                snn_require(utf8::encode(0x30A, append_to) == "\xCC\x8A");
                snn_require(append_to == "\xCC\x8A");
                // Invalid
                snn_require(utf8::encode(0xFFFFFF, append_to) == "\xEF\xBF\xBD"); // U+FFFD
                snn_require(append_to == "\xCC\x8A\xEF\xBF\xBD");
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_encode());
    }
}
