// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/json/chr/common.hh"

#include "snn-core/unittest.hh"
#include "snn-core/range/integral.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            usize count = 0;
            for (const auto c : range::integral<char>{})
            {
                if (c >= '0' && c <= '9')
                {
                    snn_require(json::chr::is_digit(c));
                }
                else
                {
                    snn_require(!json::chr::is_digit(c));
                }

                if (to_byte(c) >= 32 && to_byte(c) <= 127 && c != '"' && c != '\\')
                {
                    snn_require(json::chr::is_non_special_ascii_string(c));
                }
                else
                {
                    snn_require(!json::chr::is_non_special_ascii_string(c));
                }

                if (to_byte(c) < ' ' || c == '"' || c == '\\')
                {
                    snn_require(json::chr::is_special_string(c));
                }
                else
                {
                    snn_require(!json::chr::is_special_string(c));
                }

                if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
                {
                    snn_require(json::chr::is_whitespace(c));
                }
                else
                {
                    snn_require(!json::chr::is_whitespace(c));
                }

                ++count;
            }
            snn_require(count == 256);

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
