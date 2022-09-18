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

    namespace detail::thimphu
    {
        inline constexpr array<country::code, 1> country_codes{
            "BT",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 21516, false},
            {"+0530", 19800, false},
            {"+06", 21600, false},
        }};

        inline constexpr array<zone::transition, 3> transitions{{
            {1, -706341516},
            {2, 560025000},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry thimphu{"Asia/Thimphu",
                                       "",
                                       "+2728+08939",
                                       detail::thimphu::country_codes,
                                       detail::thimphu::offsets,
                                       detail::thimphu::transitions};

    // clang-format on
}
