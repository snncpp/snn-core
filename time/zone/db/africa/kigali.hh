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
#include "snn-core/time/zone/db/africa/maputo.hh"

namespace snn::time::zone::db::africa
{
    // clang-format off

    namespace detail::kigali
    {
        inline constexpr array<country::code, 1> country_codes{
            "RW",
        };
    }

    inline constexpr db::entry kigali{"Africa/Kigali",
                                      "",
                                      "-0157+03004",
                                      detail::kigali::country_codes,
                                      africa::detail::maputo::offsets,
                                      africa::detail::maputo::transitions};

    // clang-format on
}
