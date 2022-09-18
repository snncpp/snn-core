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
#include "snn-core/time/zone/db/america/puerto_rico.hh"

namespace snn::time::zone::db::america
{
    // clang-format off

    namespace detail::blanc_sablon
    {
        inline constexpr array<country::code, 1> country_codes{
            "CA",
        };
    }

    inline constexpr db::entry blanc_sablon{"America/Blanc-Sablon",
                                            "AST - QC (Lower North Shore)",
                                            "+5125-05707",
                                            detail::blanc_sablon::country_codes,
                                            america::detail::puerto_rico::offsets,
                                            america::detail::puerto_rico::transitions};

    // clang-format on
}
