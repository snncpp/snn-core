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

    namespace detail::yellowknife
    {
        inline constexpr array<country::code, 1> country_codes{
            "CA",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"-00", 0, false},
            {"MWT", -21600, true},
            {"MPT", -21600, true},
            {"MST", -25200, false},
            {"MDDT", -18000, true},
            {"MDT", -21600, true},
        }};

        inline constexpr array<zone::transition, 122> transitions{{
            {3, -1104537600},
            {1, -880210800},
            {2, -769395600},
            {3, -765388800},
            {4, -147891600},
            {3, -131562000},
            {5, 325674000},
            {3, 341395200},
            {5, 357123600},
            {3, 372844800},
            {5, 388573200},
            {3, 404899200},
            {5, 420022800},
            {3, 436348800},
            {5, 452077200},
            {3, 467798400},
            {5, 483526800},
            {3, 499248000},
            {5, 514976400},
            {3, 530697600},
            {5, 544611600},
            {3, 562147200},
            {5, 576061200},
            {3, 594201600},
            {5, 607510800},
            {3, 625651200},
            {5, 638960400},
            {3, 657100800},
            {5, 671014800},
            {3, 688550400},
            {5, 702464400},
            {3, 720000000},
            {5, 733914000},
            {3, 752054400},
            {5, 765363600},
            {3, 783504000},
            {5, 796813200},
            {3, 814953600},
            {5, 828867600},
            {3, 846403200},
            {5, 860317200},
            {3, 877852800},
            {5, 891766800},
            {3, 909302400},
            {5, 923216400},
            {3, 941356800},
            {5, 954666000},
            {3, 972806400},
            {5, 986115600},
            {3, 1004256000},
            {5, 1018170000},
            {3, 1035705600},
            {5, 1049619600},
            {3, 1067155200},
            {5, 1081069200},
            {3, 1099209600},
            {5, 1112518800},
            {3, 1130659200},
            {5, 1143968400},
            {3, 1162108800},
            {5, 1173603600},
            {3, 1194163200},
            {5, 1205053200},
            {3, 1225612800},
            {5, 1236502800},
            {3, 1257062400},
            {5, 1268557200},
            {3, 1289116800},
            {5, 1300006800},
            {3, 1320566400},
            {5, 1331456400},
            {3, 1352016000},
            {5, 1362906000},
            {3, 1383465600},
            {5, 1394355600},
            {3, 1414915200},
            {5, 1425805200},
            {3, 1446364800},
            {5, 1457859600},
            {3, 1478419200},
            {5, 1489309200},
            {3, 1509868800},
            {5, 1520758800},
            {3, 1541318400},
            {5, 1552208400},
            {3, 1572768000},
            {5, 1583658000},
            {3, 1604217600},
            {5, 1615712400},
            {3, 1636272000},
            {5, 1647162000},
            {3, 1667721600},
            {5, 1678611600},
            {3, 1699171200},
            {5, 1710061200},
            {3, 1730620800},
            {5, 1741510800},
            {3, 1762070400},
            {5, 1772960400},
            {3, 1793520000},
            {5, 1805014800},
            {3, 1825574400},
            {5, 1836464400},
            {3, 1857024000},
            {5, 1867914000},
            {3, 1888473600},
            {5, 1899363600},
            {3, 1919923200},
            {5, 1930813200},
            {3, 1951372800},
            {5, 1962867600},
            {3, 1983427200},
            {5, 1994317200},
            {3, 2014876800},
            {5, 2025766800},
            {3, 2046326400},
            {5, 2057216400},
            {3, 2077776000},
            {5, 2088666000},
            {3, 2109225600},
            {5, 2120115600},
            {3, 2140675200},
        }};
    }

    inline constexpr db::entry yellowknife{"America/Yellowknife",
                                           "Mountain - NT (central)",
                                           "+6227-11421",
                                           detail::yellowknife::country_codes,
                                           detail::yellowknife::offsets,
                                           detail::yellowknife::transitions};

    // clang-format on
}
