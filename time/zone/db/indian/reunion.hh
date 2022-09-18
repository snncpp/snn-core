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
#include "snn-core/time/zone/db/asia/dubai.hh"

namespace snn::time::zone::db::indian
{
    // clang-format off

    namespace detail::reunion
    {
        inline constexpr array<country::code, 1> country_codes{
            "RE",
        };
    }

    inline constexpr db::entry reunion{"Indian/Reunion",
                                       "",
                                       "-2052+05528",
                                       detail::reunion::country_codes,
                                       asia::detail::dubai::offsets,
                                       asia::detail::dubai::transitions};

    // clang-format on
}
