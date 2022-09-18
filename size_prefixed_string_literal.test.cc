// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/size_prefixed_string_literal.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // First byte of the string literal is the size (here as an octal escape sequence).
            size_prefixed_string_literal abc = "\003abc";
            snn_require(abc.view().size() == 3);
            snn_require(abc.view() == "abc");
            snn_require(abc == "abc");
            snn_require(abc != "\003abc");
            snn_require(algo::is_equal(abc.range(), {'a', 'b', 'c'}));

            // Implicit conversion.
            cstrview alt = abc;
            snn_require(alt == "abc");

            size_prefixed_string_literal empty{"\0"};
            snn_require(empty.view().size() == 0);
            snn_require(empty.view() == "");
            snn_require(empty == "");
            snn_require(empty != "\0");
            snn_require(empty.range().is_empty());

            return true;
        }

        constexpr bool test_size_prefixed_string_literal()
        {
            // Non-ascii (also the constructor isn't explicit).
            constexpr size_prefixed_string_literal non_ascii = {"\006ÅÄÖ"};
            static_assert(non_ascii.view().size() == 6);
            static_assert(non_ascii.view() == "ÅÄÖ");

            // Octal 011 is 9 in decimal.
            constexpr size_prefixed_string_literal with_zero_byte = {"\011with\0byte"};
            static_assert(with_zero_byte.view().size() == 9);
            static_assert(with_zero_byte.view() == "with\0byte");

            // `size_prefixed_string_literal` supports string literals up to 255 bytes in size.
            // Octal 377 is 255 in decimal.
            constexpr size_prefixed_string_literal max = {
                "\377aaaaaaaaaaaaaaa"
                "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
                "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
                "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"};
            static_assert(max.view().size() == 255);
            static_assert(
                max.view() ==
                "aaaaaaaaaaaaaaa"
                "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
                "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"
                "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd");

            // Comparison
            {
                size_prefixed_string_literal a{"\003ABC"};
                size_prefixed_string_literal b{"\003ABC"};
                size_prefixed_string_literal c{"\003abc"};
                snn_require(a == "ABC");
                snn_require("ABC" == b);
                snn_require(a == b);
                snn_require(a != c);
                snn_require(c != a);
                snn_require(!(a < b));
                snn_require(a < c);
            }
            {
                size_prefixed_string_literal a{"\003ABC"};
                cstrview b{"ABC"};
                str c{"abc"};
                snn_require(a == b);
                snn_require(b == a);
                snn_require(a != c);
                snn_require(c != a);
                snn_require(!(a < b));
                snn_require(a < c);
            }

            static_assert(std::is_standard_layout_v<size_prefixed_string_literal>);
            static_assert(sane<size_prefixed_string_literal>);
            static_assert(is_trivially_relocatable_v<size_prefixed_string_literal>);
            static_assert(sizeof(size_prefixed_string_literal) == sizeof(const char*));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_size_prefixed_string_literal());
    }
}
