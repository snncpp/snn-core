// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/reverse.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/range/view/drop_last.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array numbers{3, 8, 2, 7, 9};

            algo::reverse(numbers.range());
            snn_require(algo::is_equal(numbers.range(), {9, 7, 2, 8, 3}));

            algo::reverse(numbers.range());
            snn_require(algo::is_equal(numbers.range(), {3, 8, 2, 7, 9}));

            return true;
        }

        constexpr bool test_reverse()
        {
            // Random access range & legacy iterable.
            {
                array numbers{3, 8, 2, 7, 9};

                using range_type = decltype(numbers.range());
                static_assert(bidirectional_range<range_type>);
                static_assert(random_access_range<range_type>);
                static_assert(legacy_iterable<range_type>);

                algo::reverse(numbers.range()); // 2 swaps.
                snn_require(algo::is_equal(numbers.range(), {9, 7, 2, 8, 3}));
                algo::reverse(numbers.range()); // 2 swaps.
                snn_require(algo::is_equal(numbers.range(), {3, 8, 2, 7, 9}));
            }
            {
                array numbers{3, 8, 2, 7};
                algo::reverse(numbers.range()); // 2 swaps.
                snn_require(algo::is_equal(numbers.range(), {7, 2, 8, 3}));
                algo::reverse(numbers.range()); // 2 swaps.
                snn_require(algo::is_equal(numbers.range(), {3, 8, 2, 7}));
            }
            {
                array numbers{3, 8, 2};
                algo::reverse(numbers.range()); // 1 swap.
                snn_require(algo::is_equal(numbers.range(), {2, 8, 3}));
                algo::reverse(numbers.range()); // 1 swap.
                snn_require(algo::is_equal(numbers.range(), {3, 8, 2}));
            }
            {
                array numbers{3, 8};
                algo::reverse(numbers.range()); // 1 swap.
                snn_require(algo::is_equal(numbers.range(), {8, 3}));
                algo::reverse(numbers.range()); // 1 swap.
                snn_require(algo::is_equal(numbers.range(), {3, 8}));
            }
            {
                array numbers{3};
                algo::reverse(numbers.range()); // Single element, 0 swaps.
                snn_require(numbers.at(0).value() == 3);
            }
            {
                array<int, 0> numbers;
                snn_require(numbers.range().is_empty());
                algo::reverse(numbers.range()); // Empty range, 0 swaps.
            }

            // Non-random access range & non-legacy iterable.
            {
                array numbers{3, 8, 2, 7, 9};

                using range_type = decltype(numbers.range() | range::v::drop_last{0});
                static_assert(bidirectional_range<range_type>);
                static_assert(!random_access_range<range_type>);
                static_assert(!legacy_iterable<range_type>);

                algo::reverse(numbers.range() | range::v::drop_last{0}); // 2 swaps.
                snn_require(algo::is_equal(numbers.range(), {9, 7, 2, 8, 3}));
                algo::reverse(numbers.range() | range::v::drop_last{1}); // 2 swaps.
                snn_require(algo::is_equal(numbers.range(), {8, 2, 7, 9, 3}));
                algo::reverse(numbers.range() | range::v::drop_last{2}); // 1 swap.
                snn_require(algo::is_equal(numbers.range(), {7, 2, 8, 9, 3}));
                algo::reverse(numbers.range() | range::v::drop_last{3}); // 1 swap.
                snn_require(algo::is_equal(numbers.range(), {2, 7, 8, 9, 3}));
                algo::reverse(numbers.range() | range::v::drop_last{4}); // Single element, 0 swaps.
                snn_require(algo::is_equal(numbers.range(), {2, 7, 8, 9, 3}));
                algo::reverse(numbers.range() | range::v::drop_last{5}); // Empty range, 0 swaps.
                snn_require(algo::is_equal(numbers.range(), {2, 7, 8, 9, 3}));
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
        snn_static_require(app::test_reverse());
    }
}
