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

    namespace detail::anadyr
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 10> offsets{{
            {"LMT", 42596, false},
            {"+12", 43200, false},
            {"+14", 50400, true},
            {"+13", 46800, false},
            {"+13", 46800, true},
            {"+12", 43200, false},
            {"+13", 46800, true},
            {"+12", 43200, true},
            {"+11", 39600, false},
            {"+12", 43200, false},
        }};

        inline constexpr array<zone::transition, 65> transitions{{
            {1, -1441194596},
            {3, -1247572800},
            {2, 354884400},
            {3, 370692000},
            {4, 386420400},
            {1, 402231600},
            {4, 417960000},
            {1, 433767600},
            {4, 449582400},
            {5, 465314400},
            {6, 481039200},
            {5, 496764000},
            {6, 512488800},
            {5, 528213600},
            {6, 543938400},
            {5, 559663200},
            {6, 575388000},
            {5, 591112800},
            {6, 606837600},
            {5, 622562400},
            {6, 638287200},
            {5, 654616800},
            {7, 670341600},
            {8, 686070000},
            {5, 695746800},
            {6, 701791200},
            {5, 717516000},
            {6, 733240800},
            {5, 748965600},
            {6, 764690400},
            {5, 780415200},
            {6, 796140000},
            {5, 811864800},
            {6, 828194400},
            {5, 846338400},
            {6, 859644000},
            {5, 877788000},
            {6, 891093600},
            {5, 909237600},
            {6, 922543200},
            {5, 941292000},
            {6, 953992800},
            {5, 972741600},
            {6, 985442400},
            {5, 1004191200},
            {6, 1017496800},
            {5, 1035640800},
            {6, 1048946400},
            {5, 1067090400},
            {6, 1080396000},
            {5, 1099144800},
            {6, 1111845600},
            {5, 1130594400},
            {6, 1143295200},
            {5, 1162044000},
            {6, 1174744800},
            {5, 1193493600},
            {6, 1206799200},
            {5, 1224943200},
            {6, 1238248800},
            {5, 1256392800},
            {7, 1269698400},
            {8, 1288450800},
            {5, 1301151600},
            {5, 2147483647},
        }};
    }

    inline constexpr db::entry anadyr{"Asia/Anadyr",
                                      "MSK+09 - Bering Sea",
                                      "+6445+17729",
                                      detail::anadyr::country_codes,
                                      detail::anadyr::offsets,
                                      detail::anadyr::transitions};

    // clang-format on
}
