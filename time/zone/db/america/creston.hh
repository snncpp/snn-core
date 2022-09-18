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
#include "snn-core/time/zone/db/america/phoenix.hh"

namespace snn::time::zone::db::america
{
    // clang-format off

    namespace detail::creston
    {
        inline constexpr array<country::code, 1> country_codes{
            "CA",
        };
    }

    inline constexpr db::entry creston{"America/Creston",
                                       "MST - BC (Creston)",
                                       "+4906-11631",
                                       detail::creston::country_codes,
                                       america::detail::phoenix::offsets,
                                       america::detail::phoenix::transitions};

    // clang-format on
}
