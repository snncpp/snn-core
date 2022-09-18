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
#include "snn-core/time/zone/db/africa/nairobi.hh"

namespace snn::time::zone::db::indian
{
    // clang-format off

    namespace detail::mayotte
    {
        inline constexpr array<country::code, 1> country_codes{
            "YT",
        };
    }

    inline constexpr db::entry mayotte{"Indian/Mayotte",
                                       "",
                                       "-1247+04514",
                                       detail::mayotte::country_codes,
                                       africa::detail::nairobi::offsets,
                                       africa::detail::nairobi::transitions};

    // clang-format on
}
