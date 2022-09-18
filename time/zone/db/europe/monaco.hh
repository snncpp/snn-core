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
#include "snn-core/time/zone/db/europe/paris.hh"

namespace snn::time::zone::db::europe
{
    // clang-format off

    namespace detail::monaco
    {
        inline constexpr array<country::code, 1> country_codes{
            "MC",
        };
    }

    inline constexpr db::entry monaco{"Europe/Monaco",
                                      "",
                                      "+4342+00723",
                                      detail::monaco::country_codes,
                                      europe::detail::paris::offsets,
                                      europe::detail::paris::transitions};

    // clang-format on
}
