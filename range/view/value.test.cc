// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/value.hh"

#include "snn-core/array.hh"
#include "snn-core/optional.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/count.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/math/fn/common.hh"
#include "snn-core/num/safe.hh"
#include "snn-core/range/generate.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array arr{num::safe{12}, 42, 17, 9};

            snn_require(algo::is_equal(arr.range() | range::v::value{}, {12, 42, 17, 9}));

            auto plain_rng = arr.range();
            auto value_rng = range::view::value{plain_rng};

            using plain_rng_front_type = decltype(plain_rng.front(assume::not_empty));
            using value_rng_front_type = decltype(value_rng.front(assume::not_empty));

            static_assert(std::is_same_v<plain_rng_front_type, num::safe<int>&>);
            static_assert(std::is_same_v<value_rng_front_type, int>);

            return true;
        }

        constexpr bool test_value()
        {
            {
                int a = 9;
                int b = 21;

                array<optional<int&>, 2> arr{a, b};

                range::view::value rng{arr.range()};

                snn_require(algo::count(rng) == 2);

                // This loop will not modify the range when it is copy-constructible.
                for (auto&& i : rng)
                {
                    snn_require(math::fn::is_odd{}(i));
                }

                snn_require(rng);
                snn_require(!rng.is_empty());

                snn_require(rng.front(assume::not_empty) == 9);
                rng.front(assume::not_empty) = 123;
                rng.drop_front(assume::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());

                snn_require(rng.front(assume::not_empty) == 21);
                rng.front(assume::not_empty) = 456;
                rng.drop_front(assume::not_empty);

                snn_require(!rng);
                snn_require(rng.is_empty());

                snn_require(a == 123);
                snn_require(b == 456);

                // Types

                using range_type = range::view::value<range::contiguous<optional<int&>*>>;

                static_assert(std::is_same_v<decltype(rng), range_type>);

                static_assert(std::is_same_v<decltype(rng.front(assume::not_empty)), int&>);

                static_assert(
                    std::is_same_v<decltype(rng.begin()), range::iter::forward<range_type>>);

                static_assert(std::is_same_v<decltype(rng.end()), range::iter::forward_end>);

                // Concepts
                static_assert(forward_range<range_type>);
                static_assert(!bidirectional_range<range_type>);
                static_assert(!random_access_range<range_type>);
                static_assert(!contiguous_range<range_type>);
            }
            {
                range::generate gen_rng{[i = 0]() mutable { return optional<int>{i++}; }};

                // `range::generate` is never empty.
                snn_require(gen_rng);
                snn_require(!gen_rng.is_empty());

                decltype(auto) opt = gen_rng.front(assume::not_empty);
                static_assert(std::is_same_v<decltype(opt), optional<int>>);
                snn_require(opt.value() == 0);

                static_assert(
                    std::is_same_v<decltype(gen_rng.front(assume::not_empty).value()), int&&>);

                range::view::value rng{gen_rng};

                // If `range::view::value` did not drop rvalue references this would result in "read
                // of temporary whose lifetime has ended".

                decltype(auto) one = rng.front(assume::not_empty);
                static_assert(std::is_same_v<decltype(one), int>);
                snn_require(one == 1);
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
        snn_static_require(app::test_value());
    }
}
