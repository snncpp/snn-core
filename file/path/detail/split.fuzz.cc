// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/path/split.hh"

#include "snn-core/fuzz.hh"
#include "snn-core/fn/common.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        const auto [parts, err] = file::path::split<cstrview>(input).unpack();
        if (err)
        {
            snn_assert((err == error_code{ENAMETOOLONG, system::error_category} ||
                        err == generic::error::unexpected_null_character));
        }
        else
        {
            snn_assert(parts.directory().none(fn::is_zero{}));
            snn_assert(parts.base().none(fn::is_zero{}));
            snn_assert(parts.extension().none(fn::is_zero{}));

            snn_assert(parts.combined() == input);
        }

        return constant::exit::success;
    }
}
