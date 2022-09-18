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
#include "snn-core/time/zone/db/asia/yangon.hh"

namespace snn::time::zone::db::indian
{
    // clang-format off

    namespace detail::cocos
    {
        inline constexpr array<country::code, 1> country_codes{
            "CC",
        };
    }

    inline constexpr db::entry cocos{"Indian/Cocos",
                                     "",
                                     "-1210+09655",
                                     detail::cocos::country_codes,
                                     asia::detail::yangon::offsets,
                                     asia::detail::yangon::transitions};

    // clang-format on
}
