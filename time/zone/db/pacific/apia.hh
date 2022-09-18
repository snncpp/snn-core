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

    namespace detail::apia
    {
        inline constexpr array<country::code, 1> country_codes{
            "WS",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", 45184, false},
            {"LMT", -41216, false},
            {"-1130", -41400, false},
            {"-10", -36000, true},
            {"-11", -39600, false},
            {"+13", 46800, false},
            {"+14", 50400, true},
        }};

        inline constexpr array<zone::transition, 27> transitions{{
            {1, -2445424384},
            {2, -1861878784},
            {4, -631110600},
            {3, 1285498800},
            {4, 1301752800},
            {3, 1316872800},
            {6, 1325239200},
            {5, 1333202400},
            {6, 1348927200},
            {5, 1365256800},
            {6, 1380376800},
            {5, 1396706400},
            {6, 1411826400},
            {5, 1428156000},
            {6, 1443276000},
            {5, 1459605600},
            {6, 1474725600},
            {5, 1491055200},
            {6, 1506175200},
            {5, 1522504800},
            {6, 1538229600},
            {5, 1554559200},
            {6, 1569679200},
            {5, 1586008800},
            {6, 1601128800},
            {5, 1617458400},
            {5, 2147483647},
        }};
    }

    inline constexpr db::entry apia{"Pacific/Apia",
                                    "",
                                    "-1350-17144",
                                    detail::apia::country_codes,
                                    detail::apia::offsets,
                                    detail::apia::transitions};

    // clang-format on
}
