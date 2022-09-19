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

    namespace detail::winnipeg
    {
        inline constexpr array<country::code, 1> country_codes{
            "CA",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", -23316, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
            {"CWT", -18000, true},
            {"CPT", -18000, true},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 186> transitions{{
            {2, -2602258284},
            {1, -1694368800},
            {2, -1681671600},
            {1, -1632067200},
            {2, -1615136400},
            {1, -1029686400},
            {2, -1018198800},
            {3, -880214400},
            {4, -769395600},
            {2, -765392400},
            {1, -746035200},
            {2, -732733200},
            {1, -715795200},
            {2, -702493200},
            {1, -684345600},
            {2, -671043600},
            {1, -652896000},
            {2, -639594000},
            {1, -620755200},
            {2, -607626000},
            {1, -589392000},
            {2, -576090000},
            {1, -557942400},
            {2, -544640400},
            {1, -526492800},
            {2, -513190800},
            {1, -495043200},
            {2, -481741200},
            {1, -463593600},
            {2, -450291600},
            {1, -431539200},
            {2, -418237200},
            {1, -400089600},
            {2, -386787600},
            {1, -368640000},
            {2, -355338000},
            {1, -337190400},
            {2, -321469200},
            {1, -305740800},
            {2, -292438800},
            {1, -210787200},
            {2, -198090000},
            {5, -116438400},
            {6, -100108800},
            {5, -84384000},
            {6, -68659200},
            {5, -52934400},
            {6, -37209600},
            {5, -21484800},
            {6, -5760000},
            {5, 9964800},
            {6, 25689600},
            {5, 41414400},
            {6, 57744000},
            {5, 73468800},
            {6, 89193600},
            {5, 104918400},
            {6, 120643200},
            {5, 136368000},
            {6, 152092800},
            {5, 167817600},
            {6, 183542400},
            {5, 199267200},
            {6, 215596800},
            {5, 230716800},
            {6, 247046400},
            {5, 262771200},
            {6, 278496000},
            {5, 294220800},
            {6, 309945600},
            {5, 325670400},
            {6, 341395200},
            {5, 357120000},
            {6, 372844800},
            {5, 388569600},
            {6, 404899200},
            {5, 420019200},
            {6, 436348800},
            {5, 452073600},
            {6, 467798400},
            {5, 483523200},
            {6, 499248000},
            {5, 514972800},
            {6, 530697600},
            {5, 544608000},
            {6, 562147200},
            {5, 576057600},
            {6, 594201600},
            {5, 607507200},
            {6, 625651200},
            {5, 638956800},
            {6, 657100800},
            {5, 671011200},
            {6, 688550400},
            {5, 702460800},
            {6, 720000000},
            {5, 733910400},
            {6, 752054400},
            {5, 765360000},
            {6, 783504000},
            {5, 796809600},
            {6, 814953600},
            {5, 828864000},
            {6, 846403200},
            {5, 860313600},
            {6, 877852800},
            {5, 891763200},
            {6, 909302400},
            {5, 923212800},
            {6, 941356800},
            {5, 954662400},
            {6, 972806400},
            {5, 986112000},
            {6, 1004256000},
            {5, 1018166400},
            {6, 1035705600},
            {5, 1049616000},
            {6, 1067155200},
            {5, 1081065600},
            {6, 1099209600},
            {5, 1112515200},
            {6, 1130659200},
            {1, 1143964800},
            {2, 1162105200},
            {1, 1173600000},
            {2, 1194159600},
            {1, 1205049600},
            {2, 1225609200},
            {1, 1236499200},
            {2, 1257058800},
            {1, 1268553600},
            {2, 1289113200},
            {1, 1300003200},
            {2, 1320562800},
            {1, 1331452800},
            {2, 1352012400},
            {1, 1362902400},
            {2, 1383462000},
            {1, 1394352000},
            {2, 1414911600},
            {1, 1425801600},
            {2, 1446361200},
            {1, 1457856000},
            {2, 1478415600},
            {1, 1489305600},
            {2, 1509865200},
            {1, 1520755200},
            {2, 1541314800},
            {1, 1552204800},
            {2, 1572764400},
            {1, 1583654400},
            {2, 1604214000},
            {1, 1615708800},
            {2, 1636268400},
            {1, 1647158400},
            {2, 1667718000},
            {1, 1678608000},
            {2, 1699167600},
            {1, 1710057600},
            {2, 1730617200},
            {1, 1741507200},
            {2, 1762066800},
            {1, 1772956800},
            {2, 1793516400},
            {1, 1805011200},
            {2, 1825570800},
            {1, 1836460800},
            {2, 1857020400},
            {1, 1867910400},
            {2, 1888470000},
            {1, 1899360000},
            {2, 1919919600},
            {1, 1930809600},
            {2, 1951369200},
            {1, 1962864000},
            {2, 1983423600},
            {1, 1994313600},
            {2, 2014873200},
            {1, 2025763200},
            {2, 2046322800},
            {1, 2057212800},
            {2, 2077772400},
            {1, 2088662400},
            {2, 2109222000},
            {1, 2120112000},
            {2, 2140671600},
        }};
    }

    inline constexpr db::entry winnipeg{"America/Winnipeg",
                                        "Central - ON (west); Manitoba",
                                        "+4953-09709",
                                        detail::winnipeg::country_codes,
                                        detail::winnipeg::offsets,
                                        detail::winnipeg::transitions};

    // clang-format on
}