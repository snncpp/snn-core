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

namespace snn::time::zone::db::pacific
{
    // clang-format off

    namespace detail::pago_pago
    {
        inline constexpr array<country::code, 2> country_codes{
            "AS",
            "UM",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 45432, false},
            {"LMT", -40968, false},
            {"SST", -39600, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -2445424632},
            {2, -1861879032},
        }};
    }

    inline constexpr db::entry pago_pago{"Pacific/Pago_Pago",
                                         "Midway",
                                         "-1416-17042",
                                         detail::pago_pago::country_codes,
                                         detail::pago_pago::offsets,
                                         detail::pago_pago::transitions};

    // clang-format on
}
