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

    namespace detail::kabul
    {
        inline constexpr array<country::code, 1> country_codes{
            "AF",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 16608, false},
            {"+04", 14400, false},
            {"+0430", 16200, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -2524538208},
            {2, -788932800},
        }};
    }

    inline constexpr db::entry kabul{"Asia/Kabul",
                                     "",
                                     "+3431+06912",
                                     detail::kabul::country_codes,
                                     detail::kabul::offsets,
                                     detail::kabul::transitions};

    // clang-format on
}
