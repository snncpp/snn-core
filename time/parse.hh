// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Parse functions

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/optional.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/time/core.hh"
#include "snn-core/time/error.hh"

namespace snn::time
{
    // ## Functions

    // ### parse_month_name

    // Relaxed parsing of month name or month abbreviation.

    [[nodiscard]] constexpr optional<u8> parse_month_name(const transient<cstrview> name) noexcept
    {
        const cstrview s = name.get();
        if (s.size() >= string_size("May") && s.size() <= string_size("September"))
        {
            const char c0 = chr::to_alpha_lower(s.at(0, assume::within_bounds));
            const char c1 = chr::to_alpha_lower(s.at(1, assume::within_bounds));
            const char c2 = chr::to_alpha_lower(s.at(2, assume::within_bounds));

            switch (c0)
            {
                // Apr, Aug
                case 'a':
                {
                    if (c1 == 'p')
                    {
                        return time::april;
                    }
                    if (c1 == 'u')
                    {
                        return time::august;
                    }
                    break;
                }

                // Dec
                case 'd':
                    return time::december;

                // Feb
                case 'f':
                    return time::february;

                // Jan, Jul, Jun
                case 'j':
                {
                    if (c1 == 'a')
                    {
                        return time::january;
                    }
                    if (c2 == 'l')
                    {
                        return time::july;
                    }
                    if (c2 == 'n')
                    {
                        return time::june;
                    }
                    break;
                }

                // Mar, May
                case 'm':
                {
                    if (c2 == 'r')
                    {
                        return time::march;
                    }
                    if (c2 == 'y')
                    {
                        return time::may;
                    }
                    break;
                }

                // Nov
                case 'n':
                    return time::november;

                // Oct
                case 'o':
                    return time::october;

                // Sep
                case 's':
                    return time::september;

                default:
                    break;
            }
        }
        return nullopt;
    }

    // ### parse_rfc822_zone_name

    // Get offset from a subset of the zone names defined in RFC 822.
    // "UTC" is accepted and the only military zone accepted is "Z".

    // "UT"  / "GMT" / "Z" / "UTC" = UTC
    // "EST" / "EDT"               = Eastern:  -5 / -4
    // "CST" / "CDT"               = Central:  -6 / -5
    // "MST" / "MDT"               = Mountain: -7 / -6
    // "PST" / "PDT"               = Pacific:  -8 / -7

    [[nodiscard]] constexpr optional<i32> parse_rfc822_zone_name(
        const transient<cstrview> name) noexcept
    {
        const cstrview s = name.get();
        if (s.size() == 1)
        {
            const char c0 = s.at(0, assume::within_bounds);
            if (c0 == 'Z' || c0 == 'z')
            {
                return 0; // Z
            }
        }
        else if (s.size() == 3)
        {
            const char c0 = chr::to_alpha_lower(s.at(0, assume::within_bounds));
            const char c1 = chr::to_alpha_lower(s.at(1, assume::within_bounds));
            const char c2 = chr::to_alpha_lower(s.at(2, assume::within_bounds));

            switch (c0)
            {
                case 'g':
                    if (c1 == 'm' && c2 == 't')
                    {
                        return 0; // GMT
                    }
                    break;

                case 'p':
                    if (c1 == 's' && c2 == 't')
                    {
                        return -8 * time::seconds_per_hour<i32>; // PST -8
                    }
                    else if (c1 == 'd' && c2 == 't')
                    {
                        return -7 * time::seconds_per_hour<i32>; // PDT -7
                    }
                    break;

                case 'e':
                    if (c1 == 's' && c2 == 't')
                    {
                        return -5 * time::seconds_per_hour<i32>; // EST -5
                    }
                    else if (c1 == 'd' && c2 == 't')
                    {
                        return -4 * time::seconds_per_hour<i32>; // EDT -4
                    }
                    break;

                case 'm':
                    if (c1 == 's' && c2 == 't')
                    {
                        return -7 * time::seconds_per_hour<i32>; // MST -7
                    }
                    else if (c1 == 'd' && c2 == 't')
                    {
                        return -6 * time::seconds_per_hour<i32>; // MDT -6
                    }
                    break;

                case 'c':
                    if (c1 == 's' && c2 == 't')
                    {
                        return -6 * time::seconds_per_hour<i32>; // CST -6
                    }
                    else if (c1 == 'd' && c2 == 't')
                    {
                        return -5 * time::seconds_per_hour<i32>; // CDT -5
                    }
                    break;

                case 'u':
                    if (c1 == 't' && c2 == 'c')
                    {
                        return 0; // UTC
                    }
                    break;

                default:
                    break;
            }
        }
        else if (s.size() == 2)
        {
            const char c0 = chr::to_alpha_lower(s.at(0, assume::within_bounds));
            const char c1 = chr::to_alpha_lower(s.at(1, assume::within_bounds));
            if (c0 == 'u' && c1 == 't')
            {
                return 0; // UT
            }
        }

        return nullopt;
    }

