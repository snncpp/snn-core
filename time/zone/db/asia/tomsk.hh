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

    namespace detail::tomsk
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 10> offsets{{
            {"LMT", 20391, false},
            {"+06", 21600, false},
            {"+08", 28800, true},
            {"+07", 25200, false},
            {"+07", 25200, false},
            {"+08", 28800, true},
            {"+07", 25200, true},
            {"+06", 21600, false},
            {"+07", 25200, true},
            {"+07", 25200, false},
        }};

        inline constexpr array<zone::transition, 67> transitions{{
            {1, -1578807591},
            {3, -1247551200},
            {2, 354906000},
            {3, 370713600},
            {2, 386442000},
            {3, 402249600},
            {2, 417978000},
            {3, 433785600},
            {2, 449600400},
            {4, 465332400},
            {5, 481057200},
            {4, 496782000},
            {5, 512506800},
            {4, 528231600},
            {5, 543956400},
            {4, 559681200},
            {5, 575406000},
            {4, 591130800},
            {5, 606855600},
            {4, 622580400},
            {5, 638305200},
            {4, 654634800},
            {6, 670359600},
            {7, 686088000},
            {4, 695764800},
            {5, 701809200},
            {4, 717534000},
            {5, 733258800},
            {4, 748983600},
            {5, 764708400},
            {4, 780433200},
            {5, 796158000},
            {4, 811882800},
            {5, 828212400},
            {4, 846356400},
            {5, 859662000},
            {4, 877806000},
            {5, 891111600},
            {4, 909255600},
            {5, 922561200},
            {4, 941310000},
            {5, 954010800},
            {4, 972759600},
            {5, 985460400},
            {4, 1004209200},
            {5, 1017514800},
            {8, 1020193200},
            {7, 1035662400},
            {6, 1048968000},
            {7, 1067112000},
            {6, 1080417600},
            {7, 1099166400},
            {6, 1111867200},
            {7, 1130616000},
            {6, 1143316800},
            {7, 1162065600},
            {6, 1174766400},
            {7, 1193515200},
            {6, 1206820800},
            {7, 1224964800},
            {6, 1238270400},
            {7, 1256414400},
            {6, 1269720000},
            {7, 1288468800},
            {4, 1301169600},
            {7, 1414263600},
            {4, 1464465600},
        }};
    }

    inline constexpr db::entry tomsk{"Asia/Tomsk",
                                     "MSK+04 - Tomsk",
                                     "+5630+08458",
                                     detail::tomsk::country_codes,
                                     detail::tomsk::offsets,
                                     detail::tomsk::transitions};

    // clang-format on
}
