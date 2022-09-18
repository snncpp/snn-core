// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/drop_last.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/count.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/math/fn/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array arr{12, 42, 17, 9};

            snn_require(algo::is_equal(arr.range(), {12, 42, 17, 9}));
            snn_require(algo::is_equal(arr.range() | range::v::drop_last{0}, {12, 42, 17, 9}));
            snn_require(algo::is_equal(arr.range() | range::v::drop_last{1}, {12, 42, 17}));
            snn_require(algo::is_equal(arr.range() | range::v::drop_last{2}, {12, 42}));
            snn_require(algo::is_equal(arr.range() | range::v::drop_last{3}, {12}));

            snn_require(range::view::drop_last{arr.range(), 4}.is_empty());
            snn_require(range::view::drop_last{arr.range(), 5}.is_empty());
            snn_require(range::view::drop_last{arr.range(), 99}.is_empty());

            return true;
        }

        constexpr bool test_drop_last()
        {
            array arr{3, 7, 47, 9, 21};

            range::view::drop_last rng{arr.range(), 2};

            snn_require(algo::count(rng) == 3);

            // This loop will not modify the range when it is copy-constructible.
            for (auto&& i : rng)
            {
                snn_require(math::fn::is_odd{}(i));
            }

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(promise::not_empty) == 3);
            snn_require(rng.back(promise::not_empty) == 47);
            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(promise::not_empty) == 7);
            snn_require(rng.back(promise::not_empty) == 47);
            rng.drop_back(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(promise::not_empty) == 7);
            snn_require(rng.back(promise::not_empty) == 7);
            rng.drop_front(promise::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            // Types
            using range_type = range::view::drop_last<range::contiguous<int*>>;
            static_assert(std::is_same_v<decltype(rng), range_type>);
            static_assert(std::is_same_v<decltype(rng.front(promise::not_empty)), int&>);
            static_assert(std::is_same_v<decltype(rng.back(promise::not_empty)), int&>);
            static_assert(std::is_same_v<decltype(rng.begin()), range::iter::forward<range_type>>);
            static_assert(std::is_same_v<decltype(rng.end()), range::iter::forward_end>);

            // Concepts
            static_assert(forward_range<range::view::drop_last<cstrrng>>);
            static_assert(bidirectional_range<range::view::drop_last<cstrrng>>);
            static_assert(!random_access_range<range::view::drop_last<cstrrng>>);
            static_assert(!contiguous_range<range::view::drop_last<cstrrng>>);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_drop_last());
    }
}
