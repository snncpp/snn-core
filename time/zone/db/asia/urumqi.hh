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

    namespace detail::urumqi
    {
        inline constexpr array<country::code, 1> country_codes{
            "CN",
        };

        inline constexpr array<zone::offset, 2> offsets{{
            {"LMT", 21020, false},
            {"+06", 21600, false},
        }};

        inline constexpr array<zone::transition, 1> transitions{{
            {1, -1325483420},
        }};
    }

    inline constexpr db::entry urumqi{"Asia/Urumqi",
                                      "Xinjiang Time",
                                      "+4348+08735",
                                      detail::urumqi::country_codes,
                                      detail::urumqi::offsets,
                                      detail::urumqi::transitions};

    // clang-format on
}
