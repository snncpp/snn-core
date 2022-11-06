// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Common generic/integral functions

#pragma once

#include "snn-core/math/fn/common.hh"
#include "snn-core/num/bounded.hh"

namespace snn::math
{
    // ## Constants

    // ### base

    enum class base : u8
    {
        binary  = 2,
        octal   = 8,
        decimal = 10,
        hex     = 16,
    };

    // ## Generic

    // For all arithmetic types.

    // ### clamp

    template <arithmetic Num>
    [[nodiscard]] constexpr Num clamp(const Num n, const not_deduced_t<Num> min,
                                      const not_deduced_t<Num> max) noexcept
    {
        snn_should(min <= max);
        if (n < min)
        {
            return min;
        }
        if (n > max)
        {
            return max;
        }
        return n;
    }

    // ### max

    template <arithmetic Num>
    [[nodiscard]] constexpr Num max(const Num a) noexcept
    {
        return a;
    }

    template <arithmetic Num>
    [[nodiscard]] constexpr Num max(const Num a, const not_deduced_t<Num> b) noexcept
    {
        return (a < b) ? b : a;
    }

    template <arithmetic Num, same_as<Num>... Nums>
    [[nodiscard]] constexpr Num max(const Num a, const not_deduced_t<Num> b,
                                    const Nums... nums) noexcept
    {
        return max(max(a, b), nums...);
    }

    // ### min

    template <arithmetic Num>
    [[nodiscard]] constexpr Num min(const Num a) noexcept
    {
        return a;
    }

    template <arithmetic Num>
    [[nodiscard]] constexpr Num min(const Num a, const not_deduced_t<Num> b) noexcept
    {
        return (a < b) ? a : b;
    }

    template <arithmetic Num, same_as<Num>... Nums>
    [[nodiscard]] constexpr Num min(const Num a, const not_deduced_t<Num> b,
                                    const Nums... nums) noexcept
    {
        return min(min(a, b), nums...);
    }

    // ## Integral

    // For integral types only.

    namespace detail
    {
        inline constexpr u64 special_pow10_lookup[20] = {
            0u, // Special case for count_digits_with_clz(), must be 0.
            10u,
            100u,
            1000u,
            10000u,
            100000u,
            1000000u,
            10000000u,
            100000000u,
            1000000000u,
            10000000000u,
            100000000000u,
            1000000000000u,
            10000000000000u,
            100000000000000u,
            1000000000000000u,
            10000000000000000u,
            100000000000000000u,
            1000000000000000000u,
            10000000000000000000u,
        };

        constexpr not_zero<usize> count_digits_with_clz(u64 n) noexcept
        {
            // Slightly modified bth-clz from:
            // https://github.com/localvoid/cxx-benchmark-count-digits
            // Which is based on code that is in the public domain:
            // http://graphics.stanford.edu/~seander/bithacks.html#IntegerLog10

            // https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
            // int __builtin_clzll (unsigned long long x)
            // "Returns the number of leading 0-bits in x, starting at the most significant bit
            // position. If x is 0, the result is undefined."

            static_assert(sizeof(u64) == 8);

            // First step: 0-63
            const u32 leading_zero_bit_count = static_cast<u32>(__builtin_clzll(n | 1u));
            // Second step: 0-19
            const u32 approx = ((64u - leading_zero_bit_count) * 1233u) >> 12u;
            // Third step: 1-20
            const u32 exact = (approx - static_cast<u32>(n < special_pow10_lookup[approx])) + 1u;

            return not_zero<usize>{exact};
        }
    }

    // ### add/multiply/negate/subtract_with_overflow

    // No integral promotion/conversion takes place.

    template <strict_unsigned_integral UInt>
    [[nodiscard]] constexpr UInt add_with_overflow(const UInt a,
                                                   const not_deduced_t<UInt> b) noexcept
    {
        return fn::add_with_overflow{}(a, b);
    }

    template <strict_unsigned_integral UInt>
    [[nodiscard]] constexpr UInt multiply_with_overflow(const UInt a,
                                                        const not_deduced_t<UInt> b) noexcept
    {
        return fn::multiply_with_overflow{}(a, b);
    }

