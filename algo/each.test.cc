// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/each.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/fn/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            str src{"abcdefghijklmnopqrstuvwxyz"};
            algo::each(src.range(), chr::to_alpha_upper_inplace);
            snn_require(src == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

            array a{1, 2, 3};
            algo::each(a.range(), [](int& i) { i *= 2; });
            snn_require(a.at(0).value() == 2);
            snn_require(a.at(1).value() == 4);
            snn_require(a.at(2).value() == 6);

            return true;
        }

        constexpr bool test_each()
        {
            {
                const str src{"abcdefghijklmnopqrstuvwxyz"};
                str dest;
                snn_require(src == "abcdefghijklmnopqrstuvwxyz");
                snn_require(dest == "");
                algo::each(src.range(), fn::stream_to{dest});
                snn_require(src == "abcdefghijklmnopqrstuvwxyz");
                snn_require(dest == "abcdefghijklmnopqrstuvwxyz");
            }
            {
                const str src;
                str dest;
                snn_require(src == "");
                snn_require(dest == "");
                algo::each(src.range(), fn::stream_to{dest});
                snn_require(src == "");
                snn_require(dest == "");
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
        snn_static_require(app::test_each());
    }
}
