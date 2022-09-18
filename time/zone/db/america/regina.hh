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

    namespace detail::regina
    {
        inline constexpr array<country::code, 1> country_codes{
            "CA",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -25116, false},
            {"MDT", -21600, true},
            {"MST", -25200, false},
            {"MWT", -21600, true},
            {"MPT", -21600, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 53> transitions{{
            {2, -2030202084},
            {1, -1632063600},
            {2, -1615132800},
            {1, -1251651600},
            {2, -1238349600},
            {1, -1220202000},
            {2, -1206900000},
            {1, -1188752400},
            {2, -1175450400},
            {1, -1156698000},
            {2, -1144000800},
            {1, -1125248400},
            {2, -1111946400},
            {1, -1032714000},
            {2, -1016992800},
            {1, -1001264400},
            {2, -986148000},
            {1, -969814800},
            {2, -954093600},
            {1, -937760400},
            {2, -922039200},
            {1, -906310800},
            {2, -890589600},
            {3, -880210800},
            {4, -769395600},
            {2, -765388800},
            {1, -748450800},
            {2, -732729600},
            {1, -715791600},
            {2, -702489600},
            {1, -684342000},
            {2, -671040000},
            {1, -652892400},
            {2, -639590400},
            {1, -620838000},
            {2, -608140800},
            {1, -589388400},
            {2, -576086400},
            {1, -557938800},
            {2, -544636800},
            {1, -526489200},
            {2, -513187200},
            {1, -495039600},
            {2, -481737600},
            {1, -463590000},
            {2, -450288000},
            {1, -431535600},
            {2, -418233600},
            {1, -400086000},
            {2, -386784000},
            {1, -337186800},
            {2, -321465600},
            {5, -305737200},
        }};
    }

    inline constexpr db::entry regina{"America/Regina",
                                      "CST - SK (most areas)",
                                      "+5024-10439",
                                      detail::regina::country_codes,
                                      detail::regina::offsets,
                                      detail::regina::transitions};

    // clang-format on
}
