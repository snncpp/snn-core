// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Core constants, functions and structures

#pragma once

#include "snn-core/optional.hh"
#include "snn-core/time/error.hh"

namespace snn::time
{
    // ## Constants

    // ### Months

    inline constexpr u8 january   = 1;
    inline constexpr u8 february  = 2;
    inline constexpr u8 march     = 3;
    inline constexpr u8 april     = 4;
    inline constexpr u8 may       = 5;
    inline constexpr u8 june      = 6;
    inline constexpr u8 july      = 7;
    inline constexpr u8 august    = 8;
    inline constexpr u8 september = 9;
    inline constexpr u8 october   = 10;
    inline constexpr u8 november  = 11;
    inline constexpr u8 december  = 12;

    // ### Day of the week

    inline constexpr u8 sunday    = 0;
    inline constexpr u8 monday    = 1;
    inline constexpr u8 tuesday   = 2;
    inline constexpr u8 wednesday = 3;
    inline constexpr u8 thursday  = 4;
    inline constexpr u8 friday    = 5;
    inline constexpr u8 saturday  = 6;

    // ### Seconds per

    template <strict_integral_min<8> Int>
    inline constexpr Int seconds_per_minute = 60;

    template <strict_integral_min<16> Int>
    inline constexpr Int seconds_per_hour = 3'600;

    template <strict_integral_min<32> Int>
    inline constexpr Int seconds_per_day = 86'400;

    template <strict_integral_min<32> Int>
    inline constexpr Int seconds_per_week = 604'800;

    // ### Milliseconds per

    template <strict_integral_min<16> Int>
    inline constexpr Int milliseconds_per_second = 1'000;

    template <strict_integral_min<32> Int>
    inline constexpr Int milliseconds_per_minute = 60'000;

    template <strict_integral_min<32> Int>
    inline constexpr Int milliseconds_per_hour = 3'600'000;

    template <strict_integral_min<32> Int>
    inline constexpr Int milliseconds_per_day = 86'400'000;

    // ### Default min/max year

    inline constexpr i32 default_min_year = -9999;
    inline constexpr i32 default_max_year = 9999;

    namespace detail
    {
        // The number of days in a non-leap year before a month begins. The 12th index returns the
        // number of days before January of next year (365).
        inline constexpr u16 days_before_month[13] = {
            0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365,
        };
    }

    // ## Helper functions

    // ### is_leap_year

    template <integral Int>
    [[nodiscard]] constexpr bool is_leap_year(const Int year) noexcept
    {
        return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
    }

    // ### days_in_month

    [[nodiscard]] constexpr optional<u8> days_in_month(const i32 year, const i32 month) noexcept
    {
        if (month == time::february && is_leap_year(year))
        {
            return 29;
        }

        if (month >= time::january && month <= time::december)
        {
            return static_cast<u8>(detail::days_before_month[month] -
                                   detail::days_before_month[month - 1]);
        }

        return nullopt; // Invalid month.
    }

    // ### normalize

    template <integral Int>
    struct normalized final
    {
        Int large;
        Int small;

        constexpr bool operator==(const normalized&) const noexcept = default;
    };

    template <i32 Base, integral Int>
    constexpr void normalize_inplace(Int& large, Int& small) noexcept
    {
        static_assert(Base >= 2);
        if (small >= Base)
        {
            const Int n = small / Base;
            large += n;
            small -= n * Base;
        }
        else if (small < 0)
        {
            // This works even when `small` is `constant::limit<Int>::min`. (`large` can
            // of course still overflow.)
            const Int n = -(small + 1) / Base;
            large -= n + 1;
            small += Base;
            small += n * Base;
        }
    }

    template <i32 Base, integral Int>
    [[nodiscard]] constexpr normalized<Int> normalize(Int large, Int small) noexcept
    {
        normalize_inplace<Base>(large, small);
        return normalized<Int>{large, small};
    }

    // ## Date and time structures

    // These structures are only for storage, use `time::point` for arithmetic.

    // Data member types are selected for optimal layout.

    // ### hour_minute_second

    struct hour_minute_second final
    {
        u16 h = 0; // 0-23
        u8 m  = 0; // 0-59
        u8 s  = 0; // 0-59

        // #### Constructors

        constexpr hour_minute_second() noexcept = default;

        constexpr explicit hour_minute_second(u16 hour, u8 minute, u8 second) noexcept
            : h{hour},
              m{minute},
              s{second}
        {
        }

        // #### Member access

        [[nodiscard]] constexpr u16 hour() const noexcept
        {
            return h;
        }

        [[nodiscard]] constexpr u8 minute() const noexcept
        {
            return m;
        }

        [[nodiscard]] constexpr u8 second() const noexcept
        {
            return s;
        }

        // #### 12-hour clock

