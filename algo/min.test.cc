// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/min.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/pair/common.hh"
#include "snn-core/range/view/element.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                array numbers{7, 82, 2, 45};
                snn_require(algo::min(numbers.range()).value() == 2);
                snn_require(algo::min(numbers.range()).value_or(-1) == 2);

                auto empty_rng = numbers.view(99).range();
                snn_require(empty_rng.is_empty());
                snn_require(algo::min(empty_rng).value_or(-1) == -1);

                auto min = algo::min(numbers.range());
                static_assert(std::is_same_v<decltype(min), optional<int&>>);
                snn_require(min.has_value());
                snn_require(min.value() == 2);
            }
            {
                array<pair::name_value<cstrview, int>, 4> pairs{
                    {{"One", 1}, {"Two", 22}, {"Three", 333}, {"Four", 4444}}};
                snn_require(algo::min(pairs.range() | range::v::element<0>{}).value() == "Four");
                snn_require(algo::min(pairs.range() | range::v::element<1>{}).value() == 1);
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
