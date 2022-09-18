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

    namespace detail::dawson_creek
    {
        inline constexpr array<country::code, 1> country_codes{
            "CA",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -28856, false},
            {"PDT", -25200, true},
            {"PST", -28800, false},
            {"PWT", -25200, true},
            {"PPT", -25200, true},
            {"MST", -25200, false},
        }};

        inline constexpr array<zone::transition, 58> transitions{{
            {2, -2713881544},
            {1, -1632060000},
            {2, -1615129200},
            {3, -880207200},
            {4, -769395600},
            {2, -765385200},
            {1, -715788000},
            {2, -702486000},
            {1, -684338400},
            {2, -671036400},
            {1, -652888800},
            {2, -639586800},
            {1, -620834400},
            {2, -608137200},
            {1, -589384800},
            {2, -576082800},
            {1, -557935200},
            {2, -544633200},
            {1, -526485600},
            {2, -513183600},
            {1, -495036000},
            {2, -481734000},
            {1, -463586400},
            {2, -450284400},
            {1, -431532000},
            {2, -418230000},
            {1, -400082400},
            {2, -386780400},
            {1, -368632800},
            {2, -355330800},
            {1, -337183200},
            {2, -323881200},
            {1, -305733600},
            {2, -292431600},
            {1, -273679200},
            {2, -260982000},
            {1, -242229600},
            {2, -226508400},
            {1, -210780000},
            {2, -195058800},
            {1, -179330400},
            {2, -163609200},
            {1, -147880800},
            {2, -131554800},
            {1, -116431200},
            {2, -100105200},
            {1, -84376800},
            {2, -68655600},
            {1, -52927200},
            {2, -37206000},
            {1, -21477600},
            {2, -5756400},
            {1, 9972000},
            {2, 25693200},
            {1, 41421600},
            {2, 57747600},
            {1, 73476000},
            {5, 84013200},
        }};
    }

    inline constexpr db::entry dawson_creek{"America/Dawson_Creek",
                                            "MST - BC (Dawson Cr, Ft St John)",
                                            "+5546-12014",
                                            detail::dawson_creek::country_codes,
                                            detail::dawson_creek::offsets,
                                            detail::dawson_creek::transitions};

    // clang-format on
}
