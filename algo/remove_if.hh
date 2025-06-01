// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Remove elements matching predicate

#pragma once

#include "snn-core/algo/remove_if.fwd.hh"
#include <algorithm> // remove_if

namespace snn::algo
{
    // ## Functions

    // ### remove_if

    template <forward_range ForwardRng, typename OneArgPred>
        requires legacy_iterable<ForwardRng> && constructible_from_iterators<ForwardRng>
    [[nodiscard]] constexpr ForwardRng remove_if(ForwardRng rng, OneArgPred p)
    {
        const auto first      = rng.begin();
        const auto last       = rng.end();
        const auto last_after = std::remove_if(first, last, std::move(p));
        return ForwardRng{meta::iterators, first, last_after};
    }
}
