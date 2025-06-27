// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fmt/fn/format.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool test_format()
        {
            {
                str out;
                fmt::fn::format format{"{}, {}, {}\n", out};
                format('a', 123, "abc");
                format(1, 2, 3);
                snn_require(out == "a, 123, abc\n1, 2, 3\n");
            }
            {
                str out;
                constexpr cstrview format_string{"{}: {}, "};
                fmt::fn::format format{format_string, out, assume::no_overlap};
                format('a', 'b');
                format(7, 8);
                snn_require(out == "a: b, 7: 8, ");
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::test_format());
    }
}
