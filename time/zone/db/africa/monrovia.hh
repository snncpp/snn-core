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

namespace snn::time::zone::db::africa
{
    // clang-format off

    namespace detail::monrovia
    {
        inline constexpr array<country::code, 1> country_codes{
            "LR",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -2588, false},
            {"MMT", -2588, false},
            {"MMT", -2670, false},
            {"GMT", 0, false},
        }};

        inline constexpr array<zone::transition, 3> transitions{{
            {1, -2776979812},
            {2, -1604359012},
            {3, 63593070},
        }};
    }

    inline constexpr db::entry monrovia{"Africa/Monrovia",
                                        "",
                                        "+0618-01047",
                                        detail::monrovia::country_codes,
                                        detail::monrovia::offsets,
                                        detail::monrovia::transitions};

    // clang-format on
}
