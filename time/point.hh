// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0 AND BSD-3-Clause
// Go's time library was used as a reference for parts of this file (see LICENSE.md).
// Copyright (c) 2009 The Go Authors. All rights reserved.

// # Time point and `now()` function

// Represents a single point in time in UTC.

// The supported range is:
//       Unix timestamp     Formatted
// Min: -67768100567971200 -2147483648-01-01T00:00:00+00:00
// Max:  67767976233532799  2147483647-12-31T23:59:59+00:00

// Values outside this range might silently overflow (even with integer sanitizer).

#pragma once

#include "snn-core/array.hh"
#include "snn-core/exception.hh"
#include "snn-core/formatter.hh"
#include "snn-core/strcore.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/fmt/context.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/math/common.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/time/core.hh"
#include "snn-core/time/duration.hh"
#include "snn-core/time/error.hh"
#include "snn-core/time/unit.hh"
#include "snn-core/time/wall/since_epoch.hh"
#include "snn-core/time/zone/location.hh"

namespace snn::time
{
    // ## Constants

    // ### Format strings helpers

    namespace format_string
    {
        // #### For use with or without location

        inline constexpr char date[]  = "f";   // 2001-02-03
        inline constexpr char local[] = "f t"; // 2001-02-03 04:05:06

        // Quick/readable, will include fraction (trimmed) if any.
        inline constexpr char quick[] = "q"; // 2001-02-03 04:05:06[.123456789] +0700

        inline constexpr char rfc2822[] = "r"; // Sat, 03 Feb 2001 04:05:06 +0700

        // Note: ISO 8601/RFC 3339 are the same here.
        inline constexpr char iso8601[] = "c"; // 2001-02-03T04:05:06+07:00
        inline constexpr char rfc3339[] = "c"; // 2001-02-03T04:05:06+07:00

        // #### For use without location

        // Note: `iso8601_*z` format strings are also RFC 3339 compatible.
        inline constexpr char iso8601_z[]       = "z";   // 2001-02-03T04:05:06Z
        inline constexpr char iso8601_milli_z[] = "zz";  // 2001-02-03T04:05:06.123Z
        inline constexpr char iso8601_nano_z[]  = "zzz"; // 2001-02-03T04:05:06.123456789Z

        inline constexpr char rfc1123[] = "l"; // Sat, 03 Feb 2001 04:05:06 GMT
    }

    // ## Classes

    // ### point

    class point final
    {
      public:
        // #### Default constructor

        constexpr point() noexcept
            : point{i64{}}
        {
        }

        // #### Explicit constructors

        constexpr explicit point(const i64 unixtime) noexcept
            : sec_{unixtime + unix_to_internal_},
              nano_{0}
        {
        }

        constexpr explicit point(const time::duration d) noexcept
            : sec_{d.seconds() + unix_to_internal_},
              nano_{d.nanoseconds()}
        {
        }

        // Date/time structures are not validated but normalized, e.g. (non-leap year):
        // `time::point{time::ymd{2017, 2, 29}} == time::point{2017, 3, 1}`

        constexpr explicit point(const year_month_day ymd) noexcept
            : point{ymd.y, ymd.m, ymd.d}
        {
        }

        constexpr explicit point(const year_month_day_hour_minute_second ymdhms) noexcept
            : point{ymdhms.ymd.y, ymdhms.ymd.m, ymdhms.ymd.d,
                    ymdhms.hms.h, ymdhms.hms.m, ymdhms.hms.s}
        {
        }

        constexpr explicit point(
            const year_month_day_hour_minute_second_nanosecond_offset c) noexcept
            : point{c.ymd.y, c.ymd.m, c.ymd.d, c.hms.h, c.hms.m, c.hms.s, c.n, c.o}
        {
        }

        // #### Converting constructors

        // Values are normalized, e.g.:
        // `time::point{2017, 5, 0} == time::point{2017, 4, 30}`
        // `time::point{2017, -1, 15} == time::point{2016, 11, 15}`

        constexpr point(i32 year, i32 month, i32 day) noexcept
            : sec_{to_unix_(year, month, day, 0, 0, 0) + unix_to_internal_},
              nano_{0}
        {
        }

        constexpr point(i32 year, i32 month, i32 day, zone::location& loc) noexcept
            : point{year, month, day, 0, 0, 0, loc}
        {
        }

        constexpr point(const year_month_day ymd, zone::location& loc) noexcept
            : point{ymd.y, ymd.m, ymd.d, loc}
        {
        }

        constexpr point(i32 year, i32 month, i32 day, i32 hour, i32 min, i32 sec) noexcept
            : sec_{to_unix_(year, month, day, hour, min, sec) + unix_to_internal_},
              nano_{0}
        {
        }

        constexpr point(i32 year, i32 month, i32 day, i32 hour, i32 min, i32 sec,
                        zone::location& loc) noexcept
            : sec_{0},
              nano_{0}
        {
            i64 unix = to_unix_(year, month, day, hour, min, sec);

            i32 offset_sec = loc.offset(unix).seconds();
            if (offset_sec != 0)
            {
                const i64 from = loc.last_offset_from();
                const i64 to   = loc.last_offset_to();

                const i64 utc = unix - offset_sec;
                if (utc < from)
                {
                    offset_sec = loc.offset(from - 1).seconds();
                }
                else if (utc >= to)
                {
                    offset_sec = loc.offset(to).seconds();
                }

                unix -= offset_sec;
            }

            sec_ = unix + unix_to_internal_;
        }

