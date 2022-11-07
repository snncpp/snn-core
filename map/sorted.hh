// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Sorted map

#pragma once

#include "snn-core/fn/common.hh"
#include "snn-core/map/facade.hh"
#include <map> // map

namespace snn::map
{
    // ## Aliases

    // ### sorted

    template <typename Key, typename Value, typename Compare = fn::less_than>
    using sorted = core<std::map<Key, Value, Compare>>;
}
