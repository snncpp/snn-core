// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Optimal byte size for an allocation

#pragma once

#include "snn-core/math/common.hh"

namespace snn::mem::raw
{
    // ## Functions

    // ### optimal_size

    [[nodiscard]] constexpr not_zero<usize> optimal_size(const not_zero<usize> size) noexcept
    {
        // Round up to multiple of X, where X is the most significant set bit in size shifted two
        // bits to the right.
        // E.g. if size is 234 (0b1110'1010), the most significant set bit is 128 (0b1000'0000),
        // shifted two bits to the right gives 32 (0b0010'0000). So 234 is rounded up to the nearest
        // multiple of 32, which is 256.

        // This matches jemalloc size classes almost perfectly, except that the smallest optimal
        // size returned from this function is 16.

        // https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
        // int __builtin_clzll(unsigned long long x)
        // "Returns the number of leading 0-bits in x, starting at the most significant bit
        // position. If x is 0, the result is undefined."

        static_assert(sizeof(usize) == 8);
        static_assert(sizeof(unsigned long long) == 8);

        constexpr usize min_size         = 64;
        const int leading_zero_bit_count = __builtin_clzll(size.get() | min_size); // 0-57
        const usize power_of_two         = usize{1} << to_usize(61 - leading_zero_bit_count);
        const usize optimal              = (size.get() + (power_of_two - 1)) & ~(power_of_two - 1);
        return not_zero{math::max(size.get(), optimal)}; // In case of overflow.
    }
}
