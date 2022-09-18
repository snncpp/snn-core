// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Common function objects

#pragma once

#include "snn-core/core.hh"

namespace snn::math::fn
{
    // ## Arithmetic

    // ### With overflow

    // No integral promotion/conversion takes place.

    // clang-format off

    struct add_with_overflow final
    {
        explicit add_with_overflow() = default;

        template <strict_unsigned_integral UInt>
        [[nodiscard]] __attribute__((__no_sanitize__("unsigned-integer-overflow")))
        constexpr UInt operator()(const UInt a, const not_deduced_t<UInt> b) const noexcept
        {
            return static_cast<UInt>(a + b);
        }
    };

    struct multiply_with_overflow final
    {
        explicit multiply_with_overflow() = default;

        template <strict_unsigned_integral UInt>
        [[nodiscard]] __attribute__((__no_sanitize__("unsigned-integer-overflow")))
        constexpr UInt operator()(const UInt a, const not_deduced_t<UInt> b) const noexcept
        {
            return static_cast<UInt>(a * b);
        }
    };

    struct negate_with_overflow final
    {
        explicit negate_with_overflow() = default;

        template <strict_unsigned_integral UInt>
        [[nodiscard]] __attribute__((__no_sanitize__("unsigned-integer-overflow")))
        constexpr UInt operator()(const UInt i) const noexcept
        {
            return static_cast<UInt>(-i);
        }
    };

    struct subtract_with_overflow final
    {
        explicit subtract_with_overflow() = default;

        template <strict_unsigned_integral UInt>
        [[nodiscard]] __attribute__((__no_sanitize__("unsigned-integer-overflow")))
        constexpr UInt operator()(const UInt a, const not_deduced_t<UInt> b) const noexcept
        {
            return static_cast<UInt>(a - b);
        }
    };

    // clang-format on

    // ### With saturation

    // No integral promotion/conversion takes place.

    struct add_with_saturation final
    {
        explicit add_with_saturation() = default;

        template <strict_unsigned_integral UInt>
        [[nodiscard]] constexpr UInt operator()(const UInt a,
                                                const not_deduced_t<UInt> b) const noexcept
        {
            UInt sum = 0;
            if (__builtin_add_overflow(a, b, &sum))
            {
                sum = constant::limit<UInt>::max;
            }
            return sum;
        }
    };

    struct multiply_with_saturation final
    {
        explicit multiply_with_saturation() = default;

        template <strict_unsigned_integral UInt>
        [[nodiscard]] constexpr UInt operator()(const UInt a,
                                                const not_deduced_t<UInt> b) const noexcept
        {
            UInt prod = 0;
            if (__builtin_mul_overflow(a, b, &prod))
            {
                prod = constant::limit<UInt>::max;
            }
            return prod;
        }
    };

    struct subtract_with_saturation final
    {
        explicit subtract_with_saturation() = default;

        template <strict_unsigned_integral UInt>
        [[nodiscard]] constexpr UInt operator()(const UInt a,
                                                const not_deduced_t<UInt> b) const noexcept
        {
            UInt diff = 0;
            if (__builtin_sub_overflow(a, b, &diff))
            {
                diff = 0;
            }
            return diff;
        }
    };

    // ## Comparison

    // Safe comparison of any combination of signed and unsigned integrals.

    struct equal_to final
    {
        explicit equal_to() = default;

        template <strict_integral A, strict_integral B>
        [[nodiscard]] constexpr bool operator()(const A a, const B b) const noexcept
        {
            if constexpr (same_signedness_as<A, B>)
            {
                return a == b;
            }
            else if constexpr (std::is_signed_v<A>)
            {
                // A is signed, B is unsigned.
                return a >= 0 && static_cast<std::make_unsigned_t<A>>(a) == b;
            }
            else
            {
                // B is signed, A is unsigned.
                return b >= 0 && a == static_cast<std::make_unsigned_t<B>>(b);
            }
        }
    };

    struct not_equal_to final
    {
        explicit not_equal_to() = default;

        template <strict_integral A, strict_integral B>
        [[nodiscard]] constexpr bool operator()(const A a, const B b) const noexcept
        {
            return !math::fn::equal_to{}(a, b);
        }
    };

    struct less_than final
    {
        explicit less_than() = default;

        template <strict_integral A, strict_integral B>
        [[nodiscard]] constexpr bool operator()(const A a, const B b) const noexcept
        {
            if constexpr (same_signedness_as<A, B>)
            {
                return a < b;
            }
            else if constexpr (std::is_signed_v<A>)
            {
                // A is signed, B is unsigned (min: 0).
                return a < 0 || static_cast<std::make_unsigned_t<A>>(a) < b;
            }
            else
            {
                // B is signed, A is unsigned (min: 0).
                return b > 0 && a < static_cast<std::make_unsigned_t<B>>(b);
            }
        }
    };

    struct greater_than final
    {
        explicit greater_than() = default;

        template <strict_integral A, strict_integral B>
        [[nodiscard]] constexpr bool operator()(const A a, const B b) const noexcept
        {
            return math::fn::less_than{}(b, a);
        }
    };

    struct less_than_or_equal_to final
    {
        explicit less_than_or_equal_to() = default;

        template <strict_integral A, strict_integral B>
        [[nodiscard]] constexpr bool operator()(const A a, const B b) const noexcept
        {
            return !math::fn::greater_than{}(a, b);
        }
    };

    struct greater_than_or_equal_to final
    {
        explicit greater_than_or_equal_to() = default;

        template <strict_integral A, strict_integral B>
        [[nodiscard]] constexpr bool operator()(const A a, const B b) const noexcept
        {
            return !math::fn::less_than{}(a, b);
        }
    };

    // Idiomatic shorthand versions.

    using eq  = equal_to;
    using gt  = greater_than;
    using gte = greater_than_or_equal_to;
    using lt  = less_than;
    using lte = less_than_or_equal_to;
    using ne  = not_equal_to;

    // ## Filter / Validation

    struct is_even final
    {
        explicit is_even() = default;

        template <integral Int>
        [[nodiscard]] constexpr bool operator()(const Int i) const noexcept
        {
            return (i % 2) == 0;
        }
    };

    struct is_odd final
    {
        explicit is_odd() = default;

        template <integral Int>
        [[nodiscard]] constexpr bool operator()(const Int i) const noexcept
        {
            return (i % 2) != 0;
        }
    };
}
