// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#define SNN_DEBUG 1

#include "snn-core/debug.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            snn_debug("Testing debug function macros: ", 123, " foobar ", true, ' ', false, '\n');
            snn_debug_line("Floating point: ", 123.456, " ", 0.333f);
            snn_debug_line("Non-ascii string: ", "UTF-8: ", cstrview{"åäö"});
            snn_debug_line("Binary string: ", cstrview{"\x03\x02\x01\x00"});

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());
    }
}
