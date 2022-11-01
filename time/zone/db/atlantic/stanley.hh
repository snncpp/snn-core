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

namespace snn::time::zone::db::atlantic
{
    // clang-format off

    namespace detail::stanley
    {
        inline constexpr array<country::code, 1> country_codes{
            "FK",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", -13884, false},
            {"SMT", -13884, false},
            {"-03", -10800, true},
            {"-04", -14400, false},
            {"-02", -7200, true},
            {"-03", -10800, false},
            {"-03", -10800, true},
        }};

        inline constexpr array<zone::transition, 70> transitions{{
            {1, -2524507716},
            {3, -1824235716},
            {2, -1018209600},
            {3, -1003093200},
            {2, -986760000},
            {3, -971643600},
            {2, -954705600},
            {3, -939589200},
            {2, -923256000},
            {3, -908139600},
            {2, -891806400},
            {3, -876690000},
            {2, -860356800},
            {3, -852066000},
            {5, 420609600},
            {4, 433306800},
            {5, 452052000},
            {4, 464151600},
            {5, 483501600},
            {2, 495601200},
            {3, 514350000},
            {2, 527054400},
            {3, 545799600},
            {2, 558504000},
            {3, 577249200},
            {2, 589953600},
            {3, 608698800},
            {2, 621403200},
            {3, 640753200},
            {2, 652852800},
            {3, 672202800},
            {2, 684907200},
            {3, 703652400},
            {2, 716356800},
            {3, 735102000},
            {2, 747806400},
            {3, 766551600},
            {2, 779256000},
            {3, 798001200},
            {2, 810705600},
            {3, 830055600},
            {2, 842760000},
            {3, 861505200},
            {2, 874209600},
            {3, 892954800},
            {2, 905659200},
            {3, 924404400},
            {2, 937108800},
            {3, 955854000},
            {2, 968558400},
            {3, 987310800},
            {2, 999410400},
            {3, 1019365200},
            {2, 1030860000},
            {3, 1050814800},
            {2, 1062914400},
            {3, 1082264400},
            {2, 1094364000},
            {3, 1113714000},
            {2, 1125813600},
            {3, 1145163600},
            {2, 1157263200},
            {3, 1176613200},
            {2, 1188712800},
            {3, 1208667600},
            {2, 1220767200},
            {3, 1240117200},
            {2, 1252216800},
            {3, 1271566800},
            {5, 1283666400},
        }};
    }

    inline constexpr db::entry stanley{"Atlantic/Stanley",
                                       "",
                                       "-5142-05751",
                                       detail::stanley::country_codes,
                                       detail::stanley::offsets,
                                       detail::stanley::transitions};

    // clang-format on
}
