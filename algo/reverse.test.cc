// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/reverse.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                array numbers{3, 8, 2};
                algo::reverse(numbers.range());
                snn_require(algo::is_equal(numbers.range(), {2, 8, 3}));
                algo::reverse(numbers.range());
                snn_require(algo::is_equal(numbers.range(), {3, 8, 2}));
            }
            {
                array numbers{3, 8};
                algo::reverse(numbers.range());
                snn_require(algo::is_equal(numbers.range(), {8, 3}));
                algo::reverse(numbers.range());
                snn_require(algo::is_equal(numbers.range(), {3, 8}));
            }
            {
                array numbers{3};
                algo::reverse(numbers.range()); // Single element, does nothing.
                snn_require(numbers.at(0).value() == 3);
            }
            {
                array<int, 0> numbers;
                snn_require(numbers.range().is_empty());
                algo::reverse(numbers.range()); // Empty range, does nothing.
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
    }
}
