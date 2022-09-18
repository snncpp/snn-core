// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/remove_consecutive_duplicates.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/fn/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                array numbers{3, 3, 8, 8, 2};

                auto rng = algo::remove_consecutive_duplicates(numbers.range());
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.count() == 3);
                snn_require(algo::is_equal(rng, {3, 8, 2}));

                // The source array has been rewritten (elements are shifted/moved).
                snn_require(numbers.at(0).value() == 3);
                snn_require(numbers.at(1).value() == 8);
                snn_require(numbers.at(2).value() == 2);
                snn_require(numbers.at(3).value() == 8);
                snn_require(numbers.at(4).value() == 2);
            }
            {
                str s{"a bb     c    d  eee  f"};
                snn_require(s.size() == 23);

                auto rng = algo::remove_consecutive_duplicates(s.range(), fn::is{fn::eq{}, ' '});
                snn_require(rng.count() == 14);

                snn_require(s == "a bb c d eee fd  eee  f");
                s.truncate(rng.count());
                snn_require(s == "a bb c d eee f");
            }

            return true;
        }

        constexpr bool test_remove_consecutive_duplicates()
        {
            {
                array numbers{3, 8, 2};
                auto rng = algo::remove_consecutive_duplicates(numbers.range());
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.count() == 3);
                snn_require(algo::is_equal(rng, {3, 8, 2}));
            }
            {
                array numbers{3, 8};
                auto rng = algo::remove_consecutive_duplicates(numbers.range());
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.count() == 2);
                snn_require(algo::is_equal(rng, {3, 8}));
            }
            {
                array numbers{3};
                auto rng = algo::remove_consecutive_duplicates(numbers.range());
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.count() == 1);
                snn_require(algo::is_equal(rng, {3}));
            }
            {
                array numbers{3, 3};
                auto rng = algo::remove_consecutive_duplicates(numbers.range());
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.count() == 1);
                snn_require(algo::is_equal(rng, {3}));
            }
            {
                array numbers{3, 3, 3};
                auto rng = algo::remove_consecutive_duplicates(numbers.range());
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.count() == 1);
                snn_require(algo::is_equal(rng, {3}));
            }
            {
                array<int, 0> numbers;
                auto rng = algo::remove_consecutive_duplicates(numbers.range());
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.is_empty());
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
        snn_static_require(app::test_remove_consecutive_duplicates());
    }
}
