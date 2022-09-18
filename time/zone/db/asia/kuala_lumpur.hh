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
#include "snn-core/time/zone/db/asia/singapore.hh"

namespace snn::time::zone::db::asia
{
    // clang-format off

    namespace detail::kuala_lumpur
    {
        inline constexpr array<country::code, 1> country_codes{
            "MY",
        };
    }

    inline constexpr db::entry kuala_lumpur{"Asia/Kuala_Lumpur",
                                            "Malaysia (peninsula)",
                                            "+0310+10142",
                                            detail::kuala_lumpur::country_codes,
                                            asia::detail::singapore::offsets,
                                            asia::detail::singapore::transitions};

    // clang-format on
}
