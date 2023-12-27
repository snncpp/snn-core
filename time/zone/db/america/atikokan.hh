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
#include "snn-core/time/zone/db/america/panama.hh"

namespace snn::time::zone::db::america
{
    // clang-format off

    namespace detail::atikokan
    {
        inline constexpr array<country::code, 1> country_codes{
            "CA",
        };
    }

    inline constexpr db::entry atikokan{"America/Atikokan",
                                        "EST - ON (Atikokan), NU (Coral H)",
                                        "+484531-0913718",
                                        detail::atikokan::country_codes,
                                        america::detail::panama::offsets,
                                        america::detail::panama::transitions};

    // clang-format on
}
