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

namespace snn::time::zone::db::america
{
    // clang-format off

    namespace detail::whitehorse
    {
        inline constexpr array<country::code, 1> country_codes{
            "CA",
        };

        inline constexpr array<zone::offset, 9> offsets{{
            {"LMT", -32412, false},
            {"YDT", -28800, true},
            {"YST", -32400, false},
            {"YWT", -28800, true},
            {"YPT", -28800, true},
            {"YDDT", -25200, true},
            {"PST", -28800, false},
            {"PDT", -25200, true},
            {"MST", -25200, false},
        }};

        inline constexpr array<zone::transition, 93> transitions{{
            {2, -2188997988},
            {1, -1632056400},
            {2, -1615125600},
            {1, -1596978000},
            {2, -1583164800},
            {3, -880203600},
            {4, -769395600},
            {2, -765381600},
            {5, -147884400},
            {2, -131554800},
            {6, -81961200},
            {7, 325677600},
            {6, 341398800},
            {7, 357127200},
            {6, 372848400},
            {7, 388576800},
            {6, 404902800},
            {7, 420026400},
            {6, 436352400},
            {7, 452080800},
            {6, 467802000},
            {7, 483530400},
            {6, 499251600},
            {7, 514980000},
            {6, 530701200},
            {7, 544615200},
            {6, 562150800},
            {7, 576064800},
            {6, 594205200},
            {7, 607514400},
            {6, 625654800},
            {7, 638964000},
            {6, 657104400},
            {7, 671018400},
            {6, 688554000},
            {7, 702468000},
            {6, 720003600},
            {7, 733917600},
            {6, 752058000},
            {7, 765367200},
            {6, 783507600},
            {7, 796816800},
            {6, 814957200},
            {7, 828871200},
            {6, 846406800},
            {7, 860320800},
            {6, 877856400},
            {7, 891770400},
            {6, 909306000},
            {7, 923220000},
            {6, 941360400},
            {7, 954669600},
            {6, 972810000},
            {7, 986119200},
            {6, 1004259600},
            {7, 1018173600},
            {6, 1035709200},
            {7, 1049623200},
            {6, 1067158800},
            {7, 1081072800},
            {6, 1099213200},
            {7, 1112522400},
            {6, 1130662800},
            {7, 1143972000},
            {6, 1162112400},
            {7, 1173607200},
            {6, 1194166800},
            {7, 1205056800},
            {6, 1225616400},
            {7, 1236506400},
            {6, 1257066000},
            {7, 1268560800},
            {6, 1289120400},
            {7, 1300010400},
            {6, 1320570000},
            {7, 1331460000},
            {6, 1352019600},
            {7, 1362909600},
            {6, 1383469200},
            {7, 1394359200},
            {6, 1414918800},
            {7, 1425808800},
            {6, 1446368400},
            {7, 1457863200},
            {6, 1478422800},
            {7, 1489312800},
            {6, 1509872400},
            {7, 1520762400},
            {6, 1541322000},
            {7, 1552212000},
            {6, 1572771600},
            {7, 1583661600},
            {8, 1604214000},
        }};
    }

    inline constexpr db::entry whitehorse{"America/Whitehorse",
                                          "MST - Yukon (east)",
                                          "+6043-13503",
                                          detail::whitehorse::country_codes,
                                          detail::whitehorse::offsets,
                                          detail::whitehorse::transitions};

    // clang-format on
}
