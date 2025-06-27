// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Unit

#pragma once

#include "snn-core/optional.hh"
#include "snn-core/num/safe.hh"
#include "snn-core/time/core.hh"
#include "snn-core/time/duration.hh"
#include "snn-core/time/unit.fwd.hh"

namespace snn::time
{
    // ## Classes

    // ### unit

    template <i64 Num, i64 Den>
    class unit final
    {
      public:
        static_assert((Num == 1 && Den >= 1) || (Num > 1 && Den == 1));

        static constexpr i64 numerator   = Num;
        static constexpr i64 denominator = Den;

        // #### Default constructor

        constexpr unit() noexcept = default;

        // #### Explicit constructors

        constexpr explicit unit(const i64 value) noexcept
            : safe_{value}
        {
        }

        constexpr explicit unit(const time::duration d) noexcept
        {
            i64 sec  = d.seconds();
            i64 nano = d.nanoseconds();

            // Convert/truncate negative durations correctly.
            if (sec < 0 && nano > 0)
            {
                ++sec;
                nano -= 1'000'000'000;
            }

            add(time::seconds{sec});

            if constexpr (is_smaller_than<time::seconds>())
            {
                add(time::nanoseconds{nano});
            }
        }

        // #### Converting constructors

        template <i64 N, i64 D>
        constexpr unit(const unit<N, D> u) noexcept
            : unit{u.template to<unit>()}
        {
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return bool{safe_};
        }

        // #### Value

        [[nodiscard]] constexpr bool has_value() const noexcept
        {
            return safe_.has_value();
        }

        [[nodiscard]] constexpr i64 value() const
        {
            return safe_.value();
        }

        [[nodiscard]] constexpr i64 value(assume::has_value_t) const noexcept
        {
            return safe_.value(assume::has_value);
        }

        [[nodiscard]] constexpr i64 value_or(const i64 alt) const noexcept
        {
            return safe_.value_or(alt);
        }

        // #### Addition

        template <integral I>
        constexpr unit& add(const I i) noexcept
        {
            safe_.add(i);
            return *this;
        }

        constexpr unit& add(const unit other) noexcept
        {
            safe_.add(other.safe_);
            return *this;
        }

        template <typename V>
        constexpr unit& operator+=(const V v) noexcept
        {
            return add(v);
        }

        template <integral I>
        [[nodiscard]] constexpr unit operator+(const I i) const noexcept
        {
            return unit{*this}.add(i);
        }

        template <any_unit Unit>
        [[nodiscard]] constexpr auto operator+(const Unit u) const noexcept
        {
            // Return the smaller unit.
            if constexpr (is_smaller_than<Unit>())
            {
                return unit{*this}.add(u);
            }
            else
            {
                return to<Unit>().add(u);
            }
        }

        constexpr unit& operator++() noexcept
        {
            return add(1);
        }

        [[nodiscard]] constexpr unit operator++(int) noexcept
        {
            unit cpy{*this};
            add(1);
            return cpy;
        }

        // #### Subtraction

        template <integral I>
        constexpr unit& subtract(const I i) noexcept
        {
            safe_.subtract(i);
            return *this;
        }

        constexpr unit& subtract(const unit other) noexcept
        {
            safe_.subtract(other.safe_);
            return *this;
        }

        template <typename V>
        constexpr unit& operator-=(const V v) noexcept
        {
            return subtract(v);
        }

        template <integral I>
        [[nodiscard]] constexpr unit operator-(const I i) const noexcept
        {
            return unit{*this}.subtract(i);
        }

        template <any_unit Unit>
        [[nodiscard]] constexpr auto operator-(const Unit u) const noexcept
        {
            // Return the smaller unit.
            if constexpr (is_smaller_than<Unit>())
            {
                return unit{*this}.subtract(u);
            }
            else
            {
                return to<Unit>().subtract(u);
            }
        }

        constexpr unit& operator--() noexcept
        {
            return subtract(1);
        }

        [[nodiscard]] constexpr unit operator--(int) noexcept
        {
            unit cpy{*this};
            subtract(1);
            return cpy;
        }

        // #### Multiplication

        template <integral I>
        constexpr unit& multiply(const I i) noexcept
        {
            safe_.multiply(i);
            return *this;
        }

        constexpr unit& multiply(const unit other) noexcept
        {
            safe_.multiply(other.safe_);
            return *this;
        }

        template <typename V>
        constexpr unit& operator*=(const V v) noexcept
        {
            return multiply(v);
        }

        template <integral I>
        [[nodiscard]] constexpr unit operator*(const I i) const noexcept
        {
            return unit{*this}.multiply(i);
        }

        template <any_unit Unit>
        [[nodiscard]] constexpr auto operator*(const Unit u) const noexcept
        {
            // Return the smaller unit.
            if constexpr (is_smaller_than<Unit>())
            {
                return unit{*this}.multiply(u);
            }
            else
            {
                return to<Unit>().multiply(u);
            }
        }

        // #### Division

        template <integral I>
        constexpr unit& divide(const I i) noexcept
        {
            safe_.divide(i);
            return *this;
        }

        constexpr unit& divide(const unit other) noexcept
        {
            safe_.divide(other.safe_);
            return *this;
        }

        template <typename V>
        constexpr unit& operator/=(const V v) noexcept
        {
            return divide(v);
        }

        template <integral I>
        [[nodiscard]] constexpr unit operator/(const I i) const noexcept
        {
            return unit{*this}.divide(i);
        }

        template <any_unit Unit>
        [[nodiscard]] constexpr auto operator/(const Unit u) const noexcept
        {
            // Return the smaller unit.
            if constexpr (is_smaller_than<Unit>())
            {
                return unit{*this}.divide(u);
            }
            else
            {
                return to<Unit>().divide(u);
            }
        }

