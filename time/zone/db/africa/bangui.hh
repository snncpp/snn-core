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
#include "snn-core/time/zone/db/africa/lagos.hh"

namespace snn::time::zone::db::africa
{
    // clang-format off

    namespace detail::bangui
    {
        inline constexpr array<country::code, 1> country_codes{
            "CF",
        };
    }

    inline constexpr db::entry bangui{"Africa/Bangui",
                                      "",
                                      "+0422+01835",
                                      detail::bangui::country_codes,
                                      africa::detail::lagos::offsets,
                                      africa::detail::lagos::transitions};

    // clang-format on
}
