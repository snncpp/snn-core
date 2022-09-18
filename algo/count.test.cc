// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/count.hh"

#include "snn-core/array_view.hh"
#include "snn-core/unittest.hh"
#include "snn-core/range/step.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::count(cstrrng{}) == 0);
            snn_require(algo::count(cstrrng{"a"}) == 1);
            snn_require(algo::count(cstrrng{"abc"}) == 3);
            snn_require(algo::count(cstrrng{"åäö"}) == 6);

            snn_require(algo::count(range::step{0, 0}) == 0);
            snn_require(algo::count(range::step{0, 4}) == 4);
            snn_require(algo::count(range::step{5, 6}) == 1);
            snn_require(algo::count(range::step{5, 7}) == 2);

            snn_require(algo::count(cstrrng{}, 'a') == 0);
            snn_require(algo::count(cstrrng{"a"}, 'a') == 1);
            snn_require(algo::count(cstrrng{"abc"}, 'a') == 1);
            snn_require(algo::count(cstrrng{"aba"}, 'a') == 2);
            snn_require(algo::count(cstrrng{"aaa"}, 'a') == 3);
            snn_require(algo::count(cstrrng{"åäö"}, 'a') == 0);

            snn_require(algo::count(range::step{0, 0}, 1) == 0);
            snn_require(algo::count(range::step{0, 3}, 1) == 1);
            snn_require(algo::count(range::step{2, 3}, 1) == 0);

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
