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

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", -20824, false},
            {"CST", -21600, false},
            {"EDT", -14400, true},
            {"EST", -18000, false},
            {"CST", -21600, false},
            {"CDT", -18000, true},
            {"EST", -18000, false},
        }};

        inline constexpr array<zone::transition, 42> transitions{{
            {1, -1514743200},
            {3, 377935200},
            {2, 828860400},
            {3, 846396000},
            {2, 860310000},
            {3, 877845600},
            {2, 891759600},
            {5, 902037600},
            {4, 909298800},
            {5, 923212800},
            {4, 941353200},
            {5, 954662400},
            {4, 972802800},
            {5, 989136000},
            {4, 1001833200},
            {5, 1018166400},
            {4, 1035702000},
            {5, 1049616000},
            {4, 1067151600},
            {5, 1081065600},
            {4, 1099206000},
            {5, 1112515200},
            {4, 1130655600},
            {5, 1143964800},
            {4, 1162105200},
            {5, 1175414400},
            {4, 1193554800},
            {5, 1207468800},
            {4, 1225004400},
            {5, 1238918400},
            {4, 1256454000},
            {5, 1270368000},
            {4, 1288508400},
            {5, 1301817600},
            {4, 1319958000},
            {5, 1333267200},
            {4, 1351407600},
            {5, 1365321600},
            {4, 1382857200},
            {5, 1396771200},
            {4, 1414306800},
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
