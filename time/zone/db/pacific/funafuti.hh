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
#include "snn-core/time/zone/db/pacific/tarawa.hh"

namespace snn::time::zone::db::pacific
{
    // clang-format off

    namespace detail::funafuti
    {
        inline constexpr array<country::code, 1> country_codes{
            "TV",
        };
    }

    inline constexpr db::entry funafuti{"Pacific/Funafuti",
                                        "",
                                        "-0831+17913",
                                        detail::funafuti::country_codes,
                                        pacific::detail::tarawa::offsets,
                                        pacific::detail::tarawa::transitions};

    // clang-format on
}
