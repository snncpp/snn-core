// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/base64/encode.hh"

#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        const auto with_padding = base64::encode(input);
        snn_assert(with_padding.size() % 4 == 0);

        const auto without_padding = base64::encode(input, base64::without_padding);
        snn_assert(with_padding.has_front(without_padding));

        return constant::exit::success;
    }
}
