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

    namespace detail::tongatapu
    {
        inline constexpr array<country::code, 1> country_codes{
            "TO",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", 44352, false},
            {"+1220", 44400, false},
            {"+13", 46800, false},
            {"+14", 50400, true},
            {"+13", 46800, false},
            {"+14", 50400, true},
        }};

        inline constexpr array<zone::transition, 11> transitions{{
            {1, -767189952},
            {2, -284041200},
            {3, 939214800},
            {4, 953384400},
            {5, 973342800},
            {2, 980596800},
            {5, 1004792400},
            {2, 1012046400},
            {5, 1478350800},
            {2, 1484398800},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry tongatapu{"Pacific/Tongatapu",
                                         "",
                                         "-210800-1751200",
                                         detail::tongatapu::country_codes,
                                         detail::tongatapu::offsets,
                                         detail::tongatapu::transitions};

    // clang-format on
}
