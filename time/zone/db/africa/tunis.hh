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

namespace snn::time::zone::db::africa
{
    // clang-format off

    namespace detail::tunis
    {
        inline constexpr array<country::code, 1> country_codes{
            "TN",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", 2444, false},
            {"PMT", 561, false},
            {"CEST", 7200, true},
            {"CET", 3600, false},
            {"CET", 3600, false},
            {"CEST", 7200, true},
        }};

        inline constexpr array<zone::transition, 34> transitions{{
            {1, -2797202444},
            {4, -1855958961},
            {2, -969242400},
            {3, -950493600},
            {2, -941940000},
            {4, -891136800},
            {5, -877827600},
            {4, -857257200},
            {5, -844556400},
            {4, -842918400},
            {5, -842223600},
            {4, -828230400},
            {5, -812502000},
            {4, -796269600},
            {5, -781052400},
            {4, -766634400},
            {2, 231202800},
            {3, 243903600},
            {2, 262825200},
            {3, 276044400},
            {2, 581122800},
            {3, 591145200},
            {2, 606870000},
            {3, 622594800},
            {2, 641516400},
            {3, 654649200},
            {2, 1114902000},
            {3, 1128038400},
            {2, 1143334800},
            {3, 1162083600},
            {2, 1174784400},
            {3, 1193533200},
            {2, 1206838800},
            {3, 1224982800},
        }};
    }

    inline constexpr db::entry tunis{"Africa/Tunis",
                                     "",
                                     "+3648+01011",
                                     detail::tunis::country_codes,
                                     detail::tunis::offsets,
                                     detail::tunis::transitions};

    // clang-format on
}
