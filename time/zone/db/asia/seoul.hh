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

    namespace detail::seoul
    {
        inline constexpr array<country::code, 1> country_codes{
            "KR",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", 30472, false},
            {"KST", 30600, false},
            {"JST", 32400, false},
            {"KDT", 36000, true},
            {"KST", 32400, false},
            {"KDT", 34200, true},
            {"KDT", 36000, true},
        }};

        inline constexpr array<zone::transition, 29> transitions{{
            {1, -1948782472},
            {2, -1830414600},
            {4, -767350800},
            {3, -681210000},
            {4, -672228000},
            {3, -654771600},
            {4, -640864800},
            {3, -623408400},
            {4, -609415200},
            {3, -588848400},
            {4, -577965600},
            {1, -498128400},
            {5, -462702600},
            {1, -451733400},
            {5, -429784200},
            {1, -418296600},
            {5, -399544200},
            {1, -387451800},
            {5, -368094600},
            {1, -356002200},
            {5, -336645000},
            {1, -324552600},
            {5, -305195400},
            {1, -293103000},
            {4, -264933000},
            {3, 547578000},
            {4, 560883600},
            {3, 579027600},
            {4, 592333200},
        }};
    }

    inline constexpr db::entry seoul{"Asia/Seoul",
                                     "",
                                     "+3733+12658",
                                     detail::seoul::country_codes,
                                     detail::seoul::offsets,
                                     detail::seoul::transitions};

    // clang-format on
}
