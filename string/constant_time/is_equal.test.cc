// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/string/constant_time/is_equal.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            snn_require(string::constant_time::is_equal("", ""));
            snn_require(string::constant_time::is_equal("one", "one"));
            snn_require(string::constant_time::is_equal("abcdefghijklmnopqrstuvwxyz",
                                                        "abcdefghijklmnopqrstuvwxyz"));

            snn_require(!string::constant_time::is_equal("a", ""));
            snn_require(!string::constant_time::is_equal("", "a"));
            snn_require(!string::constant_time::is_equal("A", "a"));
            snn_require(!string::constant_time::is_equal("a", "A"));
            snn_require(!string::constant_time::is_equal("aa", "a"));
            snn_require(!string::constant_time::is_equal("a", "aa"));
            snn_require(!string::constant_time::is_equal("abcdefghijklmnopqrstuvwxyz", ""));
            snn_require(!string::constant_time::is_equal("", "abcdefghijklmnopqrstuvwxyz"));
            snn_require(!string::constant_time::is_equal("one", "One"));
            snn_require(!string::constant_time::is_equal("One", "one"));
            snn_require(!string::constant_time::is_equal("abcdefghijklmnopqrstuvwxyz",
                                                         "abcdefghijklmnopqrstuvwxyZ"));
            snn_require(!string::constant_time::is_equal("abcdefghijklmnopqrstuvwxyZ",
                                                         "abcdefghijklmnopqrstuvwxyz"));

            return true;
        }

        bool test_is_equal()
        {
            snn_require(string::constant_time::is_equal("\x00", "\x00"));
            snn_require(string::constant_time::is_equal("\x00\x00", "\x00\x00"));
            snn_require(string::constant_time::is_equal("\x00\x00\x32", "\x00\x00\x32"));
            snn_require(string::constant_time::is_equal("\x32\x00\x32", "\x32\x00\x32"));

            snn_require(!string::constant_time::is_equal("\x00\x00\x32", "\x00\x00\x33"));
            snn_require(!string::constant_time::is_equal("\x00\x00\x32", "\x32\x00\x32"));
            snn_require(!string::constant_time::is_equal("\x32\x00\x32", "\x32\x00\x00"));
            snn_require(!string::constant_time::is_equal("\x32\x00\x32", "\x00\x00\x00"));
            snn_require(!string::constant_time::is_equal("\x01\x02\x03", "\x01\x02\x04"));
            snn_require(!string::constant_time::is_equal("\x01\x02\x03", "\x01\x02\x02"));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());
        snn_require(app::test_is_equal());
    }
}
