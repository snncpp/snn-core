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

namespace snn::time::zone::db::asia
{
    // clang-format off

    namespace detail::choibalsan
    {
        inline constexpr array<country::code, 1> country_codes{
            "MN",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", 27480, false},
            {"+07", 25200, false},
            {"+08", 28800, false},
            {"+09", 32400, false},
            {"+10", 36000, true},
            {"+09", 32400, true},
            {"+08", 28800, false},
        }};

        inline constexpr array<zone::transition, 52> transitions{{
            {1, -2032933080},
            {2, 252435600},
            {4, 417974400},
            {3, 433778400},
            {4, 449593200},
            {3, 465314400},
            {4, 481042800},
            {3, 496764000},
            {4, 512492400},
            {3, 528213600},
            {4, 543942000},
            {3, 559663200},
            {4, 575391600},
            {3, 591112800},
            {4, 606841200},
            {3, 622562400},
            {4, 638290800},
            {3, 654616800},
            {4, 670345200},
            {3, 686066400},
            {4, 701794800},
            {3, 717516000},
            {4, 733244400},
            {3, 748965600},
            {4, 764694000},
            {3, 780415200},
            {4, 796143600},
            {3, 811864800},
            {4, 828198000},
            {3, 843919200},
            {4, 859647600},
            {3, 875368800},
            {4, 891097200},
            {3, 906818400},
            {4, 988390800},
            {3, 1001692800},
            {4, 1017421200},
            {3, 1033142400},
            {4, 1048870800},
            {3, 1064592000},
            {4, 1080320400},
            {3, 1096041600},
            {4, 1111770000},
            {3, 1127491200},
            {4, 1143219600},
            {3, 1159545600},
            {2, 1206889200},
            {5, 1427479200},
            {2, 1443193200},
            {5, 1458928800},
            {2, 1474642800},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry choibalsan{"Asia/Choibalsan",
                                          "Dornod, Sükhbaatar",
                                          "+4804+11430",
                                          detail::choibalsan::country_codes,
                                          detail::choibalsan::offsets,
                                          detail::choibalsan::transitions};

    // clang-format on
}
