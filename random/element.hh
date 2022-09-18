// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Random element from container

#pragma once

#include "snn-core/optional.hh"
#include "snn-core/random/number.hh"

namespace snn::random
{
    // ## Functions

    // ### element

    template <typename Container>
    [[nodiscard]] auto element(Container& container)
        -> optional<decltype(container.at(usize{}, promise::within_bounds))>
    {
        const auto random_index = random::number<usize>(0, container.count());
        if (random_index < container.count())
        {
            return container.at(random_index, promise::within_bounds);
        }
        snn_should(container.count() == 0);
        return nullopt;
    }

    template <typename Container>
    void element(const Container&&) = delete;
}
