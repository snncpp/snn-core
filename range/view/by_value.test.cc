// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/by_value.hh"

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
            snn_require(algo::is_equal(arr.range() | range::v::by_value{}, {12, 42, 17, 9}));

            auto ref_rng = arr.range();
            auto val_rng = range::view::by_value{arr.range()};

            snn_require(algo::is_equal(ref_rng, val_rng));

            using ref_rng_front_type = decltype(ref_rng.front(assume::not_empty));
            using val_rng_front_type = decltype(val_rng.front(assume::not_empty));

            static_assert(std::is_same_v<ref_rng_front_type, int&>);
            static_assert(std::is_same_v<val_rng_front_type, int>);

            return true;
        }

        constexpr bool test_by_value()
        {
            const array arr{9, 21};

            range::view::by_value rng{arr.range()};

            snn_require(algo::count(rng) == 2);

            // This loop will not modify the range when it is copy-constructible.
            for (auto&& i : rng)
            {
                snn_require(math::fn::is_odd{}(i));
            }

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(assume::not_empty) == 9);
            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(assume::not_empty) == 21);
            rng.drop_front(assume::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            // Types
            using range_type = range::view::by_value<range::contiguous<const int*>>;
            static_assert(std::is_same_v<decltype(rng), range_type>);
            static_assert(std::is_same_v<decltype(rng.front(assume::not_empty)), int>);
            static_assert(std::is_same_v<decltype(rng.begin()), range::iter::forward<range_type>>);
            static_assert(std::is_same_v<decltype(rng.end()), range::iter::forward_end>);

            // Concepts
            static_assert(forward_range<range::view::by_value<cstrrng>>);
            static_assert(!bidirectional_range<range::view::by_value<cstrrng>>);
            static_assert(!random_access_range<range::view::by_value<cstrrng>>);
            static_assert(!contiguous_range<range::view::by_value<cstrrng>>);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_by_value());
    }
}
