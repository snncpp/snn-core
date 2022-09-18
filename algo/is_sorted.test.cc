// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/is_sorted.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::is_sorted(cstrrng{""}));
            snn_require(algo::is_sorted(cstrrng{"a"}));
            snn_require(algo::is_sorted(cstrrng{"aaa"}));
            snn_require(algo::is_sorted(cstrrng{"0az"}));
            snn_require(algo::is_sorted(cstrrng{"abcdefghi"}));

            snn_require(!algo::is_sorted(cstrrng{"ba"}));
            snn_require(!algo::is_sorted(cstrrng{"90"}));
            snn_require(!algo::is_sorted(cstrrng{"zx"}));
            snn_require(!algo::is_sorted(cstrrng{"abca"}));

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
