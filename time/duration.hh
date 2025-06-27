// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Duration

// Supports +/- billions (10^9) of years with nanosecond precision.

// Has a signed `seconds()` member (i64) and an unsigned `nanoseconds()` member (u32). The
// nanoseconds member is always less than 1'000'000'000 nanoseconds (1 second).

// Note that `time::duration{-3, 999'999'999}` is not -3.999999999 seconds,
// it's -3 seconds + 0.999999999 seconds, i.e. -2.000000001 seconds.

// Duration math does not protect against integer overflows.

#pragma once

#include "snn-core/time/core.hh"
#include "snn-core/time/unit.fwd.hh"

namespace snn::time
{
    // ## Classes

    // ### duration

    class duration final
    {
      public:
        // #### Default constructor

        constexpr duration() noexcept = default;

        // #### Explicit constructors

        constexpr explicit duration(const i64 seconds) noexcept
            : sec_{seconds}
        {
        }

        constexpr explicit duration(i64 seconds, i64 nanoseconds) noexcept
            : duration{}
        {
            time::normalize_inplace<1'000'000'000>(seconds, nanoseconds);
            sec_  = seconds;
            nano_ = static_cast<u32>(nanoseconds);
        }

        constexpr explicit duration(const i64 seconds, const u32 nanoseconds,
                                    assume::is_valid_t) noexcept
            : sec_{seconds},
              nano_{nanoseconds}
        {
            snn_should(nano_ < 1'000'000'000);
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const noexcept
        {
            return sec_ || nano_;
        }

        // #### Values

        [[nodiscard]] constexpr i64 seconds() const noexcept
        {
            return sec_;
        }

        [[nodiscard]] constexpr u32 nanoseconds() const noexcept
        {
            return nano_;
        }

        // #### Addition

        constexpr duration& add(const duration other) noexcept
        {
            sec_ += other.sec_;
            nano_ += other.nano_;
            if (nano_ >= 1'000'000'000)
            {
                ++sec_;
                nano_ -= 1'000'000'000;
            }
            return *this;
        }

        constexpr duration& operator+=(const duration other) noexcept
        {
            return add(other);
        }

        [[nodiscard]] constexpr duration operator+(const duration other) const noexcept
        {
            return duration{*this}.add(other);
        }

        // #### Subtraction

        constexpr duration& subtract(const duration other) noexcept
        {
            sec_ -= other.sec_;
            if (nano_ < other.nano_)
            {
                --sec_;
                nano_ += 1'000'000'000;
            }
            nano_ -= other.nano_;
            return *this;
        }

        constexpr duration& operator-=(const duration other) noexcept
        {
            return subtract(other);
        }

        [[nodiscard]] constexpr duration operator-(const duration other) const noexcept
        {
            return duration{*this}.subtract(other);
        }

        // #### Conversion

        template <any_unit Unit>
        [[nodiscard]] constexpr Unit to() const noexcept
        {
            return Unit{*this};
        }

        // #### Comparison

        constexpr auto operator<=>(const duration&) const noexcept = default;

      private:
        i64 sec_{0};  // Seconds
        u32 nano_{0}; // Nanoseconds, 0-999'999'999.
    };
}
