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

    namespace detail::pontianak
    {
        inline constexpr array<country::code, 1> country_codes{
            "ID",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", 26240, false},
            {"PMT", 26240, false},
            {"+0730", 27000, false},
            {"+09", 32400, false},
            {"+08", 28800, false},
            {"WITA", 28800, false},
            {"WIB", 25200, false},
        }};

        inline constexpr array<zone::transition, 8> transitions{{
            {1, -1946186240},
            {2, -1172906240},
            {3, -881220600},
            {2, -766054800},
            {4, -683883000},
            {2, -620812800},
            {5, -189415800},
            {6, 567964800},
        }};
    }

    inline constexpr db::entry pontianak{"Asia/Pontianak",
                                         "Borneo (west, central)",
                                         "-0002+10920",
                                         detail::pontianak::country_codes,
                                         detail::pontianak::offsets,
                                         detail::pontianak::transitions};

    // clang-format on
}
