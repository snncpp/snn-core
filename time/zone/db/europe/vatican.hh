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
#include "snn-core/time/zone/db/europe/rome.hh"

namespace snn::time::zone::db::europe
{
    // clang-format off

    namespace detail::vatican
    {
        inline constexpr array<country::code, 1> country_codes{
            "VA",
        };
    }

    inline constexpr db::entry vatican{"Europe/Vatican",
                                       "",
                                       "+415408+0122711",
                                       detail::vatican::country_codes,
                                       europe::detail::rome::offsets,
                                       europe::detail::rome::transitions};

    // clang-format on
}
