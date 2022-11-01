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

namespace snn::time::zone::db::pacific
{
    // clang-format off

    namespace detail::marquesas
    {
        inline constexpr array<country::code, 1> country_codes{
            "PF",
        };

        inline constexpr array<zone::offset, 2> offsets{{
            {"LMT", -33480, false},
            {"-0930", -34200, false},
        }};

        inline constexpr array<zone::transition, 1> transitions{{
            {1, -1806676920},
        }};
    }

    inline constexpr db::entry marquesas{"Pacific/Marquesas",
                                         "Marquesas Islands",
                                         "-0900-13930",
                                         detail::marquesas::country_codes,
                                         detail::marquesas::offsets,
                                         detail::marquesas::transitions};

    // clang-format on
}
