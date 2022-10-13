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

    namespace detail::phoenix
    {
        inline constexpr array<country::code, 2> country_codes{
            "US",
            "CA",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", -26898, false},
            {"MDT", -21600, true},
            {"MST", -25200, false},
            {"MWT", -21600, true},
            {"MST", -25200, false},
        }};

        inline constexpr array<zone::transition, 11> transitions{{
            {4, -2717643600},
            {1, -1633273200},
            {2, -1615132800},
            {1, -1601823600},
            {2, -1583683200},
            {3, -880210800},
            {2, -820519140},
            {3, -812653140},
            {2, -796845540},
            {1, -84380400},
            {2, -68659200},
        }};
    }

    inline constexpr db::entry phoenix{"America/Phoenix",
                                       "MST - Arizona (except Navajo), Creston BC",
                                       "+332654-1120424",
                                       detail::phoenix::country_codes,
                                       detail::phoenix::offsets,
                                       detail::phoenix::transitions};

    // clang-format on
}
