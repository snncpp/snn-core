// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Common functions (min/max)

#pragma once

#include "snn-core/fn/common.hh"

namespace snn::cmp
{
    // ## Functions

    // ### max

    // Returns a reference to the greater of two arguments.
    // Arguments that are temporaries (rvalues) will not compile.

    template <typename A, typename B, typename TwoArgPred = fn::less_than>
    [[nodiscard]] constexpr decltype(auto) max(
        A&& a, B&& b, TwoArgPred is_less = TwoArgPred{}) noexcept(noexcept(is_less(a, b)))
    {
        return fn::max{std::move(is_less)}(std::forward<A>(a), std::forward<B>(b));
    }

    // ### min

    // Returns a reference to the smaller of two arguments.
    // Arguments that are temporaries (rvalues) will not compile.

    template <typename A, typename B, typename TwoArgPred = fn::less_than>
    [[nodiscard]] constexpr decltype(auto) min(
        A&& a, B&& b, TwoArgPred is_less = TwoArgPred{}) noexcept(noexcept(is_less(b, a)))
    {
        return fn::min{std::move(is_less)}(std::forward<A>(a), std::forward<B>(b));
    }
}
