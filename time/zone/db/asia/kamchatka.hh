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

    namespace detail::kamchatka
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 9> offsets{{
            {"LMT", 38076, false},
            {"+11", 39600, false},
            {"+13", 46800, true},
            {"+12", 43200, false},
            {"+12", 43200, false},
            {"+13", 46800, true},
            {"+12", 43200, true},
            {"+11", 39600, false},
            {"+12", 43200, false},
        }};

        inline constexpr array<zone::transition, 64> transitions{{
            {1, -1487759676},
            {3, -1247569200},
            {2, 354888000},
            {3, 370695600},
            {2, 386424000},
            {3, 402231600},
            {2, 417960000},
            {3, 433767600},
            {2, 449582400},
            {4, 465314400},
            {5, 481039200},
            {4, 496764000},
            {5, 512488800},
            {4, 528213600},
            {5, 543938400},
            {4, 559663200},
            {5, 575388000},
            {4, 591112800},
            {5, 606837600},
            {4, 622562400},
            {5, 638287200},
            {4, 654616800},
            {6, 670341600},
            {7, 686070000},
            {4, 695746800},
            {5, 701791200},
            {4, 717516000},
            {5, 733240800},
            {4, 748965600},
            {5, 764690400},
            {4, 780415200},
            {5, 796140000},
            {4, 811864800},
            {5, 828194400},
            {4, 846338400},
            {5, 859644000},
            {4, 877788000},
            {5, 891093600},
            {4, 909237600},
            {5, 922543200},
            {4, 941292000},
            {5, 953992800},
            {4, 972741600},
            {5, 985442400},
            {4, 1004191200},
            {5, 1017496800},
            {4, 1035640800},
            {5, 1048946400},
            {4, 1067090400},
            {5, 1080396000},
            {4, 1099144800},
            {5, 1111845600},
            {4, 1130594400},
            {5, 1143295200},
            {4, 1162044000},
            {5, 1174744800},
            {4, 1193493600},
            {5, 1206799200},
            {4, 1224943200},
            {5, 1238248800},
            {4, 1256392800},
            {6, 1269698400},
            {7, 1288450800},
            {4, 1301151600},
        }};
    }

    inline constexpr db::entry kamchatka{"Asia/Kamchatka",
                                         "MSK+09 - Kamchatka",
                                         "+5301+15839",
                                         detail::kamchatka::country_codes,
                                         detail::kamchatka::offsets,
                                         detail::kamchatka::transitions};

    // clang-format on
}
