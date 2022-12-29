// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/parse.hh"

#include "snn-core/unittest.hh"
#include "snn-core/ascii/lower.hh"
#include "snn-core/ascii/upper.hh"
#include "snn-core/random/number.hh"
#include "snn-core/time/point.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(time::parse_month_name("january").value() == time::january);
            snn_require(time::parse_month_name("JAN").value() == time::january);
            snn_require(time::parse_month_name("jaX").value() == time::january);
            snn_require(!time::parse_month_name("XaX"));

            snn_require(time::parse_rfc822_zone_name("GMT").value() == 0);
            snn_require(time::parse_rfc822_zone_name("CST").value() == -21'600);
            snn_require(!time::parse_rfc822_zone_name("ABC"));

            const cstrview with_leap_second = "2001-02-03 04:05:60";

            snn_require(!time::parse(with_leap_second));
            snn_require(!time::parse(with_leap_second, time::leap_second::invalid));

            time::ymdhmsno v = time::parse(with_leap_second, time::leap_second::discard).value();
            snn_require(v.is_valid());
            snn_require(v == time::ymdhmsno{2001, 2, 3, 4, 5, 59, 0, 0});

            v = time::parse_without_validation(with_leap_second).value();
            snn_require(!v.is_valid());
            snn_require(v.validate().error_code() == time::error::invalid_second);
            snn_require(v == time::ymdhmsno{2001, 2, 3, 4, 5, 60, 0, 0});

            return true;
        }

        i64 to_unix(const cstrview datetime)
        {
            const i64 a = time::point{time::parse(datetime).value()}.unix();

            const str lo = ascii::lower(datetime);
            const str up = ascii::upper(datetime);

            const i64 b = time::point{time::parse(lo).value()}.unix();
            const i64 c = time::point{time::parse(up).value()}.unix();

            if (a == b && b == c)
            {
                return a;
            }

            throw std::runtime_error{"Case mismatch."};
        }

        bool is_equal(const str& datetime, const i64 unixtime)
        {
            try
            {
                if (to_unix(datetime) == unixtime)
                {
                    return true;
                }

                throw std::runtime_error{"Not equal."};
            }
            catch (...)
            {
                fmt::print_error("{}\n{}\n", datetime, unixtime);
                throw;
            }
        }

        constexpr time::error to_error(const cstrview datetime)
        {
            const auto r = time::parse(datetime);
            if (!r)
            {
                const i32 error_value = r.error_code().value();
                static_assert(static_cast<i32>(time::error::no_error) == 0);
                if (error_value > static_cast<i32>(time::error::no_error) &&
                    error_value < static_cast<i32>(time::error_count))
                {
                    return static_cast<time::error>(error_value);
                }
                throw std::runtime_error{"Error value is not within enum range."};
            }
            return time::error::no_error;
        }

        constexpr bool test_parse_month_name()
        {
            static_assert(time::parse_month_name("january") == time::january);
            static_assert(time::parse_month_name("february") == time::february);
            static_assert(time::parse_month_name("march") == time::march);
            static_assert(time::parse_month_name("april") == time::april);
            static_assert(time::parse_month_name("may") == time::may);
            static_assert(time::parse_month_name("june") == time::june);
            static_assert(time::parse_month_name("july") == time::july);
            static_assert(time::parse_month_name("august") == time::august);
            static_assert(time::parse_month_name("september") == time::september);
            static_assert(time::parse_month_name("october") == time::october);
            static_assert(time::parse_month_name("november") == time::november);
            static_assert(time::parse_month_name("december") == time::december);

            static_assert(time::parse_month_name("JAN") == time::january);
            static_assert(time::parse_month_name("FEB") == time::february);
            static_assert(time::parse_month_name("MAR") == time::march);
            static_assert(time::parse_month_name("APR") == time::april);
            static_assert(time::parse_month_name("MAY") == time::may);
            static_assert(time::parse_month_name("JUN") == time::june);
            static_assert(time::parse_month_name("JUL") == time::july);
            static_assert(time::parse_month_name("AUG") == time::august);
            static_assert(time::parse_month_name("SEP") == time::september);
            static_assert(time::parse_month_name("OCT") == time::october);
            static_assert(time::parse_month_name("NOV") == time::november);
            static_assert(time::parse_month_name("DEC") == time::december);

            static_assert(time::parse_month_name("jaX") == time::january);
            static_assert(time::parse_month_name("fXX") == time::february);
            static_assert(time::parse_month_name("mXr") == time::march);
            static_assert(time::parse_month_name("apX") == time::april);
            static_assert(time::parse_month_name("mXy") == time::may);
            static_assert(time::parse_month_name("jXn") == time::june);
            static_assert(time::parse_month_name("jXl") == time::july);
            static_assert(time::parse_month_name("auX") == time::august);
            static_assert(time::parse_month_name("sXX") == time::september);
            static_assert(time::parse_month_name("oXX") == time::october);
            static_assert(time::parse_month_name("nXX") == time::november);
            static_assert(time::parse_month_name("dXX") == time::december);

            static_assert(!time::parse_month_name(""));
            static_assert(!time::parse_month_name("j"));
            static_assert(!time::parse_month_name("jX"));
            static_assert(!time::parse_month_name("jXX"));
            static_assert(!time::parse_month_name("septemberx"));

            return true;
        }

        constexpr bool test_parse_rfc822_zone_name()
        {
            static_assert(time::parse_rfc822_zone_name("Z").value() == 0);
            static_assert(time::parse_rfc822_zone_name("UT").value() == 0);
            static_assert(time::parse_rfc822_zone_name("GMT").value() == 0);
            static_assert(time::parse_rfc822_zone_name("UTC").value() == 0);
            static_assert(time::parse_rfc822_zone_name("EST").value() == -18'000);
            static_assert(time::parse_rfc822_zone_name("EDT").value() == -14'400);
            static_assert(time::parse_rfc822_zone_name("CST").value() == -21'600);
            static_assert(time::parse_rfc822_zone_name("CDT").value() == -18'000);
            static_assert(time::parse_rfc822_zone_name("MST").value() == -25'200);
            static_assert(time::parse_rfc822_zone_name("MDT").value() == -21'600);
            static_assert(time::parse_rfc822_zone_name("PST").value() == -28'800);
            static_assert(time::parse_rfc822_zone_name("PDT").value() == -25'200);

            static_assert(!time::parse_rfc822_zone_name(""));
            static_assert(!time::parse_rfc822_zone_name("A"));
            static_assert(!time::parse_rfc822_zone_name("AB"));
            static_assert(!time::parse_rfc822_zone_name("UTZ"));
            static_assert(!time::parse_rfc822_zone_name("ZZZZ"));

            return true;
        }

        constexpr bool test_parse_without_validation()
        {
            constexpr cstrview with_leap_second = "2001-02-03 04:05:60";

            static_assert(!time::parse(with_leap_second));
            static_assert(!time::parse(with_leap_second, time::leap_second::invalid));
            static_assert(time::parse(with_leap_second, time::leap_second::discard));

            constexpr time::ymdhmsno v = time::parse_without_validation(with_leap_second).value();
            static_assert(!v.is_valid());
            static_assert(v.validate().error_code() == time::error::invalid_second);
            static_assert(v.second() == 60);

            return true;
        }

        constexpr bool test_parse()
        {
            static_assert(time::parse("1970-01-01 00:00:00 +0000") == time::ymdhmsno{1970, 1, 1});
            static_assert(time::parse("2001-02-03 04:05:06 +0789") ==
                          time::ymdhmsno{2001, 2, 3, 4, 5, 6, 0, 30'540});
            static_assert(time::parse("2001-02-03 04:05:06 -07:89") ==
                          time::ymdhmsno{2001, 2, 3, 4, 5, 6, 0, -30'540});

            static_assert(time::parse("2016") == time::ymdhmsno{2016, 1, 1});
            static_assert(time::parse("2016-10") == time::ymdhmsno{2016, 10, 1});
            static_assert(time::parse("2016-10-08") == time::ymdhmsno{2016, 10, 8});
            static_assert(time::parse("2016-10-08 01:22") == time::ymdhmsno{2016, 10, 8, 1, 22, 0});
            static_assert(time::parse("2016-10-08 01:22:43") ==
                          time::ymdhmsno{2016, 10, 8, 1, 22, 43});

            static_assert(time::parse("2016-10-08 01:22.5") ==
                          time::ymdhmsno{2016, 10, 8, 1, 22, 0, 500'000'000, 0});
            static_assert(time::parse("2016-10-08 01:22:43.5") ==
                          time::ymdhmsno{2016, 10, 8, 1, 22, 43, 500'000'000, 0});

            static_assert(time::parse("2016-10-08 01:22:03.000001") ==
                          time::ymdhmsno{2016, 10, 8, 1, 22, 3, 1'000, 0});

            static_assert(time::parse("Abc, 22 Feb 2068") == time::ymdhmsno{2068, 2, 22});
            static_assert(time::parse("Abc, 22-Feb-2068") == time::ymdhmsno{2068, 2, 22});

            static_assert(time::parse("Abc, 22 Feb -2068") == time::ymdhmsno{-2068, 2, 22});
            static_assert(time::parse("Abc, 22-Feb--2068") == time::ymdhmsno{-2068, 2, 22});

            constexpr cstrview longest{"Wednesday, 19 September -9999 23:59:59.123456789 +03:21"};
            static_assert(time::parse(longest) ==
                          time::ymdhmsno{-9999, 9, 19, 23, 59, 59, 123'456'789, 12'060});

            // Non-leap second and leap second (invalid by default).
            static_assert(time::parse("2016-12-31 23:59:59") ==
                          time::ymdhmsno{2016, 12, 31, 23, 59, 59});
            static_assert(!time::parse("2016-12-31 23:59:60"));
            static_assert(time::parse("2016-12-31 23:59:60", time::leap_second::discard) ==
                          time::ymdhmsno{2016, 12, 31, 23, 59, 59});

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_parse_month_name());
        snn_static_require(app::test_parse_rfc822_zone_name());
        snn_static_require(app::test_parse_without_validation());
        snn_static_require(app::test_parse());

        {
            using time::error;

            // No error.
            static_assert(app::to_error("2001-02-03 04:05:06 +0789") == error::no_error);
            static_assert(app::to_error("2001-02-03 04:05:06.123 +0789") == error::no_error);
            static_assert(app::to_error("2001-02-03 04:05.123 +0789") == error::no_error);
            static_assert(app::to_error("Abc, 22 Feb 3099 09:02:07") == error::no_error);
            static_assert(app::to_error("Abc, 22 Feb -2345 09:02:07") == error::no_error);
            static_assert(app::to_error("3099-01-01T00:00:00") == error::no_error);
            static_assert(app::to_error("-2000-01-01T00:00:00") == error::no_error);

            // Empty
            static_assert(app::to_error("") == error::invalid_year);

            // Garbage
            static_assert(app::to_error("-") == error::invalid_year);
            static_assert(app::to_error(" ") == error::invalid_year);
            static_assert(app::to_error("  ") == error::invalid_year);
            static_assert(app::to_error("\0") == error::invalid_year);
            static_assert(app::to_error("\0\0") == error::invalid_year);

            // Invalid year.
            static_assert(app::to_error("Abc, 22 Feb 12345 09:02:07") == error::invalid_year);
            static_assert(app::to_error("Abc, 22 Feb 123 09:02:07") == error::invalid_year);
            static_assert(app::to_error("Abc, 22 Feb 1 09:02:07") == error::invalid_year);
            static_assert(app::to_error("Abc, 22 Feb 0 09:02:07") == error::invalid_year);
            static_assert(app::to_error("Abc, 22 Feb - 09:02:07") == error::invalid_year);
            static_assert(app::to_error("Abc, 22 Feb -09:02:07") == error::invalid_year);
            static_assert(app::to_error("Abc, 22 Feb -0000 09:02:07") == error::invalid_year);
            static_assert(app::to_error("Abc, 22 Feb -000 09:02:07") == error::invalid_year);
            static_assert(app::to_error("Abc, 22 Feb -00 09:02:07") == error::invalid_year);
            static_assert(app::to_error("Abc, 22 Feb -0 09:02:07") == error::invalid_year);
            static_assert(app::to_error("Abc, 22 Feb -68 09:02:07") == error::invalid_year);
            static_assert(app::to_error("Abc, 22 Feb -0000 09:02:07") == error::invalid_year);
            static_assert(app::to_error("Abc, 22 Feb -00 09:02:07") == error::invalid_year);
            static_assert(app::to_error("Abc, 22-Feb -68") == error::invalid_year);
            static_assert(app::to_error("Abc, 22-Feb--68") == error::invalid_year);
            static_assert(app::to_error("Abc, 22-Feb -2068") == error::invalid_year);
            static_assert(app::to_error("Abc, 22 Feb--2068") == error::invalid_year);
            static_assert(app::to_error("12345-01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("123-01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("12-01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("1-01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("--01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("---01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("-0000-01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("-000-01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("-00-01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("-0-01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("000-01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("00-01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("0-01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("-01-01T00:00:00") == error::invalid_year);
            static_assert(app::to_error("01-01T00:00:00") == error::invalid_year);

            // Invalid month.
            static_assert(app::to_error("Wed, 29 Xeb 2068 09:02:07 EST") ==
                          error::invalid_month_name);
            static_assert(app::to_error("2068-00-29 09:02:07 EST") == error::invalid_month);
            static_assert(app::to_error("2068-13-29 09:02:07 EST") == error::invalid_month);

            // Invalid day.
            static_assert(app::to_error("Wed, 30 Feb 2068 09:02:07 EST") == error::invalid_day);
            static_assert(app::to_error("2068-02-30 09:02:07 EST") == error::invalid_day);

            // Invalid hour.
            static_assert(app::to_error("Wed, 29 Feb 2068 24:02:07 EST") == error::invalid_hour);
            static_assert(app::to_error("2068-02-29 24:02:07 EST") == error::invalid_hour);
            // 09 is parsed as year:
            static_assert(app::to_error("Abc, 22 Feb 09:02:07") == error::invalid_hour);

            // Invalid minute.
            static_assert(app::to_error("Wed, 29 Feb 2068 09:60:07 EST") == error::invalid_minute);
            static_assert(app::to_error("2068-02-29 09:60:07 EST") == error::invalid_minute);

            // Invalid second.
            static_assert(app::to_error("Wed, 29 Feb 2068 09:02:60 EST") == error::invalid_second);
            static_assert(app::to_error("2068-02-29 09:02:60 EST") == error::invalid_second);
            static_assert(app::to_error("Wed, 29 Feb 2068 09:02:61 EST") == error::invalid_second);
            static_assert(app::to_error("2068-02-29 09:02:61 EST") == error::invalid_second);

            // Invalid fraction.
            static_assert(app::to_error("2001-02-03 04:05:06.") == error::invalid_fraction);
            static_assert(app::to_error("2001-02-03 04:05:06. ") == error::invalid_fraction);
            static_assert(app::to_error("2001-02-03 04:05:06. +0789") == error::invalid_fraction);
            static_assert(app::to_error("2001-02-03 04:05:06.. +0789") == error::invalid_fraction);
            static_assert(app::to_error("2001-02-03 04:05:06.+0789") == error::invalid_fraction);
            static_assert(app::to_error("2001-02-03 04:05:06..+0789") == error::invalid_fraction);
            static_assert(app::to_error("2001-02-03 04:05:06.1234567890") ==
                          error::invalid_fraction);

            // Invalid offset.
            static_assert(app::to_error("2001-02-03 04:05:06.0. +0789") == error::invalid_offset);
            static_assert(app::to_error("2001-02-03 04:05:06.0.+0789") == error::invalid_offset);
            static_assert(app::to_error("2000-01-01T00:00:00+1a00") == error::invalid_offset_hour);
            static_assert(app::to_error("2000-01-01T00:00:00+10:0r") ==
                          error::invalid_offset_minute);
            static_assert(app::to_error("2000-01-01T00:00:00+1801") == error::invalid_offset);
            static_assert(app::to_error("2000-12-31T23:59:59-1801") == error::invalid_offset);
            static_assert(app::to_error("Abc, 22 Feb 2068 09:02:07 ") == error::invalid_offset);
            static_assert(app::to_error("Abc, 22 Feb 2068 09:02:07  ") == error::invalid_offset);

            // Invalid time zone.
            static_assert(app::to_error("Abc, 22 Feb 2068 09:02:07 X") == error::invalid_zone_name);
            static_assert(app::to_error("Abc, 22 Feb 2068 09:02:07 XY") ==
                          error::invalid_zone_name);
            static_assert(app::to_error("Abc, 22 Feb 2068 09:02:07 XYZ") ==
                          error::invalid_zone_name);
            static_assert(app::to_error("Abc, 22 Feb 2068 09:02:07 ZXYZ") ==
                          error::invalid_zone_name);

            // Trailing whitespace.
            static_assert(app::to_error("2000-01-01T00:00:00+0500 ") == error::trailing_characters);
            static_assert(app::to_error("2000-01-01T00:00:00+0500x") == error::trailing_characters);
            static_assert(app::to_error("Abc, 22 Feb 2068 09:02:07 GMT9") ==
                          error::trailing_characters);
            static_assert(app::to_error("Abc, 22 Feb 2068 09:02:07 GMT ") ==
                          error::trailing_characters);

            constexpr cstrview too_long{"Wednesday, 19 September -9999 23:59:59.1234567890 +00:00"};
            static_assert(app::to_error(too_long) == error::string_exceeds_max_size);
        }

        {
            snn_require(app::to_unix("-9999-01-01T00:00:00+1600") == -377705174400);
            snn_require(app::to_unix("9999-12-31T23:59:59-1600") == 253402358399);
        }

        {
            snn_require(app::to_unix("0001-02-03T04:05:06Z") == -62132730894);

            // clang-format off
            constexpr array dates{
                cstrview{"0001-02-03"},
                "Abc, 03 Feb 0001",
                "Abc, 03-Feb-0001",
                "Abc 03 Feb 0001",
                "Abc 03-Feb-0001",
                "Abcdef, 03 February 0001",
                "Abcdef, 03-February-0001",
                "Abcdef 03 February 0001",
                "Abcdef 03-February-0001",
                "Abc, 3 Feb 0001",
                "Abc, 3-Feb-0001",
                "Abc 3 Feb 0001",
                "Abc 3-Feb-0001",
                "Abcdef, 3 February 0001",
                "Abcdef, 3-February-0001",
                "Abcdef 3 February 0001",
                "Abcdef 3-February-0001",
            };
            constexpr array separators{
                cstrview{" "},
                "T",
            };
            constexpr array times{
                cstrview{"04:05:06"},
                "04:05:06.0",
                "04:05:06.12",
                "04:05:06.999",
            };
            constexpr array offsets{
                cstrview{""},
                "+0000",
                "+00:00",
                "+00",
                "Z",
                "UT",
                "UTC",
                "GMT",
                " +0000",
                " +00:00",
                " +00",
                " Z",
                " UT",
                " UTC",
                " GMT",
            };
            // clang-format on

            strbuf s{container::reserve, 64};
            usize count = 0;
            for (const auto& date : dates)
            {
                for (const auto& sep : separators)
                {
                    for (const auto& time : times)
                    {
                        for (const auto& offset : offsets)
                        {
                            s.clear();
                            s << date << sep << time << offset;
                            snn_require(app::to_unix(s) == -62132730894);
                            ++count;
                        }
                    }
                }
            }
            snn_require(count == 2'040);
        }

        {
            snn_require(app::to_unix("2014-03-30T01:39:15+01:00") == 1396139955);

            // clang-format off
            constexpr array dates{
                cstrview{"2014-03-30"},
                "Sun, 30 Mar 2014",
                "Sun, 30-Mar-2014",
                "Sun 30 Mar 2014",
                "Sun 30-Mar-2014",
                "Sunday, 30 March 2014",
                "Sunday, 30-March-2014",
                "Sunday 30 March 2014",
                "Sunday 30-March-2014",
            };
            constexpr array separators{
                cstrview{" "},
                "T",
            };
            constexpr array times{
                cstrview{"01:39:15"},
                "01:39:15.0",
                "01:39:15.12",
                "01:39:15.999",
            };
            constexpr array offsets{
                cstrview{"+0100"},
                "+01:00",
                "+01",
                " +0100",
                " +01:00",
                " +01",
            };
            // clang-format on

            strbuf s{container::reserve, 64};

            usize count = 0;
            for (const auto& date : dates)
            {
                for (const auto& sep : separators)
                {
                    for (const auto& time : times)
                    {
                        for (const auto& offset : offsets)
                        {
                            s.clear();
                            s << date << sep << time << offset;
                            snn_require(app::to_unix(s) == 1396139955);
                            ++count;
                        }
                    }
                }
            }
            snn_require(count == 432);
        }

        {
            snn_require(app::to_unix("Wed, 29 Feb 2068 09:02:07 EST") == 3097749727);

            // clang-format off
            constexpr array dates{
                cstrview{"2068-02-29"},
                "Wed, 29 Feb 68",
                "Wed, 29-Feb-68",
                "Wed, 29 Feb 2068",
                "Wed, 29-Feb-2068",
                "Wed 29 Feb 68",
                "Wed 29-Feb-68",
                "Wed 29 Feb 2068",
                "Wed 29-Feb-2068",
                "Wednesday, 29 February 68",
                "Wednesday, 29-February-68",
                "Wednesday, 29 February 2068",
                "Wednesday, 29-February-2068",
                "Wednesday 29 February 68",
                "Wednesday 29-February-68",
                "Wednesday 29 February 2068",
                "Wednesday 29-February-2068",
            };

            constexpr array separators{
                cstrview{" "},
                "T",
            };
            constexpr array times{
                cstrview{"09:02:07"},
                "09:02:07.9",
                "09:02:07.45",
                "09:02:07.321",
            };
            constexpr array offsets{
                cstrview{"-0500"},
                "-05:00",
                "-05",
                "EST",
                " -0500",
                " -05:00",
                " -05",
                " EST",
            };
            // clang-format on

            strbuf s{container::reserve, 64};

            usize count = 0;
            for (const auto& date : dates)
            {
                for (const auto& sep : separators)
                {
                    for (const auto& time : times)
                    {
                        for (const auto& offset : offsets)
                        {
                            s.clear();
                            s << date << sep << time << offset;
                            snn_require(app::to_unix(s) == 3097749727);
                            ++count;
                        }
                    }
                }
            }
            snn_require(count == 1'088);
        }

        {
            snn_require(app::to_unix("Wed, 31 Dec 1969 23:59:59 EST") == 17999);

            // clang-format off
            constexpr array dates{
                cstrview{"1969-12-31"},
                "Wed, 31 Dec 69",
                "Wed, 31-Dec-69",
                "Wed, 31 Dec 1969",
                "Wed, 31-Dec-1969",
                "Wed 31 Dec 69",
                "Wed 31-Dec-69",
                "Wed 31 Dec 1969",
                "Wed 31-Dec-1969",
                "Wednesday, 31 December 69",
                "Wednesday, 31-December-69",
                "Wednesday, 31 December 1969",
                "Wednesday, 31-December-1969",
                "Wednesday 31 December 69",
                "Wednesday 31-December-69",
                "Wednesday 31 December 1969",
                "Wednesday 31-December-1969",
            };

            constexpr array separators{
                cstrview{" "},
                "T",
            };
            constexpr array times{
                cstrview{"23:59:59"},
                "23:59:59.9",
                "23:59:59.45",
                "23:59:59.321",
            };
            constexpr array offsets{
                cstrview{"-0500"},
                "-05:00",
                "-05",
                "EST",
                " -0500",
                " -05:00",
                " -05",
                " EST",
            };
            // clang-format on

            strbuf s{container::reserve, 64};

            usize count = 0;
            for (const auto& date : dates)
            {
                for (const auto& sep : separators)
                {
                    for (const auto& time : times)
                    {
                        for (const auto& offset : offsets)
                        {
                            s.clear();
                            s << date << sep << time << offset;
                            snn_require(app::to_unix(s) == 17999);
                            ++count;
                        }
                    }
                }
            }
            snn_require(count == 1'088);
        }

        {
            for (loop::count lc{1000}; lc--;)
            {
                const auto y = random::number<i16>(-9999, 10000);
                const auto m = random::number<u8>(1, 13);
                const auto d =
                    random::number<u8>(1, time::days_in_month(y, m).value_or_default() + 1);
                const auto h = random::number<u8>(0, 24);
                const auto i = random::number<u8>(0, 60);
                const auto s = random::number<u8>(0, 61);

                time::point p{y, m, d, h, i, s};
                const auto u = p.unix();

                snn_require(app::is_equal(p.format(time::format_string::iso8601), u));
                snn_require(app::is_equal(p.format(time::format_string::local), u));
                snn_require(app::is_equal(p.format(time::format_string::quick), u));
                snn_require(app::is_equal(p.format(time::format_string::rfc2822), u));
                snn_require(app::is_equal(p.format(time::format_string::rfc3339), u));

                snn_require(app::is_equal(p.format(time::format_string::iso8601_z), u));
                snn_require(app::is_equal(p.format(time::format_string::rfc1123), u));
            }
        }
    }
}
