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

    namespace detail::gaborone
    {
        inline constexpr array<country::code, 1> country_codes{
            "BW",
        };
    }

    inline constexpr db::entry gaborone{"Africa/Gaborone",
                                        "",
                                        "-2439+02555",
                                        detail::gaborone::country_codes,
                                        africa::detail::maputo::offsets,
                                        africa::detail::maputo::transitions};

    // clang-format on
}
