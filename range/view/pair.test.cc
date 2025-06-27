// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/pair.hh"

#include "snn-core/array.hh"
#include "snn-core/strcore.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/range/step.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array a{2, 5, 99};
            array b{1, 3};

            range::view::pair combined{a.range(), b.range()};

            snn_require(combined);
            snn_require(!combined.is_empty());

            // Both ranges are contiguous, so we have a count.
            snn_require(combined.count() == 2);

            snn_require(algo::is_equal(combined,
                                       init_list<pair::first_second<int>>{{2, 1}, {5, 3}}));

            static_assert(std::is_same_v<decltype(combined.front(assume::not_empty)),
                                         pair::first_second<int&, int&>>);

            return true;
        }

        constexpr bool test_pair()
        {
            {
                array a{2, 5, 99};
                array b{1, 3};

                snn_require(algo::is_equal(range::view::pair{a.range(), b.range()},
                                           init_list<pair::first_second<int>>{{2, 1}, {5, 3}}));
                snn_require(algo::is_equal(range::view::pair{b.range(), a.range()},
                                           init_list<pair::first_second<int>>{{1, 2}, {3, 5}}));

                snn_require(algo::is_equal(range::view::pair{a.range(), range::step{7, 9}},
                                           init_list<pair::first_second<int>>{{2, 7}, {5, 8}}));
                snn_require(algo::is_equal(range::view::pair{range::step{7, 9}, a.range()},
                                           init_list<pair::first_second<int>>{{7, 2}, {8, 5}}));

                snn_require(range::view::pair{a.view(99).range(), a.range()}.is_empty());
                snn_require(range::view::pair{a.view(99).range(), a.range()}.count() == 0);
                snn_require(range::view::pair{a.range(), a.view(99).range()}.is_empty());
                snn_require(range::view::pair{a.range(), a.view(99).range()}.count() == 0);

                range::view::pair rng{a.range(), range::step{1, 2}};
                static_assert(!has_count<decltype(rng)>);
                snn_require(rng);
                snn_require(!rng.is_empty());

                auto p = rng.front(assume::not_empty);
                rng.drop_front(assume::not_empty);
                static_assert(std::is_same_v<decltype(p), pair::first_second<int&, int>>);
                snn_require(p.first == 2);
                snn_require(p.second == 1);

                snn_require(!rng);
                snn_require(rng.is_empty());
            }
            {
                array<str, 4> a{"abc", "ABC", "abcdefghijklmnopqrstuvwxyz", "123"};
                constexpr array<usize, 6> b{5u, 2u, 9u, 1u, 12u, 17u};

                auto rng = range::view::pair{a.range(), b.range()};
                snn_require(rng.count() == 4);

                usize count = 0;
                for (auto p : rng)
                {
                    p.first.truncate(p.second);
                    ++count;
                }
                snn_require(count == 4);

                snn_require(a.at(0).value() == "abc");
                snn_require(a.at(1).value() == "AB");
                snn_require(a.at(2).value() == "abcdefghi");
                snn_require(a.at(3).value() == "1");

                snn_require(rng.count() == 4);

                auto p = rng.front(assume::not_empty);
                static_assert(std::is_same_v<decltype(p), pair::first_second<str&, const usize&>>);
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
        snn_static_require(app::test_pair());
    }
}
