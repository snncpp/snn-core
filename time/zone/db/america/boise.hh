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

    namespace detail::boise
    {
        inline constexpr array<country::code, 1> country_codes{
            "US",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", -27889, false},
            {"PDT", -25200, true},
            {"PST", -28800, false},
            {"MWT", -21600, true},
            {"MPT", -21600, true},
            {"MST", -25200, false},
            {"MDT", -21600, true},
        }};

        inline constexpr array<zone::transition, 151> transitions{{
            {2, -2717640000},
            {1, -1633269600},
            {2, -1615129200},
            {1, -1601820000},
            {2, -1583679600},
            {5, -1471788000},
            {3, -880210800},
            {4, -769395600},
            {5, -765388800},
            {6, -84380400},
            {5, -68659200},
            {6, -52930800},
            {5, -37209600},
            {6, -21481200},
            {5, -5760000},
            {6, 9968400},
            {5, 25689600},
            {6, 41418000},
            {5, 57744000},
            {6, 73472400},
            {5, 89193600},
            {6, 104922000},
            {5, 120643200},
            {6, 129114000},
            {5, 152092800},
            {6, 162378000},
            {5, 183542400},
            {6, 199270800},
            {5, 215596800},
            {6, 230720400},
            {5, 247046400},
            {6, 262774800},
            {5, 278496000},
            {6, 294224400},
            {5, 309945600},
            {6, 325674000},
            {5, 341395200},
            {6, 357123600},
            {5, 372844800},
            {6, 388573200},
            {5, 404899200},
            {6, 420022800},
            {5, 436348800},
            {6, 452077200},
            {5, 467798400},
            {6, 483526800},
            {5, 499248000},
            {6, 514976400},
            {5, 530697600},
            {6, 544611600},
            {5, 562147200},
            {6, 576061200},
            {5, 594201600},
            {6, 607510800},
            {5, 625651200},
            {6, 638960400},
            {5, 657100800},
            {6, 671014800},
            {5, 688550400},
            {6, 702464400},
            {5, 720000000},
            {6, 733914000},
            {5, 752054400},
            {6, 765363600},
            {5, 783504000},
            {6, 796813200},
            {5, 814953600},
            {6, 828867600},
            {5, 846403200},
            {6, 860317200},
            {5, 877852800},
            {6, 891766800},
            {5, 909302400},
            {6, 923216400},
            {5, 941356800},
            {6, 954666000},
            {5, 972806400},
            {6, 986115600},
            {5, 1004256000},
            {6, 1018170000},
            {5, 1035705600},
            {6, 1049619600},
            {5, 1067155200},
            {6, 1081069200},
            {5, 1099209600},
            {6, 1112518800},
            {5, 1130659200},
            {6, 1143968400},
            {5, 1162108800},
            {6, 1173603600},
            {5, 1194163200},
            {6, 1205053200},
            {5, 1225612800},
            {6, 1236502800},
            {5, 1257062400},
            {6, 1268557200},
            {5, 1289116800},
            {6, 1300006800},
            {5, 1320566400},
            {6, 1331456400},
            {5, 1352016000},
            {6, 1362906000},
            {5, 1383465600},
            {6, 1394355600},
            {5, 1414915200},
            {6, 1425805200},
            {5, 1446364800},
            {6, 1457859600},
            {5, 1478419200},
            {6, 1489309200},
            {5, 1509868800},
            {6, 1520758800},
            {5, 1541318400},
            {6, 1552208400},
            {5, 1572768000},
            {6, 1583658000},
            {5, 1604217600},
            {6, 1615712400},
            {5, 1636272000},
            {6, 1647162000},
            {5, 1667721600},
            {6, 1678611600},
            {5, 1699171200},
            {6, 1710061200},
            {5, 1730620800},
            {6, 1741510800},
            {5, 1762070400},
            {6, 1772960400},
            {5, 1793520000},
            {6, 1805014800},
            {5, 1825574400},
            {6, 1836464400},
            {5, 1857024000},
            {6, 1867914000},
            {5, 1888473600},
            {6, 1899363600},
            {5, 1919923200},
            {6, 1930813200},
            {5, 1951372800},
            {6, 1962867600},
            {5, 1983427200},
            {6, 1994317200},
            {5, 2014876800},
            {6, 2025766800},
            {5, 2046326400},
            {6, 2057216400},
            {5, 2077776000},
            {6, 2088666000},
            {5, 2109225600},
            {6, 2120115600},
            {5, 2140675200},
        }};
    }

    inline constexpr db::entry boise{"America/Boise",
                                     "Mountain - ID (south); OR (east)",
                                     "+433649-1161209",
                                     detail::boise::country_codes,
                                     detail::boise::offsets,
                                     detail::boise::transitions};

    // clang-format on
}