        [[nodiscard]] constexpr u16 hour_12() const noexcept
        {
            u16 th = h;
            if (th >= 12)
            {
                th -= 12;
            }
            if (th == 0)
            {
                th = 12;
            }
            return th;
        }

        [[nodiscard]] constexpr bool is_am() const noexcept
        {
            return h < 12;
        }

        [[nodiscard]] constexpr bool is_pm() const noexcept
        {
            return !is_am();
        }

        // #### Validation

        [[nodiscard]] constexpr result<void> validate() const noexcept
        {
            if (h <= 23)
            {
                if (m <= 59)
                {
                    if (s <= 59)
                    {
                        return {};
                    }
                    return error::invalid_second;
                }
                return error::invalid_minute;
            }
            return error::invalid_hour;
        }

        [[nodiscard]] constexpr bool is_valid() const noexcept
        {
            return validate().has_value();
        }

        // #### Comparison

        constexpr auto operator<=>(const hour_minute_second&) const noexcept = default;
    };

    // #### Alias

    using hms = hour_minute_second;

    // ### year_month_day

    struct year_month_day final
    {
        i32 y = 1970;
        u16 m = 1; // 1-12
        u16 d = 1; // 1-31

        // #### Constructors

        constexpr year_month_day() noexcept = default;

        constexpr explicit year_month_day(i32 year, u16 month, u16 day) noexcept
            : y{year},
              m{month},
              d{day}
        {
        }

        // #### Member access

        [[nodiscard]] constexpr i32 year() const noexcept
        {
            return y;
        }

        [[nodiscard]] constexpr u16 month() const noexcept
        {
            return m;
        }

        [[nodiscard]] constexpr u16 day() const noexcept
        {
            return d;
        }

        // #### Validation

        [[nodiscard]] constexpr result<void> validate(
            const i32 min_year_incl = time::default_min_year,
            const i32 max_year_incl = time::default_max_year) const noexcept
        {
            if (y >= min_year_incl && y <= max_year_incl)
            {
                const u8 days = days_in_month(y, m).value_or_default();
                if (days)
                {
                    if (d >= 1 && d <= days)
                    {
                        return {};
                    }
                    return error::invalid_day;
                }
                return error::invalid_month;
            }
            return error::invalid_year;
        }

        [[nodiscard]] constexpr bool is_valid(
            const i32 min_year_incl = time::default_min_year,
            const i32 max_year_incl = time::default_max_year) const noexcept
        {
            return validate(min_year_incl, max_year_incl).has_value();
        }

        // #### Comparison

        constexpr auto operator<=>(const year_month_day&) const noexcept = default;
    };

    // #### Alias

    using ymd = year_month_day;

    // ### year_week

    struct year_week final
    {
        i32 y = 1970; // Year
        u32 w = 1;    // Week, 1-53 (ISO 8601).

        // #### Constructors

        constexpr year_week() noexcept = default;

        constexpr explicit year_week(i32 year, u32 week) noexcept
            : y{year},
              w{week}
        {
        }

        // #### Member access

        [[nodiscard]] constexpr i32 year() const noexcept
        {
            return y;
        }

        [[nodiscard]] constexpr u32 week() const noexcept
        {
            return w;
        }

        // #### Validation

        [[nodiscard]] constexpr result<void> validate(
            const i32 min_year_incl = time::default_min_year,
            const i32 max_year_incl = time::default_max_year) const noexcept
        {
            if (y >= min_year_incl && y <= max_year_incl)
            {
                if (w >= 1 && w <= 53)
                {
                    return {};
                }
                return error::invalid_week;
            }
            return error::invalid_year;
        }

        [[nodiscard]] constexpr bool is_valid(
            const i32 min_year_incl = time::default_min_year,
            const i32 max_year_incl = time::default_max_year) const noexcept
        {
            return validate(min_year_incl, max_year_incl).has_value();
        }

        // #### Comparison

        constexpr auto operator<=>(const year_week&) const noexcept = default;
    };

    // ### year_month_day_hour_minute_second

    struct year_month_day_hour_minute_second final
    {
        year_month_day ymd{};
        hour_minute_second hms{};

        // #### Constructors

        constexpr year_month_day_hour_minute_second() noexcept = default;

        constexpr explicit year_month_day_hour_minute_second(i32 year, u16 month, u16 day) noexcept
            : ymd{year, month, day}
        {
        }

        constexpr explicit year_month_day_hour_minute_second(i32 year, u16 month, u16 day, u16 hour,
                                                             u8 minute, u8 second) noexcept
            : ymd{year, month, day},
              hms{hour, minute, second}
        {
        }

        // #### Member access

        [[nodiscard]] constexpr i32 year() const noexcept
        {
            return ymd.y;
        }

        [[nodiscard]] constexpr u16 month() const noexcept
        {
            return ymd.m;
        }

        [[nodiscard]] constexpr u16 day() const noexcept
        {
            return ymd.d;
        }

