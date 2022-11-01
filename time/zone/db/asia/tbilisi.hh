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

    namespace detail::tbilisi
    {
        inline constexpr array<country::code, 1> country_codes{
            "GE",
        };

        inline constexpr array<zone::offset, 11> offsets{{
            {"LMT", 10751, false},
            {"TBMT", 10751, false},
            {"+03", 10800, false},
            {"+05", 18000, true},
            {"+04", 14400, false},
            {"+04", 14400, false},
            {"+05", 18000, true},
            {"+04", 14400, true},
            {"+03", 10800, false},
            {"+04", 14400, true},
            {"+04", 14400, false},
        }};

        inline constexpr array<zone::transition, 52> transitions{{
            {1, -2840151551},
            {2, -1441162751},
            {4, -405140400},
            {3, 354916800},
            {4, 370724400},
            {3, 386452800},
            {4, 402260400},
            {3, 417988800},
            {4, 433796400},
            {3, 449611200},
            {5, 465343200},
            {6, 481068000},
            {5, 496792800},
            {6, 512517600},
            {5, 528242400},
            {6, 543967200},
            {5, 559692000},
            {6, 575416800},
            {5, 591141600},
            {6, 606866400},
            {5, 622591200},
            {6, 638316000},
            {5, 654645600},
            {7, 670370400},
            {8, 686098800},
            {9, 701816400},
            {2, 717537600},
            {9, 733266000},
            {2, 748987200},
            {9, 764715600},
            {4, 780436800},
            {3, 796161600},
            {4, 811882800},
            {3, 828216000},
            {3, 859662000},
            {4, 877806000},
            {3, 891115200},
            {4, 909255600},
            {3, 922564800},
            {4, 941310000},
            {3, 954014400},
            {4, 972759600},
            {3, 985464000},
            {4, 1004209200},
            {3, 1017518400},
            {4, 1035658800},
            {3, 1048968000},
            {4, 1067108400},
            {3, 1080417600},
            {9, 1088276400},
            {8, 1099177200},
            {4, 1111878000},
        }};
    }

    inline constexpr db::entry tbilisi{"Asia/Tbilisi",
                                       "",
                                       "+4143+04449",
                                       detail::tbilisi::country_codes,
                                       detail::tbilisi::offsets,
                                       detail::tbilisi::transitions};

    // clang-format on
}
