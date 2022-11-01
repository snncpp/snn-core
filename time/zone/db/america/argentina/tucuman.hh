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

namespace snn::time::zone::db::america::argentina
{
    // clang-format off

    namespace detail::tucuman
    {
        inline constexpr array<country::code, 1> country_codes{
            "AR",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -15652, false},
            {"CMT", -15408, false},
            {"-04", -14400, false},
            {"-03", -10800, true},
            {"-02", -7200, true},
            {"-03", -10800, false},
        }};

        inline constexpr array<zone::transition, 63> transitions{{
            {1, -2372096348},
            {2, -1567453392},
            {3, -1233432000},
            {2, -1222981200},
            {3, -1205956800},
            {2, -1194037200},
            {3, -1172865600},
            {2, -1162501200},
            {3, -1141329600},
            {2, -1130965200},
            {3, -1109793600},
            {2, -1099429200},
            {3, -1078257600},
            {2, -1067806800},
            {3, -1046635200},
            {2, -1036270800},
            {3, -1015099200},
            {2, -1004734800},
            {3, -983563200},
            {2, -973198800},
            {3, -952027200},
            {2, -941576400},
            {3, -931032000},
            {2, -900882000},
            {3, -890337600},
            {2, -833749200},
            {3, -827265600},
            {2, -752274000},
            {3, -733780800},
            {2, -197326800},
            {3, -190843200},
            {2, -184194000},
            {3, -164491200},
            {2, -152658000},
            {3, -132955200},
            {2, -121122000},
            {3, -101419200},
            {2, -86821200},
            {3, -71092800},
            {2, -54766800},
            {3, -39038400},
            {2, -23317200},
            {5, -7588800},
            {4, 128142000},
            {5, 136605600},
            {4, 596948400},
            {5, 605066400},
            {4, 624423600},
            {5, 636516000},
            {4, 656478000},
            {2, 667965600},
            {4, 687931200},
            {5, 699415200},
            {4, 719377200},
            {5, 731469600},
            {3, 938919600},
            {5, 952052400},
            {2, 1086058800},
            {5, 1087099200},
            {4, 1198983600},
            {5, 1205632800},
            {4, 1224385200},
            {5, 1237082400},
        }};
    }

    inline constexpr db::entry tucuman{"America/Argentina/Tucuman",
                                       "Tucumán (TM)",
                                       "-2649-06513",
                                       detail::tucuman::country_codes,
                                       detail::tucuman::offsets,
                                       detail::tucuman::transitions};

    // clang-format on
}
