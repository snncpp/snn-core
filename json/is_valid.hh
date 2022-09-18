// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is valid

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/json/decoder.hh"

namespace snn::json
{
    // ## Functions

    // ### is_valid

    [[nodiscard]] constexpr bool is_valid(const cstrview string,
                                          const u16 depth_limit = decoder::default_depth_limit,
                                          const u32 node_limit  = decoder::default_node_limit)
    {
        str buf{string};
        decoder d;
        d.set_depth_limit(depth_limit);
        d.set_node_limit(node_limit);
        return d.decode_inplace(buf.range()).has_value();
    }
}
