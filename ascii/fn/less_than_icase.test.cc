// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/ascii/fn/less_than_icase.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(ascii::fn::less_than_icase{}('a', 'B'));
            snn_require(ascii::fn::less_than_icase{}('0', '1'));

            snn_require(!ascii::fn::less_than_icase{}(' ', ' '));
            snn_require(!ascii::fn::less_than_icase{}('a', 'A'));
            snn_require(!ascii::fn::less_than_icase{}('A', 'a'));
            snn_require(!ascii::fn::less_than_icase{}('B', 'A'));
            snn_require(!ascii::fn::less_than_icase{}('B', 'a'));

            snn_require(ascii::fn::less_than_icase{}("", "a"));
            snn_require(ascii::fn::less_than_icase{}("a", "B"));
            snn_require(ascii::fn::less_than_icase{}("aa", "AB"));
            snn_require(ascii::fn::less_than_icase{}("aa", "aaa"));
            snn_require(ascii::fn::less_than_icase{}("aa", "AAA"));
            snn_require(ascii::fn::less_than_icase{}("One Two Three", "ONE TWO THREE!"));

            snn_require(!ascii::fn::less_than_icase{}("a", ""));
            snn_require(!ascii::fn::less_than_icase{}("a", "a"));
            snn_require(!ascii::fn::less_than_icase{}("A", "a"));
            snn_require(!ascii::fn::less_than_icase{}("ab", "a"));
            snn_require(!ascii::fn::less_than_icase{}("AB", "ab"));
            snn_require(!ascii::fn::less_than_icase{}("abc", "abc"));
            snn_require(!ascii::fn::less_than_icase{}("abcd", "abc"));
            snn_require(!ascii::fn::less_than_icase{}("One Two Three", "ONE TWO THREE"));

            return true;
        }

        constexpr bool test_less_than_icase()
        {
            // Alias
            snn_require(ascii::fn::lt_icase{}('a', 'B'));
            snn_require(ascii::fn::lt_icase{}('0', '1'));
            snn_require(!ascii::fn::lt_icase{}('A', 'a'));
            snn_require(!ascii::fn::lt_icase{}('B', 'A'));
            snn_require(ascii::fn::lt_icase{}("aa", "AB"));
            snn_require(ascii::fn::lt_icase{}("aa", "aaa"));
            snn_require(!ascii::fn::lt_icase{}("ab", "a"));
            snn_require(!ascii::fn::lt_icase{}("AB", "ab"));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_less_than_icase());
    }
}
