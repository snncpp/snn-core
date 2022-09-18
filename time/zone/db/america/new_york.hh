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

    namespace detail::new_york
    {
        inline constexpr array<country::code, 1> country_codes{
            "US",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -17762, false},
            {"EDT", -14400, true},
            {"EST", -18000, false},
            {"EST", -18000, false},
            {"EWT", -14400, true},
            {"EPT", -14400, true},
        }};

        inline constexpr array<zone::transition, 236> transitions{{
            {3, -2717650800},
            {1, -1633280400},
            {2, -1615140000},
            {1, -1601830800},
            {2, -1583690400},
            {1, -1570381200},
            {2, -1551636000},
            {1, -1536512400},
            {2, -1523210400},
            {1, -1504458000},
            {2, -1491760800},
            {1, -1473008400},
            {2, -1459706400},
            {1, -1441558800},
            {2, -1428256800},
            {1, -1410109200},
            {2, -1396807200},
            {1, -1378659600},
            {2, -1365357600},
            {1, -1347210000},
            {2, -1333908000},
            {1, -1315155600},
            {2, -1301853600},
            {1, -1283706000},
            {2, -1270404000},
            {1, -1252256400},
            {2, -1238954400},
            {1, -1220806800},
            {2, -1207504800},
            {1, -1189357200},
            {2, -1176055200},
            {1, -1157302800},
            {2, -1144605600},
            {1, -1125853200},
            {2, -1112551200},
            {1, -1094403600},
            {2, -1081101600},
            {1, -1062954000},
            {2, -1049652000},
            {1, -1031504400},
            {2, -1018202400},
            {1, -1000054800},
            {2, -986752800},
            {1, -968000400},
            {2, -955303200},
            {1, -936550800},
            {2, -923248800},
            {1, -905101200},
            {2, -891799200},
            {4, -880218000},
            {5, -769395600},
            {2, -765396000},
            {1, -747248400},
            {2, -733946400},
            {1, -715798800},
            {2, -702496800},
            {1, -684349200},
            {2, -671047200},
            {1, -652899600},
            {2, -639597600},
            {1, -620845200},
            {2, -608148000},
            {1, -589395600},
            {2, -576093600},
            {1, -557946000},
            {2, -544644000},
            {1, -526496400},
            {2, -513194400},
            {1, -495046800},
            {2, -481744800},
            {1, -463597200},
            {2, -447271200},
            {1, -431542800},
            {2, -415821600},
            {1, -400093200},
            {2, -384372000},
            {1, -368643600},
            {2, -352922400},
            {1, -337194000},
            {2, -321472800},
            {1, -305744400},
            {2, -289418400},
            {1, -273690000},
            {2, -257968800},
            {1, -242240400},
            {2, -226519200},
            {1, -210790800},
            {2, -195069600},
            {1, -179341200},
            {2, -163620000},
            {1, -147891600},
            {2, -131565600},
            {1, -116442000},
            {2, -100116000},
            {1, -84387600},
            {2, -68666400},
            {1, -52938000},
            {2, -37216800},
            {1, -21488400},
            {2, -5767200},
            {1, 9961200},
            {2, 25682400},
            {1, 41410800},
            {2, 57736800},
            {1, 73465200},
            {2, 89186400},
            {1, 104914800},
            {2, 120636000},
            {1, 126687600},
            {2, 152085600},
            {1, 162370800},
            {2, 183535200},
            {1, 199263600},
            {2, 215589600},
            {1, 230713200},
            {2, 247039200},
            {1, 262767600},
            {2, 278488800},
            {1, 294217200},
            {2, 309938400},
            {1, 325666800},
            {2, 341388000},
            {1, 357116400},
            {2, 372837600},
            {1, 388566000},
            {2, 404892000},
            {1, 420015600},
            {2, 436341600},
            {1, 452070000},
            {2, 467791200},
            {1, 483519600},
            {2, 499240800},
            {1, 514969200},
            {2, 530690400},
            {1, 544604400},
            {2, 562140000},
            {1, 576054000},
            {2, 594194400},
            {1, 607503600},
            {2, 625644000},
            {1, 638953200},
            {2, 657093600},
            {1, 671007600},
            {2, 688543200},
            {1, 702457200},
            {2, 719992800},
            {1, 733906800},
            {2, 752047200},
            {1, 765356400},
            {2, 783496800},
            {1, 796806000},
            {2, 814946400},
            {1, 828860400},
            {2, 846396000},
            {1, 860310000},
            {2, 877845600},
            {1, 891759600},
            {2, 909295200},
            {1, 923209200},
            {2, 941349600},
            {1, 954658800},
            {2, 972799200},
            {1, 986108400},
            {2, 1004248800},
            {1, 1018162800},
            {2, 1035698400},
            {1, 1049612400},
            {2, 1067148000},
            {1, 1081062000},
            {2, 1099202400},
            {1, 1112511600},
            {2, 1130652000},
            {1, 1143961200},
            {2, 1162101600},
            {1, 1173596400},
            {2, 1194156000},
            {1, 1205046000},
            {2, 1225605600},
            {1, 1236495600},
            {2, 1257055200},
            {1, 1268550000},
            {2, 1289109600},
            {1, 1299999600},
            {2, 1320559200},
            {1, 1331449200},
            {2, 1352008800},
            {1, 1362898800},
            {2, 1383458400},
            {1, 1394348400},
            {2, 1414908000},
            {1, 1425798000},
            {2, 1446357600},
            {1, 1457852400},
            {2, 1478412000},
            {1, 1489302000},
            {2, 1509861600},
            {1, 1520751600},
            {2, 1541311200},
            {1, 1552201200},
            {2, 1572760800},
            {1, 1583650800},
            {2, 1604210400},
            {1, 1615705200},
            {2, 1636264800},
            {1, 1647154800},
            {2, 1667714400},
            {1, 1678604400},
            {2, 1699164000},
            {1, 1710054000},
            {2, 1730613600},
            {1, 1741503600},
            {2, 1762063200},
            {1, 1772953200},
            {2, 1793512800},
            {1, 1805007600},
            {2, 1825567200},
            {1, 1836457200},
            {2, 1857016800},
            {1, 1867906800},
            {2, 1888466400},
            {1, 1899356400},
            {2, 1919916000},
            {1, 1930806000},
            {2, 1951365600},
            {1, 1962860400},
            {2, 1983420000},
            {1, 1994310000},
            {2, 2014869600},
            {1, 2025759600},
            {2, 2046319200},
            {1, 2057209200},
            {2, 2077768800},
            {1, 2088658800},
            {2, 2109218400},
            {1, 2120108400},
            {2, 2140668000},
        }};
    }

    inline constexpr db::entry new_york{"America/New_York",
                                        "Eastern (most areas)",
                                        "+404251-0740023",
                                        detail::new_york::country_codes,
                                        detail::new_york::offsets,
                                        detail::new_york::transitions};

    // clang-format on
}
