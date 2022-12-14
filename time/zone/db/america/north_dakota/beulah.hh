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

namespace snn::time::zone::db::america::north_dakota
{
    // clang-format off

    namespace detail::beulah
    {
        inline constexpr array<country::code, 1> country_codes{
            "US",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", -24427, false},
            {"MDT", -21600, true},
            {"MST", -25200, false},
            {"MWT", -21600, true},
            {"MPT", -21600, true},
            {"MST", -25200, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 150> transitions{{
            {5, -2717643600},
            {1, -1633273200},
            {2, -1615132800},
            {1, -1601823600},
            {2, -1583683200},
            {3, -880210800},
            {4, -769395600},
            {2, -765388800},
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
            {7, 1289116800},
            {6, 1300003200},
            {7, 1320562800},
            {6, 1331452800},
            {7, 1352012400},
            {6, 1362902400},
            {7, 1383462000},
            {6, 1394352000},
            {7, 1414911600},
            {6, 1425801600},
            {7, 1446361200},
            {6, 1457856000},
            {7, 1478415600},
            {6, 1489305600},
            {7, 1509865200},
            {6, 1520755200},
            {7, 1541314800},
            {6, 1552204800},
            {7, 1572764400},
            {6, 1583654400},
            {7, 1604214000},
            {6, 1615708800},
            {7, 1636268400},
            {6, 1647158400},
            {7, 1667718000},
            {6, 1678608000},
            {7, 1699167600},
            {6, 1710057600},
            {7, 1730617200},
            {6, 1741507200},
            {7, 1762066800},
            {6, 1772956800},
            {7, 1793516400},
            {6, 1805011200},
            {7, 1825570800},
            {6, 1836460800},
            {7, 1857020400},
            {6, 1867910400},
            {7, 1888470000},
            {6, 1899360000},
            {7, 1919919600},
            {6, 1930809600},
            {7, 1951369200},
            {6, 1962864000},
            {7, 1983423600},
            {6, 1994313600},
            {7, 2014873200},
            {6, 2025763200},
            {7, 2046322800},
            {6, 2057212800},
            {7, 2077772400},
            {6, 2088662400},
            {7, 2109222000},
            {6, 2120112000},
            {7, 2140671600},
        }};
    }

    inline constexpr db::entry beulah{"America/North_Dakota/Beulah",
                                      "Central - ND (Mercer)",
                                      "+471551-1014640",
                                      detail::beulah::country_codes,
                                      detail::beulah::offsets,
                                      detail::beulah::transitions};

    // clang-format on
}
