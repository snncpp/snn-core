// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/transform.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/chr/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                str s{"abcdefghijklmnopqrstuvwxyz"};
                algo::transform(s.range(), chr::to_alpha_upper);
                snn_require(s == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
                algo::transform(s.range(), chr::to_alpha_lower);
                snn_require(s == "abcdefghijklmnopqrstuvwxyz");
                algo::transform(s.view(3, 20).range(), chr::to_alpha_upper);
                snn_require(s == "abcDEFGHIJKLMNOPQRSTUVWxyz");
            }
            {
                array numbers{3, 7, 11};
                algo::transform(numbers.range(), fn::bind{fn::multiply{}, 2});
                snn_require(algo::is_equal(numbers.range(), {6, 14, 22}));
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
