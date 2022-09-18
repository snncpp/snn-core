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
#include "snn-core/time/zone/db/pacific/guadalcanal.hh"

namespace snn::time::zone::db::pacific
{
    // clang-format off

    namespace detail::pohnpei
    {
        inline constexpr array<country::code, 1> country_codes{
            "FM",
        };
    }

    inline constexpr db::entry pohnpei{"Pacific/Pohnpei",
                                       "Pohnpei/Ponape",
                                       "+0658+15813",
                                       detail::pohnpei::country_codes,
                                       pacific::detail::guadalcanal::offsets,
                                       pacific::detail::guadalcanal::transitions};

    // clang-format on
}
