// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/count_if.hh"

#include "snn-core/unittest.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/math/fn/common.hh"
#include "snn-core/range/step.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::count_if(cstrrng{}, chr::is_alpha_lower) == 0);
            snn_require(algo::count_if(cstrrng{"a"}, chr::is_alpha_lower) == 1);
            snn_require(algo::count_if(cstrrng{"abc"}, chr::is_alpha_lower) == 3);
            snn_require(algo::count_if(cstrrng{"aBc"}, chr::is_alpha_lower) == 2);
            snn_require(algo::count_if(cstrrng{"ABC"}, chr::is_alpha_lower) == 0);
            snn_require(algo::count_if(cstrrng{"åäö"}, chr::is_alpha_lower) == 0);

            snn_require(algo::count_if(range::step{0, 0}, math::fn::is_odd{}) == 0);
            snn_require(algo::count_if(range::step{0, 1}, math::fn::is_odd{}) == 0);
            snn_require(algo::count_if(range::step{0, 2}, math::fn::is_odd{}) == 1);
            snn_require(algo::count_if(range::step{0, 3}, math::fn::is_odd{}) == 1);
            snn_require(algo::count_if(range::step{0, 4}, math::fn::is_odd{}) == 2);

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
