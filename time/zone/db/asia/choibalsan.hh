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
#include "snn-core/time/zone/db/asia/ulaanbaatar.hh"

namespace snn::time::zone::db::asia
{
    // clang-format off

    namespace detail::choibalsan
    {
        inline constexpr array<country::code, 1> country_codes{
            "MN",
        };
    }

    inline constexpr db::entry choibalsan{"Asia/Choibalsan",
                                          "most of Mongolia",
                                          "+4755+10653",
                                          detail::choibalsan::country_codes,
                                          asia::detail::ulaanbaatar::offsets,
                                          asia::detail::ulaanbaatar::transitions};

    // clang-format on
}
