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

    namespace detail::metlakatla
    {
        inline constexpr array<country::code, 1> country_codes{
            "US",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", 54822, false},
            {"LMT", -31578, false},
            {"PST", -28800, false},
            {"PWT", -25200, true},
            {"PPT", -25200, true},
            {"PDT", -25200, true},
            {"AKST", -32400, false},
            {"AKDT", -28800, true},
        }};

        inline constexpr array<zone::transition, 81> transitions{{
            {1, -3225223727},
            {2, -2188955622},
            {3, -880207200},
            {4, -769395600},
            {2, -765385200},
            {5, -21477600},
            {2, -5756400},
            {5, 9972000},
            {2, 25693200},
            {5, 41421600},
            {2, 57747600},
            {5, 73476000},
            {2, 89197200},
            {5, 104925600},
            {2, 120646800},
            {5, 126698400},
            {2, 152096400},
            {5, 162381600},
            {2, 183546000},
            {5, 199274400},
            {2, 215600400},
            {5, 230724000},
            {2, 247050000},
            {5, 262778400},
            {2, 278499600},
            {5, 294228000},
            {2, 309949200},
            {5, 325677600},
            {2, 341398800},
            {5, 357127200},
            {2, 372848400},
            {5, 388576800},
            {2, 404902800},
            {5, 420026400},
            {2, 436352400},
            {6, 1446372000},
            {7, 1457866800},
            {6, 1478426400},
            {7, 1489316400},
            {6, 1509876000},
            {7, 1520766000},
            {2, 1541325600},
            {6, 1547978400},
            {7, 1552215600},
            {6, 1572775200},
            {7, 1583665200},
            {6, 1604224800},
            {7, 1615719600},
            {6, 1636279200},
            {7, 1647169200},
            {6, 1667728800},
            {7, 1678618800},
            {6, 1699178400},
            {7, 1710068400},
            {6, 1730628000},
            {7, 1741518000},
            {6, 1762077600},
            {7, 1772967600},
            {6, 1793527200},
            {7, 1805022000},
            {6, 1825581600},
            {7, 1836471600},
            {6, 1857031200},
            {7, 1867921200},
            {6, 1888480800},
            {7, 1899370800},
            {6, 1919930400},
            {7, 1930820400},
            {6, 1951380000},
            {7, 1962874800},
            {6, 1983434400},
            {7, 1994324400},
            {6, 2014884000},
            {7, 2025774000},
            {6, 2046333600},
            {7, 2057223600},
            {6, 2077783200},
            {7, 2088673200},
            {6, 2109232800},
            {7, 2120122800},
            {6, 2140682400},
        }};
    }

    inline constexpr db::entry metlakatla{"America/Metlakatla",
                                          "Alaska - Annette Island",
                                          "+550737-1313435",
                                          detail::metlakatla::country_codes,
                                          detail::metlakatla::offsets,
                                          detail::metlakatla::transitions};

    // clang-format on
}
