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

    namespace detail::ciudad_juarez
    {
        inline constexpr array<country::code, 1> country_codes{
            "MX",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", -25556, false},
            {"MST", -25200, false},
            {"CST", -21600, false},
            {"MDT", -21600, true},
            {"MST", -25200, false},
            {"CDT", -18000, true},
            {"MDT", -21600, true},
        }};

        inline constexpr array<zone::transition, 91> transitions{{
            {1, -1514739600},
            {2, -1343066400},
            {4, -1234807200},
            {3, -1220292000},
            {4, -1207159200},
            {2, -1191344400},
            {5, 828864000},
            {2, 846399600},
            {5, 860313600},
            {2, 877849200},
            {3, 891766800},
            {4, 909302400},
            {3, 923216400},
            {4, 941356800},
            {3, 954666000},
            {4, 972806400},
            {3, 989139600},
            {4, 1001836800},
            {3, 1018170000},
            {4, 1035705600},
            {3, 1049619600},
            {4, 1067155200},
            {3, 1081069200},
            {4, 1099209600},
            {3, 1112518800},
            {4, 1130659200},
            {3, 1143968400},
            {4, 1162108800},
            {3, 1175418000},
            {4, 1193558400},
            {3, 1207472400},
            {4, 1225008000},
            {3, 1238922000},
            {4, 1256457600},
            {3, 1268557200},
            {4, 1289116800},
            {3, 1300006800},
            {4, 1320566400},
            {3, 1331456400},
            {4, 1352016000},
            {3, 1362906000},
            {4, 1383465600},
            {3, 1394355600},
            {4, 1414915200},
            {3, 1425805200},
            {4, 1446364800},
            {3, 1457859600},
            {4, 1478419200},
            {3, 1489309200},
            {4, 1509868800},
            {3, 1520758800},
            {4, 1541318400},
            {3, 1552208400},
            {4, 1572768000},
            {3, 1583658000},
            {4, 1604217600},
            {3, 1615712400},
            {4, 1636272000},
            {3, 1647162000},
            {2, 1667116800},
            {4, 1669788000},
            {3, 1678611600},
            {4, 1699171200},
            {3, 1710061200},
            {4, 1730620800},
            {3, 1741510800},
            {4, 1762070400},
            {3, 1772960400},
            {4, 1793520000},
            {3, 1805014800},
            {4, 1825574400},
            {3, 1836464400},
            {4, 1857024000},
            {3, 1867914000},
            {4, 1888473600},
            {3, 1899363600},
            {4, 1919923200},
            {3, 1930813200},
            {4, 1951372800},
            {3, 1962867600},
            {4, 1983427200},
            {3, 1994317200},
            {4, 2014876800},
            {3, 2025766800},
            {4, 2046326400},
            {3, 2057216400},
            {4, 2077776000},
            {3, 2088666000},
            {4, 2109225600},
            {3, 2120115600},
            {4, 2140675200},
        }};
    }

    inline constexpr db::entry ciudad_juarez{"America/Ciudad_Juarez",
                                             "Chihuahua (US border - west)",
                                             "+3144-10629",
                                             detail::ciudad_juarez::country_codes,
                                             detail::ciudad_juarez::offsets,
                                             detail::ciudad_juarez::transitions};

    // clang-format on
}
