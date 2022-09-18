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
#include "snn-core/time/zone/db/asia/urumqi.hh"

namespace snn::time::zone::db::antarctica
{
    // clang-format off

    namespace detail::vostok
    {
        inline constexpr array<country::code, 1> country_codes{
            "AQ",
        };
    }

    inline constexpr db::entry vostok{"Antarctica/Vostok",
                                      "Vostok",
                                      "-7824+10654",
                                      detail::vostok::country_codes,
                                      asia::detail::urumqi::offsets,
                                      asia::detail::urumqi::transitions};

    // clang-format on
}
