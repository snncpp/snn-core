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

    namespace detail::danmarkshavn
    {
        inline constexpr array<country::code, 1> country_codes{
            "GL",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -4480, false},
            {"-03", -10800, false},
            {"-03", -10800, false},
            {"-02", -7200, true},
            {"-02", -7200, true},
            {"GMT", 0, false},
        }};

        inline constexpr array<zone::transition, 34> transitions{{
            {1, -1686091520},
            {4, 323845200},
            {2, 338950800},
            {3, 354675600},
            {2, 370400400},
            {3, 386125200},
            {2, 401850000},
            {3, 417574800},
            {2, 433299600},
            {3, 449024400},
            {2, 465354000},
            {3, 481078800},
            {2, 496803600},
            {3, 512528400},
            {2, 528253200},
            {3, 543978000},
            {2, 559702800},
            {3, 575427600},
            {2, 591152400},
            {3, 606877200},
            {2, 622602000},
            {3, 638326800},
            {2, 654656400},
            {3, 670381200},
            {2, 686106000},
            {3, 701830800},
            {2, 717555600},
            {3, 733280400},
            {2, 749005200},
            {3, 764730000},
            {2, 780454800},
            {3, 796179600},
            {2, 811904400},
            {5, 820465200},
        }};
    }

    inline constexpr db::entry danmarkshavn{"America/Danmarkshavn",
                                            "National Park (east coast)",
                                            "+7646-01840",
                                            detail::danmarkshavn::country_codes,
                                            detail::danmarkshavn::offsets,
                                            detail::danmarkshavn::transitions};

    // clang-format on
}
