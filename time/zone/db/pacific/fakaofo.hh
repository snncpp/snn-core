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

    namespace detail::fakaofo
    {
        inline constexpr array<country::code, 1> country_codes{
            "TK",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -41096, false},
            {"-11", -39600, false},
            {"+13", 46800, false},
        }};

        inline constexpr array<zone::transition, 3> transitions{{
            {1, -2177411704},
            {2, 1325242800},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry fakaofo{"Pacific/Fakaofo",
                                       "",
                                       "-0922-17114",
                                       detail::fakaofo::country_codes,
                                       detail::fakaofo::offsets,
                                       detail::fakaofo::transitions};

    // clang-format on
}
