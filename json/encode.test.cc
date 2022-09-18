// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/json/encode.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(json::encode("") == R"("")");
            snn_require(json::encode("abc") == R"("abc")");
            snn_require(json::encode("One Two 123") == R"("One Two 123")");
            snn_require(json::encode("a\0c") == R"("a\u0000c")");
            snn_require(json::encode("One\r\nTwo") == R"("One\r\nTwo")");
            snn_require(json::encode("\"\\\b\f\n\r\t") == R"("\"\\\b\f\n\r\t")");

            snn_require(json::encode("<p>&nbsp;") == R"("<p>&nbsp;")");
            snn_require(json::encode("<p>&nbsp;", json::option::escape_less_than) ==
                        R"("\u003cp>&nbsp;")");

            return true;
        }

        constexpr bool test_encode()
        {
            snn_require(json::encode("") == R"("")");
            snn_require(json::encode(" ") == R"(" ")");
            snn_require(json::encode("a") == R"("a")");
            snn_require(json::encode("abc") == R"("abc")");
            snn_require(json::encode("One Two 123") == R"("One Two 123")");

            {
                str append_to = ", ";
                json::encode("abc", json::option::none, append_to, promise::no_overlap);
                snn_require(append_to == R"(, "abc")");
            }

            snn_require(json::encode("a\0c") == R"("a\u0000c")");
            snn_require(json::encode("One\r\nTwo") == R"("One\r\nTwo")");
            snn_require(json::encode("\"\\\b\f\n\r\t") == R"("\"\\\b\f\n\r\t")");

            // UTF-8 BMP.
            snn_require(json::encode("åäöÅÄ\xc3\x96") == R"("åäöÅÄÖ")");
            snn_require(json::encode("ÅÄÖ123") == R"("ÅÄÖ123")");
            snn_require(json::encode("123ÅÄÖ") == R"("123ÅÄÖ")");
            snn_require(json::encode("One \xe2\x98\x83") == R"("One ☃")");

            // UTF-8 BMP special.
            snn_require(json::encode("\xe2\x80\xa8\xe2\x80\xa9") == R"("\u2028\u2029")");

            // UTF-8 outside BMP.
            snn_require(json::encode("\U0001F419") == R"("\ud83d\udc19")");
            snn_require(json::encode("\U0001F343") == R"("\ud83c\udf43")");
            snn_require(json::encode("\U0001F419\U0001F343") == R"("\ud83d\udc19\ud83c\udf43")");

            // UTF-8 mixed.
            snn_require(json::encode("\U0001F419 \U0001F343 ") ==
                        R"("\ud83d\udc19 \ud83c\udf43 ")");
            snn_require(json::encode("ÅÄÖ\U0001F419123") == R"("ÅÄÖ\ud83d\udc19123")");

            snn_require_throws_code(json::encode("\xe2"), json::error::invalid_utf8);
            snn_require_throws_code(json::encode("\x83"), json::error::invalid_utf8);
            snn_require_throws_code(json::encode("\xe2\x80"), json::error::invalid_utf8);
            snn_require_throws_code(json::encode("One\xe2Two"), json::error::invalid_utf8);
            snn_require_throws_code(json::encode("\xf0 \x9f \x8d \x8a"), json::error::invalid_utf8);

            // Options

            snn_require(json::encode("<p>&nbsp;") == R"("<p>&nbsp;")");

            snn_require(json::encode("<p>&nbsp;", json::option::escape_ampersand) ==
                        R"("<p>\u0026nbsp;")");
            snn_require(json::encode("<p>&nbsp;", json::option::escape_greater_than) ==
                        R"("<p\u003e&nbsp;")");
            snn_require(json::encode("<p>&nbsp;", json::option::escape_less_than) ==
                        R"("\u003cp>&nbsp;")");

            snn_require(json::encode("<p>&nbsp;", json::option::escape_amp |
                                                      json::option::escape_gt |
                                                      json::option::escape_lt) ==
                        R"("\u003cp\u003e\u0026nbsp;")");

            // Both of these will break the script block:
            // <script type="text/javascript">
            //     var a = "</script>";    // Will close early.
            //     var b = "<!--<script>"; // Will consume past the </script> end tag.
            // </script>
            snn_require(json::encode("</script>") == R"("</script>")");

            // This is solved by escaping '<':
            snn_require(json::encode("</script>", json::option::escape_lt) ==
                        R"("\u003c/script>")");

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

        static_assert(to_underlying(json::option::escape_ampersand) == 1);
        static_assert(to_underlying(json::option::escape_amp) == 1);

        static_assert(to_underlying(json::option::escape_greater_than) == 2);
        static_assert(to_underlying(json::option::escape_gt) == 2);

        static_assert(to_underlying(json::option::escape_less_than) == 4);
        static_assert(to_underlying(json::option::escape_lt) == 4);
    }
}
