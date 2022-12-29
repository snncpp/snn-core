// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Load entry by name (non-constexpr)

// Non-`constexpr` lookup of an database entry. Uses a separate translation unit for all tables.

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/optional.hh"
#include "snn-core/time/zone/db/entry.hh"

namespace snn::time::zone::db
{
    // ## Functions

    // ### load

    [[nodiscard]] optional<const db::entry&> load(transient<cstrview> /* name */) noexcept;
}
