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
#include "snn-core/time/zone/db/pacific/port_moresby.hh"

namespace snn::time::zone::db::pacific
{
    // clang-format off

    namespace detail::chuuk
    {
        inline constexpr array<country::code, 1> country_codes{
            "FM",
        };
    }

    inline constexpr db::entry chuuk{"Pacific/Chuuk",
                                     "Chuuk/Truk, Yap",
                                     "+0725+15147",
                                     detail::chuuk::country_codes,
                                     pacific::detail::port_moresby::offsets,
                                     pacific::detail::port_moresby::transitions};

    // clang-format on
}
