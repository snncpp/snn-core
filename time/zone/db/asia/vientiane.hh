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
#include "snn-core/time/zone/db/asia/bangkok.hh"

namespace snn::time::zone::db::asia
{
    // clang-format off

    namespace detail::vientiane
    {
        inline constexpr array<country::code, 1> country_codes{
            "LA",
        };
    }

    inline constexpr db::entry vientiane{"Asia/Vientiane",
                                         "",
                                         "+1758+10236",
                                         detail::vientiane::country_codes,
                                         asia::detail::bangkok::offsets,
                                         asia::detail::bangkok::transitions};

    // clang-format on
}
