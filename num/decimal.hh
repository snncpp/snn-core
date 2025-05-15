// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Decimal with a static or a dynamic scale

// Scale is the number of digits to the right of the decimal point.
// Rounds "half away from zero", e.g. 23.5 to 24 and -23.5 to -24.

// Scale is currently limited to 18 (to support an `i64` denominator).

#pragma once

#include "snn-core/exception.hh"
#include "snn-core/optional.hh"
#include "snn-core/strcore.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/fmt/integral.hh"
#include "snn-core/math/common.hh"
#include "snn-core/math/fp/common.hh"
#include "snn-core/num/bounded.hh"
#include "snn-core/num/error.hh"
#include "snn-core/num/safe.hh"

namespace snn::num
{
    // ## Constants

    // ### dynamic_scale

    inline constexpr u16 dynamic_scale = constant::limit<u16>::max;

    // ## Classes

    // ### imprecise

    // Imprecise floating-point wrapper.

    template <floating_point Fp>
    class imprecise final
    {
      public:
        constexpr explicit imprecise(const Fp value) noexcept
            : value_{value}
        {
        }

        [[nodiscard]] constexpr Fp get() const noexcept
        {
            return value_;
        }

      private:
        Fp value_;
    };

    // ### decimal_base

    // Holds a static or a dynamic scale.

    template <u16 Scale>
    class decimal_base
    {
      public:
        static_assert(Scale <= 18);

        [[nodiscard]] constexpr u64 denominator() const noexcept
        {
            return denominator_;
        }

        [[nodiscard]] constexpr u16 scale() const noexcept
        {
            return Scale;
        }

      private:
        static constexpr u64 denominator_ = math::pow10(Scale).get();
    };

    template <>
    class decimal_base<dynamic_scale>
    {
      public:
        constexpr explicit decimal_base(const num::bounded<usize, 0, 18> scale) noexcept
            : scale_{scale}
        {
        }

        [[nodiscard]] constexpr u64 denominator() const noexcept
        {
            return math::pow10(scale_).get();
        }

        [[nodiscard]] constexpr u16 scale() const noexcept
        {
            return scale_.get();
        }

      private:
        num::bounded<u16, 0, 18> scale_;
    };

    // ### decimal

    template <u16 Scale = dynamic_scale, strict_integral Int = i64,
              template <typename> typename Safe = num::safe>
    class decimal final : public decimal_base<Scale>
    {
      public:
        using decimal_base<Scale>::denominator;
        using decimal_base<Scale>::scale;

        constexpr explicit decimal(const not_deduced_t<Int> sub_unit = 0) noexcept
            requires(Scale != dynamic_scale)
            : i_{sub_unit}
        {
        }

        constexpr explicit decimal(const not_deduced_t<Int> sub_unit,
                                   const num::bounded<usize, 0, 18> scale) noexcept
            requires(Scale == dynamic_scale)
            : decimal_base<Scale>{scale},
              i_{sub_unit}
        {
        }

        template <floating_point Fp>
        decimal(const imprecise<Fp> fp)
            requires(Scale != dynamic_scale)
            : i_{}
        {
            init_(fp.get());
        }

        template <floating_point Fp>
        decimal(const imprecise<Fp> fp, const num::bounded<usize, 0, 18> scale)
            requires(Scale == dynamic_scale)
            : decimal_base<Scale>{scale},
              i_{}
        {
            init_(fp.get());
        }

        // Exact, readable, compile-time initialization.
        template <same_as<const char> ConstChar, usize N, usize Size = N - 1>
            requires(Size >= 1)
        consteval decimal(ConstChar (&s)[N])
            requires(Scale != dynamic_scale)
            : i_{}
        {
            init_(s);
        }

        // Exact, readable, compile-time initialization.
        template <same_as<const char> ConstChar, usize N, usize Size = N - 1>
            requires(Size >= 1)
        consteval decimal(ConstChar (&s)[N], const num::bounded<usize, 0, 18> scale)
            requires(Scale == dynamic_scale)
            : decimal_base<Scale>{scale},
              i_{}
        {
            init_(s);
        }

        constexpr explicit operator bool() const noexcept
        {
            return bool{i_};
        }

