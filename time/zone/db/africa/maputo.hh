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

    namespace detail::maputo
    {
        inline constexpr array<country::code, 8> country_codes{
            "MZ",
            "BI",
            "BW",
            "CD",
            "MW",
            "RW",
            "ZM",
            "ZW",
        };

        inline constexpr array<zone::offset, 2> offsets{{
            {"LMT", 7820, false},
            {"CAT", 7200, false},
        }};

        inline constexpr array<zone::transition, 1> transitions{{
            {1, -2109291020},
        }};
    }

    inline constexpr db::entry maputo{"Africa/Maputo",
                                      "Central Africa Time",
                                      "-2558+03235",
                                      detail::maputo::country_codes,
                                      detail::maputo::offsets,
                                      detail::maputo::transitions};

    // clang-format on
}
