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

namespace snn::time::zone::db::asia
{
    // clang-format off

    namespace detail::qatar
    {
        inline constexpr array<country::code, 2> country_codes{
            "QA",
            "BH",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 12368, false},
            {"+04", 14400, false},
            {"+03", 10800, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -1577935568},
            {2, 76190400},
        }};
    }

    inline constexpr db::entry qatar{"Asia/Qatar",
                                     "",
                                     "+2517+05132",
                                     detail::qatar::country_codes,
                                     detail::qatar::offsets,
                                     detail::qatar::transitions};

    // clang-format on
}
