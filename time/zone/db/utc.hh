// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # UTC entry

#pragma once

#include "snn-core/array.hh"
#include "snn-core/time/zone/offset.hh"
#include "snn-core/time/zone/db/entry.hh"

namespace snn::time::zone::db
{
    namespace detail::utc
    {
        inline constexpr array<zone::offset, 1> offsets{{
            {"UTC", 0, false},
        }};
    }

    inline constexpr db::entry utc{"UTC", "", "", {}, detail::utc::offsets, {}};
}
