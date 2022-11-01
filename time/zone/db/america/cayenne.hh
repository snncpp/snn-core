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

namespace snn::time::zone::db::america
{
    // clang-format off

    namespace detail::cayenne
    {
        inline constexpr array<country::code, 1> country_codes{
            "GF",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -12560, false},
            {"-04", -14400, false},
            {"-03", -10800, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -1846269040},
            {2, -71092800},
        }};
    }

    inline constexpr db::entry cayenne{"America/Cayenne",
                                       "",
                                       "+0456-05220",
                                       detail::cayenne::country_codes,
                                       detail::cayenne::offsets,
                                       detail::cayenne::transitions};

    // clang-format on
}
