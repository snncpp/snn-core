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

namespace snn::time::zone::db::australia
{
    // clang-format off

    namespace detail::melbourne
    {
        inline constexpr array<country::code, 1> country_codes{
            "AU",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", 34792, false},
            {"AEDT", 39600, true},
            {"AEST", 36000, false},
            {"AEST", 36000, false},
        }};

        inline constexpr array<zone::transition, 142> transitions{{
            {3, -2364111592},
            {1, -1672560000},
            {2, -1665388800},
            {1, -883641600},
            {2, -876124800},
            {1, -860400000},
            {2, -844675200},
            {1, -828345600},
            {2, -813225600},
            {1, 57686400},
            {2, 67968000},
            {1, 89136000},
            {2, 100022400},
            {1, 120585600},
            {2, 131472000},
            {1, 152035200},
            {2, 162921600},
            {1, 183484800},
            {2, 194976000},
            {1, 215539200},
            {2, 226425600},
            {1, 246988800},
            {2, 257875200},
            {1, 278438400},
            {2, 289324800},
            {1, 309888000},
            {2, 320774400},
            {1, 341337600},
            {2, 352224000},
            {1, 372787200},
            {2, 384278400},
            {1, 404841600},
            {2, 415728000},
            {1, 436291200},
            {2, 447177600},
            {1, 467740800},
            {2, 478627200},
            {1, 499190400},
            {2, 511286400},
            {1, 530035200},
            {2, 542736000},
            {1, 561484800},
            {2, 574790400},
            {1, 594144000},
            {2, 606240000},
            {1, 625593600},
            {2, 637689600},
            {1, 657043200},
            {2, 667929600},
            {1, 688492800},
            {2, 699379200},
            {1, 719942400},
            {2, 731433600},
            {1, 751996800},
            {2, 762883200},
            {1, 783446400},
            {2, 796147200},
            {1, 814896000},
            {2, 828201600},
            {1, 846345600},
            {2, 859651200},
            {1, 877795200},
            {2, 891100800},
            {1, 909244800},
            {2, 922550400},
            {1, 941299200},
            {2, 954000000},
            {1, 967305600},
            {2, 985449600},
            {1, 1004198400},
            {2, 1017504000},
            {1, 1035648000},
            {2, 1048953600},
            {1, 1067097600},
            {2, 1080403200},
            {1, 1099152000},
            {2, 1111852800},
            {1, 1130601600},
            {2, 1143907200},
            {1, 1162051200},
            {2, 1174752000},
            {1, 1193500800},
            {2, 1207411200},
            {1, 1223136000},
            {2, 1238860800},
            {1, 1254585600},
            {2, 1270310400},
            {1, 1286035200},
            {2, 1301760000},
            {1, 1317484800},
            {2, 1333209600},
            {1, 1349539200},
            {2, 1365264000},
            {1, 1380988800},
            {2, 1396713600},
            {1, 1412438400},
            {2, 1428163200},
            {1, 1443888000},
            {2, 1459612800},
            {1, 1475337600},
            {2, 1491062400},
            {1, 1506787200},
            {2, 1522512000},
            {1, 1538841600},
            {2, 1554566400},
            {1, 1570291200},
            {2, 1586016000},
            {1, 1601740800},
            {2, 1617465600},
            {1, 1633190400},
            {2, 1648915200},
            {1, 1664640000},
            {2, 1680364800},
            {1, 1696089600},
            {2, 1712419200},
            {1, 1728144000},
            {2, 1743868800},
            {1, 1759593600},
            {2, 1775318400},
            {1, 1791043200},
            {2, 1806768000},
            {1, 1822492800},
            {2, 1838217600},
            {1, 1853942400},
            {2, 1869667200},
            {1, 1885996800},
            {2, 1901721600},
            {1, 1917446400},
            {2, 1933171200},
            {1, 1948896000},
            {2, 1964620800},
            {1, 1980345600},
            {2, 1996070400},
            {1, 2011795200},
            {2, 2027520000},
            {1, 2043244800},
            {2, 2058969600},
            {1, 2075299200},
            {2, 2091024000},
            {1, 2106748800},
            {2, 2122473600},
            {1, 2138198400},
        }};
    }

    inline constexpr db::entry melbourne{"Australia/Melbourne",
                                         "Victoria",
                                         "-3749+14458",
                                         detail::melbourne::country_codes,
                                         detail::melbourne::offsets,
                                         detail::melbourne::transitions};

    // clang-format on
}