    template <strict_unsigned_integral UInt>
    [[nodiscard]] constexpr UInt negate_with_overflow(const UInt i) noexcept
    {
        return fn::negate_with_overflow{}(i);
    }

    template <strict_unsigned_integral UInt>
    [[nodiscard]] constexpr UInt subtract_with_overflow(const UInt a,
                                                        const not_deduced_t<UInt> b) noexcept
    {
        return fn::subtract_with_overflow{}(a, b);
    }

    // ### add/multiply/subtract_with_saturation

    // No integral promotion/conversion takes place.

    template <strict_unsigned_integral UInt>
    [[nodiscard]] constexpr UInt add_with_saturation(const UInt a,
                                                     const not_deduced_t<UInt> b) noexcept
    {
        return fn::add_with_saturation{}(a, b);
    }

    template <strict_unsigned_integral UInt>
    [[nodiscard]] constexpr UInt multiply_with_saturation(const UInt a,
                                                          const not_deduced_t<UInt> b) noexcept
    {
        return fn::multiply_with_saturation{}(a, b);
    }

    template <strict_unsigned_integral UInt>
    [[nodiscard]] constexpr UInt subtract_with_saturation(const UInt a,
                                                          const not_deduced_t<UInt> b) noexcept
    {
        return fn::subtract_with_saturation{}(a, b);
    }

    // ### abs

    template <strict_signed_integral SInt>
    [[nodiscard]] constexpr auto abs(const SInt i) noexcept
    {
        using UInt = std::make_unsigned_t<SInt>;
        if (i < 0)
        {
            return negate_with_overflow(static_cast<UInt>(i));
        }
        return static_cast<UInt>(i);
    }

    template <strict_unsigned_integral UInt>
    [[nodiscard]] constexpr UInt abs(const UInt i) noexcept
    {
        return i;
    }

    // ### abs_diff

    template <strict_integral Int>
    [[nodiscard]] constexpr std::make_unsigned_t<Int> abs_diff(const Int a,
                                                               const not_deduced_t<Int> b) noexcept
    {
        using UInt = std::make_unsigned_t<Int>;
        if (a < b)
        {
            return subtract_with_overflow(static_cast<UInt>(b), static_cast<UInt>(a));
        }
        else
        {
            return subtract_with_overflow(static_cast<UInt>(a), static_cast<UInt>(b));
        }
    }

    // ### clamp

    // Clamp an integral value of type `FromInt` within the min-max bounds of `ToInt` (inclusive).

    template <strict_integral ToInt, strict_integral FromInt>
    [[nodiscard]] constexpr ToInt clamp(const FromInt i) noexcept
    {
        if (math::fn::less_than{}(i, constant::limit<ToInt>::min))
        {
            return constant::limit<ToInt>::min;
        }

        if (math::fn::less_than{}(constant::limit<ToInt>::max, i))
        {
            return constant::limit<ToInt>::max;
        }

        return static_cast<ToInt>(i);
    }

    // ### count_digits

    template <base Base, strict_unsigned_integral UInt>
    [[nodiscard]] constexpr not_zero<usize> count_digits(UInt n) noexcept
    {
        usize count = 0;
        do
        {
            ++count;
            n /= static_cast<u8>(Base);
        } while (n);
        return not_zero{count};
    }

    template <>
    [[nodiscard]] constexpr not_zero<usize> count_digits<math::base::decimal, u8>(u8 n) noexcept
    {
        if (n < 10)
        {
            return not_zero<usize>{1};
        }
        if (n < 100)
        {
            return not_zero<usize>{2};
        }
        return not_zero<usize>{3};
    }

    template <>
    [[nodiscard]] constexpr not_zero<usize> count_digits<math::base::decimal, u16>(u16 n) noexcept
    {
        if (n < 10)
        {
            return not_zero<usize>{1};
        }
        if (n < 100)
        {
            return not_zero<usize>{2};
        }
        if (n < 1000)
        {
            return not_zero<usize>{3};
        }
        if (n < 10000)
        {
            return not_zero<usize>{4};
        }
        return not_zero<usize>{5};
    }

    template <>
    [[nodiscard]] constexpr not_zero<usize> count_digits<math::base::decimal, u32>(u32 n) noexcept
    {
        return detail::count_digits_with_clz(n);
    }

