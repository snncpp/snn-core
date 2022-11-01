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

    namespace detail::mawson
    {
        inline constexpr array<country::code, 1> country_codes{
            "AQ",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"-00", 0, false},
            {"+06", 21600, false},
            {"+05", 18000, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -501206400},
            {2, 1255809600},
        }};
    }

    inline constexpr db::entry mawson{"Antarctica/Mawson",
                                      "Mawson",
                                      "-6736+06253",
                                      detail::mawson::country_codes,
                                      detail::mawson::offsets,
                                      detail::mawson::transitions};

    // clang-format on
}
