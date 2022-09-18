// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/zone/transition.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                time::zone::transition t{1, 123456789};
                snn_require(t.offset_index == 1);
                snn_require(t.when == 123456789);
            }
            {
                time::zone::transition t{2, -123456789};
                snn_require(t.offset_index == 2);
                snn_require(t.when == -123456789);
            }

            return true;
        }

        constexpr bool test_transition()
        {
            static_assert(sizeof(time::zone::transition) == 8);
            static_assert(is_trivially_relocatable_v<time::zone::transition>);

            // Min
            {
                time::zone::transition t{0, -36028797018963968};
                snn_require(t.offset_index == 0);
                snn_require(t.when == -36028797018963968);

                static_assert(time::zone::transition::is_valid(0, -36028797018963968));
                static_assert(!time::zone::transition::is_valid(0, -36028797018963969));
            }

            // Max
            {
                time::zone::transition t{255, 36028797018963967};
                snn_require(t.offset_index == 255);
                snn_require(t.when == 36028797018963967);

                static_assert(time::zone::transition::is_valid(255, 36028797018963967));
                static_assert(!time::zone::transition::is_valid(255, 36028797018963968));
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
        snn_static_require(app::test_transition());
    }
}