        constexpr point(const year_month_day_hour_minute_second ymdhms,
                        zone::location& loc) noexcept
            : point{ymdhms.ymd.y, ymdhms.ymd.m, ymdhms.ymd.d, ymdhms.hms.h,
                    ymdhms.hms.m, ymdhms.hms.s, loc}
        {
        }

        constexpr point(i32 year, i32 month, i32 day, i32 hour, i32 min, i32 sec, i64 nano,
                        i32 offset_sec) noexcept
            : sec_{to_unix_(year, month, day, hour, min, sec) - offset_sec + unix_to_internal_},
              nano_{0}
        {
            time::normalize_inplace<1'000'000'000>(sec_, nano);
            nano_ = static_cast<u32>(nano);
        }

        // #### Observers

        [[nodiscard]] constexpr year_month_day date() const noexcept
        {
            return date_(absolute_());
        }

        [[nodiscard]] constexpr year_month_day date(zone::location& loc) const noexcept
        {
            return date_(absolute_(loc));
        }

        [[nodiscard]] constexpr u16 day() const noexcept
        {
            return date_(absolute_()).d;
        }

        [[nodiscard]] constexpr u16 day(zone::location& loc) const noexcept
        {
            return date_(absolute_(loc)).d;
        }

        [[nodiscard]] constexpr u8 day_of_week() const noexcept
        {
            return day_of_week_(absolute_());
        }

        [[nodiscard]] constexpr u8 day_of_week(zone::location& loc) const noexcept
        {
            return day_of_week_(absolute_(loc));
        }

        [[nodiscard]] constexpr u16 day_of_year() const noexcept
        {
            return year_day_of_year_(absolute_()).day_of_year + 1; // 1-366
        }

        [[nodiscard]] constexpr u16 day_of_year(zone::location& loc) const noexcept
        {
            return year_day_of_year_(absolute_(loc)).day_of_year + 1; // 1-366
        }

        [[nodiscard]] constexpr u8 hour() const noexcept
        {
            return static_cast<u8>((absolute_() % time::seconds_per_day<u64>) /
                                   time::seconds_per_hour<u64>);
        }

        [[nodiscard]] constexpr u8 hour(zone::location& loc) const noexcept
        {
            return static_cast<u8>((absolute_(loc) % time::seconds_per_day<u64>) /
                                   time::seconds_per_hour<u64>);
        }

        [[nodiscard]] constexpr u8 minute() const noexcept
        {
            return static_cast<u8>((absolute_() % time::seconds_per_hour<u64>) /
                                   time::seconds_per_minute<u64>);
        }

        [[nodiscard]] constexpr u8 minute(zone::location& loc) const noexcept
        {
            return static_cast<u8>((absolute_(loc) % time::seconds_per_hour<u64>) /
                                   time::seconds_per_minute<u64>);
        }

        [[nodiscard]] constexpr u16 month() const noexcept
        {
            return date_(absolute_()).m;
        }

        [[nodiscard]] constexpr u16 month(zone::location& loc) const noexcept
        {
            return date_(absolute_(loc)).m;
        }

        [[nodiscard]] constexpr u32 nanosecond() const noexcept
        {
            return nano_;
        }

        [[nodiscard]] constexpr zone::offset offset(zone::location& loc) const noexcept
        {
            return loc.offset(sec_ + internal_to_unix_);
        }

        [[nodiscard]] constexpr u8 second() const noexcept
        {
            return static_cast<u8>(absolute_() % time::seconds_per_minute<u64>);
        }

        [[nodiscard]] constexpr u8 second(zone::location& loc) const noexcept
        {
            return static_cast<u8>(absolute_(loc) % time::seconds_per_minute<u64>);
        }

        [[nodiscard]] constexpr hour_minute_second time() const noexcept
        {
            return time_(absolute_());
        }

        [[nodiscard]] constexpr hour_minute_second time(zone::location& loc) const noexcept
        {
            return time_(absolute_(loc));
        }

        [[nodiscard]] constexpr i64 unix() const noexcept
        {
            return sec_ + internal_to_unix_;
        }

        [[nodiscard]] constexpr year_week week() const noexcept
        {
            return week_iso8601_(absolute_());
        }

        [[nodiscard]] constexpr year_week week(zone::location& loc) const noexcept
        {
            return week_iso8601_(absolute_(loc));
        }

        [[nodiscard]] constexpr i32 year() const noexcept
        {
            return year_day_of_year_(absolute_()).year;
        }

        [[nodiscard]] constexpr i32 year(zone::location& loc) const noexcept
        {
            return year_day_of_year_(absolute_(loc)).year;
        }

        // #### Format

        // Format without location.

        template <typename Buf>
        constexpr void format(const cstrview string, strcore<Buf>& append_to,
                              assume::no_overlap_t) const
        {
            snn_should(std::is_constant_evaluated() || !string.overlaps(append_to));
            format_(absolute_(), string, zone::offset::utc(), append_to, assume::no_overlap);
        }

