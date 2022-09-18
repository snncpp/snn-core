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

    namespace detail::boa_vista
    {
        inline constexpr array<country::code, 1> country_codes{
            "BR",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -14560, false},
            {"-03", -10800, true},
            {"-04", -14400, false},
        }};

        inline constexpr array<zone::transition, 34> transitions{{
            {2, -1767211040},
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
            {1, 938923200},
            {2, 951620400},
            {1, 970977600},
            {2, 971578800},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry boa_vista{"America/Boa_Vista",
                                         "Roraima",
                                         "+0249-06040",
                                         detail::boa_vista::country_codes,
                                         detail::boa_vista::offsets,
                                         detail::boa_vista::transitions};

    // clang-format on
}
