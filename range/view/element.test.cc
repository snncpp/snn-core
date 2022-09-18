// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/element.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/algo/sum.hh"
#include "snn-core/pair/common.hh"
#include "snn-core/range/view/to_prefix.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array<pair::key_value<int>, 3> a{{{1, 3}, {3, 5}, {7, 9}}};
            snn_require(algo::sum(a.range() | range::v::element<0>{}) == 11);
            snn_require(algo::sum(a.range() | range::v::element<1>{}) == 17);

            return true;
        }

        constexpr bool test_element()
        {
            {
                array a{"12", "456_", "9abc"};
                static_assert(std::is_same_v<decltype(a), array<cstrview, 3>>);
                snn_require(algo::is_equal(
                    a.range() | range::v::to_prefix<int>{} | range::v::element<0>{}, {12, 456, 9}));
                snn_require(algo::is_equal(
                    a.range() | range::v::to_prefix<int>{} | range::v::element<1>{}, {2u, 3u, 1u}));
            }
            {
                array<pair::key_value<str, int>, 2> a{{{"abc", 123}, {"def", 456}}};

                range::view::element rng{a.range(), meta::index<0>};

                snn_require(rng);
                snn_require(!rng.is_empty());

                decltype(auto) key1 = rng.front(promise::not_empty);
                static_assert(std::is_same_v<decltype(key1), str&>);
                snn_require(key1 == "abc");

                rng.drop_front(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());

                decltype(auto) key2 = rng.front(promise::not_empty);
                rng.drop_front(promise::not_empty);
                static_assert(std::is_same_v<decltype(key2), str&>);
                snn_require(key2 == "def");

                snn_require(!rng);
                snn_require(rng.is_empty());
            }
            {
                array<pair::key_value<str, int>, 2> a{{{"abc", 123}, {"def", 456}}};

                range::view::element rng{a.range(), meta::index<1>};

                usize count = 0;
                for (decltype(auto) i : rng)
                {
                    static_assert(std::is_same_v<decltype(i), int&>);

                    if (count == 0)
                    {
                        snn_require(i == 123);
                        i *= 2;
                    }
                    else
                    {
                        snn_require(i == 456);
                        i *= 2;
                    }
                    ++count;
                }
                snn_require(count == 2);

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == 246);
                rng.drop_front(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == 912);
                rng.drop_front(promise::not_empty);

                snn_require(!rng);
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
        snn_static_require(app::test_element());
    }
}