        [[nodiscard]] constexpr bool has_value() const noexcept
        {
            return i_.has_value();
        }

        [[nodiscard]] constexpr Int value() const
        {
            return i_.value();
        }

        [[nodiscard]] constexpr Int value(promise::has_value_t) const noexcept
        {
            return i_.value(promise::has_value);
        }

        template <typename Buf>
        constexpr void format(const num::bounded<usize, 0, 18> decimal_places,
                              const char decimal_point, const optional<char> thousands_separator,
                              strcore<Buf>& append_to) const
        {
            const auto i = i_.value();
            auto abs     = force_unsigned(i);

            if constexpr (std::is_signed_v<Int>)
            {
                if (i < 0)
                {
                    abs = math::negate_with_overflow(abs);
                    append_to.append('-');
                }
            }

            const u64 initial_denom = denominator();
            const u16 current_scale = scale();

            u64 denom = initial_denom;

            if (decimal_places.get() < current_scale)
            {
                // Use a different denominator.
                denom = math::pow10(decimal_places).get();

                // Calculate a new sub-unit and round (these calculations can't overflow).

                const u64 denom_round = initial_denom / denom;
                snn_should(denom_round >= 10);

                decltype(abs) base      = abs / denom_round;
                const decltype(abs) sub = abs % denom_round;

                if (sub >= (denom_round / 2))
                {
                    ++base;
                }

                abs = base;
            }

            const decltype(abs) base = abs / denom;
            const decltype(abs) sub  = abs % denom;

            if (thousands_separator)
            {
                fmt::integral(base, thousands_separator.value(promise::has_value), append_to);
            }
            else
            {
                // Without thousands separator.
                append_to.append_integral(base);
            }

            if (decimal_places)
            {
                append_to.append(decimal_point);

                if (decimal_places.get() <= current_scale) [[likely]]
                {
                    const usize min_digits = decimal_places.get();
                    append_to.append_integral(sub, min_digits);
                }
                else
                {
                    const usize missing_zero_count = decimal_places.get() - current_scale;

                    if (current_scale > 0)
                    {
                        const usize min_digits = current_scale;
                        append_to.append_integral(sub, min_digits);
                    }

                    append_to.append_for_overwrite(missing_zero_count).fill('0');
                }
            }
        }

        template <any_strcore Str = str>
        [[nodiscard]] constexpr Str format(const num::bounded<usize, 0, 18> decimal_places = 2,
                                           const char decimal_point                        = '.',
                                           const optional<char> thousands_separator = nullopt) const
        {
            Str append_to;
            format(decimal_places, decimal_point, thousands_separator, append_to);
            return append_to;
        }

        constexpr decimal& round() noexcept
        {
            const u64 denom = denominator();
            if (denom > 1)
            {
                round_(denom);
            }
            return *this;
        }

        constexpr decimal& round(const usize decimal_places) noexcept
        {
            if (decimal_places < scale())
            {
                round_(denominator() / math::pow10(decimal_places).get());
            }
            return *this;
        }

        template <floating_point Fp>
        [[nodiscard]] constexpr optional<Fp> to() const noexcept
        {
            if (i_.has_value())
            {
                const auto i = i_.value(promise::has_value);
                if (math::fp::is_within_bounds<Fp>(i))
                {
                    const u64 denom = denominator();
                    if (math::fp::is_within_bounds<Fp>(denom))
                    {
                        return static_cast<Fp>(i) / static_cast<Fp>(denom);
                    }
                }
            }
            return nullopt;
        }

        // Addition

        constexpr decimal& operator+=(const decimal other) noexcept
        {
            i_ += other.i_;
            return *this;
        }

        [[nodiscard]] constexpr decimal operator+(const decimal other) const noexcept
        {
            decimal cpy{*this};
            cpy.i_ += other.i_;
            return cpy;
        }

        // Subtraction

        constexpr decimal& operator-=(const decimal other) noexcept
        {
            i_ -= other.i_;
            return *this;
        }

        [[nodiscard]] constexpr decimal operator-(const decimal other) const noexcept
        {
            decimal cpy{*this};
            cpy.i_ -= other.i_;
            return cpy;
        }

