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

    namespace detail::port_au_prince
    {
        inline constexpr array<country::code, 1> country_codes{
            "HT",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -17360, false},
            {"PPMT", -17340, false},
            {"EDT", -14400, true},
            {"EST", -18000, false},
            {"EDT", -14400, true},
            {"EST", -18000, false},
        }};

        inline constexpr array<zone::transition, 86> transitions{{
            {1, -2524504240},
            {3, -1670483460},
            {2, 421218000},
            {3, 436334400},
            {2, 452062800},
            {3, 467784000},
            {2, 483512400},
            {3, 499233600},
            {2, 514962000},
            {3, 530683200},
            {2, 546411600},
            {3, 562132800},
            {4, 576050400},
            {5, 594194400},
            {4, 607500000},
            {5, 625644000},
            {4, 638949600},
            {5, 657093600},
            {4, 671004000},
            {5, 688543200},
            {4, 702453600},
            {5, 719992800},
            {4, 733903200},
            {5, 752047200},
            {4, 765352800},
            {5, 783496800},
            {4, 796802400},
            {5, 814946400},
            {4, 828856800},
            {5, 846396000},
            {4, 860306400},
            {5, 877845600},
            {2, 1112504400},
            {3, 1130644800},
            {2, 1143954000},
            {3, 1162094400},
            {2, 1331449200},
            {3, 1352008800},
            {2, 1362898800},
            {3, 1383458400},
            {2, 1394348400},
            {3, 1414908000},
            {2, 1425798000},
            {3, 1446357600},
            {2, 1489302000},
            {3, 1509861600},
            {2, 1520751600},
            {3, 1541311200},
            {2, 1552201200},
            {3, 1572760800},
            {2, 1583650800},
            {3, 1604210400},
            {2, 1615705200},
            {3, 1636264800},
            {2, 1647154800},
            {3, 1667714400},
            {2, 1678604400},
            {3, 1699164000},
            {2, 1710054000},
            {3, 1730613600},
            {2, 1741503600},
            {3, 1762063200},
            {2, 1772953200},
            {3, 1793512800},
            {2, 1805007600},
            {3, 1825567200},
            {2, 1836457200},
            {3, 1857016800},
            {2, 1867906800},
            {3, 1888466400},
            {2, 1899356400},
            {3, 1919916000},
            {2, 1930806000},
            {3, 1951365600},
            {2, 1962860400},
            {3, 1983420000},
            {2, 1994310000},
            {3, 2014869600},
            {2, 2025759600},
            {3, 2046319200},
            {2, 2057209200},
            {3, 2077768800},
            {2, 2088658800},
            {3, 2109218400},
            {2, 2120108400},
            {3, 2140668000},
        }};
    }

    inline constexpr db::entry port_au_prince{"America/Port-au-Prince",
                                              "",
                                              "+1832-07220",
                                              detail::port_au_prince::country_codes,
                                              detail::port_au_prince::offsets,
                                              detail::port_au_prince::transitions};

    // clang-format on
}
