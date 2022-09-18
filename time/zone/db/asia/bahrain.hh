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
#include "snn-core/time/zone/db/asia/qatar.hh"

namespace snn::time::zone::db::asia
{
    // clang-format off

    namespace detail::bahrain
    {
        inline constexpr array<country::code, 1> country_codes{
            "BH",
        };
    }

    inline constexpr db::entry bahrain{"Asia/Bahrain",
                                       "",
                                       "+2623+05035",
                                       detail::bahrain::country_codes,
                                       asia::detail::qatar::offsets,
                                       asia::detail::qatar::transitions};

    // clang-format on
}
