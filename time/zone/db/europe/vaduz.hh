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
#include "snn-core/time/zone/db/europe/zurich.hh"

namespace snn::time::zone::db::europe
{
    // clang-format off

    namespace detail::vaduz
    {
        inline constexpr array<country::code, 1> country_codes{
            "LI",
        };
    }

    inline constexpr db::entry vaduz{"Europe/Vaduz",
                                     "",
                                     "+4709+00931",
                                     detail::vaduz::country_codes,
                                     europe::detail::zurich::offsets,
                                     europe::detail::zurich::transitions};

    // clang-format on
}
