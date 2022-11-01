// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// This file is generated, DO NOT EDIT MANUALLY.

// IANA Time Zone Database (public domain).

#pragma once

#include "snn-core/array.hh"
#include "snn-core/country/code.hh"
#include "snn-core/time/zone/offset.hh"
#include "snn-core/time/zone/transition.hh"
#include "snn-core/time/zone/db/entry.hh"

namespace snn::time::zone::db::asia
{
    // clang-format off

    namespace detail::baghdad
    {
        inline constexpr array<country::code, 1> country_codes{
            "IQ",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", 10660, false},
            {"BMT", 10656, false},
            {"+03", 10800, false},
            {"+04", 14400, true},
            {"+03", 10800, false},
            {"+04", 14400, true},
        }};

        inline constexpr array<zone::transition, 54> transitions{{
            {1, -2524532260},
            {2, -1641005856},
            {3, 389048400},
            {2, 402264000},
            {3, 417906000},
            {2, 433800000},
            {3, 449614800},
            {2, 465422400},
            {3, 481150800},
            {4, 496792800},
            {5, 512517600},
            {4, 528242400},
            {5, 543967200},
            {4, 559692000},
            {5, 575416800},
            {4, 591141600},
            {5, 606866400},
            {4, 622591200},
            {5, 638316000},
            {4, 654645600},
            {5, 670464000},
            {4, 686275200},
            {5, 702086400},
            {4, 717897600},
            {5, 733622400},
            {4, 749433600},
            {5, 765158400},
            {4, 780969600},
            {5, 796694400},
            {4, 812505600},
            {5, 828316800},
            {4, 844128000},
            {5, 859852800},
            {4, 875664000},
            {5, 891388800},
            {4, 907200000},
            {5, 922924800},
            {4, 938736000},
            {5, 954547200},
            {4, 970358400},
            {5, 986083200},
            {4, 1001894400},
            {5, 1017619200},
            {4, 1033430400},
            {5, 1049155200},
            {4, 1064966400},
            {5, 1080777600},
            {4, 1096588800},
            {5, 1112313600},
            {4, 1128124800},
            {5, 1143849600},
            {4, 1159660800},
            {5, 1175385600},
            {4, 1191196800},
        }};
    }

    inline constexpr db::entry baghdad{"Asia/Baghdad",
                                       "",
                                       "+3321+04425",
                                       detail::baghdad::country_codes,
                                       detail::baghdad::offsets,
                                       detail::baghdad::transitions};

    // clang-format on
}
