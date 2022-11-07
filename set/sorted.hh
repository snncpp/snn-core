// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Sorted set

#pragma once

#include "snn-core/fn/common.hh"
#include "snn-core/set/facade.hh"
#include <set> // set

namespace snn::set
{
    // ## Aliases

    // ### sorted

    template <typename Key, typename Compare = fn::less_than>
    using sorted = core<std::set<Key, Compare>>;
}
