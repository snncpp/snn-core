// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Unsafe integral

// Can be used instead of `safe<Int>` when safety is not a concern.

#pragma once

#include "snn-core/core.hh"

namespace snn::num
{
    // ## Classes

    // ### unsafe

    template <strict_integral Int>
    class unsafe final
    {
      public:
        // #### Constructors

        constexpr unsafe() noexcept
            : i_{0}
        {
        }

        constexpr unsafe(const Int i) noexcept
            : i_{i}
        {
        }

        // #### Value assignment

        // Value assignment is error-prone and therefore disabled.
        // The implicit copy/move assignment operator can still be used.
        template <typename U>
        void operator=(const U&) = delete;

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return static_cast<bool>(i_);
        }

        // #### Value

        [[nodiscard]] constexpr bool has_value() const noexcept
        {
            return true;
        }

        [[nodiscard]] constexpr Int value() const noexcept
        {
            return i_;
        }

        [[nodiscard]] constexpr Int value(assume::has_value_t) const noexcept
        {
            return i_;
        }

        [[nodiscard]] constexpr Int value_or(Int) const noexcept
        {
            return i_;
        }

        // #### Addition

        template <strict_integral I>
        constexpr unsafe& add(const I i) noexcept
        {
            i_ = static_cast<Int>(i_ + i);
            return *this;
        }

        template <typename I>
        constexpr unsafe& add(const unsafe<I> other) noexcept
        {
            return add(other.i_);
        }

        template <typename V>
        constexpr unsafe& operator+=(const V v) noexcept
        {
            return add(v);
        }

        template <typename V>
        [[nodiscard]] constexpr unsafe operator+(const V v) const noexcept
        {
            return unsafe{*this}.add(v);
        }

        constexpr unsafe& operator++() noexcept
        {
            return add(1);
        }

        [[nodiscard]] constexpr unsafe operator++(int) noexcept
        {
            unsafe cpy{*this};
            add(1);
            return cpy;
        }

        // #### Subtraction

        template <strict_integral I>
        constexpr unsafe& subtract(const I i) noexcept
        {
            i_ = static_cast<Int>(i_ - i);
            return *this;
        }

        template <typename I>
        constexpr unsafe& subtract(const unsafe<I> other) noexcept
        {
            return subtract(other.i_);
        }

        template <typename V>
        constexpr unsafe& operator-=(const V v) noexcept
        {
            return subtract(v);
        }

        template <typename V>
        [[nodiscard]] constexpr unsafe operator-(const V v) const noexcept
        {
            return unsafe{*this}.subtract(v);
        }

        constexpr unsafe& operator--() noexcept
        {
            return subtract(1);
        }

        [[nodiscard]] constexpr unsafe operator--(int) noexcept
        {
            unsafe cpy{*this};
            subtract(1);
            return cpy;
        }

        // #### Multiplication

        template <strict_integral I>
        constexpr unsafe& multiply(const I i) noexcept
        {
            i_ = static_cast<Int>(i_ * i);
            return *this;
        }

        template <typename I>
        constexpr unsafe& multiply(const unsafe<I> other) noexcept
        {
            return multiply(other.i_);
        }

        template <typename V>
        constexpr unsafe& operator*=(const V v) noexcept
        {
            return multiply(v);
        }

        template <typename V>
        [[nodiscard]] constexpr unsafe operator*(const V v) const noexcept
        {
            return unsafe{*this}.multiply(v);
        }

        // #### Division

        template <strict_integral I>
        constexpr unsafe& divide(const I i) noexcept
        {
            static_assert(same_signedness_as<Int, I>, "Can't divide by a different sign.");
            i_ = static_cast<Int>(i_ / i);
            return *this;
        }

        template <typename I>
        constexpr unsafe& divide(const unsafe<I> other) noexcept
        {
            return divide(other.i_);
        }

        template <typename V>
        constexpr unsafe& operator/=(const V v) noexcept
        {
            return divide(v);
        }

        template <typename V>
        [[nodiscard]] constexpr unsafe operator/(const V v) const noexcept
        {
            return unsafe{*this}.divide(v);
        }

        // #### Modulo operation / Remainder

        template <strict_integral I>
        constexpr unsafe& modulo(const I i) noexcept
        {
            i_ = static_cast<Int>(i_ % i);
            return *this;
        }

        template <typename I>
        constexpr unsafe& modulo(const unsafe<I> other) noexcept
        {
            return modulo(other.i_);
        }

        template <typename V>
        constexpr unsafe& operator%=(const V v) noexcept
        {
            return modulo(v);
        }

        template <typename V>
        [[nodiscard]] constexpr unsafe operator%(const V v) const noexcept
        {
            return unsafe{*this}.modulo(v);
        }

        // #### Conversion

        template <strict_integral ToInt>
        [[nodiscard]] constexpr unsafe<ToInt> to() const noexcept
        {
            return unsafe<ToInt>{static_cast<ToInt>(i_)};
        }

        // #### Comparison

        template <typename I>
        constexpr bool operator==(const unsafe<I> other) const noexcept
        {
            return i_ == other.i_;
        }

        template <typename I>
        constexpr std::strong_ordering operator<=>(const unsafe<I> other) const noexcept
        {
            return i_ <=> other.i_;
        }

        // #### Comparison with a strict integral

        template <strict_integral I>
        constexpr bool operator==(const I i) const noexcept
        {
            return i_ == i;
        }

        template <strict_integral I>
        constexpr std::strong_ordering operator<=>(const I i) const noexcept
        {
            return i_ <=> i;
        }

      private:
        template <strict_integral I>
        friend class unsafe;

        Int i_;
    };
}
