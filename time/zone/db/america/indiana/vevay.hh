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

namespace snn::time::zone::db::america::indiana
{
    // clang-format off

    namespace detail::vevay
    {
        inline constexpr array<country::code, 1> country_codes{
            "US",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", -20416, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
            {"CWT", -18000, true},
            {"CPT", -18000, true},
            {"CST", -21600, false},
            {"EST", -18000, false},
            {"EDT", -14400, true},
        }};

        inline constexpr array<zone::transition, 81> transitions{{
            {5, -2717647200},
            {1, -1633276800},
            {2, -1615136400},
            {1, -1601827200},
            {2, -1583686800},
            {3, -880214400},
            {4, -769395600},
            {2, -765392400},
            {6, -495043200},
            {7, -21488400},
            {6, -5767200},
            {7, 9961200},
            {6, 25682400},
            {7, 41410800},
            {6, 57736800},
            {7, 73465200},
            {6, 89186400},
            {7, 1143961200},
            {6, 1162101600},
            {7, 1173596400},
            {6, 1194156000},
            {7, 1205046000},
            {6, 1225605600},
            {7, 1236495600},
            {6, 1257055200},
            {7, 1268550000},
            {6, 1289109600},
            {7, 1299999600},
            {6, 1320559200},
            {7, 1331449200},
            {6, 1352008800},
            {7, 1362898800},
            {6, 1383458400},
            {7, 1394348400},
            {6, 1414908000},
            {7, 1425798000},
            {6, 1446357600},
            {7, 1457852400},
            {6, 1478412000},
            {7, 1489302000},
            {6, 1509861600},
            {7, 1520751600},
            {6, 1541311200},
            {7, 1552201200},
            {6, 1572760800},
            {7, 1583650800},
            {6, 1604210400},
            {7, 1615705200},
            {6, 1636264800},
            {7, 1647154800},
            {6, 1667714400},
            {7, 1678604400},
            {6, 1699164000},
            {7, 1710054000},
            {6, 1730613600},
            {7, 1741503600},
            {6, 1762063200},
            {7, 1772953200},
            {6, 1793512800},
            {7, 1805007600},
            {6, 1825567200},
            {7, 1836457200},
            {6, 1857016800},
            {7, 1867906800},
            {6, 1888466400},
            {7, 1899356400},
            {6, 1919916000},
            {7, 1930806000},
            {6, 1951365600},
            {7, 1962860400},
            {6, 1983420000},
            {7, 1994310000},
            {6, 2014869600},
            {7, 2025759600},
            {6, 2046319200},
            {7, 2057209200},
            {6, 2077768800},
            {7, 2088658800},
            {6, 2109218400},
            {7, 2120108400},
            {6, 2140668000},
        }};
    }

    inline constexpr db::entry vevay{"America/Indiana/Vevay",
                                     "Eastern - IN (Switzerland)",
                                     "+384452-0850402",
                                     detail::vevay::country_codes,
                                     detail::vevay::offsets,
                                     detail::vevay::transitions};

    // clang-format on
}
