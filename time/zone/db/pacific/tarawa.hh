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

    namespace detail::tarawa
    {
        inline constexpr array<country::code, 5> country_codes{
            "KI",
            "MH",
            "TV",
            "UM",
            "WF",
        };

        inline constexpr array<zone::offset, 2> offsets{{
            {"LMT", 41524, false},
            {"+12", 43200, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -2177494324},
            {1, 2147483647},
        }};
    }

    inline constexpr db::entry tarawa{"Pacific/Tarawa",
                                      "Gilberts, Marshalls, Tuvalu, Wallis & Futuna, Wake",
                                      "+0125+17300",
                                      detail::tarawa::country_codes,
                                      detail::tarawa::offsets,
                                      detail::tarawa::transitions};

    // clang-format on
}
