// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/zone/tzif/decode.hh"

#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        vec<time::zone::offset> offsets{init::reserve, 256};
        vec<time::zone::transition> transitions{init::reserve, 2'000};
        str footer;

        const auto res = time::zone::tzif::decode(input.range(), offsets, transitions, footer);
        if (!res)
        {
            snn_assert(res.error_code() == time::error::invalid_tzif_data);
        }

        return constant::exit::success;
    }
}
