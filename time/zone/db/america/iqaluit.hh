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

    namespace detail::iqaluit
    {
        inline constexpr array<country::code, 1> country_codes{
            "CA",
        };

        inline constexpr array<zone::offset, 10> offsets{{
            {"-00", 0, false},
            {"EPT", -14400, true},
            {"EST", -18000, false},
            {"EDDT", -10800, true},
            {"EDT", -14400, true},
            {"EWT", -14400, true},
            {"CST", -21600, false},
            {"CDT", -18000, true},
            {"EDT", -14400, true},
            {"EST", -18000, false},
        }};

        inline constexpr array<zone::transition, 121> transitions{{
            {5, -865296000},
            {1, -769395600},
            {2, -765396000},
            {3, -147898800},
            {2, -131569200},
            {4, 325666800},
            {2, 341388000},
            {4, 357116400},
            {2, 372837600},
            {4, 388566000},
            {2, 404892000},
            {4, 420015600},
            {2, 436341600},
            {4, 452070000},
            {2, 467791200},
            {4, 483519600},
            {2, 499240800},
            {4, 514969200},
            {2, 530690400},
            {4, 544604400},
            {2, 562140000},
            {4, 576054000},
            {2, 594194400},
            {4, 607503600},
            {2, 625644000},
            {4, 638953200},
            {2, 657093600},
            {4, 671007600},
            {2, 688543200},
            {4, 702457200},
            {2, 719992800},
            {4, 733906800},
            {2, 752047200},
            {4, 765356400},
            {2, 783496800},
            {4, 796806000},
            {2, 814946400},
            {4, 828860400},
            {2, 846396000},
            {4, 860310000},
            {2, 877845600},
            {4, 891759600},
            {2, 909295200},
            {4, 923209200},
            {6, 941349600},
            {7, 954662400},
            {2, 972802800},
            {4, 986108400},
            {2, 1004248800},
            {4, 1018162800},
            {2, 1035698400},
            {4, 1049612400},
            {2, 1067148000},
            {4, 1081062000},
            {2, 1099202400},
            {4, 1112511600},
            {2, 1130652000},
            {4, 1143961200},
            {2, 1162101600},
            {4, 1173596400},
            {2, 1194156000},
            {4, 1205046000},
            {2, 1225605600},
            {4, 1236495600},
            {2, 1257055200},
            {4, 1268550000},
            {2, 1289109600},
            {4, 1299999600},
            {2, 1320559200},
            {4, 1331449200},
            {2, 1352008800},
            {4, 1362898800},
            {2, 1383458400},
            {4, 1394348400},
            {2, 1414908000},
            {4, 1425798000},
            {2, 1446357600},
            {4, 1457852400},
            {2, 1478412000},
            {4, 1489302000},
            {2, 1509861600},
            {4, 1520751600},
            {2, 1541311200},
            {4, 1552201200},
            {2, 1572760800},
            {4, 1583650800},
            {2, 1604210400},
            {4, 1615705200},
            {2, 1636264800},
            {4, 1647154800},
            {2, 1667714400},
            {4, 1678604400},
            {2, 1699164000},
            {4, 1710054000},
            {2, 1730613600},
            {4, 1741503600},
            {2, 1762063200},
            {4, 1772953200},
            {2, 1793512800},
            {4, 1805007600},
            {2, 1825567200},
            {4, 1836457200},
            {2, 1857016800},
            {4, 1867906800},
            {2, 1888466400},
            {4, 1899356400},
            {2, 1919916000},
            {4, 1930806000},
            {2, 1951365600},
            {4, 1962860400},
            {2, 1983420000},
            {4, 1994310000},
            {2, 2014869600},
            {4, 2025759600},
            {2, 2046319200},
            {4, 2057209200},
            {2, 2077768800},
            {4, 2088658800},
            {2, 2109218400},
            {4, 2120108400},
            {2, 2140668000},
        }};
    }

    inline constexpr db::entry iqaluit{"America/Iqaluit",
                                       "Eastern - NU (most east areas)",
                                       "+6344-06828",
                                       detail::iqaluit::country_codes,
                                       detail::iqaluit::offsets,
                                       detail::iqaluit::transitions};

    // clang-format on
}