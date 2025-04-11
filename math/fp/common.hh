// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Common functions

#pragma once

#include "snn-core/math/common.hh"
#include <bit>   // bit_cast
#include <cmath> // isinf, round, roundf, roundl

namespace snn::math::fp
{
    // ## Functions

    // For all floating point types.

    // ### abs

    // Compute the absolute value of a floating point value. These functions simply unsets the sign
    // bit and are therefore very fast.

    [[nodiscard]] constexpr double abs(const double d) noexcept
    {
        return __builtin_fabs(d);
    }

    [[nodiscard]] constexpr float abs(const float f) noexcept
    {
        return __builtin_fabsf(f);
    }

    [[nodiscard]] constexpr long double abs(const long double ld) noexcept
    {
        return __builtin_fabsl(ld);
    }

    // ### is_within_bounds

    // Is a floating point value (significant digits) of type `Fp` within the min-max bounds of
    // `Int` (inclusive).

    // If the `Int` type supports it the max values on most systems are:
    // `float`:       2^24 - 1 = 16777216 - 1             = 16777215
    // `double`:      2^53 - 1 = 9007199254740992 - 1     = 9007199254740991
    // `long double`: 2^64 - 1 = 18446744073709551616 - 1 = 18446744073709551615

    // Technically 2^X is the max supported value, but since 2^X + 1 is also stored as 2^X it is not
    // a safe max.

    template <strict_integral Int, floating_point Fp>
    [[nodiscard]] bool is_within_bounds(const Fp fp) noexcept
    {
        static_assert(std::numeric_limits<Fp>::is_iec559); // IEEE 754
        static_assert(std::numeric_limits<Fp>::radix == 2);

        if constexpr (std::is_unsigned_v<Int>)
        {
            // Negative?
            if (__builtin_signbit(fp))
            {
                return false;
            }
        }

        constexpr usize IntBitCount = sizeof(Int) * constant::bits_per_byte;
        // Number of radix digits that can be represented without change:
        constexpr usize SafeBitCount = std::numeric_limits<Fp>::digits;

        if constexpr (IntBitCount > SafeBitCount)
        {
            // Worst case example with a `SafeBitCount` of 63 and `i64` (single bit difference):
            // `(u64{1} << 63) - 1` is 9223372036854775807 and can't overflow positive/negative.

            // Unsigned integral max (use at least `u32` so it won't be promoted to `int`).
            using UIntPromoted = promote_integral_t<std::make_unsigned_t<Int>, 32>;
            constexpr auto max = (UIntPromoted{1} << SafeBitCount) - 1;
            return math::fp::abs(fp) <= max;
        }
        else if constexpr (std::is_unsigned_v<Int>)
        {
            // This is correct even if the bit counts are equal, e.g. `long double` and `u64`.
            // Already checked that the floating point value isn't negative.
            return fp <= constant::limit<Int>::max;
        }
        else
        {
            // This is correct even if the bit counts are equal, e.g. `long double` and `i64`.
            return fp >= constant::limit<Int>::min && fp <= constant::limit<Int>::max;
        }
    }

    // ### is_within_bounds

    // Is an integral value of type `Int` within the min-max bounds of `Fp` (inclusive).

    template <floating_point Fp, strict_integral Int>
    [[nodiscard]] constexpr bool is_within_bounds(const Int i) noexcept
    {
        static_assert(std::numeric_limits<Fp>::is_iec559); // IEEE 754
        static_assert(std::numeric_limits<Fp>::radix == 2);

        constexpr usize IntBitCount = sizeof(Int) * constant::bits_per_byte;
        // Number of radix digits that can be represented without change:
        constexpr usize SafeBitCount = std::numeric_limits<Fp>::digits;

        if constexpr (IntBitCount > SafeBitCount)
        {
            // Unsigned integral max (use at least `u32` so it won't be promoted to `int`).
            using UIntPromoted = promote_integral_t<std::make_unsigned_t<Int>, 32>;
            constexpr auto max = (UIntPromoted{1} << SafeBitCount) - 1;
            return math::abs(i) <= max;
        }
        else
        {
            // This is correct even if the bit counts are equal, e.g. `long double` and `u64`.
            return true;
        }
    }

    // ### is_almost_equal

    template <floating_point Fp>
    [[nodiscard]] constexpr bool is_almost_equal(const Fp a, const not_deduced_t<Fp> b,
                                                 const not_deduced_t<Fp> max_diff) noexcept
    {
        return math::fp::abs(a - b) <= max_diff;
    }

    // ### is_bitwise_equal

    template <floating_point Fp>
    [[nodiscard]] constexpr bool is_bitwise_equal(const Fp a, const not_deduced_t<Fp> b) noexcept
    {
        static_assert(sizeof(Fp) == sizeof(float) || sizeof(Fp) == sizeof(double),
                      "Only standard floating-point types without padding are supported.");
        using UInt = promote_integral_t<u8, sizeof(Fp) * constant::bits_per_byte>;
        return std::bit_cast<UInt>(a) == std::bit_cast<UInt>(b);
    }

    // ### is_infinity

    template <floating_point Fp>
    [[nodiscard]] bool is_infinity(const Fp fp) noexcept
    {
        return std::isinf(fp);
    }

    // ### is_nan (not-a-number)

    template <floating_point Fp>
    [[nodiscard]] constexpr bool is_nan(const Fp fp) noexcept
    {
        return fp != fp; // NaN never compare equal.
    }

    // ### is_negative

    // Returns true if the sign bit is set.

    template <floating_point Fp>
    [[nodiscard]] bool is_negative(const Fp fp) noexcept
    {
        return __builtin_signbit(fp); // Type-generic
    }

    // ### round

    // Rounding halfway cases away from zero.

    [[nodiscard]] inline double round(const double d) noexcept
    {
        return std::round(d);
    }

    [[nodiscard]] inline float round(const float f) noexcept
    {
        return std::roundf(f);
    }

    [[nodiscard]] inline long double round(const long double ld) noexcept
    {
        return std::roundl(ld);
    }
}
