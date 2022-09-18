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

    namespace detail::palau
    {
        inline constexpr array<country::code, 1> country_codes{
            "PW",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -54124, false},
            {"LMT", 32276, false},
            {"+09", 32400, false},
        }};

        inline constexpr array<zone::transition, 3> transitions{{
            {1, -3944624276},
            {2, -2177485076},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry palau{"Pacific/Palau",
                                     "",
                                     "+0720+13429",
                                     detail::palau::country_codes,
                                     detail::palau::offsets,
                                     detail::palau::transitions};

    // clang-format on
}
