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

namespace snn::time::zone::db::asia
{
    // clang-format off

    namespace detail::samarkand
    {
        inline constexpr array<country::code, 1> country_codes{
            "UZ",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", 16073, false},
            {"+04", 14400, false},
            {"+05", 18000, false},
            {"+06", 21600, true},
            {"+06", 21600, false},
            {"+05", 18000, false},
            {"+06", 21600, true},
        }};

        inline constexpr array<zone::transition, 24> transitions{{
            {1, -1441168073},
            {2, -1247544000},
            {3, 354913200},
            {4, 370720800},
            {3, 386445600},
            {2, 402256800},
            {3, 417985200},
            {2, 433792800},
            {3, 449607600},
            {5, 465339600},
            {6, 481064400},
            {5, 496789200},
            {6, 512514000},
            {5, 528238800},
            {6, 543963600},
            {5, 559688400},
            {6, 575413200},
            {5, 591138000},
            {6, 606862800},
            {5, 622587600},
            {6, 638312400},
            {5, 654642000},
            {6, 670366800},
            {5, 686091600},
        }};
    }

    inline constexpr db::entry samarkand{"Asia/Samarkand",
                                         "Uzbekistan (west)",
                                         "+3940+06648",
                                         detail::samarkand::country_codes,
                                         detail::samarkand::offsets,
                                         detail::samarkand::transitions};

    // clang-format on
}
