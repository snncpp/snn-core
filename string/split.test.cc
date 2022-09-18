// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/string/split.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                const cstrview s{"a,b,c"};
                auto v = string::split(s, ',');
                static_assert(std::is_same_v<decltype(v), vec<str>>);
                snn_require(v.count() == 3);
                snn_require(v.at(0).value() == "a");
                snn_require(v.at(1).value() == "b");
                snn_require(v.at(2).value() == "c");
            }
            {
                const cstrview s{",a,b,c,"};
                auto v = string::split<cstrview>(s, ',');
                static_assert(std::is_same_v<decltype(v), vec<cstrview>>);
                snn_require(v.count() == 5);
                snn_require(v.at(0).value() == "");
                snn_require(v.at(1).value() == "a");
                snn_require(v.at(2).value() == "b");
                snn_require(v.at(3).value() == "c");
                snn_require(v.at(4).value() == "");
            }
            {
                const cstrview s{",a,b,c,"};
                auto v = string::split<cstrview>(s, ",", string::empty_elements::discard);
                snn_require(v.count() == 3);
                snn_require(v.at(0).value() == "a");
                snn_require(v.at(1).value() == "b");
                snn_require(v.at(2).value() == "c");
            }
            {
                const cstrview s{",,a,,b,c,d,,"};
                constexpr usize limit = 3;
                auto v = string::split<cstrview>(s, ",", string::empty_elements::discard, limit);
                static_assert(std::is_same_v<decltype(v), vec<cstrview>>);
                snn_require(v.count() == 3);
                snn_require(v.at(0).value() == "a");
                snn_require(v.at(1).value() == "b");
                snn_require(v.at(2).value() == "c,d,,"); // Limit reached, remainder of subject.
            }

            return true;
        }

        constexpr bool test_split()
        {
            // split(const cstrview subject, const cstrview delimiter, ...)
            {
                // Empty delimiter.
                const cstrview s{"aaa"};
                auto v = string::split<cstrview>(s, "");
                snn_require(v.is_empty());
            }
            {
                // Empty subject.
                const cstrview s{""};
                auto v = string::split<cstrview>(s, "a");
                snn_require(v.is_empty());
            }
            {
                // Don't return empty elements.
                const cstrview s{"aaa"};
                auto v = string::split<cstrview>(s, "a", string::empty_elements::discard);
                snn_require(v.is_empty());
            }
            {
                // Empty elements.
                const cstrview s{"\n"};
                auto v = string::split<cstrview>(s, "\n");
                snn_require(v.count() == 2);
                snn_require(v.at(0).value() == "");
                snn_require(v.at(1).value() == "");
            }
            {
                // Empty elements, with limit.
                const cstrview s{"\n"};
                auto v = string::split<cstrview>(s, "\n", string::empty_elements::keep, 1);
                snn_require(v.count() == 1);
                snn_require(v.at(0).value() == "\n");
            }
            {
                // Empty elements.
                const cstrview s{"\n\n"};
                auto v = string::split<cstrview>(s, "\n");
                snn_require(v.at(0).value() == "");
                snn_require(v.at(1).value() == "");
                snn_require(v.at(2).value() == "");
                snn_require(v.count() == 3);
            }
            {
                // No matches.
                const cstrview s{"abc"};
                auto v = string::split<cstrview>(s, ",");
                snn_require(v.count() == 1);
                snn_require(v.at(0).value() == "abc");
            }
            {
                // No matches, with limit.
                const cstrview s{"abc"};
                auto v = string::split<cstrview>(s, ",", string::empty_elements::discard, 1);
                snn_require(v.count() == 1);
                snn_require(v.at(0).value() == "abc");
            }
            {
                // Basic, with limit.
                const cstrview s{"a,b,c"};
                auto v = string::split<cstrview>(s, ",", string::empty_elements::discard, 1);
                snn_require(v.count() == 1);
                snn_require(v.at(0).value() == "a,b,c");
            }
            {
                const cstrview s{"a,b,c"};
                auto v = string::split<cstrview>(s, "a");
                snn_require(v.count() == 2);
                snn_require(v.at(0).value() == "");
                snn_require(v.at(1).value() == ",b,c");
            }
            {
                // Basic, with limit.
                const cstrview s{"a,b,c"};
                auto v = string::split<cstrview>(s, ",", string::empty_elements::discard, 2);
                snn_require(v.count() == 2);
                snn_require(v.at(0).value() == "a");
                snn_require(v.at(1).value() == "b,c");
            }
            {
                const cstrview s{"a,b,c"};
                auto v = string::split<cstrview>(s, ",", string::empty_elements::discard, 3);
                snn_require(v.count() == 3);
                snn_require(v.at(0).value() == "a");
                snn_require(v.at(1).value() == "b");
                snn_require(v.at(2).value() == "c");
            }
            {
                const cstrview s{",,a,,b,c,d,,"};
                auto v = string::split<cstrview>(s, ",", string::empty_elements::discard);
                snn_require(v.count() == 4);
                snn_require(v.at(0).value() == "a");
                snn_require(v.at(1).value() == "b");
                snn_require(v.at(2).value() == "c");
                snn_require(v.at(3).value() == "d");
            }
            {
                const cstrview s{",,a,,b,c,d,,"};
                auto v = string::split<cstrview>(s, ",", string::empty_elements::discard, 3);
                snn_require(v.count() == 3);
                snn_require(v.at(0).value() == "a");
                snn_require(v.at(1).value() == "b");
                snn_require(v.at(2).value() == "c,d,,");
            }
            {
                const cstrview s{",,a,,b,c,d,,"};
                auto v = string::split<cstrview>(s, ",", string::empty_elements::keep, 3);
                snn_require(v.count() == 3);
                snn_require(v.at(0).value() == "");
                snn_require(v.at(1).value() == "");
                snn_require(v.at(2).value() == "a,,b,c,d,,");
            }
            {
                // UTF-8
                const cstrview s{"ÅÄÖ"};
                auto v = string::split<cstrview>(s, "Ä");
                snn_require(v.count() == 2);
                snn_require(v.at(0).value() == "Å");
                snn_require(v.at(1).value() == "Ö");
            }

            // split(const cstrview subject, const char delimiter)
            {
                const cstrview s{""};
                const auto v = string::split<cstrview>(s, 'a');
                snn_require(v.is_empty());
            }
            {
                const cstrview s{"\n"};
                const auto v = string::split<cstrview>(s, '\n');
                snn_require(v.count() == 2);
                snn_require(v.at(0).value() == "");
                snn_require(v.at(1).value() == "");
            }
            {
                const cstrview s{"\n\n"};
                const auto v = string::split<cstrview>(s, '\n');
                snn_require(v.at(0).value() == "");
                snn_require(v.at(1).value() == "");
                snn_require(v.at(2).value() == "");
                snn_require(v.count() == 3);
            }
            {
                const cstrview s{"a,b,c"};
                const auto v = string::split<cstrview>(s, ',');
                snn_require(v.count() == 3);
                snn_require(v.at(0).value() == "a");
                snn_require(v.at(1).value() == "b");
                snn_require(v.at(2).value() == "c");
            }
            {
                const cstrview s{",a,b,c,"};
                const auto v = string::split<cstrview>(s, ',');
                snn_require(v.count() == 5);
                snn_require(v.at(0).value() == "");
                snn_require(v.at(1).value() == "a");
                snn_require(v.at(2).value() == "b");
                snn_require(v.at(3).value() == "c");
                snn_require(v.at(4).value() == "");
            }
            {
                const cstrview s{"abc"};
                const auto v = string::split<cstrview>(s, ',');
                snn_require(v.count() == 1);
                snn_require(v.at(0).value() == "abc");
            }
            {
                const cstrview s{"a,b,c"};
                const auto v = string::split<cstrview>(s, 'a');
                snn_require(v.count() == 2);
                snn_require(v.at(0).value() == "");
                snn_require(v.at(1).value() == ",b,c");
            }
            {
                const cstrview s{",,a,,b,c,d,,"};
                const auto v = string::split<cstrview>(s, ',');
                snn_require(v.count() == 9);
                snn_require(v.at(0).value() == "");
                snn_require(v.at(1).value() == "");
                snn_require(v.at(2).value() == "a");
                snn_require(v.at(3).value() == "");
                snn_require(v.at(4).value() == "b");
                snn_require(v.at(5).value() == "c");
                snn_require(v.at(6).value() == "d");
                snn_require(v.at(7).value() == "");
                snn_require(v.at(8).value() == "");
            }
            {
                // UTF-8
                const cstrview s{"Å Ä Ö"};
                const auto v = string::split<cstrview>(s, ' ');
                snn_require(v.count() == 3);
                snn_require(v.at(0).value() == "Å");
                snn_require(v.at(1).value() == "Ä");
                snn_require(v.at(2).value() == "Ö");
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
        snn_static_require(app::test_split());
    }
}