        // Multiplication

        constexpr decimal& operator*=(const decimal other) noexcept
        {
            const u64 denom = denominator();
            i_ *= other.i_;
            if (i_ < 0)
            {
                i_ -= denom / 2;
            }
            else
            {
                i_ += denom / 2;
            }
            i_ /= static_cast<compatible_denom_type>(denom);
            return *this;
        }

        template <u16 S, typename I, template <typename> typename SafeOther>
        constexpr decimal& operator*=(const decimal<S, I, SafeOther> other) noexcept
        {
            const u64 other_denom = other.denominator();
            i_ *= other.i_;
            if (i_ < 0)
            {
                i_ -= other_denom / 2;
            }
            else
            {
                i_ += other_denom / 2;
            }
            i_ /= static_cast<compatible_denom_type>(other_denom);
            return *this;
        }

        [[nodiscard]] constexpr decimal operator*(const decimal other) const noexcept
        {
            decimal cpy{*this};
            cpy *= other;
            return cpy;
        }

        template <u16 S, typename I, template <typename> typename SafeOther>
        [[nodiscard]] constexpr decimal operator*(
            const decimal<S, I, SafeOther> other) const noexcept
        {
            decimal cpy{*this};
            cpy *= other;
            return cpy;
        }

        // Comparison

        constexpr bool operator==(const decimal other) const noexcept
        {
            return i_ == other.i_;
        }

        constexpr std::strong_ordering operator<=>(const decimal other) const noexcept
        {
            return i_ <=> other.i_;
        }

      private:
        template <u16, strict_integral, template <typename> typename>
        friend class decimal;

        // Can't divide by an unsigned denominator if `Int` is signed.
        using compatible_denom_type = std::conditional_t<std::is_signed_v<Int>, i64, u64>;

        Safe<Int> i_;

        template <usize N, usize Size = N - 1>
        consteval void init_(const char (&s)[N])
        {
            Safe<Int> sub;

            usize index    = 0;
            bool is_signed = false;

            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            if constexpr (std::is_signed_v<Int>)
            {
                if (s[index] == '-')
                {
                    is_signed = true;
                    ++index;
                }
            }

            while (index < Size && chr::is_digit(s[index]))
            {
                sub *= 10;
                sub += chr::decode_digit(s[index]);
                ++index;
            }

            const u64 denom = denominator();

            sub *= denom;

            if (index < Size && s[index] == '.')
            {
                ++index;
                u64 multiplier = denom / 10;
                while (index < Size && chr::is_digit(s[index]))
                {
                    if (multiplier == 0)
                    {
                        throw_or_abort(error::out_of_range);
                    }
                    // Multiplication can't overflow:
                    sub += multiplier * chr::decode_digit(s[index]);
                    multiplier /= 10;
                    ++index;
                }
            }

            SNN_DIAGNOSTIC_POP

            if (index != Size)
            {
                // Trailing characters that are not digits.
                throw_or_abort(error::invalid_character);
            }

            if constexpr (std::is_signed_v<Int>)
            {
                if (is_signed)
                {
                    sub *= -1;
                }
            }

            i_ = Safe<Int>{sub.value()};
        }

        template <floating_point Fp>
        void init_(const Fp fp)
        {
            const u64 denom = denominator();

            if (!math::fp::is_within_bounds<Fp>(denom))
            {
                throw_or_abort(error::precision_loss);
            }

            const Fp abs_rounded = math::fp::round(math::fp::abs(fp) * static_cast<Fp>(denom));

            if (!math::fp::is_within_bounds<Int>(abs_rounded))
            {
                throw_or_abort(error::precision_loss);
            }

            if (math::fp::is_negative(fp))
            {
                i_ -= static_cast<Int>(abs_rounded);
            }
            else
            {
                i_ += static_cast<Int>(abs_rounded);
            }
        }

        constexpr void round_(const u64 denom) noexcept
        {
            if (i_ < 0)
            {
                i_ -= denom / 2;
            }
            else
            {
                i_ += denom / 2;
            }
            i_ /= static_cast<compatible_denom_type>(denom);
            i_ *= denom;
        }
    };
}
