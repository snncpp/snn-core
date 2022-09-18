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

    namespace detail::zagreb
    {
        inline constexpr array<country::code, 1> country_codes{
            "HR",
        };
    }

    inline constexpr db::entry zagreb{"Europe/Zagreb",
                                      "",
                                      "+4548+01558",
                                      detail::zagreb::country_codes,
                                      europe::detail::belgrade::offsets,
                                      europe::detail::belgrade::transitions};

    // clang-format on
}
