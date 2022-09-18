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

    namespace detail::new_salem
    {
        inline constexpr array<country::code, 1> country_codes{
            "US",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", -24339, false},
            {"MDT", -21600, true},
            {"MST", -25200, false},
            {"MWT", -21600, true},
            {"MPT", -21600, true},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 150> transitions{{
            {2, -2717643600},
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
            {6, 1067155200},
            {5, 1081065600},
            {6, 1099206000},
            {5, 1112515200},
            {6, 1130655600},
            {5, 1143964800},
            {6, 1162105200},
            {5, 1173600000},
            {6, 1194159600},
            {5, 1205049600},
            {6, 1225609200},
            {5, 1236499200},
            {6, 1257058800},
            {5, 1268553600},
            {6, 1289113200},
            {5, 1300003200},
            {6, 1320562800},
            {5, 1331452800},
            {6, 1352012400},
            {5, 1362902400},
            {6, 1383462000},
            {5, 1394352000},
            {6, 1414911600},
            {5, 1425801600},
            {6, 1446361200},
            {5, 1457856000},
            {6, 1478415600},
            {5, 1489305600},
            {6, 1509865200},
            {5, 1520755200},
            {6, 1541314800},
            {5, 1552204800},
            {6, 1572764400},
            {5, 1583654400},
            {6, 1604214000},
            {5, 1615708800},
            {6, 1636268400},
            {5, 1647158400},
            {6, 1667718000},
            {5, 1678608000},
            {6, 1699167600},
            {5, 1710057600},
            {6, 1730617200},
            {5, 1741507200},
            {6, 1762066800},
            {5, 1772956800},
            {6, 1793516400},
            {5, 1805011200},
            {6, 1825570800},
            {5, 1836460800},
            {6, 1857020400},
            {5, 1867910400},
            {6, 1888470000},
            {5, 1899360000},
            {6, 1919919600},
            {5, 1930809600},
            {6, 1951369200},
            {5, 1962864000},
            {6, 1983423600},
            {5, 1994313600},
            {6, 2014873200},
            {5, 2025763200},
            {6, 2046322800},
            {5, 2057212800},
            {6, 2077772400},
            {5, 2088662400},
            {6, 2109222000},
            {5, 2120112000},
            {6, 2140671600},
        }};
    }

    inline constexpr db::entry new_salem{"America/North_Dakota/New_Salem",
                                         "Central - ND (Morton rural)",
                                         "+465042-1012439",
                                         detail::new_salem::country_codes,
                                         detail::new_salem::offsets,
                                         detail::new_salem::transitions};

    // clang-format on
}