        // #### Modulo operation / Remainder

        template <integral I>
        constexpr unit& modulo(const I i) noexcept
        {
            safe_.modulo(i);
            return *this;
        }

        constexpr unit& modulo(const unit other) noexcept
        {
            safe_.modulo(other.safe_);
            return *this;
        }

        template <typename V>
        constexpr unit& operator%=(const V v) noexcept
        {
            return modulo(v);
        }

        template <integral I>
        [[nodiscard]] constexpr unit operator%(const I i) const noexcept
        {
            return unit{*this}.modulo(i);
        }

        template <any_unit Unit>
        [[nodiscard]] constexpr auto operator%(const Unit u) const noexcept
        {
            // Return the smaller unit.
            if constexpr (is_smaller_than<Unit>())
            {
                return unit{*this}.modulo(u);
            }
            else
            {
                return to<Unit>().modulo(u);
            }
        }

        // #### Conversion

        [[nodiscard]] constexpr optional<time::duration> duration() const noexcept
        {
            if (safe_.has_value())
            {
                const i64 v = safe_.value(assume::has_value);

                if constexpr (Num == 1 && Den == 1)
                {
                    // Seconds
                    return time::duration{v};
                }
                else if constexpr (is_smaller_than<seconds>())
                {
                    // Smaller than seconds.
                    static_assert(Den > 1 && Den < constant::limit<i32>::max);
                    // These calculations can't overflow.
                    i64 sec  = v / Den;
                    i32 nano = static_cast<i32>(v % Den) * (1'000'000'000 / i32{Den});
                    if (nano < 0)
                    {
                        --sec;
                        nano += 1'000'000'000;
                    }
                    return time::duration{sec, to_u32(nano), promise::is_valid};
                }
                else
                {
                    // Larger than seconds.
                    i64 sec = 0;
                    if (!__builtin_mul_overflow(v, Num, &sec))
                    {
                        return time::duration{sec};
                    }
                }
            }
            return nullopt;
        }

        template <any_unit Unit>
        [[nodiscard]] constexpr Unit to() const noexcept
        {
            constexpr i64 OtherNum = Unit::numerator;
            constexpr i64 OtherDen = Unit::denominator;

            if constexpr (Num == OtherNum && Den == OtherDen)
            {
                // Same unit.
                return Unit{*this};
            }
            else if constexpr (is_smaller_than<Unit>())
            {
                // Smaller unit (this) to larger unit, e.g. seconds to hours.
                constexpr i64 divisor = (Den / OtherDen) * (OtherNum / Num);
                return Unit{safe_ / divisor};
            }
            else
            {
                // Larger unit (this) to smaller, e.g. hours to seconds.
                constexpr i64 multiplier = (Num / OtherNum) * (OtherDen / Den);
                return Unit{safe_ * multiplier};
            }
        }

        // #### Comparison

        template <any_unit Unit>
        [[nodiscard]] static constexpr bool is_smaller_than() noexcept
        {
            return Num < Unit::numerator || Den > Unit::denominator;
        }

        constexpr bool operator==(const unit other) const noexcept
        {
            return safe_ == other.safe_;
        }

        template <any_unit Unit>
        constexpr bool operator==(const Unit u) const noexcept
        {
            // Left (this) vs. right (u).
            if (has_value() && u.has_value())
            {
                // Compare smaller units.
                if constexpr (is_smaller_than<Unit>())
                {
                    // Left is smaller, convert right.
                    const i64 left_value     = value(assume::has_value);
                    const auto right_smaller = u.template to<unit>();
                    if (right_smaller.has_value())
                    {
                        return left_value == right_smaller.value(assume::has_value);
                    }
                    return false; // Overflow, can't be equal.
                }
                else
                {
                    // Right is smaller, convert left.
                    const i64 right_value   = u.value(assume::has_value);
                    const auto left_smaller = to<Unit>();
                    if (left_smaller.has_value())
                    {
                        return left_smaller.value(assume::has_value) == right_value;
                    }
                    return false; // Overflow, can't be equal.
                }
            }
            return has_value() == u.has_value();
        }

        constexpr std::strong_ordering operator<=>(const unit other) const noexcept
        {
            return safe_ <=> other.safe_;
        }

        template <any_unit Unit>
        constexpr std::strong_ordering operator<=>(const Unit u) const noexcept
        {
            // Left (this) vs. right (u).
            if (has_value() && u.has_value())
            {
                // Compare smaller units.
                if constexpr (is_smaller_than<Unit>())
                {
                    // Left is smaller, convert right.
                    const i64 left_value     = value(assume::has_value);
                    const auto right_smaller = u.template to<unit>();
                    if (right_smaller.has_value())
                    {
                        return left_value <=> right_smaller.value(assume::has_value);
                    }
                    return std::strong_ordering::less; // Right overflowed, left must be less.
                }
                else
                {
                    // Right is smaller, convert left.
                    const i64 right_value   = u.value(assume::has_value);
                    const auto left_smaller = to<Unit>();
                    if (left_smaller.has_value())
                    {
                        return left_smaller.value(assume::has_value) <=> right_value;
                    }
                    return std::strong_ordering::greater; // Left overflowed, left must be greater.
                }
            }
            return has_value() <=> u.has_value();
        }

        constexpr bool operator==(const i64 i) const noexcept
        {
            return safe_ == i;
        }

        constexpr std::strong_ordering operator<=>(const i64 i) const noexcept
        {
            return safe_ <=> i;
        }

      private:
        num::safe<i64> safe_;

        template <i64, i64>
        friend class unit;

        constexpr explicit unit(const num::safe<i64> s) noexcept
            : safe_{s}
        {
        }
    };
}
