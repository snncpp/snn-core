// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/json/is_floating_point.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(json::is_floating_point("0.0"));
            snn_require(json::is_floating_point("0.75"));
            snn_require(json::is_floating_point("3.1"));
            snn_require(json::is_floating_point("3.92"));
            snn_require(json::is_floating_point("74.3"));
            snn_require(json::is_floating_point("74.13"));
            snn_require(json::is_floating_point("-0.0"));
            snn_require(json::is_floating_point("-0.75"));
            snn_require(json::is_floating_point("-3.1"));
            snn_require(json::is_floating_point("-3.92"));
            snn_require(json::is_floating_point("-74.3"));
            snn_require(json::is_floating_point("-74.13"));

            snn_require(json::is_floating_point("0.0e0"));
            snn_require(json::is_floating_point("0.75E12"));
            snn_require(json::is_floating_point("3.1e3"));
            snn_require(json::is_floating_point("3.92E789"));
            snn_require(json::is_floating_point("74.3E2"));
            snn_require(json::is_floating_point("74.13e74"));
            snn_require(json::is_floating_point("-0.0e0"));
            snn_require(json::is_floating_point("-0.75E12"));
            snn_require(json::is_floating_point("-3.1e3"));
            snn_require(json::is_floating_point("-3.92E789"));
            snn_require(json::is_floating_point("-74.3E2"));
            snn_require(json::is_floating_point("-74.13e74"));

            snn_require(json::is_floating_point("0.0e+0"));
            snn_require(json::is_floating_point("0.75E-12"));
            snn_require(json::is_floating_point("3.1e-3"));
            snn_require(json::is_floating_point("3.92E+789"));
            snn_require(json::is_floating_point("74.3E+2"));
            snn_require(json::is_floating_point("74.13e-74"));
            snn_require(json::is_floating_point("-0.0e+0"));
            snn_require(json::is_floating_point("-0.75E-12"));
            snn_require(json::is_floating_point("-3.1e-3"));
            snn_require(json::is_floating_point("-3.92E+789"));
            snn_require(json::is_floating_point("-74.3E+2"));
            snn_require(json::is_floating_point("-74.13e-74"));

            snn_require(
                json::is_floating_point("7438362456912056172.132023637912397e-743749264712047"));
            snn_require(

                json::is_floating_point("-7438362456912056172.132023637912397e-743749264712047"));

            snn_require(!json::is_floating_point("0"));
            snn_require(!json::is_floating_point("3"));
            snn_require(!json::is_floating_point("74"));
            snn_require(!json::is_floating_point("-0"));
            snn_require(!json::is_floating_point("-3"));
            snn_require(!json::is_floating_point("-74"));

            snn_require(!json::is_floating_point(""));
            snn_require(!json::is_floating_point("-"));
            snn_require(!json::is_floating_point("a"));
            snn_require(!json::is_floating_point("abc"));
            snn_require(!json::is_floating_point("00"));
            snn_require(!json::is_floating_point("3a"));
            snn_require(!json::is_floating_point("x3"));
            snn_require(!json::is_floating_point("74abc"));
            snn_require(!json::is_floating_point("-00"));
            snn_require(!json::is_floating_point("--3"));
            snn_require(!json::is_floating_point("a-74"));

            snn_require(!json::is_floating_point("00.0"));
            snn_require(!json::is_floating_point("0..75"));
            snn_require(!json::is_floating_point("0."));
            snn_require(!json::is_floating_point("0.0e"));
            snn_require(!json::is_floating_point("3.1a"));
            snn_require(!json::is_floating_point("3.92abc"));
            snn_require(!json::is_floating_point("a74.3"));
            snn_require(!json::is_floating_point("abc74.13"));
            snn_require(!json::is_floating_point("--0.0"));
            snn_require(!json::is_floating_point("-0.75a"));
            snn_require(!json::is_floating_point("-3.1abc"));
            snn_require(!json::is_floating_point("-3.92.38"));
            snn_require(!json::is_floating_point("-74.13-"));

            snn_require(!json::is_floating_point("0.0f0"));
            snn_require(!json::is_floating_point("0.75Ee12"));
            snn_require(!json::is_floating_point("3.1ee3"));
            snn_require(!json::is_floating_point("3.92E789E123"));
            snn_require(!json::is_floating_point("74.3E2a"));
            snn_require(!json::is_floating_point("74.13e74ABC"));
            snn_require(!json::is_floating_point("-0.0f0"));
            snn_require(!json::is_floating_point("-0.75Ee12"));
            snn_require(!json::is_floating_point("-3.1ee3"));
            snn_require(!json::is_floating_point("-3.92E789E123"));
            snn_require(!json::is_floating_point("-74.3E2a"));
            snn_require(!json::is_floating_point("-74.13e74ABC"));

            snn_require(!json::is_floating_point("0.0e*0"));
            snn_require(!json::is_floating_point("0.75E--12"));
            snn_require(!json::is_floating_point("3.1e-3e-3"));
            snn_require(!json::is_floating_point("3.92E++789"));
            snn_require(!json::is_floating_point("74.3E+2a"));
            snn_require(!json::is_floating_point("74.13e-74abc"));
            snn_require(!json::is_floating_point("-0.0e*0"));
            snn_require(!json::is_floating_point("-0.75E--12"));
            snn_require(!json::is_floating_point("-3.1e-3e-3"));
            snn_require(!json::is_floating_point("-3.92E++789"));
            snn_require(!json::is_floating_point("-74.3E+2a"));
            snn_require(!json::is_floating_point("-74.13e-74abc"));

            snn_require(
                !json::is_floating_point("743a8362456912056172.132023637912397e-743749264712047"));
            snn_require(
                !json::is_floating_point("-743a8362456912056172.132023637912397e-743749264712047"));
            snn_require(
                !json::is_floating_point("7438362456912056172.1320b23637912397e-743749264712047"));
            snn_require(
                !json::is_floating_point("-7438362456912056172.1320b23637912397e-743749264712047"));
            snn_require(
                !json::is_floating_point("7438362456912056172.132023637912397e-743749264712047a"));
            snn_require(
                !json::is_floating_point("-7438362456912056172.132023637912397e-743749264712047a"));
            snn_require(
                !json::is_floating_point("a7438362456912056172.132023637912397e-743749264712047"));
            snn_require(
                !json::is_floating_point("a-7438362456912056172.132023637912397e-743749264712047"));

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
