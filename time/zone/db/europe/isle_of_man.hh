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
#include "snn-core/time/zone/db/europe/london.hh"

namespace snn::time::zone::db::europe
{
    // clang-format off

    namespace detail::isle_of_man
    {
        inline constexpr array<country::code, 1> country_codes{
            "IM",
        };
    }

    inline constexpr db::entry isle_of_man{"Europe/Isle_of_Man",
                                           "",
                                           "+5409-00428",
                                           detail::isle_of_man::country_codes,
                                           europe::detail::london::offsets,
                                           europe::detail::london::transitions};

    // clang-format on
}
