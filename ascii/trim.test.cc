// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/ascii/trim.hh"

#include "snn-core/unittest.hh"
#include "snn-core/chr/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(ascii::trim(cstrview{" abc \n"}) == "abc");
            snn_require(ascii::trim(cstrview{" abc \n"}, ' ') == "abc \n");

            str s{" FooBar-123\n"};

            ascii::trim_right_inplace(s);
            snn_require(s == " FooBar-123");

            ascii::trim_left_inplace(s);
            snn_require(s == "FooBar-123");

            ascii::trim_right_inplace_if(s, chr::is_digit);
            snn_require(s == "FooBar-");

            ascii::trim_left_inplace_if(s, chr::is_alpha);
            snn_require(s == "-");

            return true;
        }

        template <typename T>
        constexpr bool test_trim()
        {
            // trim_inplace(String&)
            // trim_left_inplace(String&)
            // trim_right_inplace(String&)
            {
                T s{" FooBar123!\n"};
                ascii::trim_right_inplace(s);
                snn_require(s == " FooBar123!");
                ascii::trim_left_inplace(s);
                snn_require(s == "FooBar123!");

                s.clear();
                snn_require(s == "");
                ascii::trim_inplace(s);
                snn_require(s == "");

                s = " abc ";
                snn_require(s == " abc ");
                ascii::trim_inplace(s);
                snn_require(s == "abc");
                ascii::trim_inplace(s);
                snn_require(s == "abc");

                s = " \n\t";
                ascii::trim_left_inplace(s);
                snn_require(s == "");
                snn_require(s.is_empty());

                s = " \n\t";
                ascii::trim_right_inplace(s);
                snn_require(s == "");
                snn_require(s.is_empty());

                s = "  x  ";
                snn_require(s.size() == 5);
                ascii::trim_left_inplace(s);
                snn_require(s == "x  ");
                snn_require(s.size() == 3);
                ascii::trim_left_inplace(s);
                snn_require(s == "x  ");
                snn_require(s.size() == 3);

                s = "  x  ";
                snn_require(s.size() == 5);
                ascii::trim_right_inplace(s);
                snn_require(s == "  x");
                snn_require(s.size() == 3);
                ascii::trim_right_inplace(s);
                snn_require(s == "  x");
                snn_require(s.size() == 3);
            }

            // trim_inplace(String&, char)
            // trim_left_inplace(String&, char)
            // trim_right_inplace(String&, char)
            {
                T s{" \tFooBar123! \n"};
                ascii::trim_right_inplace(s, '\n');
                snn_require(s == " \tFooBar123! ");
                ascii::trim_left_inplace(s, ' ');
                snn_require(s == "\tFooBar123! ");

                s.clear();
                snn_require(s == "");
                ascii::trim_inplace(s, ' ');
                snn_require(s == "");

                s = "   \n-\n   ";
                ascii::trim_inplace(s, '\n');
                snn_require(s == "   \n-\n   ");
                ascii::trim_inplace(s, ' ');
                snn_require(s == "\n-\n");

                s = "\t\t\t";
                ascii::trim_left_inplace(s, '\t');
                snn_require(s == "");
                snn_require(s.is_empty());

                s = "\t\t\t";
                ascii::trim_right_inplace(s, '\t');
                snn_require(s == "");
                snn_require(s.is_empty());
            }

            // trim_inplace_if(String&, pred)
            {
                T s{"382 ?abc91_"};

                ascii::trim_inplace_if(s, [](const char c) { return c < 'a'; });
                snn_require(s == "abc");

                ascii::trim_inplace_if(s, [](const char c) { return c < 'a'; });
                snn_require(s == "abc");

                s.clear();
                snn_require(s == "");
                ascii::trim_inplace_if(s, [](const char c) { return c < 'a'; });
                snn_require(s == "");

                s = "382 ?91_";
                snn_require(s.size() == 8);
                ascii::trim_inplace_if(s, [](const char c) { return c < 'a'; });
                snn_require(s == "");
            }

            // trim(String)
            // trim(String, char)
            {
                decltype(auto) s = ascii::trim(T{" abc\n"});
                static_assert(std::is_same_v<decltype(s), T>);
                snn_require(s == "abc");

                snn_require(ascii::trim(T{}) == "");
                snn_require(ascii::trim(T{"   "}) == "");
                snn_require(ascii::trim(T{" \n\t "}) == "");
                snn_require(ascii::trim(T{" \nabc\t "}) == "abc");
                snn_require(ascii::trim(T{" \nabc\t "}, '\t') == " \nabc\t ");
                snn_require(ascii::trim(T{" \nabc\t "}, ' ') == "\nabc\t");

                snn_require(ascii::trim(T{"  abc\n\n"}) == "abc");
                snn_require(ascii::trim(T{"  abc\n\n"}, ' ') == "abc\n\n");
                snn_require(ascii::trim(T{"  abc\n\n"}, '\n') == "  abc");
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
        snn_static_require(app::test_trim<str>());
        snn_static_require(app::test_trim<strbuf>());
        snn_static_require(app::test_trim<cstrview>());
    }
}
