// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/integral.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/range/generate.hh"
#include "snn-core/range/view/reverse.hh"
#include "snn-core/range/view/take.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            range::integral<u32> rng;

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(assume::not_empty) == 0);
            snn_require(rng.back(assume::not_empty) == 4294967295);

            rng.drop_front(assume::not_empty);
            snn_require(rng.front(assume::not_empty) == 1);
            snn_require(rng.back(assume::not_empty) == 4294967295);

            rng.drop_back(assume::not_empty);
            snn_require(rng.front(assume::not_empty) == 1);
            snn_require(rng.back(assume::not_empty) == 4294967294);

            return true;
        }

        constexpr bool test_integral()
        {
            {
                range::integral<i32> rng;

                snn_require(rng);
                snn_require(!rng.is_empty());

                snn_require(rng.front(assume::not_empty) == -2147483648);
                snn_require(rng.back(assume::not_empty) == 2147483647);
            }

            {
                range::integral<i8> rng;

                // This loop will not modify the range.
                for (const auto i : rng)
                {
                    snn_require(fn::in_interval_incl{-128, 127}(i));
                }

                snn_require(rng);
                snn_require(!rng.is_empty());

                snn_require(rng.front(assume::not_empty) == -128);
                snn_require(rng.back(assume::not_empty) == 127);

                rng.drop_front(assume::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());

                snn_require(rng.front(assume::not_empty) == -127);
                snn_require(rng.back(assume::not_empty) == 127);

                rng.drop_back(assume::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());

                snn_require(rng.front(assume::not_empty) == -127);
                snn_require(rng.back(assume::not_empty) == 126);

                // Iterator types.
                static_assert(std::is_same_v<decltype(rng.begin()),
                                             range::iter::forward<range::integral<i8>>>);
                static_assert(std::is_same_v<decltype(rng.end()), range::iter::forward_end>);
            }

            {
                // Concepts
                static_assert(forward_range<range::integral<int>>);
                static_assert(bidirectional_range<range::integral<int>>);
                static_assert(!random_access_range<range::integral<int>>);
                static_assert(!contiguous_range<range::integral<int>>);
            }

            {
                // i8: -128 to 127 (inclusive)
                static_assert(algo::is_equal(range::integral<i8>{},
                                             range::generate{fn::incrementing{-128}} |
                                                 range::v::take{256}));
                // i8: 127 to -128 (reversed)
                static_assert(algo::is_equal(range::integral<i8>{} | range::v::reverse{},
                                             range::generate{fn::decrementing{127}} |
                                                 range::v::take{256}));

                // u8: 0 to 255 (inclusive)
                static_assert(algo::is_equal(range::integral<u8>{},
                                             range::generate{fn::incrementing{0}} |
                                                 range::v::take{256}));

                // u8: 255 to 0 (reversed)
                static_assert(algo::is_equal(range::integral<u8>{} | range::v::reverse{},
                                             range::generate{fn::decrementing{255}} |
                                                 range::v::take{256}));
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
        snn_static_require(app::test_integral());

        // Counter types.
        static_assert(std::is_same_v<range::detail::integral::counter<u8>::type, i16fast>);
        static_assert(std::is_same_v<range::detail::integral::counter<u16>::type, i32fast>);
        static_assert(std::is_same_v<range::detail::integral::counter<u32>::type, i64>);
        static_assert(std::is_same_v<range::detail::integral::counter<i8>::type, i16fast>);
        static_assert(std::is_same_v<range::detail::integral::counter<i16>::type, i32fast>);
        static_assert(std::is_same_v<range::detail::integral::counter<i32>::type, i64>);
    }
}
