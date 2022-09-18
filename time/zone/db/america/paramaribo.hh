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

    namespace detail::paramaribo
    {
        inline constexpr array<country::code, 1> country_codes{
            "SR",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", -13240, false},
            {"PMT", -13252, false},
            {"PMT", -13236, false},
            {"-0330", -12600, false},
            {"-03", -10800, false},
        }};

        inline constexpr array<zone::transition, 5> transitions{{
            {1, -1861906760},
            {2, -1104524348},
            {3, -765317964},
            {4, 465449400},
            {4, 2147483647},
        }};
    }

    inline constexpr db::entry paramaribo{"America/Paramaribo",
                                          "",
                                          "+0550-05510",
                                          detail::paramaribo::country_codes,
                                          detail::paramaribo::offsets,
                                          detail::paramaribo::transitions};

    // clang-format on
}
