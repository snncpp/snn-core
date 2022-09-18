// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/zone/db/load.hh"

#include "snn-core/time/zone/db/get.hh"

namespace snn::time::zone::db
{
    optional<const db::entry&> load(const transient<cstrview> name) noexcept
    {
        return db::get(name);
    }
}
