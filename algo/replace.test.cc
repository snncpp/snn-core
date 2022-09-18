// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/replace.hh"

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
                array numbers{3, 8, 8};
                algo::replace(numbers.range(), 8, 55);
                snn_require(algo::is_equal(numbers.range(), {3, 55, 55}));
            }
            {
                array numbers{3};
                algo::replace(numbers.range(), 8, 55);
                snn_require(numbers.at(0).value() == 3);
                algo::replace(numbers.range(), 3, 9);
                snn_require(numbers.at(0).value() == 9);
            }
            {
                array<int, 0> numbers;
                snn_require(numbers.range().is_empty());
                algo::replace(numbers.range(), 8, 55); // Empty range, does nothing.
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
