// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/fill.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/fn/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                str s{"abcd"};

                algo::fill(s.range(), 'a');
                snn_require(s == "aaaa");

                algo::fill(s.view(2).range(), 'z');
                snn_require(s == "aazz");

                auto rng = s.range();

                snn_require(rng.count() == 4);
                snn_require(rng.pop_front(assume::not_empty) == 'a');
                snn_require(rng.pop_back(assume::not_empty) == 'z');
                snn_require(rng.count() == 2);

                algo::fill(rng, '-');

                snn_require(s == "a--z");
            }
            {
                array numbers{3, 8, 8, 2};

                snn_require(numbers.count() == 4);
                snn_require(!numbers.all(fn::is{fn::equal_to{}, 9}));
                snn_require(numbers.none(fn::is{fn::equal_to{}, 9}));

                algo::fill(numbers.range(), 9);

                snn_require(numbers.count() == 4);
                snn_require(numbers.all(fn::is{fn::equal_to{}, 9}));
                snn_require(!numbers.none(fn::is{fn::equal_to{}, 9}));
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
    }
}
