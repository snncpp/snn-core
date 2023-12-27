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

namespace snn::time::zone::db::antarctica
{
    // clang-format off

    namespace detail::vostok
    {
        inline constexpr array<country::code, 1> country_codes{
            "AQ",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"-00", 0, false},
            {"+07", 25200, false},
            {"+05", 18000, false},
        }};

        inline constexpr array<zone::transition, 4> transitions{{
            {1, -380073600},
            {0, 760035600},
            {1, 783648000},
            {2, 1702839600},
        }};
    }

    inline constexpr db::entry vostok{"Antarctica/Vostok",
                                      "Vostok",
                                      "-7824+10654",
                                      detail::vostok::country_codes,
                                      detail::vostok::offsets,
                                      detail::vostok::transitions};

    // clang-format on
}
