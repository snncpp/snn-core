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
#include "snn-core/time/zone/db/america/panama.hh"

namespace snn::time::zone::db::america
{
    // clang-format off

    namespace detail::cayman
    {
        inline constexpr array<country::code, 1> country_codes{
            "KY",
        };
    }

    inline constexpr db::entry cayman{"America/Cayman",
                                      "",
                                      "+1918-08123",
                                      detail::cayman::country_codes,
                                      america::detail::panama::offsets,
                                      america::detail::panama::transitions};

    // clang-format on
}
