// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/detail/cityhash64/hash.hh"

#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        const usize h = detail::cityhash64::hash(input.data().get(), input.size());
        ignore_if_unused(h);

        return constant::exit::success;
    }
}
