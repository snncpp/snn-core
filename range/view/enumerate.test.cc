// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/enumerate.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/range/forward.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            const array numbers{7, 6, 42};

            snn_require(
                algo::is_equal(numbers.range() | range::v::enumerate{},
                               init_list<pair::index_value<usize, int>>{{0, 7}, {1, 6}, {2, 42}}));
            snn_require(
                algo::is_equal(numbers.range() | range::v::enumerate{3},
                               init_list<pair::index_value<usize, int>>{{3, 7}, {4, 6}, {5, 42}}));

            return true;
        }

        constexpr bool test_enumerate()
        {
            {
                array numbers{7, 6, 42, 9, 11, 8};

                range::view::enumerate rng{numbers.range()};
                snn_require(rng);
                snn_require(!rng.is_empty());

                auto front_pair = rng.front(promise::not_empty);
                snn_require(front_pair.index == 0);
                snn_require(front_pair.value == 7);

                auto back_pair = rng.back(promise::not_empty);
                snn_require(back_pair.index == 5);
                snn_require(back_pair.value == 8);

                static_assert(std::is_same_v<decltype(front_pair), pair::index_value<usize, int&>>);
                static_assert(std::is_same_v<decltype(back_pair), pair::index_value<usize, int&>>);
            }

            {
                array<str, 4> a{"abc", "ABC", "abcdefghijklmnopqrstuvwxyz", "123"};
                usize index = 0;
                for (auto p : range::view::enumerate{a.range()})
                {
                    snn_require(p.index == index);
                    ++index;
                    p.value.truncate(2);
                }
                snn_require(index == 4);
                snn_require(a.at(0).value() == "ab");
                snn_require(a.at(1).value() == "AB");
                snn_require(a.at(2).value() == "ab");
                snn_require(a.at(3).value() == "12");
            }

            // Random-access range.
            {
                const array numbers{42, 9, 11};
                auto rng = range::view::enumerate{numbers.range()};

                // Concepts
                static_assert(!contiguous_range<decltype(rng)>);
                static_assert(!random_access_range<decltype(rng)>);
                static_assert(bidirectional_range<decltype(rng)>);
                static_assert(forward_range<decltype(rng)>);
                static_assert(input_range<decltype(rng)>);

                snn_require(rng);
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.front(promise::not_empty);
                    snn_require(p.index == 0);
                    snn_require(p.value == 42);
                }
                rng.drop_front(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.front(promise::not_empty);
                    snn_require(p.index == 1);
                    snn_require(p.value == 9);
                }
                rng.drop_front(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.front(promise::not_empty);
                    snn_require(p.index == 2);
                    snn_require(p.value == 11);
                }
                rng.drop_front(promise::not_empty);

                snn_require(!rng);
                snn_require(rng.is_empty());
            }
            {
                const array numbers{42, 9, 11};
                auto rng = range::view::enumerate{numbers.range()};

                // Concepts
                static_assert(!contiguous_range<decltype(rng)>);
                static_assert(!random_access_range<decltype(rng)>);
                static_assert(bidirectional_range<decltype(rng)>);
                static_assert(forward_range<decltype(rng)>);
                static_assert(input_range<decltype(rng)>);

                snn_require(rng);
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.back(promise::not_empty);
                    snn_require(p.index == 2);
                    snn_require(p.value == 11);
                }
                rng.drop_back(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.back(promise::not_empty);
                    snn_require(p.index == 1);
                    snn_require(p.value == 9);
                }
                rng.drop_back(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.back(promise::not_empty);
                    snn_require(p.index == 0);
                    snn_require(p.value == 42);
                }
                rng.drop_back(promise::not_empty);

                snn_require(!rng);
                snn_require(rng.is_empty());
            }

            // Forward range.
            {
                const array numbers{42, 9, 11};
                auto rng = range::view::enumerate{
                    range::forward{meta::iterators, numbers.begin(), numbers.end()}};

                // Concepts
                static_assert(!contiguous_range<decltype(rng)>);
                static_assert(!random_access_range<decltype(rng)>);
                static_assert(!bidirectional_range<decltype(rng)>);
                static_assert(forward_range<decltype(rng)>);
                static_assert(input_range<decltype(rng)>);

                snn_require(rng);
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.front(promise::not_empty);
                    snn_require(p.index == 0);
                    snn_require(p.value == 42);
                }
                rng.drop_front(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.front(promise::not_empty);
                    snn_require(p.index == 1);
                    snn_require(p.value == 9);
                }
                rng.drop_front(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.front(promise::not_empty);
                    snn_require(p.index == 2);
                    snn_require(p.value == 11);
                }
                rng.drop_front(promise::not_empty);

                snn_require(!rng);
                snn_require(rng.is_empty());
            }

            {
                const array numbers{42, 9, 11};
                auto rng = numbers.range() | range::v::enumerate{};

                snn_require(!rng.is_empty());
                {
                    const auto p = rng.front(promise::not_empty);
                    rng.drop_front(promise::not_empty);
                    snn_require(p.index == 0);
                    snn_require(p.value == 42);
                }
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.front(promise::not_empty);
                    rng.drop_front(promise::not_empty);
                    snn_require(p.index == 1);
                    snn_require(p.value == 9);
                }
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.front(promise::not_empty);
                    rng.drop_front(promise::not_empty);
                    snn_require(p.index == 2);
                    snn_require(p.value == 11);
                }

                snn_require(rng.is_empty());
            }

            {
                const array numbers{42, 9, 11};
                auto rng = numbers.range() | range::v::enumerate{5};

                snn_require(!rng.is_empty());
                {
                    const auto p = rng.front(promise::not_empty);
                    rng.drop_front(promise::not_empty);
                    snn_require(p.index == 5);
                    snn_require(p.value == 42);
                }
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.front(promise::not_empty);
                    rng.drop_front(promise::not_empty);
                    snn_require(p.index == 6);
                    snn_require(p.value == 9);
                }
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.front(promise::not_empty);
                    rng.drop_front(promise::not_empty);
                    snn_require(p.index == 7);
                    snn_require(p.value == 11);
                }

                snn_require(rng.is_empty());
            }

            {
                const array numbers{42, 9, 11};
                auto rng = numbers.range() | range::v::enumerate{5};

                snn_require(!rng.is_empty());
                {
                    const auto p = rng.back(promise::not_empty);
                    rng.drop_back(promise::not_empty);
                    snn_require(p.index == 7);
                    snn_require(p.value == 11);
                }
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.back(promise::not_empty);
                    rng.drop_back(promise::not_empty);
                    snn_require(p.index == 6);
                    snn_require(p.value == 9);
                }
                snn_require(!rng.is_empty());
                {
                    const auto p = rng.back(promise::not_empty);
                    rng.drop_back(promise::not_empty);
                    snn_require(p.index == 5);
                    snn_require(p.value == 42);
                }

                snn_require(rng.is_empty());
            }

            {
                auto rng = cstrrng{} | range::v::enumerate{};
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
        snn_static_require(app::test_enumerate());
    }
}
