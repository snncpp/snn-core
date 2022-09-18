// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/transform.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/fn/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array numbers{7, 11, 14};

            snn_require(algo::is_equal(numbers.range(), {7, 11, 14}));
            snn_require(algo::is_equal(
                numbers.range() | range::v::transform{fn::bind{fn::add{}, 10}}, {17, 21, 24}));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        {
            constexpr array numbers{7, 11, 14};
            static_assert(std::is_same_v<decltype(numbers), const array<int, 3>>);

            auto rng = range::view::transform{numbers.range(), fn::bind{fn::add{}, 10}};

            // Concepts
            static_assert(!contiguous_range<decltype(rng)>);
            static_assert(!random_access_range<decltype(rng)>);
            static_assert(!bidirectional_range<decltype(rng)>);
            static_assert(forward_range<decltype(rng)>);

            // The range is copyable so the iterator holds a copy (multi-pass).
            using range_type = decltype(rng);
            static_assert(std::is_same_v<decltype(rng.begin()), range::iter::forward<range_type>>);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(promise::not_empty) == 17);
            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(promise::not_empty) == 21);
            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(promise::not_empty) == 24);
            rng.drop_front(promise::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());
        }

        {
            const array<int, 0> numbers;
            auto rng = range::view::transform{numbers.range(), fn::bind{fn::add{}, 3}};
            snn_require(!rng);
            snn_require(rng.is_empty());
        }
    }
}
