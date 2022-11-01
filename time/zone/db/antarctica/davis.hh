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

namespace snn::time::zone::db::antarctica
{
    // clang-format off

    namespace detail::davis
    {
        inline constexpr array<country::code, 1> country_codes{
            "AQ",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"-00", 0, false},
            {"+07", 25200, false},
            {"+05", 18000, false},
            {"+07", 25200, false},
        }};

        inline constexpr array<zone::transition, 7> transitions{{
            {1, -409190400},
            {0, -163062000},
            {1, -28857600},
            {2, 1255806000},
            {3, 1268251200},
            {2, 1319742000},
            {3, 1329854400},
        }};
    }

    inline constexpr db::entry davis{"Antarctica/Davis",
                                     "Davis",
                                     "-6835+07758",
                                     detail::davis::country_codes,
                                     detail::davis::offsets,
                                     detail::davis::transitions};

    // clang-format on
}
