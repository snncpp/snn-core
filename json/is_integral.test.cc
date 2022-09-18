// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/json/is_integral.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // is_integral

            snn_require(json::is_integral("0"));
            snn_require(json::is_integral("1"));
            snn_require(json::is_integral("10"));
            snn_require(json::is_integral("723"));
            snn_require(json::is_integral("383792612894027642891265701056721"));
            snn_require(json::is_integral("-0"));
            snn_require(json::is_integral("-1"));
            snn_require(json::is_integral("-10"));
            snn_require(json::is_integral("-723"));
            snn_require(json::is_integral("-383792612894027642891265701056721"));

            snn_require(!json::is_integral(""));
            snn_require(!json::is_integral("00"));
            snn_require(!json::is_integral("a"));
            snn_require(!json::is_integral("abc"));
            snn_require(!json::is_integral("0a"));
            snn_require(!json::is_integral("a0"));
            snn_require(!json::is_integral("1a"));
            snn_require(!json::is_integral("1.1"));
            snn_require(!json::is_integral("1e1"));
            snn_require(!json::is_integral("a1"));
            snn_require(!json::is_integral("123a"));
            snn_require(!json::is_integral("a123"));
            snn_require(!json::is_integral("-"));
            snn_require(!json::is_integral("-00"));
            snn_require(!json::is_integral("-a"));
            snn_require(!json::is_integral("-0a"));
            snn_require(!json::is_integral("-a0"));
            snn_require(!json::is_integral("-1a"));
            snn_require(!json::is_integral("-a1"));

            // is_signed_integral

            snn_require(json::is_signed_integral("-0"));
            snn_require(json::is_signed_integral("-1"));
            snn_require(json::is_signed_integral("-10"));
            snn_require(json::is_signed_integral("-723"));
            snn_require(json::is_signed_integral("-383792612894027642891265701056721"));

            snn_require(!json::is_signed_integral(""));
            snn_require(!json::is_signed_integral("0"));
            snn_require(!json::is_signed_integral("1"));
            snn_require(!json::is_signed_integral("10"));
            snn_require(!json::is_signed_integral("723"));
            snn_require(!json::is_signed_integral("383792612894027642891265701056721"));
            snn_require(!json::is_signed_integral("00"));
            snn_require(!json::is_signed_integral("a"));
            snn_require(!json::is_signed_integral("abc"));
            snn_require(!json::is_signed_integral("0a"));
            snn_require(!json::is_signed_integral("a0"));
            snn_require(!json::is_signed_integral("1a"));
            snn_require(!json::is_signed_integral("a1"));
            snn_require(!json::is_signed_integral("-1.1"));
            snn_require(!json::is_signed_integral("-1e1"));
            snn_require(!json::is_signed_integral("123a"));
            snn_require(!json::is_signed_integral("a123"));
            snn_require(!json::is_signed_integral("-"));
            snn_require(!json::is_signed_integral("-00"));
            snn_require(!json::is_signed_integral("-a"));
            snn_require(!json::is_signed_integral("-0a"));
            snn_require(!json::is_signed_integral("-a0"));
            snn_require(!json::is_signed_integral("-1a"));
            snn_require(!json::is_signed_integral("-a1"));

            // is_unsigned_integral

            snn_require(json::is_unsigned_integral("0"));
            snn_require(json::is_unsigned_integral("1"));
            snn_require(json::is_unsigned_integral("10"));
            snn_require(json::is_unsigned_integral("723"));
            snn_require(json::is_unsigned_integral("383792612894027642891265701056721"));

            snn_require(!json::is_unsigned_integral("-0"));
            snn_require(!json::is_unsigned_integral("-1"));
            snn_require(!json::is_unsigned_integral("-10"));
            snn_require(!json::is_unsigned_integral("-383792612894027642891265701056721"));
            snn_require(!json::is_unsigned_integral(""));
            snn_require(!json::is_unsigned_integral("00"));
            snn_require(!json::is_unsigned_integral("a"));
            snn_require(!json::is_unsigned_integral("abc"));
            snn_require(!json::is_unsigned_integral("0a"));
            snn_require(!json::is_unsigned_integral("a0"));
            snn_require(!json::is_unsigned_integral("1a"));
            snn_require(!json::is_unsigned_integral("1.1"));
            snn_require(!json::is_unsigned_integral("1e1"));
            snn_require(!json::is_unsigned_integral("a1"));
            snn_require(!json::is_unsigned_integral("123a"));
            snn_require(!json::is_unsigned_integral("a123"));
            snn_require(!json::is_unsigned_integral("-"));
            snn_require(!json::is_unsigned_integral("-00"));
            snn_require(!json::is_unsigned_integral("-a"));
            snn_require(!json::is_unsigned_integral("-0a"));
            snn_require(!json::is_unsigned_integral("-a0"));
            snn_require(!json::is_unsigned_integral("-1a"));
            snn_require(!json::is_unsigned_integral("-a1"));

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