        template <usize N, typename Buf>
        constexpr void format(const char (&string)[N], strcore<Buf>& append_to) const
        {
            format_(absolute_(), string, zone::offset::utc(), append_to, assume::no_overlap);
        }

        template <any_strcore Str = str>
        [[nodiscard]] constexpr Str format(const cstrview string) const
        {
            Str append_to;
            format_(absolute_(), string, zone::offset::utc(), append_to, assume::no_overlap);
            return append_to;
        }

        // Format with location.

        template <typename Buf>
        constexpr void format(const cstrview string, zone::location& loc, strcore<Buf>& append_to,
                              assume::no_overlap_t) const
        {
            snn_should(std::is_constant_evaluated() || !string.overlaps(append_to));
            const auto offs = offset(loc);
            format_(absolute_(offs.seconds()), string, offs, append_to, assume::no_overlap);
        }

        template <usize N, typename Buf>
        constexpr void format(const char (&string)[N], zone::location& loc,
                              strcore<Buf>& append_to) const
        {
            const auto offs = offset(loc);
            format_(absolute_(offs.seconds()), string, offs, append_to, assume::no_overlap);
        }

        template <any_strcore Str = str>
        [[nodiscard]] constexpr Str format(const cstrview string, zone::location& loc) const
        {
            Str append_to;
            const auto offs = offset(loc);
            format_(absolute_(offs.seconds()), string, offs, append_to, assume::no_overlap);
            return append_to;
        }

        // #### Hash

