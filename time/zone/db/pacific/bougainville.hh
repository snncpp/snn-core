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

    namespace detail::bougainville
    {
        inline constexpr array<country::code, 1> country_codes{
            "PG",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", 37336, false},
            {"PMMT", 35312, false},
            {"+10", 36000, false},
            {"+09", 32400, false},
            {"+11", 39600, false},
        }};

        inline constexpr array<zone::transition, 5> transitions{{
            {1, -2840178136},
            {2, -2366790512},
            {3, -868010400},
            {2, -768906000},
            {4, 1419696000},
        }};
    }

    inline constexpr db::entry bougainville{"Pacific/Bougainville",
                                            "Bougainville",
                                            "-0613+15534",
                                            detail::bougainville::country_codes,
                                            detail::bougainville::offsets,
                                            detail::bougainville::transitions};

    // clang-format on
}
