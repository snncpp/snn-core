// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/drop.hh"

#include "snn-core/array.hh"
#include "snn-core/array_view.hh"
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
            snn_require(algo::is_equal(arr.range() | range::v::drop{0}, {12, 42, 17, 9}));
            snn_require(algo::is_equal(arr.range() | range::v::drop{1}, {42, 17, 9}));
            snn_require(algo::is_equal(arr.range() | range::v::drop{2}, {17, 9}));
            snn_require(algo::is_equal(arr.range() | range::v::drop{3}, {9}));

            snn_require(range::view::drop{arr.range(), 4}.is_empty());
            snn_require(range::view::drop{arr.range(), 5}.is_empty());
            snn_require(range::view::drop{arr.range(), 99}.is_empty());

            return true;
        }

        constexpr bool test_drop()
        {
            constexpr array arr{3, 7, 47, 9, 21};

            range::view::drop rng{arr.range(), 3};

            snn_require(algo::count(rng) == 2);

            // This loop will not modify the range when it is copy-constructible.
            for (auto&& i : rng)
            {
                snn_require(math::fn::is_odd{}(i));
            }

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(promise::not_empty) == 9);
            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(promise::not_empty) == 21);
            rng.drop_front(promise::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            // Concepts
            static_assert(forward_range<range::view::drop<cstrrng>>);
            static_assert(!bidirectional_range<range::view::drop<cstrrng>>);
            static_assert(!random_access_range<range::view::drop<cstrrng>>);
            static_assert(!contiguous_range<range::view::drop<cstrrng>>);

            // Types

            using range_type = range::view::drop<range::contiguous<const int*>>;

            static_assert(std::is_same_v<decltype(rng), range_type>);
            static_assert(std::is_same_v<decltype(rng.front(promise::not_empty)), const int&>);
            static_assert(std::is_same_v<decltype(rng.begin()), range::iter::forward<range_type>>);
            static_assert(std::is_same_v<decltype(rng.end()), range::iter::forward_end>);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_drop());
    }
}
