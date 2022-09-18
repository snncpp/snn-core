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

    namespace detail::noronha
    {
        inline constexpr array<country::code, 1> country_codes{
            "BR",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -7780, false},
            {"-01", -3600, true},
            {"-02", -7200, false},
        }};

        inline constexpr array<zone::transition, 40> transitions{{
            {2, -1767217820},
            {1, -1206961200},
            {2, -1191366000},
            {1, -1175378400},
            {2, -1159830000},
            {1, -633823200},
            {2, -622072800},
            {1, -602287200},
            {2, -591836400},
            {1, -570751200},
            {2, -560214000},
            {1, -539128800},
            {2, -531356400},
            {1, -191368800},
            {2, -184201200},
            {1, -155167200},
            {2, -150073200},
            {1, -128901600},
            {2, -121129200},
            {1, -99957600},
            {2, -89593200},
            {1, -68421600},
            {2, -57970800},
            {1, 499744800},
            {2, 511232400},
            {1, 530589600},
            {2, 540262800},
            {1, 562125600},
            {2, 571194000},
            {1, 592970400},
            {2, 602038800},
            {1, 624420000},
            {2, 634698000},
            {1, 938916000},
            {2, 951613200},
            {1, 970970400},
            {2, 971571600},
            {1, 1003024800},
            {2, 1013907600},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry noronha{"America/Noronha",
                                       "Atlantic islands",
                                       "-0351-03225",
                                       detail::noronha::country_codes,
                                       detail::noronha::offsets,
                                       detail::noronha::transitions};

    // clang-format on
}
