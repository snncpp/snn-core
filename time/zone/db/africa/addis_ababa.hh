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

namespace snn::time::zone::db::africa
{
    // clang-format off

    namespace detail::addis_ababa
    {
        inline constexpr array<country::code, 1> country_codes{
            "ET",
        };
    }

    inline constexpr db::entry addis_ababa{"Africa/Addis_Ababa",
                                           "",
                                           "+0902+03842",
                                           detail::addis_ababa::country_codes,
                                           africa::detail::nairobi::offsets,
                                           africa::detail::nairobi::transitions};

    // clang-format on
}
