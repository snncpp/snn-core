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

    namespace detail::abidjan
    {
        inline constexpr array<country::code, 12> country_codes{
            "CI",
            "BF",
            "GH",
            "GM",
            "GN",
            "IS",
            "ML",
            "MR",
            "SH",
            "SL",
            "SN",
            "TG",
        };

        inline constexpr array<zone::offset, 2> offsets{{
            {"LMT", -968, false},
            {"GMT", 0, false},
        }};

        inline constexpr array<zone::transition, 1> transitions{{
            {1, -1830383032},
        }};
    }

    inline constexpr db::entry abidjan{"Africa/Abidjan",
                                       "",
                                       "+0519-00402",
                                       detail::abidjan::country_codes,
                                       detail::abidjan::offsets,
                                       detail::abidjan::transitions};

    // clang-format on
}
