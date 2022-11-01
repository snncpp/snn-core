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

    namespace detail::noumea
    {
        inline constexpr array<country::code, 1> country_codes{
            "NC",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", 39948, false},
            {"+12", 43200, true},
            {"+11", 39600, false},
            {"+12", 43200, true},
            {"+11", 39600, false},
        }};

        inline constexpr array<zone::transition, 7> transitions{{
            {2, -1829387148},
            {1, 250002000},
            {2, 257342400},
            {1, 281451600},
            {2, 288878400},
            {3, 849366000},
            {4, 857228400},
        }};
    }

    inline constexpr db::entry noumea{"Pacific/Noumea",
                                      "",
                                      "-2216+16627",
                                      detail::noumea::country_codes,
                                      detail::noumea::offsets,
                                      detail::noumea::transitions};

    // clang-format on
}
