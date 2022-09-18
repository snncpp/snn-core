// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/defer.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            int i = 40;

            {
                defer add_three{[&]() { i += 3; }};
                defer divide_by_two{[&]() { i /= 2; }};

                snn_require(i == 40);

                {
                    defer add_eight{[&]() { i += 8; }};

                    snn_require(i == 40);

                    // "add_eight" goes out of scope.
                }

                snn_require(i == 48);

                // "divide_by_two" goes out of scope.
                // "add_three" goes out of scope.
            }

            snn_require(i == 27);

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
