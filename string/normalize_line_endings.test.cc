// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/string/normalize_line_endings.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(string::normalize_line_endings("\r\n") == "\n");
            snn_require(string::normalize_line_endings("a\r\nb\r\nc") == "a\nb\nc");
            snn_require(string::normalize_line_endings("a\nb\nc") == "a\nb\nc");

            return true;
        }

        constexpr bool test_normalize_line_endings()
        {
            snn_require(string::normalize_line_endings("") == "");
            snn_require(string::normalize_line_endings("a") == "a");
            snn_require(string::normalize_line_endings("ab") == "ab");
            snn_require(string::normalize_line_endings("a b c") == "a b c");

            snn_require(string::normalize_line_endings("\n") == "\n");
            snn_require(string::normalize_line_endings("\n\n") == "\n\n");
            snn_require(string::normalize_line_endings("\na\n") == "\na\n");
            snn_require(string::normalize_line_endings("\na\nb") == "\na\nb");
            snn_require(string::normalize_line_endings("a\nb\nc") == "a\nb\nc");
            snn_require(string::normalize_line_endings("a\nb\nc\n\n") == "a\nb\nc\n\n");
            snn_require(string::normalize_line_endings("å\nä\nö\n\n") == "å\nä\nö\n\n");

            snn_require(string::normalize_line_endings("\r") == "\n");
            snn_require(string::normalize_line_endings("\r\r") == "\n\n");
            snn_require(string::normalize_line_endings("\ra\r") == "\na\n");
            snn_require(string::normalize_line_endings("\ra\rb") == "\na\nb");
            snn_require(string::normalize_line_endings("a\rb\rc") == "a\nb\nc");
            snn_require(string::normalize_line_endings("a\rb\rc\r\r") == "a\nb\nc\n\n");
            snn_require(string::normalize_line_endings("å\rä\rö\r\r") == "å\nä\nö\n\n");

            snn_require(string::normalize_line_endings("\r\n") == "\n");
            snn_require(string::normalize_line_endings("\r\n\r\n") == "\n\n");
            snn_require(string::normalize_line_endings("\r\na\r\n") == "\na\n");
            snn_require(string::normalize_line_endings("\r\na\r\nb") == "\na\nb");
            snn_require(string::normalize_line_endings("a\r\nb\r\nc") == "a\nb\nc");
            snn_require(string::normalize_line_endings("a\r\nb\r\nc\r\n\r\n") == "a\nb\nc\n\n");
            snn_require(string::normalize_line_endings("å\r\nä\r\nö\r\n\r\n") == "å\nä\nö\n\n");

            {
                constexpr cstrview before{"Å\0äö\r\nabc\n\ra\r\n\r\n123\n"};
                static_assert(before.size() == 23);
                const str after = string::normalize_line_endings(before);
                snn_require(after.size() == 20);
                snn_require(after == "Å\0äö\nabc\n\na\n\n123\n");
            }

            {
                str append_to{"\r\nSomething\r\n"};
                snn_require(append_to.size() == 13);
                const cstrview s{"Å\0äö\r\nabc\n\ra\r\n\r\n123\n"};
                string::normalize_line_endings(s, append_to);
                snn_require(append_to.size() == 33);
                snn_require(append_to == "\r\nSomething\r\nÅ\0äö\nabc\n\na\n\n123\n");
            }

            {
                str s;
                snn_require(s.is_empty());
                string::normalize_line_endings_inplace(s);
                snn_require(s.is_empty());
            }

            {
                str s{"abc"};
                snn_require(s.size() == 3);
                string::normalize_line_endings_inplace(s);
                snn_require(s.size() == 3);
                snn_require(s == "abc");
            }

            {
                strbuf s{"Å\0äö\r\nabc\n\ra\r\n\r\n123\n"};
                snn_require(s.size() == 23);
                string::normalize_line_endings_inplace(s);
                snn_require(s.size() == 20);
                snn_require(s == "Å\0äö\nabc\n\na\n\n123\n");
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
        snn_static_require(app::test_normalize_line_endings());
    }
}
