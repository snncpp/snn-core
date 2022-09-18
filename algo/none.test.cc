// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/none.hh"

#include "snn-core/unittest.hh"
#include "snn-core/fn/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::none(cstrrng{""}, fn::is{fn::equal_to{}, 'a'}));
            snn_require(algo::none(cstrrng{""}, fn::is{fn::equal_to{}, 'b'}));
            snn_require(algo::none(cstrrng{""}, fn::is{fn::equal_to{}, '\0'}));

            snn_require(!algo::none(cstrrng{"a"}, fn::is{fn::equal_to{}, 'a'}));
            snn_require(algo::none(cstrrng{"a"}, fn::is{fn::equal_to{}, 'b'}));
            snn_require(algo::none(cstrrng{"a"}, fn::is{fn::equal_to{}, '\0'}));

            snn_require(!algo::none(cstrrng{"aa"}, fn::is{fn::equal_to{}, 'a'}));
            snn_require(algo::none(cstrrng{"aa"}, fn::is{fn::equal_to{}, 'b'}));
            snn_require(algo::none(cstrrng{"aa"}, fn::is{fn::equal_to{}, '\0'}));

            snn_require(!algo::none(cstrrng{"ab"}, fn::is{fn::equal_to{}, 'a'}));
            snn_require(!algo::none(cstrrng{"ab"}, fn::is{fn::equal_to{}, 'b'}));
            snn_require(algo::none(cstrrng{"ab"}, fn::is{fn::equal_to{}, '\0'}));

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
