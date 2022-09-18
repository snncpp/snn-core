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

    namespace detail::ojinaga
    {
        inline constexpr array<country::code, 1> country_codes{
            "MX",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -25060, false},
            {"MST", -25200, false},
            {"CST", -21600, false},
            {"CDT", -18000, true},
            {"MDT", -21600, true},
            {"MST", -25200, false},
        }};

        inline constexpr array<zone::transition, 90> transitions{{
            {1, -1514739600},
            {2, -1343066400},
            {1, -1234807200},
            {2, -1220292000},
            {1, -1207159200},
            {2, -1191344400},
            {3, 828864000},
            {2, 846399600},
            {3, 860313600},
            {2, 877849200},
            {4, 891766800},
            {1, 909302400},
            {4, 923216400},
            {1, 941356800},
            {4, 954666000},
            {1, 972806400},
            {4, 989139600},
            {1, 1001836800},
            {4, 1018170000},
            {1, 1035705600},
            {4, 1049619600},
            {1, 1067155200},
            {4, 1081069200},
            {1, 1099209600},
            {4, 1112518800},
            {1, 1130659200},
            {4, 1143968400},
            {1, 1162108800},
            {4, 1175418000},
            {1, 1193558400},
            {4, 1207472400},
            {1, 1225008000},
            {4, 1238922000},
            {1, 1256457600},
            {4, 1268557200},
            {1, 1289116800},
            {4, 1300006800},
            {1, 1320566400},
            {4, 1331456400},
            {1, 1352016000},
            {4, 1362906000},
            {1, 1383465600},
            {4, 1394355600},
            {1, 1414915200},
            {4, 1425805200},
            {1, 1446364800},
            {4, 1457859600},
            {1, 1478419200},
            {4, 1489309200},
            {1, 1509868800},
            {4, 1520758800},
            {1, 1541318400},
            {4, 1552208400},
            {1, 1572768000},
            {4, 1583658000},
            {1, 1604217600},
            {4, 1615712400},
            {1, 1636272000},
            {4, 1647162000},
            {1, 1667721600},
            {4, 1678611600},
            {1, 1699171200},
            {4, 1710061200},
            {1, 1730620800},
            {4, 1741510800},
            {1, 1762070400},
            {4, 1772960400},
            {1, 1793520000},
            {4, 1805014800},
            {1, 1825574400},
            {4, 1836464400},
            {1, 1857024000},
            {4, 1867914000},
            {1, 1888473600},
            {4, 1899363600},
            {1, 1919923200},
            {4, 1930813200},
            {1, 1951372800},
            {4, 1962867600},
            {1, 1983427200},
            {4, 1994317200},
            {1, 2014876800},
            {4, 2025766800},
            {1, 2046326400},
            {4, 2057216400},
            {1, 2077776000},
            {4, 2088666000},
            {1, 2109225600},
            {4, 2120115600},
            {1, 2140675200},
        }};
    }

    inline constexpr db::entry ojinaga{"America/Ojinaga",
                                       "Mountain Time US - Chihuahua (US border)",
                                       "+2934-10425",
                                       detail::ojinaga::country_codes,
                                       detail::ojinaga::offsets,
                                       detail::ojinaga::transitions};

    // clang-format on
}
