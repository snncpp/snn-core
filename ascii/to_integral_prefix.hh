// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Range prefix to integral

#pragma once

#include "snn-core/chr/common.hh"
#include "snn-core/math/common.hh"
#include "snn-core/pair/common.hh"

namespace snn::ascii
{
    // ## Options

    enum class leading_zeros : u8
    {
        disallow,
        allow,
    };

    // ## Functions

    // ### to_integral_prefix (unsigned)

    template <strict_unsigned_integral UInt, math::base Base = math::base::decimal,
              usize MaxDigits = 0, typename Rng>
    [[nodiscard]] constexpr pair::value_count<UInt, usize> to_integral_prefix(
        Rng rng, const leading_zeros policy = leading_zeros::disallow) noexcept
    {
        static_assert(std::is_same_v<front_value_t<Rng&>, char>);
        static_assert(MaxDigits < math::max_digits<UInt, Base>().get(), "Max digits can overflow.");

        UInt val    = 0;
        usize count = 0;
        if (rng)
        {
            char c = rng.front(assume::not_empty);
            rng.drop_front(assume::not_empty);
            if (chr::is_base<Base>(c))
            {
                val   = chr::decode_base<Base>(c); // First digit can't overflow.
                count = 1;
                if (val || policy == leading_zeros::allow)
                {
                    if constexpr (MaxDigits > 0)
                    {
                        while (rng && count < MaxDigits)
                        {
                            c = rng.front(assume::not_empty);
                            rng.drop_front(assume::not_empty);

                            if (!chr::is_base<Base>(c))
                            {
                                break;
                            }

                            val *= static_cast<u8>(Base);
                            val += chr::decode_base<Base>(c);

                            ++count;
                        }
                    }
                    else
                    {
                        while (rng)
                        {
                            c = rng.front(assume::not_empty);
                            rng.drop_front(assume::not_empty);

                            if (!chr::is_base<Base>(c))
                            {
                                break;
                            }

                            UInt prod = 0;
                            if (__builtin_mul_overflow(val, static_cast<u8>(Base), &prod))
                            {
                                val   = 0;
                                count = 0;
                                break;
                            }
                            val = prod;

                            UInt sum = 0;
                            if (__builtin_add_overflow(val, chr::decode_base<Base>(c), &sum))
                            {
                                val   = 0;
                                count = 0;
                                break;
                            }
                            val = sum;

                            ++count;
                        }
                    }
                }
            }
        }
        return {val, count};
    }

    // ### to_integral_prefix (signed)

    template <strict_signed_integral SInt, math::base Base = math::base::decimal,
              usize MaxDigits = 0, typename Rng>
    [[nodiscard]] constexpr pair::value_count<SInt, usize> to_integral_prefix(
        Rng rng, const leading_zeros policy = leading_zeros::disallow) noexcept
    {
        static_assert(MaxDigits < math::max_digits<SInt, Base>().get(), "Max digits can overflow.");

        SInt val    = 0;
        usize count = 0;
        if (rng)
        {
            using UInt = std::make_unsigned_t<SInt>;

            constexpr UInt max_positive = static_cast<UInt>(constant::limit<SInt>::max);
            constexpr UInt max_negative = max_positive + 1; // Two's complement.

            bool negative = false;

            if (rng.front(assume::not_empty) == '-')
            {
                rng.drop_front(assume::not_empty);

                negative = true;
            }

            const auto p = to_integral_prefix<UInt, Base, MaxDigits>(std::move(rng), policy);

            if (negative)
            {
                if (p.count && (MaxDigits > 0 || p.value <= max_negative))
                {
                    val   = static_cast<SInt>(math::negate_with_overflow(p.value));
                    count = p.count + 1; // Plus one for '-'.
                }
            }
            else
            {
                if (MaxDigits > 0 || p.value <= max_positive)
                {
                    val   = static_cast<SInt>(p.value);
                    count = p.count;
                }
            }
        }
        return {val, count};
    }
}
