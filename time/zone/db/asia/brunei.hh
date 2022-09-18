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
#include "snn-core/time/zone/db/asia/kuching.hh"

namespace snn::time::zone::db::asia
{
    // clang-format off

    namespace detail::brunei
    {
        inline constexpr array<country::code, 1> country_codes{
            "BN",
        };
    }

    inline constexpr db::entry brunei{"Asia/Brunei",
                                      "",
                                      "+0456+11455",
                                      detail::brunei::country_codes,
                                      asia::detail::kuching::offsets,
                                      asia::detail::kuching::transitions};

    // clang-format on
}
