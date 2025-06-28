// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Duration

// Supports +/- billions (10^9) of years with nanosecond precision.

// Has a signed `seconds_part()` member (i64) and an unsigned `nanoseconds_part()` member (u32). The
// nanoseconds member is always less than 1'000'000'000 nanoseconds (1 second).

// Note that `time::duration{-3, 999'999'999}` is not -3.999999999 seconds,
// it's -3 seconds + 0.999999999 seconds, i.e. -2.000000001 seconds.

// Duration arithmetic/conversion does not protect against integer overflows nor precision loss. Use
// `time::unit` for safe arithmetic/conversion.

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

        [[nodiscard]] constexpr i64 seconds_part() const noexcept
        {
            return sec_;
        }

        [[nodiscard]] constexpr u32 nanoseconds_part() const noexcept
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

        // #### Safe conversion

        template <any_unit Unit>
        [[nodiscard]] constexpr Unit to() const noexcept
        {
            return Unit{*this};
        }

        // #### Unsafe conversion

        // Unsafe or imprecise conversion (risk of overflow or precision loss).

        // ##### To nanoseconds

        template <floating_point Fp>
        [[nodiscard]] constexpr Fp to_nanoseconds() const noexcept
        {
            return (static_cast<Fp>(sec_) * Fp{1'000'000'000}) + static_cast<Fp>(nano_);
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_nanoseconds() const noexcept
        {
            // Note: `Int` can be `i128`.
            return (Int{sec_} * Int{1'000'000'000}) + Int{nano_};
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_nanoseconds(assume::not_negative_t) const noexcept
        {
            snn_should(sec_ >= 0);
            return to_nanoseconds<Int>();
        }

        // ##### To microseconds

        template <floating_point Fp>
        [[nodiscard]] constexpr Fp to_microseconds() const noexcept
        {
            return (static_cast<Fp>(sec_) * Fp{1'000'000}) + (static_cast<Fp>(nano_) / Fp{1'000});
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_microseconds() const noexcept
        {
            const auto [sec, nano] = normalize_for_conv_();
            // Note: `Int` can be `i128`.
            return (Int{sec} * Int{1'000'000}) + (Int{nano} / Int{1'000});
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_microseconds(assume::not_negative_t) const noexcept
        {
            snn_should(sec_ >= 0);
            // Note: `Int` can be `i128`.
            return (Int{sec_} * Int{1'000'000}) + (Int{nano_} / Int{1'000});
        }

        // ##### To milliseconds

        template <floating_point Fp>
        [[nodiscard]] constexpr Fp to_milliseconds() const noexcept
        {
            return (static_cast<Fp>(sec_) * Fp{1'000}) + (static_cast<Fp>(nano_) / Fp{1'000'000});
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_milliseconds() const noexcept
        {
            const auto [sec, nano] = normalize_for_conv_();
            // Note: `Int` can be `i128`.
            return (Int{sec} * Int{1'000}) + (Int{nano} / Int{1'000'000});
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_milliseconds(assume::not_negative_t) const noexcept
        {
            snn_should(sec_ >= 0);
            // Note: `Int` can be `i128`.
            return (Int{sec_} * Int{1'000}) + (Int{nano_} / Int{1'000'000});
        }

        // ##### To seconds

        template <floating_point Fp>
        [[nodiscard]] constexpr Fp to_seconds() const noexcept
        {
            return static_cast<Fp>(sec_) + (static_cast<Fp>(nano_) / Fp{1'000'000'000});
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_seconds() const noexcept
        {
            const auto [sec, _] = normalize_for_conv_();
            return sec;
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_seconds(assume::not_negative_t) const noexcept
        {
            snn_should(sec_ >= 0);
            return sec_;
        }

        // ##### To minutes

        template <floating_point Fp>
        [[nodiscard]] constexpr Fp to_minutes() const noexcept
        {
            return to_seconds<Fp>() / Fp{60};
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_minutes() const noexcept
        {
            return to_seconds<Int>() / Int{60};
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_minutes(assume::not_negative_t) const noexcept
        {
            return to_seconds<Int>(assume::not_negative) / Int{60};
        }

        // ##### To hours

        template <floating_point Fp>
        [[nodiscard]] constexpr Fp to_hours() const noexcept
        {
            return to_seconds<Fp>() / Fp{3'600};
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_hours() const noexcept
        {
            return to_seconds<Int>() / Int{3'600};
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_hours(assume::not_negative_t) const noexcept
        {
            return to_seconds<Int>(assume::not_negative) / Int{3'600};
        }

        // ##### To days

        template <floating_point Fp>
        [[nodiscard]] constexpr Fp to_days() const noexcept
        {
            return to_seconds<Fp>() / Fp{86'400};
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_days() const noexcept
        {
            return to_seconds<Int>() / Int{86'400};
        }

        template <strict_signed_integral_min<64> Int>
        [[nodiscard]] constexpr Int to_days(assume::not_negative_t) const noexcept
        {
            return to_seconds<Int>(assume::not_negative) / Int{86'400};
        }

        // #### Comparison

        constexpr auto operator<=>(const duration&) const noexcept = default;

      private:
        i64 sec_{0};  // Seconds
        u32 nano_{0}; // Nanoseconds, 0-999'999'999.

        constexpr time::normalized<i64> normalize_for_conv_() const noexcept
        {
            i64 sec{sec_};
            i64 nano{nano_};

            // Convert/truncate negative durations correctly.
            if (sec < 0 && nano > 0)
            {
                ++sec;
                nano -= 1'000'000'000;
            }

            return time::normalized<i64>{sec, nano};
        }
    };
}
