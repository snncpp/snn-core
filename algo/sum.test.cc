// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/sum.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array numbers{7, 8, 9};

            // Default initial value is a `num::safe<...>`.
            auto sum = algo::sum(numbers.range());
            static_assert(std::is_same_v<decltype(sum), num::safe<int>>);
            snn_require(sum.has_value());
            snn_require(sum.value() == 24);

            // Custom initial value.
            snn_require(algo::sum(numbers.range(), 0) == 24);
            snn_require(algo::sum(numbers.range(), 100) == 124);

            return true;
        }

        constexpr bool test_sum()
        {
            // T sum(Rng rng, T initial_value)
            {
                snn_require(algo::sum(array<int, 0>{}.range(), 0) == 0);
                snn_require(algo::sum(array{7}.range(), 0) == 7);
                snn_require(algo::sum(array{7, 8}.range(), 0) == 15);
                snn_require(algo::sum(array{7, 8, 9}.range(), 0) == 24);
                snn_require(algo::sum(array{7, 8, 9}.range(), 100) == 124);
            }

            // auto sum(Rng rng)
            {
                snn_require(algo::sum(array<int, 0>{}.range()).value() == 0);
                snn_require(algo::sum(array{7}.range()).value() == 7);
                snn_require(algo::sum(array{7, 8}.range()).value() == 15);
                snn_require(algo::sum(array{7, 8, 9}.range()).value() == 24);

                constexpr auto min = constant::limit<int>::min;
                constexpr auto max = constant::limit<int>::max;

                snn_require(algo::sum(array{min, max}.range()).value() == -1);
                snn_require(algo::sum(array{max, min, max, min}.range()).value() == -2);

                // Overflow
                snn_require(!algo::sum(array{min, min}.range()).has_value());
                snn_require(!algo::sum(array{max, max}.range()).has_value());
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
        snn_static_require(app::test_sum());
    }
}
