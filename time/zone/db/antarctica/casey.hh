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

namespace snn::time::zone::db::antarctica
{
    // clang-format off

    namespace detail::casey
    {
        inline constexpr array<country::code, 1> country_codes{
            "AQ",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"-00", 0, false},
            {"+08", 28800, false},
            {"+11", 39600, false},
            {"+08", 28800, false},
        }};

        inline constexpr array<zone::transition, 17> transitions{{
            {1, -31536000},
            {2, 1255802400},
            {1, 1267714800},
            {2, 1319738400},
            {3, 1329843600},
            {2, 1477065600},
            {1, 1520701200},
            {2, 1538856000},
            {1, 1552752000},
            {2, 1570129200},
            {1, 1583596800},
            {2, 1601740860},
            {1, 1615640400},
            {2, 1633190460},
            {1, 1647090000},
            {2, 1664640060},
            {1, 1678291200},
        }};
    }

    inline constexpr db::entry casey{"Antarctica/Casey",
                                     "Casey",
                                     "-6617+11031",
                                     detail::casey::country_codes,
                                     detail::casey::offsets,
                                     detail::casey::transitions};

    // clang-format on
}
