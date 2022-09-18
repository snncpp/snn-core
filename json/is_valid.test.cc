// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/json/is_valid.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(json::is_valid("0"));
            snn_require(json::is_valid("true"));
            snn_require(json::is_valid(R"(["One", "Two"])"));

            snn_require(!json::is_valid(""));
            snn_require(!json::is_valid("TRUE"));
            snn_require(!json::is_valid(R"(["One", "Two",])")); // Trailing comma.

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        // More tests in: json/decoder.test.cc
    }
}
