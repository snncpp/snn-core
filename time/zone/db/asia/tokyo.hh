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

    namespace detail::tokyo
    {
        inline constexpr array<country::code, 1> country_codes{
            "JP",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", 33539, false},
            {"JDT", 36000, true},
            {"JST", 32400, false},
            {"JST", 32400, false},
        }};

        inline constexpr array<zone::transition, 9> transitions{{
            {3, -2587712400},
            {1, -683802000},
            {2, -672310800},
            {1, -654771600},
            {2, -640861200},
            {1, -620298000},
            {2, -609411600},
            {1, -588848400},
            {2, -577962000},
        }};
    }

    inline constexpr db::entry tokyo{"Asia/Tokyo",
                                     "",
                                     "+353916+1394441",
                                     detail::tokyo::country_codes,
                                     detail::tokyo::offsets,
                                     detail::tokyo::transitions};

    // clang-format on
}
