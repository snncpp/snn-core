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
#include "snn-core/time/zone/db/indian/maldives.hh"

namespace snn::time::zone::db::indian
{
    // clang-format off

    namespace detail::kerguelen
    {
        inline constexpr array<country::code, 1> country_codes{
            "TF",
        };
    }

    inline constexpr db::entry kerguelen{"Indian/Kerguelen",
                                         "",
                                         "-492110+0701303",
                                         detail::kerguelen::country_codes,
                                         indian::detail::maldives::offsets,
                                         indian::detail::maldives::transitions};

    // clang-format on
}
