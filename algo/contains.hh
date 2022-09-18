// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Does a range contain another range

#pragma once

#include "snn-core/algo/search.hh"
#include "snn-core/fn/common.hh"

namespace snn::algo
{
    // ## Functions

    // ### contains

    template <forward_range SubjectRng, forward_range NeedleRng, typename TwoArgPred = fn::equal_to>
    [[nodiscard]] constexpr bool contains(SubjectRng subject, NeedleRng needle,
                                          TwoArgPred is_equal = TwoArgPred{})
    {
        if (needle)
        {
            const auto r = algo::search(std::move(subject), std::move(needle), std::move(is_equal));
            return !r.is_empty();
        }
        return true;
    }
}
