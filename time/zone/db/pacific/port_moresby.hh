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

    namespace detail::port_moresby
    {
        inline constexpr array<country::code, 3> country_codes{
            "PG",
            "AQ",
            "FM",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 35320, false},
            {"PMMT", 35312, false},
            {"+10", 36000, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -2840176120},
            {2, -2366790512},
        }};
    }

    inline constexpr db::entry port_moresby{"Pacific/Port_Moresby",
                                            "Papua New Guinea (most areas), Chuuk, Yap, Dumont d'Urville",
                                            "-0930+14710",
                                            detail::port_moresby::country_codes,
                                            detail::port_moresby::offsets,
                                            detail::port_moresby::transitions};

    // clang-format on
}
