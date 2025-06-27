// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Get entry by name (constexpr)

// `constexpr` lookup of an database entry. Will slow down compilation time if included in more than
// one translation unit. If a `constexpr` version is not required, prefer the `load()` function
// which uses a separate translation unit for all tables.

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/optional.hh"
#include "snn-core/algo/find_greater_than_or_equal_to.hh"
#include "snn-core/ascii/is_equal_icase.hh"
#include "snn-core/ascii/fn/less_than_icase.hh"
#include "snn-core/time/zone/db/entries.hh"
#include "snn-core/time/zone/db/locations.hh"

namespace snn::time::zone::db
{
    // ## Functions

    // ### get

    [[nodiscard]] constexpr optional<const db::entry&> get(const transient<cstrview> name) noexcept
    {
        const cstrview n = name.get();

        const usize index =
            algo::find_greater_than_or_equal_to(db::locations.range(), n,
                                                ascii::fn::less_than_icase{}, assume::is_sorted)
                .value_or_npos();

        if (index < db::entries.count())
        {
            const db::entry* e = db::entries.at(index, assume::within_bounds);
            if (ascii::is_equal_icase(e->name, n))
            {
                return *e;
            }
        }

        return nullopt;
    }
}
