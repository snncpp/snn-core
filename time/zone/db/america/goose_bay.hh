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

    namespace detail::goose_bay
    {
        inline constexpr array<country::code, 1> country_codes{
            "CA",
        };

        inline constexpr array<zone::offset, 11> offsets{{
            {"LMT", -14500, false},
            {"NST", -12652, false},
            {"NDT", -9052, true},
            {"NST", -12600, false},
            {"NDT", -9000, true},
            {"NPT", -9000, true},
            {"NWT", -9000, true},
            {"ADT", -10800, true},
            {"AST", -14400, false},
            {"ADDT", -7200, true},
            {"ADT", -10800, true},
        }};

        inline constexpr array<zone::transition, 204> transitions{{
            {1, -2713895900},
            {2, -1632076148},
            {1, -1615145348},
            {3, -1096921748},
            {4, -1061670600},
            {3, -1048973400},
            {4, -1030221000},
            {3, -1017523800},
            {4, -998771400},
            {3, -986074200},
            {4, -966717000},
            {3, -954624600},
            {4, -935267400},
            {3, -922570200},
            {4, -903817800},
            {3, -891120600},
            {6, -872368200},
            {5, -769395600},
            {3, -765401400},
            {4, -746044200},
            {3, -733347000},
            {4, -714594600},
            {3, -701897400},
            {4, -683145000},
            {3, -670447800},
            {4, -651695400},
            {3, -638998200},
            {4, -619641000},
            {3, -606943800},
            {4, -589401000},
            {3, -576099000},
            {4, -557951400},
            {3, -544649400},
            {4, -526501800},
            {3, -513199800},
            {4, -495052200},
            {3, -481750200},
            {4, -463602600},
            {3, -450300600},
            {4, -431548200},
            {3, -418246200},
            {4, -400098600},
            {3, -386796600},
            {4, -368649000},
            {3, -355347000},
            {4, -337199400},
            {3, -323897400},
            {4, -305749800},
            {3, -289423800},
            {4, -273695400},
            {3, -257974200},
            {4, -242245800},
            {3, -226524600},
            {4, -210796200},
            {3, -195075000},
            {4, -179346600},
            {3, -163625400},
            {4, -147897000},
            {3, -131571000},
            {8, -119903400},
            {7, -116445600},
            {8, -100119600},
            {7, -84391200},
            {8, -68670000},
            {7, -52941600},
            {8, -37220400},
            {7, -21492000},
            {8, -5770800},
            {7, 9957600},
            {8, 25678800},
            {7, 41407200},
            {8, 57733200},
            {7, 73461600},
            {8, 89182800},
            {7, 104911200},
            {8, 120632400},
            {7, 136360800},
            {8, 152082000},
            {7, 167810400},
            {8, 183531600},
            {7, 199260000},
            {8, 215586000},
            {7, 230709600},
            {8, 247035600},
            {7, 262764000},
            {8, 278485200},
            {7, 294213600},
            {8, 309934800},
            {7, 325663200},
            {8, 341384400},
            {7, 357112800},
            {8, 372834000},
            {7, 388562400},
            {8, 404888400},
            {7, 420012000},
            {8, 436338000},
            {7, 452066400},
            {8, 467787600},
            {7, 483516000},
            {8, 499237200},
            {7, 514965600},
            {8, 530686800},
            {7, 544593660},
            {8, 562129260},
            {9, 576043260},
            {8, 594180060},
            {7, 607492860},
            {8, 625633260},
            {7, 638942460},
            {8, 657082860},
            {7, 670996860},
            {8, 688532460},
            {7, 702446460},
            {8, 719982060},
            {7, 733896060},
            {8, 752036460},
            {7, 765345660},
            {8, 783486060},
            {7, 796795260},
            {8, 814935660},
            {7, 828849660},
            {8, 846385260},
            {7, 860299260},
            {8, 877834860},
            {7, 891748860},
            {8, 909284460},
            {7, 923198460},
            {8, 941338860},
            {7, 954648060},
            {8, 972788460},
            {7, 986097660},
            {8, 1004238060},
            {7, 1018152060},
            {8, 1035687660},
            {7, 1049601660},
            {8, 1067137260},
            {7, 1081051260},
            {8, 1099191660},
            {7, 1112500860},
            {8, 1130641260},
            {7, 1143950460},
            {8, 1162090860},
            {7, 1173585660},
            {8, 1194145260},
            {7, 1205035260},
            {8, 1225594860},
            {7, 1236484860},
            {8, 1257044460},
            {7, 1268539260},
            {8, 1289098860},
            {7, 1299988860},
            {8, 1320555600},
            {7, 1331445600},
            {8, 1352005200},
            {7, 1362895200},
            {8, 1383454800},
            {7, 1394344800},
            {8, 1414904400},
            {7, 1425794400},
            {8, 1446354000},
            {7, 1457848800},
            {8, 1478408400},
            {7, 1489298400},
            {8, 1509858000},
            {7, 1520748000},
            {8, 1541307600},
            {7, 1552197600},
            {8, 1572757200},
            {7, 1583647200},
            {8, 1604206800},
            {7, 1615701600},
            {8, 1636261200},
            {7, 1647151200},
            {8, 1667710800},
            {7, 1678600800},
            {8, 1699160400},
            {7, 1710050400},
            {8, 1730610000},
            {7, 1741500000},
            {8, 1762059600},
            {7, 1772949600},
            {8, 1793509200},
            {7, 1805004000},
            {8, 1825563600},
            {7, 1836453600},
            {8, 1857013200},
            {7, 1867903200},
            {8, 1888462800},
            {7, 1899352800},
            {8, 1919912400},
            {7, 1930802400},
            {8, 1951362000},
            {7, 1962856800},
            {8, 1983416400},
            {7, 1994306400},
            {8, 2014866000},
            {7, 2025756000},
            {8, 2046315600},
            {7, 2057205600},
            {8, 2077765200},
            {7, 2088655200},
            {8, 2109214800},
            {7, 2120104800},
            {8, 2140664400},
        }};
    }

    inline constexpr db::entry goose_bay{"America/Goose_Bay",
                                         "Atlantic - Labrador (most areas)",
                                         "+5320-06025",
                                         detail::goose_bay::country_codes,
                                         detail::goose_bay::offsets,
                                         detail::goose_bay::transitions};

    // clang-format on
}