        [[nodiscard]] constexpr usize hash() const noexcept
        {
            // The hash is currently the unsigned unix time in nanoseconds (this might change in the
            // future). This will not overflow for time points between the years 1970 and 2553
            // (inclusive).
            const u64 unix_in_nano = math::multiply_with_overflow(to_u64(unix()), 1'000'000'000);
            return math::add_with_overflow(unix_in_nano, nano_);
        }

        // #### Addition

        // Duration (will never throw).

        constexpr point& add(const time::duration d) noexcept
        {
            time::duration tmp{sec_, nano_, promise::is_valid};
            tmp.add(d);
            sec_  = tmp.seconds();
            nano_ = tmp.nanoseconds();
            return *this;
        }

        constexpr point& operator+=(const time::duration d) noexcept
        {
            return add(d);
        }

        [[nodiscard]] constexpr point operator+(const time::duration d) const noexcept
        {
            return point{*this}.add(d);
        }

        // Unit (can throw).

        template <any_unit Unit>
        constexpr point& add(const Unit u)
        {
            return add(u.duration().value());
        }

        template <any_unit Unit>
        constexpr point& operator+=(const Unit u)
        {
            return add(u);
        }

        template <any_unit Unit>
        [[nodiscard]] constexpr point operator+(const Unit u) const
        {
            return point{*this}.add(u);
        }

        // #### Subtraction

        // Duration (will never throw).

        constexpr point& subtract(const time::duration d) noexcept
        {
            time::duration tmp{sec_, nano_, promise::is_valid};
            tmp.subtract(d);
            sec_  = tmp.seconds();
            nano_ = tmp.nanoseconds();
            return *this;
        }

        constexpr point& operator-=(const time::duration d) noexcept
        {
            return subtract(d);
        }

        [[nodiscard]] constexpr point operator-(const time::duration d) const noexcept
        {
            return point{*this}.subtract(d);
        }

        // Unit (can throw).

        template <any_unit Unit>
        constexpr point& subtract(const Unit u)
        {
            return subtract(u.duration().value());
        }

        template <any_unit Unit>
        constexpr point& operator-=(const Unit u)
        {
            return subtract(u);
        }

        template <any_unit Unit>
        [[nodiscard]] constexpr point operator-(const Unit u) const
        {
            return point{*this}.subtract(u);
        }

        // #### Conversion

        [[nodiscard]] constexpr time::duration duration() const noexcept
        {
            return time::duration{sec_ + internal_to_unix_, nano_, promise::is_valid};
        }

        // ### Comparison

        constexpr auto operator<=>(const point&) const noexcept = default;

      private:
        i64 sec_;  // The number of seconds elapsed since January 1, year 1 00:00:00 UTC.
        u32 nano_; // Nanoseconds, 0-999'999'999.

        // "The unsigned zero year for internal calculations. Must be 1 mod 400, and times before
        // it will not compute correctly, but otherwise can be changed at will."
        // If this is changed the constants below have to updated as well.
        static constexpr i64 abs_zero_year_ = -292277022399;

        // The year of the zero time. Assumed by the unix_to_internal_ computation below.
        // static constexpr i64 internal_year_ = 1;

        // Offsets to convert between internal and absolute times (see calculation below).
        static constexpr i64 abs_to_internal_ = -9223371966579724800;
        static constexpr i64 internal_to_abs_ = -abs_to_internal_;

        // Absolute to internal calculated:
        // static constexpr i64 abs_to_internal_ = static_cast<i64>((abs_zero_year_ -
        // internal_year_) * 365.2425) * time::seconds_per_day<i64>;

        // Offsets to convert between internal and unix times (see calculation below).
        static constexpr i64 unix_to_internal_ = 62135596800;
        static constexpr i64 internal_to_unix_ = -unix_to_internal_;

        // Unix to internal calculated:
        // static constexpr i64 unix_to_internal_
        //     = (1969 * 365 + 1969 / 4 - 1969 / 100 + 1969 / 400) * time::seconds_per_day<i64>;

        static constexpr u32 days_per_400_years_ = 146097; // 365 * 400 + 97
        static constexpr u32 days_per_100_years_ = 36524;  // 365 * 100 + 24
        static constexpr u32 days_per_4_years_   = 1461;   // 365 * 4 + 1

        static constexpr char day_abbr_[7][4]{"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

        static constexpr char month_abbr_[12][4]{"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                                 "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

        struct year_day_of_year
        {
            i32 year;        // E.g. 2016.
            u16 day_of_year; // 0-365
        };

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        static constexpr auto lookup_day_abbr_(const u8 day) noexcept -> const char (&)[4]
        {
            snn_should(day <= 6);
            return day_abbr_[day];
        }

        static constexpr auto lookup_month_abbr_(const u16 month) noexcept -> const char (&)[4]
        {
            snn_should(month >= 1 && month <= 12);
            return month_abbr_[month - 1];
        }

        SNN_DIAGNOSTIC_POP

        constexpr u64 absolute_() const noexcept
        {
            // Avoid signed integer overflow, same assembly as:
            // to_u64(sec_ + internal_to_abs_)
            return math::add_with_overflow(to_u64(sec_), to_u64(internal_to_abs_));
        }

        constexpr u64 absolute_(const i32 offset_sec) const noexcept
        {
            // Avoid signed integer overflow, same assembly as:
            // to_u64(sec_ + internal_to_abs_ + offset_sec)
            return math::add_with_overflow(absolute_(), to_u64(offset_sec));
        }

        constexpr u64 absolute_(zone::location& loc) const noexcept
        {
            i64 timestamp = sec_ + internal_to_unix_;
            timestamp += loc.offset(timestamp).seconds();
            // Avoid signed integer overflow, same assembly as:
            // to_u64(timestamp + (unix_to_internal_ + internal_to_abs_));
            return to_u64(timestamp) + to_u64(unix_to_internal_ + internal_to_abs_);
        }

        static constexpr hour_minute_second time_(const u64 abs) noexcept
        {
            u64 sec        = abs % time::seconds_per_day<u64>;
            const u64 hour = sec / time::seconds_per_hour<u64>;
            sec -= hour * time::seconds_per_hour<u64>;
            const u64 min = sec / time::seconds_per_minute<u64>;
            sec -= min * time::seconds_per_minute<u64>;
            return hour_minute_second{static_cast<u16>(hour), static_cast<u8>(min),
                                      static_cast<u8>(sec)};
        }

        static constexpr year_month_day date_(const u64 abs) noexcept
        {
            const auto ydoy = year_day_of_year_(abs);
            return year_month_day_(ydoy.year, ydoy.day_of_year);
        }

        static constexpr u8 day_of_week_(const u64 abs) noexcept
        {
            // "January 1 of the absolute year, like January 1 of 2001, was a Monday."
            // Seconds per day here is seconds after monday.
            const u64 sec = (abs + time::seconds_per_day<u64>) % time::seconds_per_week<u64>;
            return static_cast<u8>(sec / time::seconds_per_day<u64>);
        }

        template <typename Buf>
        constexpr void format_(const u64 abs, const cstrview string, const zone::offset offs,
                               strcore<Buf>& append_to, assume::no_overlap_t) const
        {
            const auto ymd = date_(abs);
            const auto hms = time_(abs);

            auto rng = string.range();
            while (rng)
            {
                const char c = rng.pop_front(assume::not_empty);

                const bool is_alpha = chr::is_alpha(c);
                usize repeat_count  = 1;

                if (is_alpha)
                {
                    repeat_count += rng.pop_front_while(fn::is{fn::equal_to{}, c}).count();
                }

                switch (c)
                {
                    // Cases are ASCII-sorted: A-Z, [backslash], a-z, default.

                    // AA - Ante meridiem (AM) or post meridiem (PM), abbreviated, upper case.
                    case 'A':
                        if (repeat_count == 2)
                        {
                            const char prefix = hms.is_am() ? 'A' : 'P';
                            append_to.append({prefix, 'M'});
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // DD - Day of the month, space padded.
                    case 'D':
                        if (repeat_count == 2)
                        {
                            char* dest = append_to.append_for_overwrite(2).begin();
                            dest       = format_2_digits_(' ', ymd.d, dest);
                            snn_should(dest == append_to.end());
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // N         - Nanosecond fraction, 1-digit, trimmed. (Tenths of a second.)
                    // NN        - Nanosecond fraction, 2-digit, trimmed. (Hundredths of a second.)
                    // NNN...
                    // NNNNNNNNN - Nanosecond fraction, 9-digit, trimmed.
                    case 'N':
                        if (repeat_count <= 9)
                        {
                            constexpr bool trim = true;
                            format_nano_(repeat_count, trim, append_to);
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // OOO - Time zone abbreviation, e.g. "GMT", "CEST" etc.
                    case 'O':
                        if (repeat_count == 3)
                        {
                            append_to.append(offs.abbr().view());
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // Escape, append character after '\', if any.
                    case '\\':
                        if (rng)
                        {
                            const char escaped = rng.pop_front(assume::not_empty);
                            append_to.append(escaped);
                        }
                        break;

                    // aa - Ante meridiem (am) or post meridiem (pm), abbreviated, lower case.
                    case 'a':
                        if (repeat_count == 2)
                        {
                            const char prefix = hms.is_am() ? 'a' : 'p';
                            append_to.append({prefix, 'm'});
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // c - ISO 8601/RFC 3339 (without fraction).
                    case 'c':
                        if (repeat_count == 1)
                        {
                            append_to.reserve_append(string_size("2001-02-03T04:05:06+07:00"));
                            format_full_date_(ymd, append_to);
                            append_to.append('T');
                            format_full_time_(hms, append_to);
                            format_full_offset_(offs.seconds(), append_to, ':');
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // d  - Day of the month.
                    // dd - Day of the month, zero padded.
                    case 'd':
                        if (repeat_count <= 2)
                        {
                            format_1_or_2_digits_(repeat_count, '0', ymd.d, append_to);
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // e   - Day of the week (single digit)
                    // eee - Day of the week (abbreviated name).
                    case 'e':
                        if (repeat_count == 3) // Most common.
                        {
                            append_to.append(lookup_day_abbr_(day_of_week_(abs)));
                            break;
                        }
                        if (repeat_count == 1)
                        {
                            append_to.append(static_cast<char>(day_of_week_(abs) + '0'));
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // f - Full date.
                    case 'f':
                        if (repeat_count == 1)
                        {
                            format_full_date_(ymd, append_to);
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // g  - Hour (12-hour clock).
                    // gg - Hour (12-hour clock), zero padded.
                    case 'g':
                        if (repeat_count <= 2)
                        {
                            format_1_or_2_digits_(repeat_count, '0', hms.hour_12(), append_to);
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // h  - Hour (24-hour clock).
                    // hh - Hour (24-hour clock), zero padded.
                    case 'h':
                        if (repeat_count <= 2)
                        {
                            format_1_or_2_digits_(repeat_count, '0', hms.h, append_to);
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // i  - Minute
                    // ii - Minute, zero padded.
                    case 'i':
                        if (repeat_count <= 2)
                        {
                            format_1_or_2_digits_(repeat_count, '0', hms.m, append_to);
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // l - RFC 1123 (time zone abbreviation is always "GMT").
                    case 'l':
                        if (repeat_count == 1)
                        {
                            append_to.reserve_append(string_size("Sat, 03 Feb 2001 04:05:06 GMT"));
                            // Day of week.
                            append_to.append(lookup_day_abbr_(day_of_week_(abs)));
                            // Separator
                            append_to.append(", ");
                            // Day of month.
                            char* dest = append_to.append_for_overwrite(2).begin();
                            dest       = format_2_digits_('0', ymd.d, dest);
                            snn_should(dest == append_to.end());
                            // Separator
                            append_to.append(' ');
                            // Month abbreviation.
                            append_to.append(lookup_month_abbr_(ymd.m));
                            // Separator
                            append_to.append(' ');
                            // Year
                            format_year_(ymd.y, append_to);
                            // Separator
                            append_to.append(' ');
                            // Full time.
                            format_full_time_(hms, append_to);
                            // Separator & time zone abbreviation.
                            append_to.append(" GMT");
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // m   - Month
                    // mm  - Month, zero padded.
                    // mmm - Month (abbreviated name).
                    case 'm':
                        if (repeat_count <= 2)
                        {
                            format_1_or_2_digits_(repeat_count, '0', ymd.m, append_to);
                            break;
                        }
                        if (repeat_count == 3)
                        {
                            append_to.append(lookup_month_abbr_(ymd.m));
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // n         - Nanosecond fraction, 1-digit, fixed. (Tenths of a second.)
                    // nn        - Nanosecond fraction, 2-digit, fixed. (Hundredths of a second.)
                    // nnn...
                    // nnnnnnnnn - Nanosecond fraction, 9-digit, fixed.
                    case 'n':
                        if (repeat_count <= 9)
                        {
                            constexpr bool trim = false;
                            format_nano_(repeat_count, trim, append_to);
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // o     - Time offset in seconds.
                    // oooo  - Time offset in +/-hours-minutes, e.g. "+0700".
                    // ooooo - Time offset in +/-hours-colon-minutes, e.g. "+07:00".
                    case 'o':
                        if (repeat_count == 4)
                        {
                            format_full_offset_(offs.seconds(), append_to);
                            break;
                        }
                        if (repeat_count == 5)
                        {
                            format_full_offset_(offs.seconds(), append_to, ':');
                            break;
                        }
                        if (repeat_count == 1)
                        {
                            append_to.append_integral(offs.seconds());
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // q - Quick/readable with optional fraction.
                    case 'q':
                        if (repeat_count == 1)
                        {
                            append_to.reserve_append(
                                string_size("2001-02-03 04:05:06.123456789 +0700"));
                            format_full_date_(ymd, append_to);
                            append_to.append(' ');
                            format_full_time_(hms, append_to);
                            if (nano_)
                            {
                                append_to.append('.');
                                constexpr bool trim = true;
                                format_nano_(9, trim, append_to);
                            }
                            append_to.append(' ');
                            format_full_offset_(offs.seconds(), append_to);
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // r - RFC 2822.
                    case 'r':
                        if (repeat_count == 1)
                        {
                            append_to.reserve_append(
                                string_size("Sat, 03 Feb 2001 04:05:06 +0700"));
                            // Day of week.
                            append_to.append(lookup_day_abbr_(day_of_week_(abs)));
                            // Separator
                            append_to.append(", ");
                            // Day of month.
                            char* dest = append_to.append_for_overwrite(2).begin();
                            dest       = format_2_digits_('0', ymd.d, dest);
                            snn_should(dest == append_to.end());
                            // Separator
                            append_to.append(' ');
                            // Month abbreviation.
                            append_to.append(lookup_month_abbr_(ymd.m));
                            // Separator
                            append_to.append(' ');
                            // Year
                            format_year_(ymd.y, append_to);
                            // Separator
                            append_to.append(' ');
                            // Full time.
                            format_full_time_(hms, append_to);
                            // Separator
                            append_to.append(' ');
                            // Time zone offset.
                            format_full_offset_(offs.seconds(), append_to);
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // s  - Second
                    // ss - Second, zero padded.
                    case 's':
                        if (repeat_count <= 2)
                        {
                            format_1_or_2_digits_(repeat_count, '0', hms.s, append_to);
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // t - Full 24-hour time.
                    case 't':
                        if (repeat_count == 1)
                        {
                            format_full_time_(hms, append_to);
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // v - Daylight saving time.
                    case 'v':
                        if (repeat_count == 1)
                        {
                            const char dst = '0' + char{offs.is_dst()};
                            append_to.append(dst);
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // yyyy - Year, 4-digit (minimum), zero padded.
                    case 'y':
                        if (repeat_count == 4)
                        {
                            format_year_(ymd.y, append_to);
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    // z   - ISO 8601/RFC 3339 with 'Z' timezone, without fraction.
                    // zz  - ISO 8601/RFC 3339 with 'Z' timezone, with millisecond fraction.
                    // zzz - ISO 8601/RFC 3339 with 'Z' timezone, with nanosecond fraction.
                    case 'z':
                        if (repeat_count <= 3)
                        {
                            // Repeat count 1: 20 characters: 2001-02-03T04:05:06Z
                            // Repeat count 2: 24 characters: 2001-02-03T04:05:06.123Z
                            // Repeat count 3: 30 characters: 2001-02-03T04:05:06.123456789Z
                            append_to.reserve_append(15 + (5 * repeat_count)); // Approx.
                            format_full_date_(ymd, append_to);
                            append_to.append('T');
                            format_full_time_(hms, append_to);
                            if (repeat_count > 1)
                            {
                                append_to.append('.');
                                constexpr bool trim = false;
                                format_nano_(repeat_count == 2 ? 3 : 9, trim, append_to);
                            }
                            append_to.append('Z');
                            break;
                        }
                        throw_or_abort(error::invalid_format_string);

                    default:
                        if (!is_alpha)
                        {
                            append_to.append(c);
                            break;
                        }
                        throw_or_abort(error::unescaped_alpha_character);
                }
            }
        }

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        template <typename Buf>
        constexpr void format_full_date_(const year_month_day ymd, strcore<Buf>& append_to) const
        {
            format_year_(ymd.y, append_to);
            char* dest = append_to.append_for_overwrite(string_size("-02-03")).begin();
            *(dest++)  = '-';
            dest       = format_2_digits_('0', ymd.m, dest);
            *(dest++)  = '-';
            dest       = format_2_digits_('0', ymd.d, dest);
            snn_should(dest == append_to.end());
        }

        template <typename Buf>
        constexpr void format_full_time_(const hour_minute_second hms,
                                         strcore<Buf>& append_to) const
        {
            char* dest = append_to.append_for_overwrite(string_size("04:05:06")).begin();
            dest       = format_2_digits_('0', hms.h, dest);
            *(dest++)  = ':';
            dest       = format_2_digits_('0', hms.m, dest);
            *(dest++)  = ':';
            dest       = format_2_digits_('0', hms.s, dest);
            snn_should(dest == append_to.end());
        }

        SNN_DIAGNOSTIC_POP

        template <typename Buf>
        constexpr void format_nano_(const usize digit_count, const bool trim,
                                    strcore<Buf>& append_to) const
        {
            array<char, 9> tmp;
            tmp.fill('0');

            if (nano_)
            {
                tmp.each_in_reverse([n = nano_](char& c) mutable {
                    c = static_cast<char>((n % 10) + '0');
                    n /= 10;
                });
            }

            cstrview formatted = tmp.view(0, digit_count);

            if (trim)
            {
                while (formatted.has_back('0'))
                {
                    formatted.drop_back_n(1);
                }

                if (formatted.is_empty())
                {
                    if (append_to.has_back('.'))
                    {
                        append_to.drop_back_n(1);
                    }
                }
            }

            append_to.append(formatted);
        }

        template <typename Buf>
        constexpr void format_full_offset_(const i32 offset_sec, strcore<Buf>& append_to,
                                           const char offset_separator = 0) const
        {
            const bool with_separator = offset_separator != 0;

            char prefix  = '+';
            u32 abs_offs = to_u32(offset_sec);
            if (offset_sec < 0)
            {
                prefix   = '-';
                abs_offs = math::negate_with_overflow(abs_offs);
            }

            const u32 h = abs_offs / time::seconds_per_hour<u32>;
            const u32 m = (abs_offs % time::seconds_per_hour<u32>) / time::seconds_per_minute<u32>;

            const usize offs_size = string_size("+0700") + usize{with_separator};
            char* dest            = append_to.append_for_overwrite(offs_size).begin();

            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            *(dest++) = prefix;

            dest = format_2_digits_('0', static_cast<u16>(h), dest);
            if (with_separator)
            {
                *(dest++) = offset_separator;
            }
            dest = format_2_digits_('0', static_cast<u16>(m), dest);

            SNN_DIAGNOSTIC_POP

            snn_should(dest == append_to.end());
        }

        template <typename Buf>
        constexpr void format_1_or_2_digits_(const usize digit_count, const char padding,
                                             const u16 i, strcore<Buf>& append_to) const
        {
            if (digit_count == 1 && i <= 9)
            {
                append_to.append(static_cast<char>(i + '0'));
            }
            else
            {
                char* dest = append_to.append_for_overwrite(2).begin();
                dest       = format_2_digits_(padding, i, dest);
                snn_should(dest == append_to.end());
            }
        }

        template <typename It>
        constexpr It format_2_digits_(const char padding, u16 i, It dest) const noexcept
        {
            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            if (i < 10)
            {
                *(dest++) = padding;
                *(dest++) = static_cast<char>(i + '0');
            }
            else
            {
                snn_should(i <= 99);
                const auto remainder = i % 10;
                i /= 10;
                *(dest++) = static_cast<char>(i + '0');
                *(dest++) = static_cast<char>(remainder + '0');
            }
            return dest;

            SNN_DIAGNOSTIC_POP
        }

        template <typename Buf>
        constexpr void format_year_(const i32 year, strcore<Buf>& append_to) const
        {
            u64 u = to_u64(year);
            if (year < 0) [[unlikely]]
            {
                append_to.append('-');
                u = math::negate_with_overflow(u);
            }
            constexpr usize min_digits = 4;
            append_to.append_integral(u, min_digits);
        }

        static constexpr i64 to_unix_(i32 year, i32 month, i32 day, i32 hour, i32 min,
                                      i32 sec) noexcept
        {
            --month; // Zero based month for normalization.
            time::normalize_inplace<12>(year, month);
            ++month;

            time::normalize_inplace<60>(min, sec);
            time::normalize_inplace<60>(hour, min);
            time::normalize_inplace<24>(day, hour);

            // Use an unsigned year for calculations.
            auto y = to_u64(year - abs_zero_year_);

            // Days since the absolute epoch.

            // Add days from 400-year cycles.
            u64 n = y / 400;
            y -= 400 * n;
            u64 d = days_per_400_years_ * n;

            // Add 100-year cycles.
            n = y / 100;
            y -= 100 * n;
            d += days_per_100_years_ * n;

            // Add 4-year cycles.
            n = y / 4;
            y -= 4 * n;
            d += days_per_4_years_ * n;

            // Add remaining non-leap years.
            d += 365 * y;

            // Add days before this month.
            d += detail::days_before_zero_based_month(month - 1);
            if (is_leap_year(year) && month >= time::march)
            {
                ++d; // February 29
            }

            // Add days before today.
            // Safe overflow/wrap around for negative values.
            d = math::add_with_overflow(d, to_u64(day - 1));

            // Seconds since the absolute epoch.
            u64 abs = d * time::seconds_per_day<u64>;

            // Add seconds elapsed on the day.
            abs += to_u64(hour) * time::seconds_per_hour<u64> +
                   to_u64(min) * time::seconds_per_minute<u64> + to_u64(sec);

            // Avoid signed integer overflow, same assembly as:
            // to_i64(abs) + (abs_to_internal_ + internal_to_unix_);
            constexpr i64 abs_to_unix = abs_to_internal_ + internal_to_unix_;
            return to_i64(math::add_with_overflow(abs, to_u64(abs_to_unix)));
        }

        static constexpr year_week week_iso8601_(const u64 abs) noexcept
        {
            const auto ydoy = year_day_of_year_(abs);

            i32 year       = ydoy.year;
            const u16 yday = ydoy.day_of_year;

            const auto ymd = year_month_day_(year, yday);

            const u16 month = ymd.m;
            const u16 day   = ymd.d;

            // Day of week where Monday is 0.
            const auto wday = static_cast<u8>((day_of_week_(abs) + 6) % 7);

            // constexpr u8 mon = 0;
            constexpr u8 tue = 1;
            constexpr u8 wed = 2;
            constexpr u8 thu = 3;
            constexpr u8 fri = 4;
            constexpr u8 sat = 5;
            // constexpr u8 sun = 6;

            // "Calculate week as number of Mondays in year up to and including today, plus 1
            // because the first week is week 0. Putting the + 1 inside the numerator as a + 7 keeps
            // the numerator from being negative, which would cause it to round incorrectly."
            u32 week = ((u32{yday} + 7) - u32{wday}) / 7;

            // "The week number is now correct under the assumption that the first Monday of the
            // year is in week 1. If Jan 1 is a Tuesday, Wednesday, or Thursday, the first Monday is
            // actually in week 2."
            const auto jan1wday = static_cast<u16>(((wday + 7 * 53) - yday) % 7);
            if (jan1wday >= tue && jan1wday <= thu)
            {
                ++week;
            }

            // "If the week number is 0, we're in early January but in the last week of last year."
            if (week == 0)
            {
                --year;
                week = 52;
                // "A year has 53 weeks when Jan 1 or Dec 31 is a Thursday, meaning Jan 1 of the
                // next year is a Friday or it was a leap year and Jan 1 of the next year is a
                // Saturday."
                if (jan1wday == fri || (jan1wday == sat && is_leap_year(year)))
                {
                    ++week;
                }
            }

            // "December 29 to 31 are in week 1 of next year if they are after the last Thursday of
            // the year and December 31 is a Monday, Tuesday, or Wednesday."
            if (month == december && day >= 29 && wday < thu)
            {
                const auto dec31wday = static_cast<u8>(((wday + 31) - day) % 7);
                if (dec31wday <= wed)
                {
                    ++year;
                    week = 1;
                }
            }

            return year_week{year, week};
        }

        static constexpr year_day_of_year year_day_of_year_(const u64 abs) noexcept
        {
            // Days
            u64 d = abs / time::seconds_per_day<u64>;

            // "Account for 400-year cycles."
            u64 n = d / days_per_400_years_;
            u64 y = 400 * n;
            d -= days_per_400_years_ * n;

            // "Cut off 100-year cycles.
            // The last cycle has one extra leap year, so on the last day of that year, day /
            // daysPer100Years will be 4 instead of 3. Cut it back down to 3 by subtracting n>>2."
            n = d / days_per_100_years_;
            n -= n >> 2u;
            y += 100 * n;
            d -= days_per_100_years_ * n;

            // "Cut off 4-year cycles.
            // The last cycle has a missing leap year, which does not affect the computation."
            n = d / days_per_4_years_;
            y += 4 * n;
            d -= days_per_4_years_ * n;

            // "Cut off years within a 4-year cycle.
            // The last year is a leap year, so on the last day of that year, day / 365 will be 4
            // instead of 3.  Cut it back down to 3 by subtracting n>>2."
            n = d / 365;
            n -= n >> 2u;
            y += n;
            d -= 365 * n;

            const auto year        = static_cast<i32>(to_i64(y) + abs_zero_year_);
            const auto day_of_year = static_cast<u16>(d); // Zero based (0-365).

            return {year, day_of_year};
        }

        static constexpr year_month_day year_month_day_(const i32 year,
                                                        const u16 day_of_year) noexcept
        {
            u16 day = day_of_year;

            if (is_leap_year(year))
            {
                if (day > (31 + 29 - 1))
                {
                    // "After leap day, pretend it wasn't there."
                    --day;
                }
                else if (day == (31 + 29 - 1))
                {
                    // Leap day.
                    return year_month_day{year, 2, 29};
                }
            }

            // "Estimate month on assumption that every month has 31 days. The estimate may be too
            // low by at most one month, so adjust."
            u16 month     = day / 31; // 0-11 inclusive.
            const u16 end = detail::days_before_zero_based_month(month + 1);
            u16 begin     = end;
            if (day >= end)
            {
                ++month;
            }
            else
            {
                begin = detail::days_before_zero_based_month(month);
            }

            ++month; // Don't return zero based month.
            return year_month_day{year, month, static_cast<u16>((day - begin) + 1)};
        }
    };

    // ## Functions

    // ### now

    [[nodiscard]] inline point now() noexcept
    {
        return point{time::wall::since_epoch()};
    }

    // ### operator<<

    template <typename Buf>
    constexpr strcore<Buf>& operator<<(strcore<Buf>& lhs, const point& rhs)
    {
        rhs.format(format_string::quick, lhs);
        return lhs;
    }
}

// ## Specializations

// ### formatter

namespace snn
{
    template <>
    struct formatter<time::point>
    {
        using type = time::point;

        template <typename Buf>
        constexpr void format(const time::point p, const cstrview format_string,
                              const fmt::context&, strcore<Buf>& append_to, assume::no_overlap_t)
        {
            if (format_string.is_empty())
            {
                p.format(time::format_string::quick, append_to);
            }
            else
            {
                p.format(format_string, append_to, assume::no_overlap);
            }
        }
    };
}

// ### std::hash

template <>
struct std::hash<snn::time::point>
{
    [[nodiscard]] constexpr std::size_t operator()(const snn::time::point p) const noexcept
    {
        return p.hash();
    }
};
