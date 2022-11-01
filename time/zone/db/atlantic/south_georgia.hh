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

namespace snn::time::zone::db::atlantic
{
    // clang-format off

    namespace detail::south_georgia
    {
        inline constexpr array<country::code, 1> country_codes{
            "GS",
        };

        inline constexpr array<zone::offset, 2> offsets{{
            {"LMT", -8768, false},
            {"-02", -7200, false},
        }};

        inline constexpr array<zone::transition, 1> transitions{{
            {1, -2524512832},
        }};
    }

    inline constexpr db::entry south_georgia{"Atlantic/South_Georgia",
                                             "",
                                             "-5416-03632",
                                             detail::south_georgia::country_codes,
                                             detail::south_georgia::offsets,
                                             detail::south_georgia::transitions};

    // clang-format on
}
