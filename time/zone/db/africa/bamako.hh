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
#include "snn-core/time/zone/db/africa/abidjan.hh"

namespace snn::time::zone::db::africa
{
    // clang-format off

    namespace detail::bamako
    {
        inline constexpr array<country::code, 1> country_codes{
            "ML",
        };
    }

    inline constexpr db::entry bamako{"Africa/Bamako",
                                      "",
                                      "+1239-00800",
                                      detail::bamako::country_codes,
                                      africa::detail::abidjan::offsets,
                                      africa::detail::abidjan::transitions};

    // clang-format on
}
