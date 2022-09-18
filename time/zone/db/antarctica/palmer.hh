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

    namespace detail::palmer
    {
        inline constexpr array<country::code, 1> country_codes{
            "AQ",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"-00", 0, false},
            {"-04", -14400, false},
            {"-03", -10800, true},
            {"-02", -7200, true},
            {"-03", -10800, false},
            {"-03", -10800, true},
            {"-04", -14400, false},
            {"-03", -10800, false},
        }};

        inline constexpr array<zone::transition, 83> transitions{{
            {2, -157766400},
            {1, -152658000},
            {2, -132955200},
            {1, -121122000},
            {2, -101419200},
            {1, -86821200},
            {2, -71092800},
            {1, -54766800},
            {2, -39038400},
            {1, -23317200},
            {4, -7588800},
            {3, 128142000},
            {4, 136605600},
            {1, 389070000},
            {5, 403070400},
            {6, 416372400},
            {5, 434520000},
            {6, 447822000},
            {5, 466574400},
            {6, 479271600},
            {5, 498024000},
            {6, 510721200},
            {5, 529473600},
            {6, 545194800},
            {5, 560923200},
            {6, 574225200},
            {5, 592372800},
            {6, 605674800},
            {5, 624427200},
            {6, 637124400},
            {5, 653457600},
            {6, 668574000},
            {5, 687326400},
            {6, 700628400},
            {5, 718776000},
            {6, 732078000},
            {5, 750225600},
            {6, 763527600},
            {5, 781675200},
            {6, 794977200},
            {5, 813729600},
            {6, 826426800},
            {5, 845179200},
            {6, 859690800},
            {5, 876628800},
            {6, 889930800},
            {5, 906868800},
            {6, 923194800},
            {5, 939528000},
            {6, 952830000},
            {5, 971582400},
            {6, 984279600},
            {5, 1003032000},
            {6, 1015729200},
            {5, 1034481600},
            {6, 1047178800},
            {5, 1065931200},
            {6, 1079233200},
            {5, 1097380800},
            {6, 1110682800},
            {5, 1128830400},
            {6, 1142132400},
            {5, 1160884800},
            {6, 1173582000},
            {5, 1192334400},
            {6, 1206846000},
            {5, 1223784000},
            {6, 1237086000},
            {5, 1255233600},
            {6, 1270350000},
            {5, 1286683200},
            {6, 1304823600},
            {5, 1313899200},
            {6, 1335668400},
            {5, 1346558400},
            {6, 1367118000},
            {5, 1378612800},
            {6, 1398567600},
            {5, 1410062400},
            {6, 1463281200},
            {5, 1471147200},
            {4, 1480820400},
            {4, 2147483647},
        }};
    }

    inline constexpr db::entry palmer{"Antarctica/Palmer",
                                      "Palmer",
                                      "-6448-06406",
                                      detail::palmer::country_codes,
                                      detail::palmer::offsets,
                                      detail::palmer::transitions};

    // clang-format on
}
