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
#include "snn-core/time/zone/db/pacific/port_moresby.hh"

namespace snn::time::zone::db::antarctica
{
    // clang-format off

    namespace detail::dumontdurville
    {
        inline constexpr array<country::code, 1> country_codes{
            "AQ",
        };
    }

    inline constexpr db::entry dumontdurville{"Antarctica/DumontDUrville",
                                              "Dumont-d'Urville",
                                              "-6640+14001",
                                              detail::dumontdurville::country_codes,
                                              pacific::detail::port_moresby::offsets,
                                              pacific::detail::port_moresby::transitions};

    // clang-format on
}
