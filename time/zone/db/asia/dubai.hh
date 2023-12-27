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

    namespace detail::dubai
    {
        inline constexpr array<country::code, 5> country_codes{
            "AE",
            "OM",
            "RE",
            "SC",
            "TF",
        };

        inline constexpr array<zone::offset, 2> offsets{{
            {"LMT", 13272, false},
            {"+04", 14400, false},
        }};

        inline constexpr array<zone::transition, 1> transitions{{
            {1, -1577936472},
        }};
    }

    inline constexpr db::entry dubai{"Asia/Dubai",
                                     "Crozet",
                                     "+2518+05518",
                                     detail::dubai::country_codes,
                                     detail::dubai::offsets,
                                     detail::dubai::transitions};

    // clang-format on
}
