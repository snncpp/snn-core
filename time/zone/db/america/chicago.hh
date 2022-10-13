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

    namespace detail::chicago
    {
        inline constexpr array<country::code, 1> country_codes{
            "US",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", -21036, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
            {"CST", -21600, false},
            {"EST", -18000, false},
            {"CWT", -18000, true},
            {"CPT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 236> transitions{{
            {3, -2717647200},
            {1, -1633276800},
            {2, -1615136400},
            {1, -1601827200},
            {2, -1583686800},
            {1, -1563724800},
            {2, -1551632400},
            {1, -1538928000},
            {2, -1520182800},
            {1, -1504454400},
            {2, -1491757200},
            {1, -1473004800},
            {2, -1459702800},
            {1, -1441555200},
            {2, -1428253200},
            {1, -1410105600},
            {2, -1396803600},
            {1, -1378656000},
            {2, -1365354000},
            {1, -1347206400},
            {2, -1333904400},
            {1, -1315152000},
            {2, -1301850000},
            {1, -1283702400},
            {2, -1270400400},
            {1, -1252252800},
            {2, -1238950800},
            {1, -1220803200},
            {2, -1207501200},
            {1, -1189353600},
            {2, -1176051600},
            {1, -1157299200},
            {2, -1144602000},
            {1, -1125849600},
            {2, -1112547600},
            {1, -1094400000},
            {2, -1081098000},
            {4, -1067788800},
            {2, -1045414800},
            {1, -1031500800},
            {2, -1018198800},
            {1, -1000051200},
            {2, -986749200},
            {1, -967996800},
            {2, -955299600},
            {1, -936547200},
            {2, -923245200},
            {1, -905097600},
            {2, -891795600},
            {5, -880214400},
            {6, -769395600},
            {2, -765392400},
            {1, -747244800},
            {2, -733942800},
            {1, -715795200},
            {2, -702493200},
            {1, -684345600},
            {2, -671043600},
            {1, -652896000},
            {2, -639594000},
            {1, -620841600},
            {2, -608144400},
            {1, -589392000},
            {2, -576090000},
            {1, -557942400},
            {2, -544640400},
            {1, -526492800},
            {2, -513190800},
            {1, -495043200},
            {2, -481741200},
            {1, -463593600},
            {2, -447267600},
            {1, -431539200},
            {2, -415818000},
            {1, -400089600},
            {2, -384368400},
            {1, -368640000},
            {2, -352918800},
            {1, -337190400},
            {2, -321469200},
            {1, -305740800},
            {2, -289414800},
            {1, -273686400},
            {2, -257965200},
            {1, -242236800},
            {2, -226515600},
            {1, -210787200},
            {2, -195066000},
            {1, -179337600},
            {2, -163616400},
            {1, -147888000},
            {2, -131562000},
            {1, -116438400},
            {2, -100112400},
            {1, -84384000},
            {2, -68662800},
            {1, -52934400},
            {2, -37213200},
            {1, -21484800},
            {2, -5763600},
            {1, 9964800},
            {2, 25686000},
            {1, 41414400},
            {2, 57740400},
            {1, 73468800},
            {2, 89190000},
            {1, 104918400},
            {2, 120639600},
            {1, 126691200},
            {2, 152089200},
            {1, 162374400},
            {2, 183538800},
            {1, 199267200},
            {2, 215593200},
            {1, 230716800},
            {2, 247042800},
            {1, 262771200},
            {2, 278492400},
            {1, 294220800},
            {2, 309942000},
            {1, 325670400},
            {2, 341391600},
            {1, 357120000},
            {2, 372841200},
            {1, 388569600},
            {2, 404895600},
            {1, 420019200},
            {2, 436345200},
            {1, 452073600},
            {2, 467794800},
            {1, 483523200},
            {2, 499244400},
            {1, 514972800},
            {2, 530694000},
            {1, 544608000},
            {2, 562143600},
            {1, 576057600},
            {2, 594198000},
            {1, 607507200},
            {2, 625647600},
            {1, 638956800},
            {2, 657097200},
            {1, 671011200},
            {2, 688546800},
            {1, 702460800},
            {2, 719996400},
            {1, 733910400},
            {2, 752050800},
            {1, 765360000},
            {2, 783500400},
            {1, 796809600},
            {2, 814950000},
            {1, 828864000},
            {2, 846399600},
            {1, 860313600},
            {2, 877849200},
            {1, 891763200},
            {2, 909298800},
            {1, 923212800},
            {2, 941353200},
            {1, 954662400},
            {2, 972802800},
            {1, 986112000},
            {2, 1004252400},
            {1, 1018166400},
            {2, 1035702000},
            {1, 1049616000},
            {2, 1067151600},
            {1, 1081065600},
            {2, 1099206000},
            {1, 1112515200},
            {2, 1130655600},
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

    inline constexpr db::entry chicago{"America/Chicago",
                                       "Central (most areas)",
                                       "+415100-0873900",
                                       detail::chicago::country_codes,
                                       detail::chicago::offsets,
                                       detail::chicago::transitions};

    // clang-format on
}
