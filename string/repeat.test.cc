// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/string/repeat.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                snn_require(string::repeat("", 0) == "");
                snn_require(string::repeat("", 1) == "");
                snn_require(string::repeat("", 2) == "");
                snn_require(string::repeat("", 3) == "");

                snn_require(string::repeat("a", 0) == "");
                snn_require(string::repeat("a", 1) == "a");
                snn_require(string::repeat("a", 2) == "aa");
                snn_require(string::repeat("a", 3) == "aaa");

                snn_require(string::repeat("abc", 0) == "");
                snn_require(string::repeat("abc", 1) == "abc");
                snn_require(string::repeat("abc", 2) == "abcabc");
                snn_require(string::repeat("abc", 3) == "abcabcabc");
            }
            {
                const auto s = string::repeat<strbuf>("åäö", 3);
                snn_require(s.size() == 18);
                snn_require(s == "åäöåäöåäö");
            }
            {
                str dest{"One two three?"};
                snn_require(dest.capacity() == str::default_capacity());
                str src{"å ä ö"};
                string::repeat(src, 2, dest, promise::no_overlap);
                snn_require(dest == "One two three?å ä öå ä ö");
                snn_require(dest.capacity() > str::default_capacity());
                snn_require(src == "å ä ö");
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
    }
}
