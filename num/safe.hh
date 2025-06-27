// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Safe integral

#pragma once

#include "snn-core/exception.hh"
#include "snn-core/num/error.hh"

namespace snn::num
{
    // ## Classes

    // ### safe

    template <strict_integral Int>
    class safe final
    {
      public:
        // #### Constructors

        constexpr safe() noexcept = default;

        constexpr safe(const Int i) noexcept
            : i_{i}
        {
        }

        // #### Value assignment

        // Value assignment is error-prone and therefore disabled.
        // The implicit copy/move assignment operator can still be used.

        template <typename U>
        void operator=(const U&) = delete;

        // #### Explicit conversion operators

        // Behave as an integral, but only if it has a value.

        constexpr explicit operator bool() const noexcept
        {
            return has_value_ && i_;
        }

        // #### Value

        [[nodiscard]] constexpr bool has_value() const noexcept
        {
            return has_value_;
        }

        [[nodiscard]] constexpr Int value() const
        {
            if (has_value_)
            {
                return i_;
            }
            throw_or_abort(error::overflow_or_divide_by_zero);
        }

        [[nodiscard]] constexpr Int value(assume::has_value_t) const noexcept
        {
            snn_assert(has_value_);
            return i_;
        }

        [[nodiscard]] constexpr Int value_or(const Int alt) const noexcept
        {
            if (has_value_)
            {
                return i_;
            }
            return alt;
        }

        // #### Addition

        template <strict_integral I>
        constexpr safe& add(const I i) noexcept
        {
            Int sum = 0;
            if (__builtin_add_overflow(i_, i, &sum))
            {
                has_value_ = false;
            }
            i_ = sum;
            return *this;
        }

        template <typename I>
        constexpr safe& add(const safe<I> other) noexcept
        {
            has_value_ &= static_cast<unsigned_type>(other.has_value_);
            return add(other.i_);
        }

        template <typename V>
        constexpr safe& operator+=(const V v) noexcept
        {
            return add(v);
        }

        template <typename V>
        [[nodiscard]] constexpr safe operator+(const V v) const noexcept
        {
            return safe{*this}.add(v);
        }

        constexpr safe& operator++() noexcept
        {
            return add(1);
        }

        [[nodiscard]] constexpr safe operator++(int) noexcept
        {
            safe cpy{*this};
            add(1);
            return cpy;
        }

        // #### Subtraction

        template <strict_integral I>
        constexpr safe& subtract(const I i) noexcept
        {
            Int diff = 0;
            if (__builtin_sub_overflow(i_, i, &diff))
            {
                has_value_ = false;
            }
            i_ = diff;
            return *this;
        }

        template <typename I>
        constexpr safe& subtract(const safe<I> other) noexcept
        {
            has_value_ &= static_cast<unsigned_type>(other.has_value_);
            return subtract(other.i_);
        }

        template <typename V>
        constexpr safe& operator-=(const V v) noexcept
        {
            return subtract(v);
        }

        template <typename V>
        [[nodiscard]] constexpr safe operator-(const V v) const noexcept
        {
            return safe{*this}.subtract(v);
        }

        constexpr safe& operator--() noexcept
        {
            return subtract(1);
        }

        [[nodiscard]] constexpr safe operator--(int) noexcept
        {
            safe cpy{*this};
            subtract(1);
            return cpy;
        }

        // #### Multiplication

        template <strict_integral I>
        constexpr safe& multiply(const I i) noexcept
        {
            Int prod = 0;
            if (__builtin_mul_overflow(i_, i, &prod))
            {
                has_value_ = false;
            }
            i_ = prod;
            return *this;
        }

        template <typename I>
        constexpr safe& multiply(const safe<I> other) noexcept
        {
            has_value_ &= static_cast<unsigned_type>(other.has_value_);
            return multiply(other.i_);
        }

        template <typename V>
        constexpr safe& operator*=(const V v) noexcept
        {
            return multiply(v);
        }

        template <typename V>
        [[nodiscard]] constexpr safe operator*(const V v) const noexcept
        {
            return safe{*this}.multiply(v);
        }

        // #### Division

        template <strict_integral I>
        constexpr safe& divide(const I i) noexcept
        {
            static_assert(same_signedness_as<Int, I>, "Can't divide by a different sign.");

            // Divide by zero?
            if (i == 0)
            {
                has_value_ = false;
                return *this;
            }

            // Risk of overflow?
            if constexpr (std::is_signed_v<Int>)
            {
                // Would overflow?
                if (i_ == constant::limit<Int>::min && i == -1)
                {
                    has_value_ = false;
                    return *this;
                }
            }

            i_ = static_cast<Int>(i_ / i);
            return *this;
        }

        template <typename I>
        constexpr safe& divide(const safe<I> other) noexcept
        {
            has_value_ &= static_cast<unsigned_type>(other.has_value_);
            return divide(other.i_);
        }

