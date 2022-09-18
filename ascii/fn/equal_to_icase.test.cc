// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/ascii/fn/equal_to_icase.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(ascii::fn::equal_to_icase{}(' ', ' '));
            snn_require(ascii::fn::equal_to_icase{}('1', '1'));
            snn_require(ascii::fn::equal_to_icase{}('a', 'a'));
            snn_require(ascii::fn::equal_to_icase{}('a', 'A'));
            snn_require(ascii::fn::equal_to_icase{}('A', 'a'));
            snn_require(ascii::fn::equal_to_icase{}('A', 'A'));

            snn_require(!ascii::fn::equal_to_icase{}(' ', '.'));
            snn_require(!ascii::fn::equal_to_icase{}('a', ' '));
            snn_require(!ascii::fn::equal_to_icase{}('a', 'b'));
            snn_require(!ascii::fn::equal_to_icase{}('b', 'a'));
            snn_require(!ascii::fn::equal_to_icase{}('1', '2'));
            snn_require(!ascii::fn::equal_to_icase{}('\xE5', '\xC5')); // Latin1 å and Å.

            snn_require(ascii::fn::equal_to_icase{}("", ""));
            snn_require(ascii::fn::equal_to_icase{}("a", "a"));
            snn_require(ascii::fn::equal_to_icase{}("A", "a"));
            snn_require(ascii::fn::equal_to_icase{}("a", "A"));
            snn_require(ascii::fn::equal_to_icase{}("A", "A"));
            snn_require(ascii::fn::equal_to_icase{}("ab", "AB"));
            snn_require(ascii::fn::equal_to_icase{}("AB", "ab"));
            snn_require(ascii::fn::equal_to_icase{}("AB", "AB"));
            snn_require(ascii::fn::equal_to_icase{}("One two three.", "One two THREE."));

            snn_require(!ascii::fn::equal_to_icase{}("", "a"));
            snn_require(!ascii::fn::equal_to_icase{}("a", ""));
            snn_require(!ascii::fn::equal_to_icase{}("ab", "a"));
            snn_require(!ascii::fn::equal_to_icase{}("a", "ab"));
            snn_require(!ascii::fn::equal_to_icase{}("abc", "ACB"));
            snn_require(!ascii::fn::equal_to_icase{}("One two three.", "One TWO three!"));

            return true;
        }

        constexpr bool test_equal_to_icase()
        {
            // Alias
            snn_require(ascii::fn::eq_icase{}('A', 'a'));
            snn_require(ascii::fn::eq_icase{}('A', 'A'));
            snn_require(!ascii::fn::eq_icase{}('a', 'b'));
            snn_require(!ascii::fn::eq_icase{}('b', 'a'));
            snn_require(ascii::fn::eq_icase{}("AB", "ab"));
            snn_require(ascii::fn::eq_icase{}("AB", "AB"));
            snn_require(!ascii::fn::eq_icase{}("a", "ab"));
            snn_require(!ascii::fn::eq_icase{}("abc", "ACB"));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_equal_to_icase());
    }
}
