// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/to_prefix.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/range/view/element.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array a{"12", "456_", "9abc"};
            static_assert(std::is_same_v<decltype(a), array<cstrview, 3>>);

            snn_require(algo::is_equal(
                a.range() | range::v::to_prefix<int>{} | range::v::element<0>{}, {12, 456, 9}));
            snn_require(algo::is_equal(
                a.range() | range::v::to_prefix<int>{} | range::v::element<1>{}, {2u, 3u, 1u}));

            return true;
        }

        constexpr bool test_to_prefix()
        {
            array a{"12", "456_", "9abc"};
            static_assert(std::is_same_v<decltype(a), array<cstrview, 3>>);

            auto rng = a.range() | range::v::to_prefix<int>{};

            usize count = 0;
            for (auto p : rng)
            {
                static_assert(std::is_same_v<decltype(p), pair::value_count<int, usize>>);

                if (count == 0)
                {
                    snn_require(p.value == 12);
                    snn_require(p.count == 2);
                }
                else if (count == 1)
                {
                    snn_require(p.value == 456);
                    snn_require(p.count == 3);
                }
                else
                {
                    snn_require(p.value == 9);
                    snn_require(p.count == 1);
                }
                ++count;
            }
            snn_require(count == 3);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(assume::not_empty) == pair::value_count<int, usize>{12, 2});
            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(assume::not_empty) == pair::value_count<int, usize>{456, 3});
            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(assume::not_empty) == pair::value_count<int, usize>{9, 1});
            rng.drop_front(assume::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_to_prefix());
    }
}
