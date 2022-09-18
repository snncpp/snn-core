// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Convert entire range to integral

#pragma once

#include "snn-core/optional.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/math/common.hh"

namespace snn::ascii
{
    // ## Functions

    // ### to_integral (unsigned)

    // The entire character range must be convertible to `UInt`.

    template <strict_unsigned_integral UInt, math::base Base = math::base::decimal, typename Rng>
        requires same_as<front_value_t<Rng&>, char>
    [[nodiscard]] constexpr optional<UInt> to_integral(Rng rng) noexcept
    {
        if (rng && chr::is_base<Base>(rng.front(promise::not_empty)))
        {
            UInt val = chr::decode_base<Base>(rng.front(promise::not_empty)); // Can't overflow.
            rng.drop_front(promise::not_empty);

            if (val || rng.is_empty()) // Not a leading zero or the only digit?
            {
                while (rng)
                {
                    const char c = rng.front(promise::not_empty);
                    rng.drop_front(promise::not_empty);

                    if (!chr::is_base<Base>(c))
                    {
                        return nullopt;
                    }

                    UInt prod = 0;
                    if (__builtin_mul_overflow(val, to_underlying(Base), &prod))
                    {
                        return nullopt;
                    }
                    val = prod;

                    UInt sum = 0;
                    if (__builtin_add_overflow(val, chr::decode_base<Base>(c), &sum))
                    {
                        return nullopt;
                    }
                    val = sum;
                }

                return val;
            }
        }

        return nullopt;
    }

    // ### to_integral (signed)

    // The entire character range must be convertible to `SInt`.

    template <strict_signed_integral SInt, math::base Base = math::base::decimal, typename Rng>
        requires same_as<front_value_t<Rng&>, char>
    [[nodiscard]] constexpr optional<SInt> to_integral(Rng rng) noexcept
    {
        using UInt = std::make_unsigned_t<SInt>;

        if (rng)
        {
            if (rng.front(promise::not_empty) == '-')
            {
                rng.drop_front(promise::not_empty);

                const auto u = to_integral<UInt, Base>(rng).value_or(constant::limit<UInt>::max);

                constexpr UInt max = math::abs(constant::limit<SInt>::min);
                if (u <= max)
                {
                    return static_cast<SInt>(math::negate_with_overflow(u));
                }
            }
            else
            {
                const auto u = to_integral<UInt, Base>(rng).value_or(constant::limit<UInt>::max);

                constexpr UInt max = static_cast<UInt>(constant::limit<SInt>::max);
                if (u <= max)
                {
                    return static_cast<SInt>(u);
                }
            }
        }

        return nullopt;
    }
}
