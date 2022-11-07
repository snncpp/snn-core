// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Unsorted map

#pragma once

#include "snn-core/fn/common.hh"
#include "snn-core/map/facade.hh"
#include <unordered_map> // unordered_map

namespace snn::map
{
    // ## Aliases

    // ### unsorted

    template <typename Key, typename Value, typename Hash = std::hash<Key>,
              typename KeyEqual = fn::equal_to>
    using unsorted = facade<std::unordered_map<Key, Value, Hash, KeyEqual>>;
}
