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

    namespace detail::blantyre
    {
        inline constexpr array<country::code, 1> country_codes{
            "MW",
        };
    }

    inline constexpr db::entry blantyre{"Africa/Blantyre",
                                        "",
                                        "-1547+03500",
                                        detail::blantyre::country_codes,
                                        africa::detail::maputo::offsets,
                                        africa::detail::maputo::transitions};

    // clang-format on
}
