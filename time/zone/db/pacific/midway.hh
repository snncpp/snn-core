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
#include "snn-core/time/zone/db/pacific/pago_pago.hh"

namespace snn::time::zone::db::pacific
{
    // clang-format off

    namespace detail::midway
    {
        inline constexpr array<country::code, 1> country_codes{
            "UM",
        };
    }

    inline constexpr db::entry midway{"Pacific/Midway",
                                      "Midway Islands",
                                      "+2813-17722",
                                      detail::midway::country_codes,
                                      pacific::detail::pago_pago::offsets,
                                      pacific::detail::pago_pago::transitions};

    // clang-format on
}
