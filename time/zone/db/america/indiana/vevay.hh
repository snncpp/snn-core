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

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", -20416, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
            {"CWT", -18000, true},
            {"CPT", -18000, true},
            {"EST", -18000, false},
            {"EDT", -14400, true},
        }};

        inline constexpr array<zone::transition, 81> transitions{{
            {2, -2717647200},
            {1, -1633276800},
            {2, -1615136400},
            {1, -1601827200},
            {2, -1583686800},
            {3, -880214400},
            {4, -769395600},
            {2, -765392400},
            {5, -495043200},
            {6, -21488400},
            {5, -5767200},
            {6, 9961200},
            {5, 25682400},
            {6, 41410800},
            {5, 57736800},
            {6, 73465200},
            {5, 89186400},
            {6, 1143961200},
            {5, 1162101600},
            {6, 1173596400},
            {5, 1194156000},
            {6, 1205046000},
            {5, 1225605600},
            {6, 1236495600},
            {5, 1257055200},
            {6, 1268550000},
            {5, 1289109600},
            {6, 1299999600},
            {5, 1320559200},
            {6, 1331449200},
            {5, 1352008800},
            {6, 1362898800},
            {5, 1383458400},
            {6, 1394348400},
            {5, 1414908000},
            {6, 1425798000},
            {5, 1446357600},
            {6, 1457852400},
            {5, 1478412000},
            {6, 1489302000},
            {5, 1509861600},
            {6, 1520751600},
            {5, 1541311200},
            {6, 1552201200},
            {5, 1572760800},
            {6, 1583650800},
            {5, 1604210400},
            {6, 1615705200},
            {5, 1636264800},
            {6, 1647154800},
            {5, 1667714400},
            {6, 1678604400},
            {5, 1699164000},
            {6, 1710054000},
            {5, 1730613600},
            {6, 1741503600},
            {5, 1762063200},
            {6, 1772953200},
            {5, 1793512800},
            {6, 1805007600},
            {5, 1825567200},
            {6, 1836457200},
            {5, 1857016800},
            {6, 1867906800},
            {5, 1888466400},
            {6, 1899356400},
            {5, 1919916000},
            {6, 1930806000},
            {5, 1951365600},
            {6, 1962860400},
            {5, 1983420000},
            {6, 1994310000},
            {5, 2014869600},
            {6, 2025759600},
            {5, 2046319200},
            {6, 2057209200},
            {5, 2077768800},
            {6, 2088658800},
            {5, 2109218400},
            {6, 2120108400},
            {5, 2140668000},
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
