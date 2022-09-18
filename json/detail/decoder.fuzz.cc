// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/json/decoder.hh"

#include "snn-core/fuzz.hh"
#include "snn-core/strcore.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        strbuf buf{input};
        json::decoder d;
        d.set_depth_limit(10);
        d.set_node_limit(1000);
        const auto r = d.decode_inplace(buf.range());
        ignore_if_unused(r);

        return constant::exit::success;
    }
}
