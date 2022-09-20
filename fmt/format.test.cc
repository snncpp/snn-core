// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fmt/format.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/pair/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // Positional arguments.
            snn_require(fmt::format("{}", "Hello") == "Hello");
            snn_require(fmt::format("{} {}", "Hello", "world!") == "Hello world!");

            // Indexed arguments.
            snn_require(fmt::format("{1} {0}", "Hello", "world!") == "world! Hello");

            // Escaped curly brackets.
            snn_require(fmt::format("Escaped {{") == "Escaped {");
            snn_require(fmt::format("Escaped }}") == "Escaped }");
            snn_require(fmt::format("Escaped {{0}}") == "Escaped {0}");

            // Width, left aligned (the default), with and without index.
            snn_require(fmt::format("{0,10}", "Hello") == "Hello     ");
            snn_require(fmt::format("{,10}", "Hello") == "Hello     ");
            snn_require(fmt::format("{0,10<}", "Hello") == "Hello     ");
            snn_require(fmt::format("{,10<}", "Hello") == "Hello     ");

            // Width as an argument.
            snn_require(fmt::format("{,{}}", "Hello", 10) == "Hello     ");
            snn_require(fmt::format("{,{}<}", "Hello", 10) == "Hello     ");

            // Width, right aligned, with and without index.
            snn_require(fmt::format("{0,10>}", "Hello") == "     Hello");
            snn_require(fmt::format("{,10>}", "Hello") == "     Hello");

            // Width, center aligned, with and without index.
            snn_require(fmt::format("{0,10^}", "Hello") == "  Hello   ");
            snn_require(fmt::format("{,10^}", "Hello") == "  Hello   ");

            // Width, with a fill string (the string must not contain ':' or '}').
            snn_require(fmt::format("{,15<-~}", "Hello") == "Hello-~-~-~-~-~");
            snn_require(fmt::format("{,15^-~}", "Hello") == "-~-~-Hello-~-~-");

            // Width and fill string as arguments (now the fill string can contain all characters).
            snn_require(fmt::format("{,{}^{}}", "Hello", 13, "~::~") == "~::~Hello~::~");

            // Type specific format string (everything after ':'), here `formatter<i64>` in
            // `fmt/integral.hh` is selected and passed the string `+#8b'4`.
            snn_require(fmt::format("{:+#8b'4}", 123) == "+0b0111'1011");

            // Width and type specific format string combined.
            snn_require(fmt::format("{0,20^-~:+#8b'4}", 123) == "-~-~+0b0111'1011-~-~");

            // See `test_integral_format_string()` below for integral formatting.

            return true;
        }

        constexpr bool test_format()
        {
            snn_require(fmt::format("") == "");

            snn_require(fmt::format("Hello") == "Hello");
            snn_require(fmt::format("{}", "Hello") == "Hello");
            snn_require(fmt::format("{0}", "Hello") == "Hello");
            snn_require(fmt::format("{} {}", "Hello", "world!") == "Hello world!");
            snn_require(fmt::format("{0} {1}", "Hello", "world!") == "Hello world!");

            snn_require(fmt::format("{0} {0}", "Hello", "world!") == "Hello Hello");
            snn_require(fmt::format("{1} {1}", "Hello", "world!") == "world! world!");
            snn_require(fmt::format("{0} {}", "Hello", "world!") == "Hello Hello");
            snn_require(fmt::format("{} {0}", "Hello", "world!") == "Hello Hello");

            snn_require(fmt::format("Escaped {{") == "Escaped {");
            snn_require(fmt::format("Escaped }}") == "Escaped }");
            snn_require(fmt::format("Escaped {{0}}") == "Escaped {0}");

            using fmt::error;

            snn_require_throws_code(fmt::format("{}"), error::argument_index_out_of_bounds);
            snn_require_throws_code(fmt::format("{0}"), error::argument_index_out_of_bounds);
            snn_require_throws_code(fmt::format("{1}", "..."), error::argument_index_out_of_bounds);
            snn_require_throws_code(fmt::format("{"), error::unexpected_end_of_format_string);
            snn_require_throws_code(fmt::format("}"), error::unexpected_character_in_format_string);

            snn_require_throws_code(fmt::format("The answer is {00}.", "..."),
                                    error::invalid_index);
            snn_require_throws_code(fmt::format("The answer is {999}.", "..."),
                                    error::invalid_index);

            snn_require(fmt::format("The answer is {}.", true) == "The answer is true.");
            snn_require(fmt::format("The answer is {}.", false) == "The answer is false.");
            snn_require(fmt::format("The answer is {}.", '?') == "The answer is ?.");
            snn_require(fmt::format("The answer is {}.", 42) == "The answer is 42.");
            snn_require(fmt::format("The answer is {}.", 42, 41u) == "The answer is 42.");
            snn_require(fmt::format("The answer is {} or {}.", 42, 41u) ==
                        "The answer is 42 or 41.");
            snn_require(fmt::format("The answer is forty-two.", 42) == "The answer is forty-two.");

            snn_require(fmt::format("The answer is {{{}}} or {}.", 42, "forty-one") ==
                        "The answer is {42} or forty-one.");
            snn_require(fmt::format<strbuf>("The answer is {} or {}.", 42, "forty-one") ==
                        "The answer is 42 or forty-one.");

            if (!std::is_constant_evaluated())
            {
                constexpr pair::value_count<cstrview, u32> vc{"abc", 123};
                snn_require(fmt::format("{}", vc) == "(abc, 123)");

                constexpr array abc{'a', 'b', 'c', 'd', 'e', 'f'};
                static_assert(abc.count() == 6);
                snn_require(fmt::format("{}", abc) == "(a, b, c, ...)");
            }

            // Width, alignment & fill.

            snn_require(fmt::format("{,10}", "Hello") == "Hello     ");
            snn_require(fmt::format("{,10<}", "Hello") == "Hello     ");
            snn_require(fmt::format("{,10>}", "Hello") == "     Hello");
            snn_require(fmt::format("{,10^}", "Hello") == "  Hello   ");

            snn_require(fmt::format("{,{}}", "Hello", 10) == "Hello     ");
            snn_require(fmt::format("{,{}<}", "Hello", 10) == "Hello     ");
            snn_require(fmt::format("{,{}>}", "Hello", 10) == "     Hello");
            snn_require(fmt::format("{,{}^}", "Hello", 10) == "  Hello   ");

            snn_require(fmt::format("{0,{1}}", "Hello", 10) == "Hello     ");
            snn_require(fmt::format("{0,{1}<}", "Hello", 10) == "Hello     ");
            snn_require(fmt::format("{0,{1}>}", "Hello", 10) == "     Hello");
            snn_require(fmt::format("{0,{1}^}", "Hello", 10) == "  Hello   ");

            snn_require(fmt::format("{,10<-~}", "Hello") == "Hello-~-~-");
            snn_require(fmt::format("{,10>-~}", "Hello") == "-~-~-Hello");
            snn_require(fmt::format("{,10^-~}", "Hello") == "-~Hello-~-");

            snn_require(fmt::format("{,{}<-~}", "Hello", 10) == "Hello-~-~-");
            snn_require(fmt::format("{,{}>-~}", "Hello", 10) == "-~-~-Hello");
            snn_require(fmt::format("{,{}^-~}", "Hello", 10) == "-~Hello-~-");

            snn_require(fmt::format("{0,{1}<-~}", "Hello", 10) == "Hello-~-~-");
            snn_require(fmt::format("{0,{1}>-~}", "Hello", 10) == "-~-~-Hello");
            snn_require(fmt::format("{0,{1}^-~}", "Hello", 10) == "-~Hello-~-");

            snn_require(fmt::format("{,{}<{}}", "Hello", 10, "-~") == "Hello-~-~-");
            snn_require(fmt::format("{0,{1}>{2}}", "Hello", 10, "-~") == "-~-~-Hello");
            snn_require(fmt::format("{0,{1}^{2}}", "Hello", 10, "-~") == "-~Hello-~-");

            snn_require_throws_code(fmt::format("{0,{}}", "Hello", 10),
                                    error::referenced_argument_must_be_an_integral);
            snn_require_throws_code(fmt::format("{,}", "..."),
                                    error::invalid_width_in_format_string);
            snn_require_throws_code(fmt::format("{,{}^}", "..."),
                                    error::argument_index_out_of_bounds);

            // Width is currently limited to 4 digits.
            snn_require_throws_code(fmt::format("{,99999}", "Hello"),
                                    error::invalid_width_in_format_string);
            snn_require_throws_code(fmt::format("{,{}}", "Hello", 99999),
                                    error::integral_argument_is_too_large);
            snn_require_throws_code(fmt::format("{,{}}", "Hello", -99),
                                    error::integral_argument_is_negative);

            // Example inspired by: https://fmt.dev/
            snn_require(fmt::format("┌{0,{2}^─}┐\n"
                                    "│{1,{2}^ }│\n"
                                    "└{0,{2}^─}┘\n",
                                    "", "Hello world!", 20) == "┌────────────────────┐\n"
                                                               "│    Hello world!    │\n"
                                                               "└────────────────────┘\n");

            return true;
        }

        constexpr bool test_format_append()
        {
            str append_to;

            fmt::format_append("{}", append_to, promise::no_overlap, "Hello");
            snn_require(append_to == "Hello");

            fmt::format_append(", the answer is {} or {}.", append_to, promise::no_overlap, 42, 41);
            snn_require(append_to == "Hello, the answer is 42 or 41.");

            return true;
        }

        constexpr bool test_integral_format_string()
        {
            // Indirectly test the default `formatter<Int>`.

            // Empty

            snn_require(fmt::format("{:}", 123) == "123");
            snn_require(fmt::format("{:}", -123) == "-123");

            // Sign

            snn_require(fmt::format("{: }", 123) == " 123");
            snn_require(fmt::format("{:+}", 123) == "+123");
            snn_require(fmt::format("{: }", -123) == "-123");
            snn_require(fmt::format("{:+}", -123) == "-123");

            // Base

            snn_require(fmt::format("{:d}", 123) == "123");
            snn_require(fmt::format("{:d}", -123) == "-123");
            snn_require(fmt::format("{:x}", 123) == "7b");
            snn_require(fmt::format("{:x}", -123) == "-7b");
            snn_require(fmt::format("{:X}", 123) == "7B");
            snn_require(fmt::format("{:X}", -123) == "-7B");
            snn_require(fmt::format("{:o}", 123) == "173");
            snn_require(fmt::format("{:o}", -123) == "-173");
            snn_require(fmt::format("{:b}", 123) == "1111011");
            snn_require(fmt::format("{:b}", -123) == "-1111011");

            // Show prefix.

            snn_require(fmt::format("{:#}", 123) == "123");   // No prefix with decimal.
            snn_require(fmt::format("{:#}", -123) == "-123"); // No prefix with decimal.

            // Show prefix with base.

            snn_require(fmt::format("{:#d}", 123) == "123");   // No prefix with decimal.
            snn_require(fmt::format("{:#d}", -123) == "-123"); // No prefix with decimal.
            snn_require(fmt::format("{:#x}", 123) == "0x7b");
            snn_require(fmt::format("{:#x}", -123) == "-0x7b");
            snn_require(fmt::format("{:#X}", 123) == "0x7B");
            snn_require(fmt::format("{:#X}", -123) == "-0x7B");
            snn_require(fmt::format("{:#o}", 123) == "0o173");
            snn_require(fmt::format("{:#o}", -123) == "-0o173");
            snn_require(fmt::format("{:#b}", 123) == "0b1111011");
            snn_require(fmt::format("{:#b}", -123) == "-0b1111011");

            // Minimum digits.

            snn_require(fmt::format("{:0}", 123) == "123");
            snn_require(fmt::format("{:0}", -123) == "-123");
            snn_require(fmt::format("{:3}", 123) == "123");
            snn_require(fmt::format("{:3}", -123) == "-123");
            snn_require(fmt::format("{:4}", 123) == "0123");
            snn_require(fmt::format("{:4}", -123) == "-0123");

            // Minimum digits & base.

            snn_require(fmt::format("{:4x}", 123) == "007b");
            snn_require(fmt::format("{:4x}", -123) == "-007b");
            snn_require(fmt::format("{:4X}", 123) == "007B");
            snn_require(fmt::format("{:4X}", -123) == "-007B");
            snn_require(fmt::format("{:4o}", 123) == "0173");
            snn_require(fmt::format("{:4o}", -123) == "-0173");
            snn_require(fmt::format("{:8b}", 123) == "01111011");
            snn_require(fmt::format("{:8b}", -123) == "-01111011");

            // Group separator (can be anything except digits).
            // Digits per group default to 3.

            snn_require(fmt::format("{:,}", 1234) == "1,234");

            // Some separators require show-prefix/base options.
            snn_require(fmt::format("{: }", 1234) == " 1234"); // Sign
            snn_require(fmt::format("{:d }", 1234) == "1 234");
            snn_require(fmt::format("{:#}", 1234) == "1234"); // Show prefix.
            snn_require(fmt::format("{:##}", 1234) == "1#234");
            snn_require(fmt::format("{:d#}", 1234) == "1#234");
            snn_require(fmt::format("{:d}", 1234) == "1234"); // Base
            snn_require(fmt::format("{:dd}", 1234) == "1d234");

            snn_require(fmt::format("{:d }", 123) == "123");
            snn_require(fmt::format("{:d }", -123) == "-123");
            snn_require(fmt::format("{:d }", 1234) == "1 234");
            snn_require(fmt::format("{:d }", -1234) == "-1 234");
            snn_require(fmt::format("{:x }", 123) == "7b");
            snn_require(fmt::format("{:x }", -123) == "-7b");
            snn_require(fmt::format("{:X }", 123) == "7B");
            snn_require(fmt::format("{:X }", -123) == "-7B");
            snn_require(fmt::format("{:x }", 123456) == "1e 240");
            snn_require(fmt::format("{:x }", -123456) == "-1e 240");
            snn_require(fmt::format("{:X }", 123456) == "1E 240");
            snn_require(fmt::format("{:X }", -123456) == "-1E 240");
            snn_require(fmt::format("{:o }", 123) == "173");
            snn_require(fmt::format("{:o }", -123) == "-173");
            snn_require(fmt::format("{:o }", 1234) == "2 322");
            snn_require(fmt::format("{:o }", -1234) == "-2 322");
            snn_require(fmt::format("{:b }", 123) == "1 111 011");
            snn_require(fmt::format("{:b }", -123) == "-1 111 011");

            snn_require(fmt::format("{:, }", 123) == "123");
            snn_require(fmt::format("{:, }", -123) == "-123");
            snn_require(fmt::format("{:, }", 1234) == "1, 234");
            snn_require(fmt::format("{:, }", -1234) == "-1, 234");
            snn_require(fmt::format("{:x, }", 123) == "7b");
            snn_require(fmt::format("{:x, }", -123) == "-7b");
            snn_require(fmt::format("{:X, }", 123) == "7B");
            snn_require(fmt::format("{:X, }", -123) == "-7B");
            snn_require(fmt::format("{:x, }", 123456) == "1e, 240");
            snn_require(fmt::format("{:x, }", -123456) == "-1e, 240");
            snn_require(fmt::format("{:X, }", 123456) == "1E, 240");
            snn_require(fmt::format("{:X, }", -123456) == "-1E, 240");
            snn_require(fmt::format("{:o, }", 123) == "173");
            snn_require(fmt::format("{:o, }", -123) == "-173");
            snn_require(fmt::format("{:o, }", 1234) == "2, 322");
            snn_require(fmt::format("{:o, }", -1234) == "-2, 322");
            snn_require(fmt::format("{:b, }", 123) == "1, 111, 011");
            snn_require(fmt::format("{:b, }", -123) == "-1, 111, 011");

            // Group separator & digits per group.

            snn_require(fmt::format("{:,2}", 123) == "1,23");
            snn_require(fmt::format("{:,2}", -123) == "-1,23");
            snn_require(fmt::format("{:,2}", 1234) == "12,34");
            snn_require(fmt::format("{:,2}", -1234) == "-12,34");

            snn_require(fmt::format("{:d 2}", 123) == "1 23");
            snn_require(fmt::format("{:d 2}", -123) == "-1 23");
            snn_require(fmt::format("{:x 2}", 123) == "7b");
            snn_require(fmt::format("{:x 2}", -123) == "-7b");
            snn_require(fmt::format("{:X 2}", 123) == "7B");
            snn_require(fmt::format("{:X 2}", -123) == "-7B");
            snn_require(fmt::format("{:x 2}", 123456) == "1 e2 40");
            snn_require(fmt::format("{:x 2}", -123456) == "-1 e2 40");
            snn_require(fmt::format("{:X 2}", 123456) == "1 E2 40");
            snn_require(fmt::format("{:X 2}", -123456) == "-1 E2 40");
            snn_require(fmt::format("{:o 2}", 123) == "1 73");
            snn_require(fmt::format("{:o 2}", -123) == "-1 73");
            snn_require(fmt::format("{:b 2}", 123) == "1 11 10 11");
            snn_require(fmt::format("{:b 2}", -123) == "-1 11 10 11");

            snn_require(fmt::format("{:d 1}", 123) == "1 2 3");
            snn_require(fmt::format("{:d 1}", -123) == "-1 2 3");
            snn_require(fmt::format("{:x 1}", 123) == "7 b");
            snn_require(fmt::format("{:x 1}", -123) == "-7 b");
            snn_require(fmt::format("{:X 1}", 123) == "7 B");
            snn_require(fmt::format("{:X 1}", -123) == "-7 B");
            snn_require(fmt::format("{:x 1}", 123456) == "1 e 2 4 0");
            snn_require(fmt::format("{:x 1}", -123456) == "-1 e 2 4 0");
            snn_require(fmt::format("{:X 1}", 123456) == "1 E 2 4 0");
            snn_require(fmt::format("{:X 1}", -123456) == "-1 E 2 4 0");
            snn_require(fmt::format("{:o 1}", 123) == "1 7 3");
            snn_require(fmt::format("{:o 1}", -123) == "-1 7 3");
            snn_require(fmt::format("{:b 1}", 123) == "1 1 1 1 0 1 1");
            snn_require(fmt::format("{:b 1}", -123) == "-1 1 1 1 0 1 1");

            snn_require(fmt::format("{:d 0}", 123) == "123");
            snn_require(fmt::format("{:d 0}", -123) == "-123");
            snn_require(fmt::format("{:x 0}", 123) == "7b");
            snn_require(fmt::format("{:x 0}", -123) == "-7b");
            snn_require(fmt::format("{:X 0}", 123) == "7B");
            snn_require(fmt::format("{:X 0}", -123) == "-7B");
            snn_require(fmt::format("{:x 0}", 123456) == "1e240");
            snn_require(fmt::format("{:x 0}", -123456) == "-1e240");
            snn_require(fmt::format("{:X 0}", 123456) == "1E240");
            snn_require(fmt::format("{:X 0}", -123456) == "-1E240");
            snn_require(fmt::format("{:o 0}", 123) == "173");
            snn_require(fmt::format("{:o 0}", -123) == "-173");
            snn_require(fmt::format("{:b 0}", 123) == "1111011");
            snn_require(fmt::format("{:b 0}", -123) == "-1111011");

            // Combined

            snn_require(fmt::format("{:+#8b'4}", 123) == "+0b0111'1011");
            snn_require(fmt::format("{:+#8b'4}", -123) == "-0b0111'1011");

            snn_require(fmt::format("{:{}}", 123, "+#8b'4") == "+0b0111'1011");
            snn_require(fmt::format("{:{}}", -123, "+#8b'4") == "-0b0111'1011");

            snn_require(fmt::format("{,10>_:d }", 123456) == "___123 456");
            snn_require(fmt::format("{,10>_:9d }", 123456) == "000 123 456");

            snn_require(fmt::format("\\x{:4X\\x2}", u16{1234}) == "\\x04\\xD2");

#if SNN_INT128_BOOL
            snn_require(fmt::format("{,60>:+0 }\n"
                                    "{,60>:+0 }\n",
                                    constant::limit<i128>::min, constant::limit<i128>::max) ==
                        "        -170 141 183 460 469 231 731 687 303 715 884 105 728\n"
                        "        +170 141 183 460 469 231 731 687 303 715 884 105 727\n");
#endif

            // Invalid

            // Minimum digits is currently limited to 4 digits.
            snn_require_throws_code(fmt::format("{:99999}", 123),
                                    fmt::error::invalid_minimum_digits_in_format_string);
            // Digits per group is currently limited to 2 digits.
            snn_require_throws_code(fmt::format("{:d 999}", 123),
                                    fmt::error::invalid_digits_per_group_in_format_string);
            // Trailing character.
            snn_require_throws_code(fmt::format("{:d 3x}", 123),
                                    fmt::error::unexpected_character_in_format_string);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_format());
        snn_static_require(app::test_format_append());
        snn_static_require(app::test_integral_format_string());
    }
}
