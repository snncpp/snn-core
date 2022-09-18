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

namespace snn::time::zone::db::america
{
    // clang-format off

    namespace detail::la_paz
    {
        inline constexpr array<country::code, 1> country_codes{
            "BO",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -16356, false},
            {"CMT", -16356, false},
            {"BST", -12756, true},
            {"-04", -14400, false},
        }};

        inline constexpr array<zone::transition, 4> transitions{{
            {1, -2524505244},
            {2, -1205954844},
            {3, -1192307244},
            {3, 2147483647},
        }};
    }

    inline constexpr db::entry la_paz{"America/La_Paz",
                                      "",
                                      "-1630-06809",
                                      detail::la_paz::country_codes,
                                      detail::la_paz::offsets,
                                      detail::la_paz::transitions};

    // clang-format on
}
