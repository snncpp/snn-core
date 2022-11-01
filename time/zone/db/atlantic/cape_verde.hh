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

namespace snn::time::zone::db::atlantic
{
    // clang-format off

    namespace detail::cape_verde
    {
        inline constexpr array<country::code, 1> country_codes{
            "CV",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", -5644, false},
            {"-02", -7200, false},
            {"-01", -3600, true},
            {"-02", -7200, false},
            {"-01", -3600, false},
        }};

        inline constexpr array<zone::transition, 4> transitions{{
            {1, -1830376800},
            {2, -862610400},
            {3, -764118000},
            {4, 186120000},
        }};
    }

    inline constexpr db::entry cape_verde{"Atlantic/Cape_Verde",
                                          "",
                                          "+1455-02331",
                                          detail::cape_verde::country_codes,
                                          detail::cape_verde::offsets,
                                          detail::cape_verde::transitions};

    // clang-format on
}
