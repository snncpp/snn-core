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

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", -20824, false},
            {"CST", -21600, false},
            {"EST", -18000, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
            {"EDT", -14400, true},
            {"CDT", -18000, true},
            {"EST", -18000, false},
        }};

        inline constexpr array<zone::transition, 43> transitions{{
            {1, -1514743200},
            {2, 378201600},
            {4, 410504400},
            {3, 828864000},
            {4, 846399600},
            {3, 860313600},
            {2, 877849200},
            {5, 891759600},
            {3, 902037600},
            {4, 909298800},
            {3, 923212800},
            {4, 941353200},
            {3, 954662400},
            {4, 972802800},
            {3, 989136000},
            {4, 1001833200},
            {3, 1018166400},
            {4, 1035702000},
            {3, 1049616000},
            {4, 1067151600},
            {3, 1081065600},
            {4, 1099206000},
            {3, 1112515200},
            {4, 1130655600},
            {3, 1143964800},
            {4, 1162105200},
            {3, 1175414400},
            {4, 1193554800},
            {3, 1207468800},
            {4, 1225004400},
            {3, 1238918400},
            {4, 1256454000},
            {3, 1270368000},
            {4, 1288508400},
            {3, 1301817600},
            {4, 1319958000},
            {3, 1333267200},
            {4, 1351407600},
            {3, 1365321600},
            {4, 1382857200},
            {3, 1396771200},
            {4, 1414306800},
            {2, 1422777600},
        }};
    }

    inline constexpr db::entry cancun{"America/Cancun",
                                      "Quintana Roo",
                                      "+2105-08646",
                                      detail::cancun::country_codes,
                                      detail::cancun::offsets,
                                      detail::cancun::transitions};

    // clang-format on
}