        [[nodiscard]] constexpr u16 hour() const noexcept
        {
            return hms.h;
        }

        [[nodiscard]] constexpr u8 minute() const noexcept
        {
            return hms.m;
        }

        [[nodiscard]] constexpr u8 second() const noexcept
        {
            return hms.s;
        }

        // #### Combined

        [[nodiscard]] constexpr year_month_day date() const noexcept
        {
            return ymd;
        }

        [[nodiscard]] constexpr hour_minute_second time() const noexcept
        {
            return hms;
        }

        // #### Validation

        [[nodiscard]] constexpr result<void> validate(
            const i32 min_year_incl = time::default_min_year,
            const i32 max_year_incl = time::default_max_year) const noexcept
        {
            if (auto res = ymd.validate(min_year_incl, max_year_incl); !res)
            {
                return res.error_code();
            }

            if (auto res = hms.validate(); !res)
            {
                return res.error_code();
            }

            return {};
        }

        [[nodiscard]] constexpr bool is_valid(
            const i32 min_year_incl = time::default_min_year,
            const i32 max_year_incl = time::default_max_year) const noexcept
        {
            return validate(min_year_incl, max_year_incl).has_value();
        }

        // #### Comparison

        constexpr auto operator<=>(
            const year_month_day_hour_minute_second&) const noexcept = default;
    };

    // #### Alias

    using ymdhms = year_month_day_hour_minute_second;

    // ### year_month_day_hour_minute_second_nanosecond_offset

    struct year_month_day_hour_minute_second_nanosecond_offset final
    {
        year_month_day ymd{};
        hour_minute_second hms{};
        u32 n = 0; // Nanoseconds: 0-999'999'999
        i32 o = 0; // Offset from UTC in seconds (+/-18 hours): -64'800-64'800

        // #### Constructors

        constexpr year_month_day_hour_minute_second_nanosecond_offset() noexcept = default;

        constexpr explicit year_month_day_hour_minute_second_nanosecond_offset(i32 year, u16 month,
                                                                               u16 day) noexcept
            : ymd{year, month, day}
        {
        }

        constexpr explicit year_month_day_hour_minute_second_nanosecond_offset(
            i32 year, u16 month, u16 day, u16 hour, u8 minute, u8 second) noexcept
            : ymd{year, month, day},
              hms{hour, minute, second}
        {
        }

        constexpr explicit year_month_day_hour_minute_second_nanosecond_offset(
            i32 year, u16 month, u16 day, u16 hour, u8 minute, u8 second, u32 nanosecond,
            i32 offset) noexcept
            : ymd{year, month, day},
              hms{hour, minute, second},
              n{nanosecond},
              o{offset}
        {
        }

        // #### Member access

        [[nodiscard]] constexpr i32 year() const noexcept
        {
            return ymd.y;
        }

        [[nodiscard]] constexpr u16 month() const noexcept
        {
            return ymd.m;
        }

        [[nodiscard]] constexpr u16 day() const noexcept
        {
            return ymd.d;
        }

        [[nodiscard]] constexpr u16 hour() const noexcept
        {
            return hms.h;
        }

        [[nodiscard]] constexpr u8 minute() const noexcept
        {
            return hms.m;
        }

        [[nodiscard]] constexpr u8 second() const noexcept
        {
            return hms.s;
        }

        [[nodiscard]] constexpr u32 nanosecond() const noexcept
        {
            return n;
        }

        [[nodiscard]] constexpr i32 offset() const noexcept
        {
            return o;
        }

        // #### Combined

        [[nodiscard]] constexpr year_month_day date() const noexcept
        {
            return ymd;
        }

        [[nodiscard]] constexpr hour_minute_second time() const noexcept
        {
            return hms;
        }

        // #### Validation

        [[nodiscard]] constexpr result<void> validate(
            const i32 min_year_incl = time::default_min_year,
            const i32 max_year_incl = time::default_max_year) const noexcept
        {
            if (auto res = ymd.validate(min_year_incl, max_year_incl); !res)
            {
                return res.error_code();
            }

            if (auto res = hms.validate(); !res)
            {
                return res.error_code();
            }

            if (n <= 999'999'999)
            {
                if (o >= -64'800 && o <= 64'800) // +/-18 hour offset.
                {
                    return {};
                }
                return error::invalid_offset;
            }
            return error::invalid_nanosecond;
        }

        [[nodiscard]] constexpr bool is_valid(
            const i32 min_year_incl = time::default_min_year,
            const i32 max_year_incl = time::default_max_year) const noexcept
        {
            return validate(min_year_incl, max_year_incl).has_value();
        }

        // #### Comparison

        // Convert to `time::point` for ordering comparison.

        constexpr bool operator==(
            const year_month_day_hour_minute_second_nanosecond_offset&) const noexcept = default;
    };

    // #### Alias

    using ymdhmsno = year_month_day_hour_minute_second_nanosecond_offset;
}
