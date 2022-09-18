// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/ascii/is_equal_icase.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(ascii::is_equal_icase("", ""));
            snn_require(ascii::is_equal_icase("a", "a"));
            snn_require(ascii::is_equal_icase("A", "a"));
            snn_require(ascii::is_equal_icase("b", "B"));
            snn_require(ascii::is_equal_icase("zz", "zZ"));
            snn_require(ascii::is_equal_icase("ABC", "abc"));
            snn_require(ascii::is_equal_icase("abcdefghijklmnopqrstuvw",
                                              "ABcDEfGHIJKLmNOPqRStuVW"));
            snn_require(ascii::is_equal_icase("ABcDEfGHIJKLmNOPqRStuVW",
                                              "abcdefghijklmnopqrstuvw"));

            snn_require(!ascii::is_equal_icase("a", "b"));
            snn_require(!ascii::is_equal_icase("aa", "ab"));
            snn_require(!ascii::is_equal_icase("aaa", "aab"));
            snn_require(!ascii::is_equal_icase("a", "."));
            snn_require(!ascii::is_equal_icase("_", "z"));
            snn_require(!ascii::is_equal_icase("ab", "ba"));
            snn_require(!ascii::is_equal_icase("abc", "bac"));
            snn_require(!ascii::is_equal_icase("abcdef", "ABcD3f"));

            snn_require(!ascii::is_equal_icase("", "A"));
            snn_require(!ascii::is_equal_icase("a", ""));
            snn_require(!ascii::is_equal_icase("A", "aA"));
            snn_require(!ascii::is_equal_icase("ab", "a"));
            snn_require(!ascii::is_equal_icase("ABC", "abcb"));
            snn_require(!ascii::is_equal_icase("ABCD", "abc"));

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
