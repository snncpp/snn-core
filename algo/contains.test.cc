// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/contains.hh"

#include "snn-core/unittest.hh"
#include "snn-core/ascii/fn/equal_to_icase.hh"
#include "snn-core/range/step.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::contains(cstrrng{"abcdef"}, cstrrng{""}));
            snn_require(algo::contains(cstrrng{"abcdef"}, cstrrng{"a"}));
            snn_require(algo::contains(cstrrng{"abcdef"}, cstrrng{"ab"}));
            snn_require(algo::contains(cstrrng{"abcdef"}, cstrrng{"abc"}));
            snn_require(algo::contains(cstrrng{"abcdef"}, cstrrng{"abcd"}));
            snn_require(algo::contains(cstrrng{"abcdef"}, cstrrng{"abcde"}));
            snn_require(algo::contains(cstrrng{"abcdef"}, cstrrng{"abcdef"}));
            snn_require(!algo::contains(cstrrng{"abcdef"}, cstrrng{"abcdefg"}));
            snn_require(!algo::contains(cstrrng{"abcdef"}, cstrrng{"x"}));
            snn_require(!algo::contains(cstrrng{"abcdef"}, cstrrng{"xyz"}));

            snn_require(algo::contains(cstrrng{"abcdef"}, cstrrng{"c"}));
            snn_require(algo::contains(cstrrng{"abcdef"}, cstrrng{"cd"}));
            snn_require(algo::contains(cstrrng{"abcdef"}, cstrrng{"cde"}));
            snn_require(algo::contains(cstrrng{"abcdef"}, cstrrng{"cdef"}));
            snn_require(!algo::contains(cstrrng{"abcdef"}, cstrrng{"cdefg"}));

            snn_require(algo::contains(cstrrng{"abcdef"}, cstrrng{"f"}));
            snn_require(!algo::contains(cstrrng{"abcdef"}, cstrrng{"fg"}));

            snn_require(algo::contains(cstrrng{"aaabc"}, cstrrng{"a"}));
            snn_require(algo::contains(cstrrng{"aaabc"}, cstrrng{"ab"}));

            snn_require(!algo::contains(cstrrng{"aaabc"}, cstrrng{"AB"}));
            snn_require(algo::contains(cstrrng{"aaabc"}, cstrrng{"AB"},
                                       ascii::fn::equal_to_icase{}));

            snn_require(algo::contains(range::step{'a', 'g'}, cstrrng{""}));
            snn_require(algo::contains(range::step{'a', 'g'}, cstrrng{"c"}));
            snn_require(algo::contains(range::step{'a', 'g'}, cstrrng{"cd"}));
            snn_require(algo::contains(range::step{'a', 'g'}, cstrrng{"cdef"}));
            snn_require(!algo::contains(range::step{'a', 'g'}, cstrrng{"g"}));
            snn_require(!algo::contains(range::step{'a', 'g'}, cstrrng{"cdefg"}));

            snn_require(!algo::contains(range::step{'a', 'g'}, cstrrng{"CD"}));
            snn_require(algo::contains(range::step{'a', 'g'}, cstrrng{"CD"},
                                       ascii::fn::equal_to_icase{}));

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
