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

    namespace detail::hovd
    {
        inline constexpr array<country::code, 1> country_codes{
            "MN",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", 21996, false},
            {"+06", 21600, false},
            {"+08", 28800, true},
            {"+07", 25200, false},
        }};

        inline constexpr array<zone::transition, 51> transitions{{
            {1, -2032927596},
            {3, 252439200},
            {2, 417978000},
            {3, 433785600},
            {2, 449600400},
            {3, 465321600},
            {2, 481050000},
            {3, 496771200},
            {2, 512499600},
            {3, 528220800},
            {2, 543949200},
            {3, 559670400},
            {2, 575398800},
            {3, 591120000},
            {2, 606848400},
            {3, 622569600},
            {2, 638298000},
            {3, 654624000},
            {2, 670352400},
            {3, 686073600},
            {2, 701802000},
            {3, 717523200},
            {2, 733251600},
            {3, 748972800},
            {2, 764701200},
            {3, 780422400},
            {2, 796150800},
            {3, 811872000},
            {2, 828205200},
            {3, 843926400},
            {2, 859654800},
            {3, 875376000},
            {2, 891104400},
            {3, 906825600},
            {2, 988398000},
            {3, 1001700000},
            {2, 1017428400},
            {3, 1033149600},
            {2, 1048878000},
            {3, 1064599200},
            {2, 1080327600},
            {3, 1096048800},
            {2, 1111777200},
            {3, 1127498400},
            {2, 1143226800},
            {3, 1159552800},
            {2, 1427482800},
            {3, 1443196800},
            {2, 1458932400},
            {3, 1474646400},
            {3, 2147483647},
        }};
    }

    inline constexpr db::entry hovd{"Asia/Hovd",
                                    "Bayan-Ölgii, Govi-Altai, Hovd, Uvs, Zavkhan",
                                    "+4801+09139",
                                    detail::hovd::country_codes,
                                    detail::hovd::offsets,
                                    detail::hovd::transitions};

    // clang-format on
}
