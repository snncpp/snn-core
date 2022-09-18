// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/cache.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/math/fn/common.hh"
#include "snn-core/range/generate.hh"
#include "snn-core/range/view/filter.hh"
#include "snn-core/range/view/take.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            range::view::cache rng{range::generate{fn::incrementing{0}}};

            snn_require(rng);
            snn_require(rng.front(promise::not_empty) == 0); // Cached
            snn_require(rng.front(promise::not_empty) == 0); // Cached

            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(rng.front(promise::not_empty) == 1); // Cached
            snn_require(rng.front(promise::not_empty) == 1); // Cached

            // INCORRECT result without cache.
            snn_require(algo::is_equal(range::generate{fn::incrementing{0}} |
                                           range::v::filter{math::fn::is_even{}} |
                                           range::v::take{3},
                                       {1, 3, 5}));

            // Correct result with cache.
            snn_require(algo::is_equal(range::generate{fn::incrementing{0}} | range::v::cache{} |
                                           range::v::filter{math::fn::is_even{}} |
                                           range::v::take{3},
                                       {0, 2, 4}));

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
