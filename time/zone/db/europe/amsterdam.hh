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
#include "snn-core/time/zone/db/europe/brussels.hh"

namespace snn::time::zone::db::europe
{
    // clang-format off

    namespace detail::amsterdam
    {
        inline constexpr array<country::code, 1> country_codes{
            "NL",
        };
    }

    inline constexpr db::entry amsterdam{"Europe/Amsterdam",
                                         "",
                                         "+5222+00454",
                                         detail::amsterdam::country_codes,
                                         europe::detail::brussels::offsets,
                                         europe::detail::brussels::transitions};

    // clang-format on
}
