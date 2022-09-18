// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/max.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/pair/common.hh"
#include "snn-core/range/step.hh"
#include "snn-core/range/view/element.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                snn_require(algo::max(range::step{0, 255, 50}).value() == 250);

                auto max = algo::max(range::step{0, 255, 50});
                static_assert(std::is_same_v<decltype(max), optional<int>>);
                snn_require(max.has_value());
                snn_require(max.value() == 250);
            }
            {
                array numbers{7, 82, 11, 45};
                snn_require(algo::max(numbers.range()).value() == 82);
                snn_require(algo::max(numbers.range()).value_or(-1) == 82);

                auto empty_rng = numbers.view(99).range();
                snn_require(empty_rng.is_empty());
                snn_require(algo::max(empty_rng).value_or(-1) == -1);

                auto max = algo::max(numbers.range());
                static_assert(std::is_same_v<decltype(max), optional<int&>>);
                snn_require(max.has_value());
                snn_require(max.value() == 82);
            }
            {
                array<pair::name_value<cstrview, int>, 3> pairs{
                    {{"One", 1}, {"Two", 22}, {"Three", 333}}};

                snn_require(algo::max(pairs.range() | range::v::element<0>{}).value() == "Two");
                snn_require(algo::max(pairs.range() | range::v::element<1>{}).value() == 333);

                auto max = algo::max(pairs.range(), fn::element{meta::index<0>, fn::less_than{}});
                static_assert(
                    std::is_same_v<decltype(max), optional<pair::name_value<cstrview, int>&>>);
                snn_require(max.has_value());
                snn_require(max.value().name == "Two");
                snn_require(max.value().value == 22);
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
