// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Time zone database entry

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/country/code.hh"
#include "snn-core/time/zone/offset.hh"
#include "snn-core/time/zone/transition.hh"

namespace snn::time::zone
{
    // Forward declare.
    class location;
}

namespace snn::time::zone::db
{
    // ## Classes

    // ### entry

    struct entry final
    {
        cstrview name; // Must not be empty and must be valid.
        cstrview comment;
        cstrview coordinates;

        array_view<const country::code> country_codes;
        array_view<const zone::offset> offsets; // Must not be empty.
        array_view<const zone::transition> transitions;

        template <typename Location = time::zone::location>
        [[nodiscard]] constexpr Location location() const noexcept
        {
            return Location{*this};
        }
    };
}
