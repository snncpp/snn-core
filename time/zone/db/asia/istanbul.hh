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
#include "snn-core/time/zone/db/europe/istanbul.hh"

namespace snn::time::zone::db::asia
{
    // clang-format off

    namespace detail::istanbul
    {
        inline constexpr array<country::code, 1> country_codes{
            "TR",
        };
    }

    inline constexpr db::entry istanbul{"Asia/Istanbul",
                                        "",
                                        "+4101+02858",
                                        detail::istanbul::country_codes,
                                        europe::detail::istanbul::offsets,
                                        europe::detail::istanbul::transitions};

    // clang-format on
}
