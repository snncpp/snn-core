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

    namespace detail::norfolk
    {
        inline constexpr array<country::code, 1> country_codes{
            "NF",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", 40312, false},
            {"+1112", 40320, false},
            {"+1130", 41400, false},
            {"+1230", 45000, true},
            {"+1130", 41400, false},
            {"+11", 39600, false},
            {"+12", 43200, true},
        }};

        inline constexpr array<zone::transition, 43> transitions{{
            {1, -2177493112},
            {2, -599656320},
            {3, 152029800},
            {4, 162916200},
            {5, 1443882600},
            {6, 1570287600},
            {5, 1586012400},
            {6, 1601737200},
            {5, 1617462000},
            {6, 1633186800},
            {5, 1648911600},
            {6, 1664636400},
            {5, 1680361200},
            {6, 1696086000},
            {5, 1712415600},
            {6, 1728140400},
            {5, 1743865200},
            {6, 1759590000},
            {5, 1775314800},
            {6, 1791039600},
            {5, 1806764400},
            {6, 1822489200},
            {5, 1838214000},
            {6, 1853938800},
            {5, 1869663600},
            {6, 1885993200},
            {5, 1901718000},
            {6, 1917442800},
            {5, 1933167600},
            {6, 1948892400},
            {5, 1964617200},
            {6, 1980342000},
            {5, 1996066800},
            {6, 2011791600},
            {5, 2027516400},
            {6, 2043241200},
            {5, 2058966000},
            {6, 2075295600},
            {5, 2091020400},
            {6, 2106745200},
            {5, 2122470000},
            {6, 2138194800},
            {6, 2147483647},
        }};
    }

    inline constexpr db::entry norfolk{"Pacific/Norfolk",
                                       "",
                                       "-2903+16758",
                                       detail::norfolk::country_codes,
                                       detail::norfolk::offsets,
                                       detail::norfolk::transitions};

    // clang-format on
}
