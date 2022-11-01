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

namespace snn::time::zone::db::pacific
{
    // clang-format off

    namespace detail::pitcairn
    {
        inline constexpr array<country::code, 1> country_codes{
            "PN",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -31220, false},
            {"-0830", -30600, false},
            {"-08", -28800, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -2177421580},
            {2, 893665800},
        }};
    }

    inline constexpr db::entry pitcairn{"Pacific/Pitcairn",
                                        "",
                                        "-2504-13005",
                                        detail::pitcairn::country_codes,
                                        detail::pitcairn::offsets,
                                        detail::pitcairn::transitions};

    // clang-format on
}
