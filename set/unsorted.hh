// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Unsorted set

#pragma once

#include "snn-core/fn/common.hh"
#include "snn-core/set/core.hh"
#include <unordered_set> // unordered_set

namespace snn::set
{
    // ## Aliases

    // ### unsorted

    template <typename Key, typename Hash = std::hash<Key>, typename KeyEqual = fn::equal_to>
    using unsorted = core<std::unordered_set<Key, Hash, KeyEqual>>;
}
