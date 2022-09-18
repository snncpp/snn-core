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
#include "snn-core/time/zone/db/europe/belgrade.hh"

namespace snn::time::zone::db::europe
{
    // clang-format off

    namespace detail::podgorica
    {
        inline constexpr array<country::code, 1> country_codes{
            "ME",
        };
    }

    inline constexpr db::entry podgorica{"Europe/Podgorica",
                                         "",
                                         "+4226+01916",
                                         detail::podgorica::country_codes,
                                         europe::detail::belgrade::offsets,
                                         europe::detail::belgrade::transitions};

    // clang-format on
}
