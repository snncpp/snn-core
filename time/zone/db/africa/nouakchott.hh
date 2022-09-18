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

    namespace detail::nouakchott
    {
        inline constexpr array<country::code, 1> country_codes{
            "MR",
        };
    }

    inline constexpr db::entry nouakchott{"Africa/Nouakchott",
                                          "",
                                          "+1806-01557",
                                          detail::nouakchott::country_codes,
                                          africa::detail::abidjan::offsets,
                                          africa::detail::abidjan::transitions};

    // clang-format on
}
