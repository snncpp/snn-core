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

    namespace detail::los_angeles
    {
        inline constexpr array<country::code, 1> country_codes{
            "US",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -28378, false},
            {"PDT", -25200, true},
            {"PST", -28800, false},
            {"PWT", -25200, true},
            {"PPT", -25200, true},
            {"PST", -28800, false},
        }};

        inline constexpr array<zone::transition, 186> transitions{{
            {5, -2717640000},
            {1, -1633269600},
            {2, -1615129200},
            {1, -1601820000},
            {2, -1583679600},
            {3, -880207200},
            {4, -769395600},
            {2, -765385200},
            {1, -687967140},
            {2, -662655600},
            {1, -620838000},
            {2, -608137200},
            {1, -589388400},
            {2, -576082800},
            {1, -557938800},
            {2, -544633200},
            {1, -526489200},
            {2, -513183600},
            {1, -495039600},
            {2, -481734000},
            {1, -463590000},
            {2, -450284400},
            {1, -431535600},
            {2, -418230000},
            {1, -400086000},
            {2, -386780400},
            {1, -368636400},
            {2, -355330800},
            {1, -337186800},
            {2, -323881200},
            {1, -305737200},
            {2, -292431600},
            {1, -273682800},
            {2, -260982000},
            {1, -242233200},
            {2, -226508400},
            {1, -210783600},
            {2, -195058800},
            {1, -179334000},
            {2, -163609200},
            {1, -147884400},
            {2, -131554800},
            {1, -116434800},
            {2, -100105200},
            {1, -84376800},
            {2, -68655600},
            {1, -52927200},
            {2, -37206000},
            {1, -21477600},
            {2, -5756400},
            {1, 9972000},
            {2, 25693200},
            {1, 41421600},
            {2, 57747600},
            {1, 73476000},
            {2, 89197200},
            {1, 104925600},
            {2, 120646800},
            {1, 126698400},
            {2, 152096400},
            {1, 162381600},
            {2, 183546000},
            {1, 199274400},
            {2, 215600400},
            {1, 230724000},
            {2, 247050000},
            {1, 262778400},
            {2, 278499600},
            {1, 294228000},
            {2, 309949200},
            {1, 325677600},
            {2, 341398800},
            {1, 357127200},
            {2, 372848400},
            {1, 388576800},
            {2, 404902800},
            {1, 420026400},
            {2, 436352400},
            {1, 452080800},
            {2, 467802000},
            {1, 483530400},
            {2, 499251600},
            {1, 514980000},
            {2, 530701200},
            {1, 544615200},
            {2, 562150800},
            {1, 576064800},
            {2, 594205200},
            {1, 607514400},
            {2, 625654800},
            {1, 638964000},
            {2, 657104400},
            {1, 671018400},
            {2, 688554000},
            {1, 702468000},
            {2, 720003600},
            {1, 733917600},
            {2, 752058000},
            {1, 765367200},
            {2, 783507600},
            {1, 796816800},
            {2, 814957200},
            {1, 828871200},
            {2, 846406800},
            {1, 860320800},
            {2, 877856400},
            {1, 891770400},
            {2, 909306000},
            {1, 923220000},
            {2, 941360400},
            {1, 954669600},
            {2, 972810000},
            {1, 986119200},
            {2, 1004259600},
            {1, 1018173600},
            {2, 1035709200},
            {1, 1049623200},
            {2, 1067158800},
            {1, 1081072800},
            {2, 1099213200},
            {1, 1112522400},
            {2, 1130662800},
            {1, 1143972000},
            {2, 1162112400},
            {1, 1173607200},
            {2, 1194166800},
            {1, 1205056800},
            {2, 1225616400},
            {1, 1236506400},
            {2, 1257066000},
            {1, 1268560800},
            {2, 1289120400},
            {1, 1300010400},
            {2, 1320570000},
            {1, 1331460000},
            {2, 1352019600},
            {1, 1362909600},
            {2, 1383469200},
            {1, 1394359200},
            {2, 1414918800},
            {1, 1425808800},
            {2, 1446368400},
            {1, 1457863200},
            {2, 1478422800},
            {1, 1489312800},
            {2, 1509872400},
            {1, 1520762400},
            {2, 1541322000},
            {1, 1552212000},
            {2, 1572771600},
            {1, 1583661600},
            {2, 1604221200},
            {1, 1615716000},
            {2, 1636275600},
            {1, 1647165600},
            {2, 1667725200},
            {1, 1678615200},
            {2, 1699174800},
            {1, 1710064800},
            {2, 1730624400},
            {1, 1741514400},
            {2, 1762074000},
            {1, 1772964000},
            {2, 1793523600},
            {1, 1805018400},
            {2, 1825578000},
            {1, 1836468000},
            {2, 1857027600},
            {1, 1867917600},
            {2, 1888477200},
            {1, 1899367200},
            {2, 1919926800},
            {1, 1930816800},
            {2, 1951376400},
            {1, 1962871200},
            {2, 1983430800},
            {1, 1994320800},
            {2, 2014880400},
            {1, 2025770400},
            {2, 2046330000},
            {1, 2057220000},
            {2, 2077779600},
            {1, 2088669600},
            {2, 2109229200},
            {1, 2120119200},
            {2, 2140678800},
        }};
    }

    inline constexpr db::entry los_angeles{"America/Los_Angeles",
                                           "Pacific",
                                           "+340308-1181434",
                                           detail::los_angeles::country_codes,
                                           detail::los_angeles::offsets,
                                           detail::los_angeles::transitions};

    // clang-format on
}