    template <>
    [[nodiscard]] constexpr not_zero<usize> count_digits<math::base::decimal, u64>(u64 n) noexcept
    {
        return detail::count_digits_with_clz(n);
    }

#if SNN_INT128_BOOL
    template <>
    [[nodiscard]] constexpr not_zero<usize> count_digits<math::base::decimal, u128>(u128 n) noexcept
    {
        // This code is based on the talk:
        // "Three Optimization Tips for C++" by Andrei Alexandrescu.

        usize count = 1;
        while (true)
        {
            if (n < 10)
            {
                return not_zero{count};
            }
            if (n < 100)
            {
                return not_zero{count + 1};
            }
            if (n < 1000)
            {
                return not_zero{count + 2};
            }
            if (n < 10000)
            {
                return not_zero{count + 3};
            }

            n /= 10000;
            count += 4;
        }
    }
#endif

    // ### max_digits

    template <strict_integral Int, base Base>
    [[nodiscard]] constexpr not_zero<usize> max_digits() noexcept
    {
        constexpr usize bit_count = sizeof(Int) * constant::bits_per_byte;

        if constexpr (Base == math::base::decimal)
        {
            constexpr bool is_signed = std::is_signed_v<Int>;
            return not_zero<usize>{(((bit_count - usize{is_signed}) * 3) / 10) + 1};
        }
        else if constexpr (Base == math::base::hex)
        {
            return not_zero<usize>{bit_count / 4};
        }
        else if constexpr (Base == math::base::binary)
        {
            return not_zero<usize>{bit_count};
        }
        else if constexpr (Base == math::base::octal)
        {
            return not_zero<usize>{(bit_count / 3) + 1};
        }
        else
        {
            static_assert(meta::always_false<decltype(Base)>, "Unsupported base.");
        }
    }

    // ### is_power_of_two

    template <strict_unsigned_integral UInt>
    [[nodiscard]] constexpr bool is_power_of_two(const UInt n) noexcept
    {
        // Based on code that is in the public domain:
        // https://graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
        return n > 0 && (n & (n - 1)) == 0;
    }

    // ### is_within_bounds

    // Is an integral value of type `FromInt` within the min-max bounds of `ToInt` (inclusive).

    template <strict_integral ToInt, strict_integral FromInt>
    [[nodiscard]] constexpr bool is_within_bounds(const FromInt i) noexcept
    {
        return fn::greater_than_or_equal_to{}(i, constant::limit<ToInt>::min) &&
               fn::less_than_or_equal_to{}(i, constant::limit<ToInt>::max);
    }

    // ### pow10

    [[nodiscard]] constexpr not_zero<u64> pow10(const num::bounded<usize, 0, 19> n) noexcept
    {
        const u64 i = detail::special_pow10_lookup[n.get()];
        return not_zero<u64>{i > 0 ? i : 1};
    }

    // ### round_up_to_multiple

    // Round up to a power of two multiple.
    // If the rounding overflows the the argument is returned unchanged.

    template <usize PowerOfTwoMultiple, strict_unsigned_integral UInt = usize>
        requires power_of_two<PowerOfTwoMultiple>
    [[nodiscard]] constexpr not_zero<UInt> round_up_to_multiple(
        const not_deduced_t<UInt> u) noexcept
    {
        static_assert(PowerOfTwoMultiple < constant::limit<UInt>::max);

        constexpr UInt PowTwo = static_cast<UInt>(PowerOfTwoMultiple);
        const UInt initial    = max(u, PowTwo);

        constexpr UInt PowTwoL = PowTwo - 1;
        const UInt rounded     = add_with_overflow(initial, PowTwoL) & static_cast<UInt>(~PowTwoL);

        return not_zero<UInt>{max(initial, rounded)};
    }

    // ### within_bounds_or

    // If the integral value of type `FromInt` is within the min-max bounds of `ToInt` (inclusive),
    // the value is returned unchanged as `ToInt`, if not the alternative value is returned.

    template <strict_integral ToInt, strict_integral FromInt>
    [[nodiscard]] constexpr ToInt within_bounds_or(const FromInt i,
                                                   const not_deduced_t<ToInt> alt) noexcept
    {
        if (is_within_bounds<ToInt>(i))
        {
            return static_cast<ToInt>(i);
        }
        return alt;
    }
}
