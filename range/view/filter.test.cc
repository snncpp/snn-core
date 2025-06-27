// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/filter.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/math/fn/common.hh"
#include "snn-core/range/step.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::is_equal(range::step{0, 5}, {0, 1, 2, 3, 4}));
            snn_require(algo::is_equal(range::step{0, 5} | range::v::filter{math::fn::is_even{}},
                                       {0, 2, 4}));
            snn_require(algo::is_equal(range::step{0, 5} | range::v::filter{math::fn::is_odd{}},
                                       {1, 3}));

            return true;
        }

        constexpr bool test_filter()
        {
            {
                array numbers{7, 6, 42, 9, 11, 8};

                auto rng = range::view::filter{numbers.range(), math::fn::is_odd{}};

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front(assume::not_empty) == 7);
                rng.drop_front(assume::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front(assume::not_empty) == 9);
                rng.drop_front(assume::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front(assume::not_empty) == 11);
                rng.drop_front(assume::not_empty);

                snn_require(!rng);
                snn_require(rng.is_empty());

                // Concepts
                static_assert(!contiguous_range<decltype(rng)>);
                static_assert(!random_access_range<decltype(rng)>);
                static_assert(!bidirectional_range<decltype(rng)>);
                static_assert(forward_range<decltype(rng)>);
            }
            {
                const array numbers{6, 42, 8};
                auto rng = range::view::filter{numbers.range(), math::fn::is_odd{}};
                snn_require(!rng);
                snn_require(rng.is_empty());
            }
            {
                const array<int, 0> numbers;
                auto rng = range::view::filter{numbers.range(), math::fn::is_odd{}};
                snn_require(!rng);
                snn_require(rng.is_empty());
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_filter());
    }
}
