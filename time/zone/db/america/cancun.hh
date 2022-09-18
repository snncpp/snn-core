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

namespace snn::time::zone::db::america
{
    // clang-format off

    namespace detail::cancun
    {
        inline constexpr array<country::code, 1> country_codes{
            "MX",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", -20824, false},
            {"CST", -21600, false},
            {"EDT", -14400, true},
            {"EST", -18000, false},
            {"CDT", -18000, true},
        }};

        inline constexpr array<zone::transition, 42> transitions{{
            {1, -1514743200},
            {3, 377935200},
            {2, 828860400},
            {3, 846396000},
            {2, 860310000},
            {3, 877845600},
            {2, 891759600},
            {4, 902037600},
            {1, 909298800},
            {4, 923212800},
            {1, 941353200},
            {4, 954662400},
            {1, 972802800},
            {4, 989136000},
            {1, 1001833200},
            {4, 1018166400},
            {1, 1035702000},
            {4, 1049616000},
            {1, 1067151600},
            {4, 1081065600},
            {1, 1099206000},
            {4, 1112515200},
            {1, 1130655600},
            {4, 1143964800},
            {1, 1162105200},
            {4, 1175414400},
            {1, 1193554800},
            {4, 1207468800},
            {1, 1225004400},
            {4, 1238918400},
            {1, 1256454000},
            {4, 1270368000},
            {1, 1288508400},
            {4, 1301817600},
            {1, 1319958000},
            {4, 1333267200},
            {1, 1351407600},
            {4, 1365321600},
            {1, 1382857200},
            {4, 1396771200},
            {1, 1414306800},
            {3, 1422777600},
        }};
    }

    inline constexpr db::entry cancun{"America/Cancun",
                                      "Eastern Standard Time - Quintana Roo",
                                      "+2105-08646",
                                      detail::cancun::country_codes,
                                      detail::cancun::offsets,
                                      detail::cancun::transitions};

    // clang-format on
}
