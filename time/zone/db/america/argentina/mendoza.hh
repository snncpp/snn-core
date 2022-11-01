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

    namespace detail::mendoza
    {
        inline constexpr array<country::code, 1> country_codes{
            "AR",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -16516, false},
            {"CMT", -15408, false},
            {"-04", -14400, false},
            {"-03", -10800, true},
            {"-02", -7200, true},
            {"-03", -10800, false},
        }};

        inline constexpr array<zone::transition, 61> transitions{{
            {1, -2372095484},
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
            {2, 636516000},
            {3, 655963200},
            {2, 667796400},
            {3, 687499200},
            {2, 699418800},
            {4, 719380800},
            {5, 731469600},
            {3, 938919600},
            {5, 952052400},
            {2, 1085281200},
            {5, 1096171200},
            {4, 1198983600},
            {5, 1205632800},
        }};
    }

    inline constexpr db::entry mendoza{"America/Argentina/Mendoza",
                                       "Mendoza (MZ)",
                                       "-3253-06849",
                                       detail::mendoza::country_codes,
                                       detail::mendoza::offsets,
                                       detail::mendoza::transitions};

    // clang-format on
}
