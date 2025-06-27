// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/reverse.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array numbers{3, 11, 7};

            snn_require(algo::is_equal(numbers.range(), {3, 11, 7}));
            snn_require(algo::is_equal(numbers.range() | range::v::reverse{}, {7, 11, 3}));

            snn_require(algo::is_equal(numbers.view(0, 2).range(), {3, 11}));
            snn_require(algo::is_equal(numbers.view(0, 2).range() | range::v::reverse{}, {11, 3}));

            snn_require(algo::is_equal(numbers.view(0, 1).range(), {3}));
            snn_require(algo::is_equal(numbers.view(0, 1).range() | range::v::reverse{}, {3}));

            snn_require(range::view::reverse{numbers.view(99).range()}.is_empty());

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        {
            array<int, 3> a{37, 365, 1};
            int last = 0;
            for (auto& i : range::view::reverse{a.range()})
            {
                last = i;
                ++i;
            }
            snn_require(last == 37);
            snn_require(a.at(0).value() == 38);
            snn_require(a.at(1).value() == 366);
            snn_require(a.at(2).value() == 2);
        }

        {
            constexpr array<int, 3> a{42, 9, 11};
            range::view::reverse rng{a.range()};

            // Concepts
            static_assert(!contiguous_range<decltype(rng)>);
            static_assert(!random_access_range<decltype(rng)>);
            static_assert(!bidirectional_range<decltype(rng)>);
            static_assert(forward_range<decltype(rng)>);
            static_assert(input_range<decltype(rng)>);

            snn_require(rng);
            snn_require(!rng.is_empty());
            {
                const auto i = rng.front(assume::not_empty);
                snn_require(i == 11);
            }
            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            {
                const auto i = rng.front(assume::not_empty);
                snn_require(i == 9);
            }
            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            {
                const auto i = rng.front(assume::not_empty);
                snn_require(i == 42);
            }
            rng.drop_front(assume::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());
        }

        {
            const array<str, 3> a{"One two three", "A long string that goes on the heap.", "ABC"};

            usize count = 0;
            for (const auto& s : range::view::reverse{a.range()})
            {
                if (count == 0)
                {
                    snn_require(s == "ABC");
                }
                else if (count == 1)
                {
                    snn_require(s == "A long string that goes on the heap.");
                }
                else if (count == 2)
                {
                    snn_require(s == "One two three");
                }

                ++count;
            }
            snn_require(count == 3);
        }
    }
}
