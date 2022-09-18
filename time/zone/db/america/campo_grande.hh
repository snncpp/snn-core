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

    namespace detail::campo_grande
    {
        inline constexpr array<country::code, 1> country_codes{
            "BR",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -13108, false},
            {"-03", -10800, true},
            {"-04", -14400, false},
        }};

        inline constexpr array<zone::transition, 92> transitions{{
            {2, -1767212492},
            {1, -1206954000},
            {2, -1191358800},
            {1, -1175371200},
            {2, -1159822800},
            {1, -633816000},
            {2, -622065600},
            {1, -602280000},
            {2, -591829200},
            {1, -570744000},
            {2, -560206800},
            {1, -539121600},
            {2, -531349200},
            {1, -191361600},
            {2, -184194000},
            {1, -155160000},
            {2, -150066000},
            {1, -128894400},
            {2, -121122000},
            {1, -99950400},
            {2, -89586000},
            {1, -68414400},
            {2, -57963600},
            {1, 499752000},
            {2, 511239600},
            {1, 530596800},
            {2, 540270000},
            {1, 562132800},
            {2, 571201200},
            {1, 592977600},
            {2, 602046000},
            {1, 624427200},
            {2, 634705200},
            {1, 656481600},
            {2, 666759600},
            {1, 687931200},
            {2, 697604400},
            {1, 719985600},
            {2, 728449200},
            {1, 750830400},
            {2, 761713200},
            {1, 782280000},
            {2, 793162800},
            {1, 813729600},
            {2, 824007600},
            {1, 844574400},
            {2, 856062000},
            {1, 876110400},
            {2, 888721200},
            {1, 908078400},
            {2, 919566000},
            {1, 938923200},
            {2, 951620400},
            {1, 970977600},
            {2, 982465200},
            {1, 1003032000},
            {2, 1013914800},
            {1, 1036296000},
            {2, 1045364400},
            {1, 1066536000},
            {2, 1076814000},
            {1, 1099368000},
            {2, 1108868400},
            {1, 1129435200},
            {2, 1140318000},
            {1, 1162699200},
            {2, 1172372400},
            {1, 1192334400},
            {2, 1203217200},
            {1, 1224388800},
            {2, 1234666800},
            {1, 1255838400},
            {2, 1266721200},
            {1, 1287288000},
            {2, 1298170800},
            {1, 1318737600},
            {2, 1330225200},
            {1, 1350792000},
            {2, 1361070000},
            {1, 1382241600},
            {2, 1392519600},
            {1, 1413691200},
            {2, 1424574000},
            {1, 1445140800},
            {2, 1456023600},
            {1, 1476590400},
            {2, 1487473200},
            {1, 1508040000},
            {2, 1518922800},
            {1, 1541304000},
            {2, 1550372400},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry campo_grande{"America/Campo_Grande",
                                            "Mato Grosso do Sul",
                                            "-2027-05437",
                                            detail::campo_grande::country_codes,
                                            detail::campo_grande::offsets,
                                            detail::campo_grande::transitions};

    // clang-format on
}
