// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/zone/location.hh"

#include "snn-core/fuzz.hh"
#include "snn-core/time/zone/db/utc.hh"
#include "snn-core/time/zone/db/america/los_angeles.hh"
#include "snn-core/time/zone/db/europe/stockholm.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        if (input.size() < 150) // Arbitrary
        {
            auto utc   = time::zone::db::utc.location();
            auto la    = time::zone::db::america::los_angeles.location();
            auto sthlm = time::zone::db::europe::stockholm.location();

            auto rng = input.range();

            for (u64 u{}; rng.drop_front_read(u);)
            {
                auto offset = utc.offset(to_i64(u));
                offset      = la.offset(to_i64(u));
                offset      = sthlm.offset(to_i64(u));
                ignore_if_unused(offset);
            }

            for (u32 u{}; rng.drop_front_read(u);)
            {
                auto offset = utc.offset(to_i64(u));
                offset      = la.offset(to_i64(u));
                offset      = sthlm.offset(to_i64(u));
                ignore_if_unused(offset);
            }

            for (u8 u{}; rng.drop_front_read(u);)
            {
                auto offset = utc.offset(to_i64(u));
                offset      = la.offset(to_i64(u));
                offset      = sthlm.offset(to_i64(u));
                ignore_if_unused(offset);
            }
        }

        return constant::exit::success;
    }
}
