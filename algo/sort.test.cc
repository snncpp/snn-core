// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/sort.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/fn/invoke.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array numbers{3, 8, 2};
            snn_require(algo::is_equal(numbers.range(), {3, 8, 2}));

            algo::sort(numbers.range());
            snn_require(algo::is_equal(numbers.range(), {2, 3, 8}));

            algo::sort(numbers.range(), fn::greater_than{});
            snn_require(algo::is_equal(numbers.range(), {8, 3, 2}));

            return true;
        }

        constexpr bool test_sort()
        {
            {
                array<int, 0> numbers;
                algo::sort(numbers.range());
            }
            {
                array numbers{3};
                algo::sort(numbers.range());
                snn_require(numbers.at(0) == 3);
            }
            {
                struct person
                {
                    int id;
                    cstrview name;
                };

                array<person, 2> persons{{{123, "foo"}, {456, "bar"}}};

                snn_require(persons.front().value().name == "foo");
                snn_require(persons.back().value().name == "bar");

                algo::sort(persons.range(), fn::invoke{fn::less_than{}, &person::name});

                snn_require(persons.front().value().name == "bar");
                snn_require(persons.back().value().name == "foo");

                algo::sort(persons.range(), fn::invoke{fn::lt{}, &person::id});

                snn_require(persons.front().value().name == "foo");
                snn_require(persons.back().value().name == "bar");
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
        snn_static_require(app::test_sort());
    }
}
