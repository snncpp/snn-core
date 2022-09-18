// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/take.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array arr{12, 42, 17, 9};

            snn_require(algo::is_equal(arr.range(), {12, 42, 17, 9}));
            snn_require(algo::is_equal(arr.range() | range::v::take{5}, {12, 42, 17, 9}));
            snn_require(algo::is_equal(arr.range() | range::v::take{4}, {12, 42, 17, 9}));
            snn_require(algo::is_equal(arr.range() | range::v::take{3}, {12, 42, 17}));
            snn_require(algo::is_equal(arr.range() | range::v::take{2}, {12, 42}));
            snn_require(algo::is_equal(arr.range() | range::v::take{1}, {12}));

            snn_require(range::view::take{arr.range(), 0}.is_empty());

            return true;
        }

        constexpr bool test_take()
        {
            {
                array numbers{7, 6, 42, 9, 11, 8};

                auto rng = range::view::take{numbers.range(), 3};

                // Concepts
                static_assert(!contiguous_range<decltype(rng)>);
                static_assert(!random_access_range<decltype(rng)>);
                static_assert(!bidirectional_range<decltype(rng)>);
                static_assert(forward_range<decltype(rng)>);

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == 7);
                rng.drop_front(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == 6);
                rng.drop_front(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == 42);
                rng.drop_front(promise::not_empty);

                snn_require(!rng);
                snn_require(rng.is_empty());
            }
            {
                array numbers{7, 6, 42, 9, 11, 8};

                auto rng = numbers.range() | range::v::take{3};

                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == 7);
                rng.drop_front(promise::not_empty);

                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == 6);
                rng.drop_front(promise::not_empty);

                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == 42);
                rng.drop_front(promise::not_empty);

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
        snn_static_require(app::test_take());
    }
}
