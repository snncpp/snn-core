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

    namespace detail::puerto_rico
    {
        inline constexpr array<country::code, 20> country_codes{
            "PR",
            "AG",
            "CA",
            "AI",
            "AW",
            "BL",
            "BQ",
            "CW",
            "DM",
            "GD",
            "GP",
            "KN",
            "LC",
            "MF",
            "MS",
            "SX",
            "TT",
            "VC",
            "VG",
            "VI",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -15865, false},
            {"AST", -14400, false},
            {"APT", -10800, true},
            {"AWT", -10800, true},
        }};

        inline constexpr array<zone::transition, 4> transitions{{
            {1, -2233035335},
            {3, -873057600},
            {2, -769395600},
            {1, -765399600},
        }};
    }

    inline constexpr db::entry puerto_rico{"America/Puerto_Rico",
                                           "AST - QC (Lower North Shore)",
                                           "+182806-0660622",
                                           detail::puerto_rico::country_codes,
                                           detail::puerto_rico::offsets,
                                           detail::puerto_rico::transitions};

    // clang-format on
}
