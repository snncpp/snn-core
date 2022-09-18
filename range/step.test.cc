// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/step.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/math/fn/common.hh"
#include "snn-core/time/point.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::is_equal(range::step{0, 5, 1}, {0, 1, 2, 3, 4}));
            snn_require(algo::is_equal(range::step{0, 5, 2}, {0, 2, 4}));
            snn_require(algo::is_equal(range::step{0, 5, 3}, {0, 3}));
            snn_require(algo::is_equal(range::step{0, 5, 4}, {0, 4}));
            snn_require(algo::is_equal(range::step{0, 5, 5}, {0}));
            snn_require(algo::is_equal(range::step{0, 5, 6}, {0}));

            snn_require(range::step{0, 0, 1}.is_empty());
            snn_require(range::step{99, 99, 1}.is_empty());
            snn_require(range::step{5, 0, 1}.is_empty());

            // For integral types "by" defaults to 1.
            snn_require(algo::is_equal(range::step{0, 5}, {0, 1, 2, 3, 4}));
            snn_require(range::step{0, 0}.is_empty());
            snn_require(range::step{99, 99}.is_empty());
            snn_require(range::step{5, 0}.is_empty());

            return true;
        }

        constexpr bool test_step()
        {
            range::step rng{time::point{2021, 12, 1}, time::point{2021, 12, 5}, time::days{2}};

            // This loop will not modify the range when it is copy-constructible.
            for (const auto p : rng)
            {
                snn_require(math::fn::is_odd{}(p.day()));
            }

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(promise::not_empty) == time::point{2021, 12, 1});
            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(promise::not_empty) == time::point{2021, 12, 3});
            rng.drop_front(promise::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            // Iterator types.
            static_assert(
                std::is_same_v<decltype(rng.begin()),
                               range::iter::forward<range::step<time::point, time::days>>>);
            static_assert(std::is_same_v<decltype(rng.end()), range::iter::forward_end>);

            // Concepts
            static_assert(forward_range<range::step<int>>);
            static_assert(!bidirectional_range<range::step<int>>);
            static_assert(!random_access_range<range::step<int>>);
            static_assert(!contiguous_range<range::step<int>>);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_step());
    }
}
