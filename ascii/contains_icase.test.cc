// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/ascii/contains_icase.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(ascii::contains_icase("", ""));
            snn_require(ascii::contains_icase("a", "a"));
            snn_require(ascii::contains_icase("A", "A"));
            snn_require(ascii::contains_icase("A", "a"));
            snn_require(ascii::contains_icase("AbC", ""));
            snn_require(ascii::contains_icase("AbC", "a"));
            snn_require(ascii::contains_icase("AbC", "ab"));
            snn_require(ascii::contains_icase("AbC", "BC"));
            snn_require(ascii::contains_icase("AbC", "aBc"));
            snn_require(ascii::contains_icase("Abcdef", "abC"));
            snn_require(ascii::contains_icase("123ABC", "abc"));
            snn_require(ascii::contains_icase("123abcdef", "ABC"));

            snn_require(!ascii::contains_icase("", "b"));
            snn_require(!ascii::contains_icase("a", "b"));
            snn_require(!ascii::contains_icase("A", "b"));
            snn_require(!ascii::contains_icase("ABC", "abb"));
            snn_require(!ascii::contains_icase("ABC", "cd"));
            snn_require(!ascii::contains_icase("ABC", "ACB"));
            snn_require(!ascii::contains_icase("ABC", "abcd"));
            snn_require(!ascii::contains_icase("ABCdef", "abb"));
            snn_require(!ascii::contains_icase("123ABC", "abb"));
            snn_require(!ascii::contains_icase("123ABCdef", "abb"));
            snn_require(!ascii::contains_icase("123ABCdef", "123ABCdef0"));

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