        template <typename V>
        constexpr safe& operator/=(const V v) noexcept
        {
            return divide(v);
        }

        template <typename V>
        [[nodiscard]] constexpr safe operator/(const V v) const noexcept
        {
            return safe{*this}.divide(v);
        }

        // #### Modulo operation / Remainder

        template <strict_integral I>
        constexpr safe& modulo(const I i) noexcept
        {
            // Divide by zero?
            if (i == 0)
            {
                has_value_ = false;
                return *this;
            }

            // Risk of overflow?
            if constexpr (std::is_signed_v<I>)
            {
                if (i == -1)
                {
                    i_ = 0;
                    return *this;
                }
            }

            i_ = static_cast<Int>(i_ % i);
            return *this;
        }

        template <typename I>
        constexpr safe& modulo(const safe<I> other) noexcept
        {
            has_value_ &= static_cast<unsigned_type>(other.has_value_);
            return modulo(other.i_);
        }

        template <typename V>
        constexpr safe& operator%=(const V v) noexcept
        {
            return modulo(v);
        }

        template <typename V>
        [[nodiscard]] constexpr safe operator%(const V v) const noexcept
        {
            return safe{*this}.modulo(v);
        }

        // #### Conversion

        template <strict_integral ToInt>
        [[nodiscard]] constexpr safe<ToInt> to() const noexcept
        {
            using UnsignedInt   = std::make_unsigned_t<Int>;
            using UnsignedToInt = std::make_unsigned_t<ToInt>;

            if constexpr (std::is_unsigned_v<Int>)
            {
                // From unsigned to any wider integral.
                if constexpr (sizeof(ToInt) > sizeof(Int))
                {
                    return safe<ToInt>{static_cast<ToInt>(i_),
                                       static_cast<UnsignedToInt>(has_value_)};
                }
                // From unsigned to unsigned of same width.
                else if constexpr (std::is_unsigned_v<ToInt> && sizeof(ToInt) == sizeof(Int))
                {
                    return safe<ToInt>{static_cast<ToInt>(i_),
                                       static_cast<UnsignedToInt>(has_value_)};
                }
                // From unsigned to signed/unsigned.
                else if (i_ <= static_cast<UnsignedToInt>(constant::limit<ToInt>::max))
                {
                    return safe<ToInt>{static_cast<ToInt>(i_),
                                       static_cast<UnsignedToInt>(has_value_)};
                }
            }
            else
            {
                // From signed to signed of at least the same width.
                if constexpr (std::is_signed_v<ToInt> && sizeof(ToInt) >= sizeof(Int))
                {
                    return safe<ToInt>{static_cast<ToInt>(i_),
                                       static_cast<UnsignedToInt>(has_value_)};
                }
                // From signed to signed.
                else if constexpr (std::is_signed_v<ToInt>)
                {
                    if (i_ >= constant::limit<ToInt>::min && i_ <= constant::limit<ToInt>::max)
                    {
                        return safe<ToInt>{static_cast<ToInt>(i_),
                                           static_cast<UnsignedToInt>(has_value_)};
                    }
                }
                // From signed to unsigned.
                else if (i_ >= 0 && static_cast<UnsignedInt>(i_) <= constant::limit<ToInt>::max)
                {
                    return safe<ToInt>{static_cast<ToInt>(i_),
                                       static_cast<UnsignedToInt>(has_value_)};
                }
            }
            return safe<ToInt>{0, false};
        }

        // #### Comparison

        template <typename I>
        constexpr bool operator==(const safe<I> other) const noexcept
        {
            if (has_value_ & other.has_value_)
            {
                return i_ == other.i_;
            }
            return has_value_ == other.has_value_;
        }

        template <typename I>
        constexpr std::strong_ordering operator<=>(const safe<I> other) const noexcept
        {
            if (has_value_ & other.has_value_)
            {
                return i_ <=> other.i_;
            }
            return has_value_ <=> other.has_value_;
        }

        // #### Comparison with a strict integral

        // A safe<Int> without a value will never be equal to an integral.
        // A safe<Int> without a value will always be less than an integral.

        template <strict_integral I>
        constexpr bool operator==(const I i) const noexcept
        {
            if (has_value_)
            {
                return i_ == i;
            }
            return false;
        }

        template <strict_integral I>
        constexpr std::strong_ordering operator<=>(const I i) const noexcept
        {
            if (has_value_)
            {
                return i_ <=> i;
            }
            return std::strong_ordering::less;
        }

      private:
        template <strict_integral I>
        friend class safe;

        using unsigned_type = std::make_unsigned_t<Int>;

        Int i_{};
        unsigned_type has_value_{true};

        constexpr safe(const Int i, const unsigned_type has_value) noexcept
            : i_{i},
              has_value_{has_value}
        {
        }
    };
}
