// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/remove_if.hh"

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
                array numbers{7, 7, 8, 8, 2};

                auto rng = algo::remove_if(numbers.range(), fn::is{fn::equal_to{}, 7});
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.count() == 3);
                snn_require(algo::is_equal(rng, {8, 8, 2}));

                // The source array has been rewritten (elements are shifted/moved).
                snn_require(numbers.at(0).value() == 8);
                snn_require(numbers.at(1).value() == 8);
                snn_require(numbers.at(2).value() == 2);
                snn_require(numbers.at(3).value() == 8);
                snn_require(numbers.at(4).value() == 2);
            }
            {
                array<str, 3> strings{"one", "two", "three"};

                auto rng = algo::remove_if(strings.range(), fn::is{fn::equal_to{}, "two"});
                static_assert(std::is_same_v<decltype(rng), range::contiguous<str*>>);
                snn_require(rng.count() == 2);
                snn_require(algo::is_equal(rng, init_list<cstrview>{"one", "three"}));

                // The source array has been rewritten (elements are shifted/moved).
                snn_require(strings.at(0).value() == "one");
                snn_require(strings.at(1).value() == "three");
                snn_require(strings.at(2).value() == "two");
            }

            return true;
        }

        constexpr bool test_remove_if()
        {
            {
                array numbers{3, 0, 8, 0, 2};

                auto rng = algo::remove_if(numbers.range(), fn::is_zero{});
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.count() == 3);
                snn_require(algo::is_equal(rng, {3, 8, 2}));

                snn_require(numbers.at(0).value() == 3);
                snn_require(numbers.at(1).value() == 8);
                snn_require(numbers.at(2).value() == 2);
            }
            {
                array numbers{0, 0, 0};
                auto rng = algo::remove_if(numbers.range(), fn::is_zero{});
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.is_empty());
            }
            {
                array numbers{0, 0};
                auto rng = algo::remove_if(numbers.range(), fn::is_zero{});
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.is_empty());
            }
            {
                array numbers{0};
                auto rng = algo::remove_if(numbers.range(), fn::is_zero{});
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.is_empty());
            }
            {
                array<int, 0> numbers;
                snn_require(numbers.range().is_empty());
                auto rng = algo::remove_if(numbers.range(), fn::is_zero{});
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.is_empty());
            }
            {
                array numbers{3, 8, 2};
                auto rng = algo::remove_if(numbers.range(), fn::is_zero{});
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.count() == 3);
                snn_require(algo::is_equal(rng, {3, 8, 2}));
            }
            {
                array numbers{3, 8};
                auto rng = algo::remove_if(numbers.range(), fn::is_zero{});
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.count() == 2);
                snn_require(algo::is_equal(rng, {3, 8}));
            }
            {
                array numbers{3};
                auto rng = algo::remove_if(numbers.range(), fn::is_zero{});
                static_assert(std::is_same_v<decltype(rng), range::contiguous<int*>>);
                snn_require(rng.count() == 1);
                snn_require(algo::is_equal(rng, {3}));
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
        snn_static_require(app::test_remove_if());
    }
}
