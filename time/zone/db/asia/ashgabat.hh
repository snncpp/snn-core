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

    namespace detail::ashgabat
    {
        inline constexpr array<country::code, 1> country_codes{
            "TM",
        };

        inline constexpr array<zone::offset, 9> offsets{{
            {"LMT", 14012, false},
            {"+04", 14400, false},
            {"+06", 21600, true},
            {"+05", 18000, false},
            {"+05", 18000, false},
            {"+06", 21600, true},
            {"+05", 18000, true},
            {"+04", 14400, false},
            {"+05", 18000, false},
        }};

        inline constexpr array<zone::transition, 26> transitions{{
            {1, -1441166012},
            {3, -1247544000},
            {2, 354913200},
            {3, 370720800},
            {2, 386449200},
            {3, 402256800},
            {2, 417985200},
            {3, 433792800},
            {2, 449607600},
            {4, 465339600},
            {5, 481064400},
            {4, 496789200},
            {5, 512514000},
            {4, 528238800},
            {5, 543963600},
            {4, 559688400},
            {5, 575413200},
            {4, 591138000},
            {5, 606862800},
            {4, 622587600},
            {5, 638312400},
            {4, 654642000},
            {6, 670366800},
            {7, 686095200},
            {3, 695772000},
            {3, 2147483647},
        }};
    }

    inline constexpr db::entry ashgabat{"Asia/Ashgabat",
                                        "",
                                        "+3757+05823",
                                        detail::ashgabat::country_codes,
                                        detail::ashgabat::offsets,
                                        detail::ashgabat::transitions};

    // clang-format on
}
