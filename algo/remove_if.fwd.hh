// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Remove elements matching predicate (forward declare)

#pragma once

#include "snn-core/core.hh"

namespace snn::algo
{
    // ## Functions

    // ### remove_if

    template <forward_range ForwardRng, typename OneArgPred>
        requires legacy_iterable<ForwardRng> && constructible_from_iterators<ForwardRng>
    [[nodiscard]] constexpr ForwardRng remove_if(ForwardRng, OneArgPred);
}
