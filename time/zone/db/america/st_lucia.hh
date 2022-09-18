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
#include "snn-core/time/zone/db/america/puerto_rico.hh"

namespace snn::time::zone::db::america
{
    // clang-format off

    namespace detail::st_lucia
    {
        inline constexpr array<country::code, 1> country_codes{
            "LC",
        };
    }

    inline constexpr db::entry st_lucia{"America/St_Lucia",
                                        "",
                                        "+1401-06100",
                                        detail::st_lucia::country_codes,
                                        america::detail::puerto_rico::offsets,
                                        america::detail::puerto_rico::transitions};

    // clang-format on
}
