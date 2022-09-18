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

namespace snn::time::zone::db::pacific
{
    // clang-format off

    namespace detail::fiji
    {
        inline constexpr array<country::code, 1> country_codes{
            "FJ",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 42944, false},
            {"+13", 46800, true},
            {"+12", 43200, false},
        }};

        inline constexpr array<zone::transition, 62> transitions{{
            {2, -1709985344},
            {1, 909842400},
            {2, 920124000},
            {1, 941896800},
            {2, 951573600},
            {1, 1259416800},
            {2, 1269698400},
            {1, 1287842400},
            {2, 1299333600},
            {1, 1319292000},
            {2, 1327154400},
            {1, 1350741600},
            {2, 1358604000},
            {1, 1382796000},
            {2, 1390050000},
            {1, 1414850400},
            {2, 1421503200},
            {1, 1446300000},
            {2, 1452952800},
            {1, 1478354400},
            {2, 1484402400},
            {1, 1509804000},
            {2, 1515852000},
            {1, 1541253600},
            {2, 1547301600},
            {1, 1573308000},
            {2, 1578751200},
            {1, 1608386400},
            {2, 1610805600},
            {1, 1668261600},
            {2, 1673704800},
            {1, 1699711200},
            {2, 1705154400},
            {1, 1731160800},
            {2, 1736604000},
            {1, 1762610400},
            {2, 1768658400},
            {1, 1794060000},
            {2, 1800108000},
            {1, 1826114400},
            {2, 1831557600},
            {1, 1857564000},
            {2, 1863007200},
            {1, 1889013600},
            {2, 1894456800},
            {1, 1920463200},
            {2, 1925906400},
            {1, 1951912800},
            {2, 1957960800},
            {1, 1983967200},
            {2, 1989410400},
            {1, 2015416800},
            {2, 2020860000},
            {1, 2046866400},
            {2, 2052309600},
            {1, 2078316000},
            {2, 2083759200},
            {1, 2109765600},
            {2, 2115813600},
            {1, 2141215200},
            {2, 2147263200},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry fiji{"Pacific/Fiji",
                                    "",
                                    "-1808+17825",
                                    detail::fiji::country_codes,
                                    detail::fiji::offsets,
                                    detail::fiji::transitions};

    // clang-format on
}
