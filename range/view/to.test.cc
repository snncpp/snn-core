// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/to.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/range/view/chunk.hh"
#include "snn-core/range/view/value.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array strings{"12", "42", "17", "9"};
            static_assert(std::is_same_v<decltype(strings), array<cstrview, 4>>);
            snn_require(algo::is_equal(strings.range() | range::v::to<int>{} | range::v::value{},
                                       {12, 42, 17, 9}));

            return true;
        }

        constexpr bool test_to()
        {
            array<char, 5> a{'a', 'b', 'c', 'd', 'e'};

            auto rng = a.range() | range::v::chunk{2} | range::v::to<strview>{};

            usize count = 0;
            for (auto s : rng)
            {
                static_assert(std::is_same_v<decltype(s), strview>);

                if (count == 0)
                {
                    snn_require(s == "ab");
                }
                else if (count == 1)
                {
                    snn_require(s == "cd");
                    s.at(0).value() = 'C';
                }
                else
                {
                    snn_require(s == "e");
                }
                ++count;
            }
            snn_require(count == 3);

            snn_require(a.view() == "abCde");

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(promise::not_empty) == "ab");
            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(promise::not_empty) == "Cd");
            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(promise::not_empty) == "e");
            rng.drop_front(promise::not_empty);

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
        snn_static_require(app::test_to());
    }
}
