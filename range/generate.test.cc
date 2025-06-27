// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/generate.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/range/view/take.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            auto rng = range::generate{fn::incrementing{0}};

            snn_require(algo::is_equal(rng | range::v::take{5}, {0, 1, 2, 3, 4}));

            snn_require(rng);
            snn_require(!rng.is_empty());

            // Values are not cached (`range::view::cache` can be used if caching is needed).
            snn_require(rng.front(assume::not_empty) == 0);
            snn_require(rng.front(assume::not_empty) == 1);
            snn_require(rng.front(assume::not_empty) == 2);

            rng.drop_front(assume::not_empty); // Does nothing.

            snn_require(rng.front(assume::not_empty) == 3);
            snn_require(rng.front(assume::not_empty) == 4);

            // `range::generate` is never empty.
            snn_require(rng);
            snn_require(!rng.is_empty());

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