    // ### parse_without_validation

    // Non-strict ISO 8601/RFC 1123 date-time parser.

    // Will not validate the `time::ymdhmsno` structure before returning.

    [[nodiscard]] constexpr result<ymdhmsno> parse_without_validation(
        const transient<cstrview> datetime) noexcept
    {
        const cstrview s = datetime.get();

        if (s.size() > string_size("Wednesday, 19 September -9999 23:59:59.123456789 +00:00"))
        {
            return error::string_exceeds_max_size;
        }

        i32 year       = constant::limit<i32>::max; // Invalid by default.
        u16 month      = 1;
        u16 day        = 1;
        u16 hour       = 0;
        u8 minute      = 0;
        u8 second      = 0;
        u32 nanosecond = 0;
        i32 offset     = 0;

        i32 offset_multiplier = 1;

        enum parse_state : u8
        {
            s_year,
            s_month,
            s_day,
            s_hour,
            s_minute,
            s_second,
            s_fraction,
            s_offset,
            s_offset_name,
            s_offset_hour,
            s_offset_minute,
            s_weekday_name,
            s_day_first,
            s_month_name,
            s_year_last,
            s_trailing,
        };

        parse_state state = s_year;

        char date_separator = ' ';

        auto rng = s.range();
        while (rng)
        {
            const char c = rng.front(assume::not_empty);

            constexpr ascii::leading_zeros allow_leading_zeros = ascii::leading_zeros::allow;
            constexpr math::base dec                           = math::base::decimal;

            switch (state)
            {
                case s_year:
                {
                    if (chr::is_alpha(c))
                    {
                        state = s_weekday_name;
                        break;
                    }

                    i32 year_multiplier = 1;
                    if (c == '-')
                    {
                        rng.drop_front(assume::not_empty);
                        year_multiplier = -1;
                    }

                    const auto p = rng.pop_front_integral<u16, dec, 4>(allow_leading_zeros);

                    // 0000 to 9999.
                    if (p.count == 4 && !rng.has_front_if(chr::is_digit))
                    {
                        // -0000 is not allowed.
                        if (p.value > 0 || year_multiplier == 1)
                        {
                            year  = p.value * year_multiplier;
                            state = s_month;
                            break;
                        }
                    }

                    return error::invalid_year;
                }

                case s_month:
                {
                    if (c == '-')
                    {
                        rng.drop_front(assume::not_empty);
                        const auto p = rng.pop_front_integral<u8, dec, 2>(allow_leading_zeros);
                        if (p.count == 2 && !rng.has_front_if(chr::is_digit))
                        {
                            month = p.value;
                            state = s_day;
                            break;
                        }
                    }

                    return error::invalid_month;
                }

                case s_day:
                {
                    if (c == '-')
                    {
                        rng.drop_front(assume::not_empty);
                        const auto p = rng.pop_front_integral<u8, dec, 2>(allow_leading_zeros);
                        if (p.count == 2 && !rng.has_front_if(chr::is_digit))
                        {
                            day   = p.value;
                            state = s_hour;
                            break;
                        }
                    }

                    return error::invalid_day;
                }

                case s_hour:
                {
                    if (c == ' ' || c == 'T' || c == 't')
                    {
                        rng.drop_front(assume::not_empty);
                        const auto p = rng.pop_front_integral<u8, dec, 2>(allow_leading_zeros);
                        if (p.count == 2 && !rng.has_front_if(chr::is_digit))
                        {
                            hour  = p.value;
                            state = s_minute;
                            break;
                        }
                    }

                    return error::invalid_hour;
                }

                case s_minute:
                {
                    if (c == ':')
                    {
                        rng.drop_front(assume::not_empty);
                        const auto p = rng.pop_front_integral<u8, dec, 2>(allow_leading_zeros);
                        if (p.count == 2 && !rng.has_front_if(chr::is_digit))
                        {
                            minute = p.value;
                            state  = s_second;
                            break;
                        }
                    }

                    return error::invalid_minute;
                }

                case s_second:
                {
                    // Seconds are optional.
                    if (c == ':')
                    {
                        rng.drop_front(assume::not_empty);
                        const auto p = rng.pop_front_integral<u8, dec, 2>(allow_leading_zeros);
                        if (p.count == 2 && !rng.has_front_if(chr::is_digit))
                        {
                            second = p.value;
                        }
                        else
                        {
                            return error::invalid_second;
                        }
                    }

                    state = s_fraction;
                    break;
                }

                case s_fraction:
                {
                    // Fraction is optional.
                    if (c == '.')
                    {
                        rng.drop_front(assume::not_empty);
                        const auto p = rng.pop_front_integral<u32, dec, 9>(allow_leading_zeros);
                        if (p.count > 0 && !rng.has_front_if(chr::is_digit))
                        {
                            nanosecond = p.value;
                            for (usize scale = p.count; scale < 9; ++scale)
                            {
                                nanosecond *= 10;
                            }
                        }
                        else
                        {
                            return error::invalid_fraction;
                        }
                    }

                    state = s_offset;
                    break;
                }

                case s_offset:
                {
                    // Skip optional space.
                    if (c == ' ')
                    {
                        rng.drop_front(assume::not_empty);
                    }

                    if (rng)
                    {
                        if (rng.has_front_if(chr::is_alpha))
                        {
                            state = s_offset_name;
                            break;
                        }

                        if (rng.drop_front('+'))
                        {
                            state = s_offset_hour;
                            break;
                        }
                        else if (rng.drop_front('-'))
                        {
                            offset_multiplier = -1;
                            state             = s_offset_hour;
                            break;
                        }
                    }

                    return error::invalid_offset;
                }

                case s_offset_name:
                {
                    snn_should(chr::is_alpha(c));

                    const cstrview name{rng.pop_front_while(chr::is_alpha)};
                    const auto opt = parse_rfc822_zone_name(name);
                    if (opt)
                    {
                        offset = opt.value(assume::has_value);
                        if (offset < 0)
                        {
                            offset *= -1;
                            offset_multiplier = -1;
                        }
                    }
                    else
                    {
                        return error::invalid_zone_name;
                    }

                    state = s_trailing;
                    break;
                }

                case s_offset_hour:
                {
                    const auto p = rng.pop_front_integral<u16, dec, 4>(allow_leading_zeros);
                    if (p.count == 2)
                    {
                        offset = p.value * time::seconds_per_hour<i32>;
                        state  = s_offset_minute;
                        break;
                    }
                    else if (p.count == 4 && !rng.has_front_if(chr::is_digit))
                    {
                        offset = (p.value / 100) * time::seconds_per_hour<i32>;
                        offset += (p.value % 100) * time::seconds_per_minute<i32>;
                        state = s_trailing;
                        break;
                    }

                    return error::invalid_offset_hour;
                }

                case s_offset_minute:
                {
                    if (c == ':')
                    {
                        rng.drop_front(assume::not_empty);
                        const auto p = rng.pop_front_integral<u8, dec, 2>(allow_leading_zeros);
                        if (p.count == 2 && !rng.has_front_if(chr::is_digit))
                        {
                            offset += p.value * time::seconds_per_minute<i32>;
                            state = s_trailing;
                            break;
                        }
                    }

                    return error::invalid_offset_minute;
                }

                case s_weekday_name:
                {
                    snn_should(chr::is_alpha(c));

                    // Skip alpha characters "Sun", "Sunday" etc.
                    const usize name_size = rng.pop_front_while(chr::is_alpha).count();
                    if (name_size >= string_size("Wed") && name_size <= string_size("Wednesday"))
                    {
                        state = s_day_first;
                        break;
                    }

                    return error::invalid_weekday_name;
                }

                case s_day_first:
                {
                    // Optional comma.
                    if (c == ',')
                    {
                        rng.drop_front(assume::not_empty);
                    }

                    if (rng.drop_front(' '))
                    {
                        const auto p = rng.pop_front_integral<u8, dec, 2>(allow_leading_zeros);
                        if (p.count == 1 || (p.count == 2 && !rng.has_front_if(chr::is_digit)))
                        {
                            day   = p.value;
                            state = s_month_name;
                            break;
                        }
                    }

                    return error::invalid_day;
                }

                case s_month_name:
                {
                    if (c == ' ' || c == '-')
                    {
                        date_separator = c;
                        rng.drop_front(assume::not_empty);
                        const auto name_rng = rng.pop_front_while(chr::is_alpha);
                        if (name_rng)
                        {
                            month = parse_month_name(cstrview{name_rng}).value_or_default();
                            if (month)
                            {
                                state = s_year_last;
                                break;
                            }
                        }
                    }

                    return error::invalid_month_name;
                }

                case s_year_last:
                {
                    if (c == date_separator)
                    {
                        rng.drop_front(assume::not_empty);

                        i16 year_multiplier = 1;
                        if (rng.drop_front('-'))
                        {
                            year_multiplier = -1;
                        }

                        const auto p = rng.pop_front_integral<u16, dec, 4>(allow_leading_zeros);
                        // 0000 to 9999.
                        if (p.count == 4 && !rng.has_front_if(chr::is_digit))
                        {
                            // -0000 is not allowed.
                            if (p.value > 0 || year_multiplier == 1)
                            {
                                year  = p.value * year_multiplier;
                                state = s_hour;
                                break;
                            }
                        }
                        // 00 to 99.
                        else if (p.count == 2 && year_multiplier == 1)
                        {
                            year = p.value;
                            // Posix rules: 69-99 = 1969-1999, 00-68 = 2000-2068
                            if (year >= 69)
                            {
                                year += 1900;
                            }
                            else
                            {
                                year += 2000;
                            }
                            state = s_hour;
                            break;
                        }
                    }

                    return error::invalid_year;
                }

                case s_trailing:
                    return error::trailing_characters;
            }
        }

        offset *= offset_multiplier;

        return ymdhmsno{year, month, day, hour, minute, second, nanosecond, offset};
    }

    // ### parse

    // Non-strict ISO 8601/RFC 1123 date-time parser.

    // Leap seconds are invalid by default, when discarded hh::mm::60 is replaced with hh::mm::59.

    enum class leap_second : u8
    {
        invalid,
        discard,
    };

    [[nodiscard]] constexpr result<ymdhmsno> parse(
        const transient<cstrview> datetime,
        const leap_second ls_policy = leap_second::invalid) noexcept
    {
        auto [combined, err] = parse_without_validation(datetime).unpack();
        if (err)
        {
            return err;
        }

        if (ls_policy == leap_second::discard)
        {
            if (combined.hms.s == 60)
            {
                combined.hms.s = 59;
            }
        }

        const auto res = combined.validate();
        if (res)
        {
            return combined;
        }
        return res.error_code();
    }
}
