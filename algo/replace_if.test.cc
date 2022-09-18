// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/replace_if.hh"

#include "snn-core/unittest.hh"
#include "snn-core/chr/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            str s = "ab\tcd\n";
            snn_require(s == "ab\tcd\n");

            algo::replace_if(s.range(), chr::is_ascii_control, '?');
            snn_require(s == "ab?cd?");

            algo::replace_if(s.range(), chr::is_ascii_control, '?');
            snn_require(s == "ab?cd?");

            s.clear();
            algo::replace_if(s.range(), chr::is_ascii_control, '?');
            snn_require(s.is_empty());

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
