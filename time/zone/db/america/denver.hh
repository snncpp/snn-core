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

    namespace detail::denver
    {
        inline constexpr array<country::code, 1> country_codes{
            "US",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", -25196, false},
            {"MDT", -21600, true},
            {"MST", -25200, false},
            {"MWT", -21600, true},
            {"MPT", -21600, true},
        }};

        inline constexpr array<zone::transition, 158> transitions{{
            {2, -2717643600},
            {1, -1633273200},
            {2, -1615132800},
            {1, -1601823600},
            {2, -1583683200},
            {1, -1570374000},
            {2, -1551628800},
            {1, -1538924400},
            {2, -1534089600},
            {3, -880210800},
            {4, -769395600},
            {2, -765388800},
            {1, -147884400},
            {2, -131558400},
            {1, -116434800},
            {2, -100108800},
            {1, -84380400},
            {2, -68659200},
            {1, -52930800},
            {2, -37209600},
            {1, -21481200},
            {2, -5760000},
            {1, 9968400},
            {2, 25689600},
            {1, 41418000},
            {2, 57744000},
            {1, 73472400},
            {2, 89193600},
            {1, 104922000},
            {2, 120643200},
            {1, 126694800},
            {2, 152092800},
            {1, 162378000},
            {2, 183542400},
            {1, 199270800},
            {2, 215596800},
            {1, 230720400},
            {2, 247046400},
            {1, 262774800},
            {2, 278496000},
            {1, 294224400},
            {2, 309945600},
            {1, 325674000},
            {2, 341395200},
            {1, 357123600},
            {2, 372844800},
            {1, 388573200},
            {2, 404899200},
            {1, 420022800},
            {2, 436348800},
            {1, 452077200},
            {2, 467798400},
            {1, 483526800},
            {2, 499248000},
            {1, 514976400},
            {2, 530697600},
            {1, 544611600},
            {2, 562147200},
            {1, 576061200},
            {2, 594201600},
            {1, 607510800},
            {2, 625651200},
            {1, 638960400},
            {2, 657100800},
            {1, 671014800},
            {2, 688550400},
            {1, 702464400},
            {2, 720000000},
            {1, 733914000},
            {2, 752054400},
            {1, 765363600},
            {2, 783504000},
            {1, 796813200},
            {2, 814953600},
            {1, 828867600},
            {2, 846403200},
            {1, 860317200},
            {2, 877852800},
            {1, 891766800},
            {2, 909302400},
            {1, 923216400},
            {2, 941356800},
            {1, 954666000},
            {2, 972806400},
            {1, 986115600},
            {2, 1004256000},
            {1, 1018170000},
            {2, 1035705600},
            {1, 1049619600},
            {2, 1067155200},
            {1, 1081069200},
            {2, 1099209600},
            {1, 1112518800},
            {2, 1130659200},
            {1, 1143968400},
            {2, 1162108800},
            {1, 1173603600},
            {2, 1194163200},
            {1, 1205053200},
            {2, 1225612800},
            {1, 1236502800},
            {2, 1257062400},
            {1, 1268557200},
            {2, 1289116800},
            {1, 1300006800},
            {2, 1320566400},
            {1, 1331456400},
            {2, 1352016000},
            {1, 1362906000},
            {2, 1383465600},
            {1, 1394355600},
            {2, 1414915200},
            {1, 1425805200},
            {2, 1446364800},
            {1, 1457859600},
            {2, 1478419200},
            {1, 1489309200},
            {2, 1509868800},
            {1, 1520758800},
            {2, 1541318400},
            {1, 1552208400},
            {2, 1572768000},
            {1, 1583658000},
            {2, 1604217600},
            {1, 1615712400},
            {2, 1636272000},
            {1, 1647162000},
            {2, 1667721600},
            {1, 1678611600},
            {2, 1699171200},
            {1, 1710061200},
            {2, 1730620800},
            {1, 1741510800},
            {2, 1762070400},
            {1, 1772960400},
            {2, 1793520000},
            {1, 1805014800},
            {2, 1825574400},
            {1, 1836464400},
            {2, 1857024000},
            {1, 1867914000},
            {2, 1888473600},
            {1, 1899363600},
            {2, 1919923200},
            {1, 1930813200},
            {2, 1951372800},
            {1, 1962867600},
            {2, 1983427200},
            {1, 1994317200},
            {2, 2014876800},
            {1, 2025766800},
            {2, 2046326400},
            {1, 2057216400},
            {2, 2077776000},
            {1, 2088666000},
            {2, 2109225600},
            {1, 2120115600},
            {2, 2140675200},
        }};
    }

    inline constexpr db::entry denver{"America/Denver",
                                      "Mountain (most areas)",
                                      "+394421-1045903",
                                      detail::denver::country_codes,
                                      detail::denver::offsets,
                                      detail::denver::transitions};

    // clang-format on
}
