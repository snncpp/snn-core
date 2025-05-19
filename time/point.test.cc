// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/point.hh"

#include "snn-core/unittest.hh"
#include "snn-core/random/number.hh"
#include "snn-core/time/zone/db/utc.hh"
#include "snn-core/time/zone/db/america/los_angeles.hh"
#include "snn-core/time/zone/db/australia/eucla.hh"
#include "snn-core/time/zone/db/europe/london.hh"
#include "snn-core/time/zone/db/europe/stockholm.hh"
#include <time.h> // gmtime_r, timegm

namespace snn::app
{
    namespace
    {
        bool example()
        {
            {
                time::point p = time::now();
                snn_require(p.year() >= 2022);
            }
            {
                time::point p; // Epoch
                snn_require(p.date() == time::year_month_day{1970, 1, 1});
                snn_require(p.time() == time::hour_minute_second{0, 0, 0});
                snn_require(p.unix() == 0);
                snn_require(p.nanosecond() == 0);
            }
            {
                time::point p{2022, 8, 18, 14, 32, 59}; // UTC

                auto sthlm = time::zone::db::europe::stockholm.location();

                // Quick format.
                snn_require(p.format("q") == "2022-08-18 14:32:59 +0000");
                snn_require(p.format("q", sthlm) == "2022-08-18 16:32:59 +0200");

                // ISO 8601 format.
                using time::format_string::iso8601;
                snn_require(p.format(iso8601) == "2022-08-18T14:32:59+00:00");
                snn_require(p.format(iso8601, sthlm) == "2022-08-18T16:32:59+02:00");

                // Offset
                snn_require(p.offset(sthlm).abbr() == "CEST");
                snn_require(p.offset(sthlm).seconds() == 7'200);

                // Addition
                p.add(time::hours{1});

                snn_require(p.format("q", sthlm) == "2022-08-18 17:32:59 +0200");
            }

            return true;
        }

        constexpr bool test_point()
        {
            auto eucla  = time::zone::db::australia::eucla.location();
            auto la     = time::zone::db::america::los_angeles.location();
            auto london = time::zone::db::europe::london.location();
            auto sthlm  = time::zone::db::europe::stockholm.location();
            auto utc    = time::zone::db::utc.location();

            static_assert(std::is_trivially_copyable_v<time::point>);
            static_assert(sizeof(time::point) == 16);

            // point{}
            {
                time::point p;
                snn_require(p.unix() == 0);
                snn_require(p.nanosecond() == 0);
                snn_require(p.date() == time::year_month_day{1970, 1, 1});
                snn_require(p.time() == time::hour_minute_second{0, 0, 0});
            }

            // point{unixtime}
            {
                time::point p{1396139955};
                snn_require(p.unix() == 1396139955);
                snn_require(p.nanosecond() == 0);
                snn_require(p.date() == time::year_month_day{2014, 3, 30});
                snn_require(p.time() == time::hour_minute_second{0, 39, 15});

                // Safe max (that will not overflow year).
                time::point max{67767976233532799};
                snn_require(max.unix() == 67767976233532799);
                snn_require(max.date() == time::year_month_day{2147483647, 12, 31});
                snn_require(max.time() == time::hour_minute_second{23, 59, 59});

                // Safe min (that will not overflow year).
                time::point min{-67768100567971200};
                snn_require(min.unix() == -67768100567971200);
                snn_require(min.date() == time::year_month_day{-2147483648, 1, 1});
                snn_require(min.time() == time::hour_minute_second{0, 0, 0});
            }

            // point(duration)
            {
                time::point p{time::duration{1396139955, 500'000'000}};
                snn_require(p.unix() == 1396139955);
                snn_require(p.nanosecond() == 500'000'000);

                decltype(auto) d = p.duration();
                static_assert(std::is_same_v<decltype(d), time::duration>);
                snn_require(d.seconds() == 1396139955);
                snn_require(d.nanoseconds() == 500'000'000);
            }

            // point{y, mon, d}
            // point{y, mon, d, h, min, s}
            // point{y, mon, d, h, min, s, nano, offset}
            // point(year_month_day)
            // point(year_month_day_hour_minute_second)
            {
                snn_require(time::point{2016, 2, 6, 11, 40, 31}.unix() == 1454758831);
                snn_require(time::point{2016, 2, 6, 11, 40, 31, 0, 3600}.unix() == 1454755231);
                snn_require(time::point{2016, 2, 6, 11, 40, 31, 0, -3600}.unix() == 1454762431);
                snn_require(time::point{2016, 2, 6}.unix() == 1454716800);
                snn_require(time::point{-2321, 1, 1, 9, 31, 21}.unix() == -135410797719);
                snn_require(time::point{-3816, 2, 6, 11, 40, 31}.unix() == -182585362769);

                snn_require(time::point{time::ymd{2016, 2, 6}} == time::point{2016, 2, 6});
                snn_require(time::point{time::ymdhms{2016, 2, 6, 7, 8, 9}} ==
                            time::point{2016, 2, 6, 7, 8, 9});

                // Non-leap year gets normalized.
                snn_require(time::point{time::ymd{2017, 2, 29}} == time::point{2017, 3, 1});

                snn_require(time::point{2016, 2, 6, 11, 40, 31, 250'000'000, 3600}.duration() ==
                            time::duration{1454755231, 250'000'000});
                snn_require(time::point{2016, 2, 6, 11, 40, 31, 500'000'000, -3600}.duration() ==
                            time::duration{1454762431, 500'000'000});

                snn_require(time::point{2016, 2, 6, 11, 40, 31, 2'000'000'000, 3600}.duration() ==
                            time::duration{1454755233, 0});
                snn_require(time::point{2016, 2, 6, 11, 40, 31, -5'500'000'000, -3600}.duration() ==
                            time::duration{1454762425, 500'000'000});

                snn_require(time::point{2017, 5, 0} == time::point{2017, 4, 30});
                snn_require(time::point{2017, -1, 15} == time::point{2016, 11, 15});
            }

            // point{year_month_day_hour_minute_second_nanosecond_offset}
            {
                static_assert(
                    std::is_same_v<time::year_month_day_hour_minute_second_nanosecond_offset,
                                   time::ymdhmsno>);
                time::ymdhmsno c1{2016, 2, 6, 11, 40, 31, 300'000'000, 0};
                time::ymdhmsno c2{2016, 2, 6, 11, 40, 31, 500'000'000, 3600};
                time::ymdhmsno c3{2016, 2, 6, 11, 40, 31, 0, -3600};
                snn_require(time::point{c1}.duration() == time::duration{1454758831, 300'000'000});
                snn_require(time::point{c2}.duration() == time::duration{1454755231, 500'000'000});
                snn_require(time::point{c3}.duration() == time::duration{1454762431, 0});
            }

            // point{y, mon, d, location}
            // point{year_month_day, location}
            // point{y, mon, d, h, min, s, location}
            // point{year_month_day_hour_minute_second, location}
            {
                // Quick test without hour, minute and second.
                snn_require(time::point{2016, 3, 13, la} == time::point{2016, 3, 13, 8, 0, 0});

                snn_require(time::point{time::ymd{2014, 5, 6}, la} == time::point{2014, 5, 6, la});
                snn_require(time::point{time::ymdhms{2014, 5, 6, 7, 8, 9}, la} ==
                            time::point{2014, 5, 6, 7, 8, 9, la});

                // PST (UTC-8), PDT (UTC-7).
                // PST -> PDT (DST starts, clock is turned forward 1 hour).
                {
                    const time::point b{2016, 3, 13, 1, 59, 59, la}; // Before
                    const time::point a{2016, 3, 13, 4, 0, 0, la};   // After
                    snn_require(b == time::point{2016, 3, 13, 9, 59, 59});
                    snn_require(a == time::point{2016, 3, 13, 11, 0, 0});
                    snn_require(b.offset(la).abbr() == "PST");
                    snn_require(a.offset(la).abbr() == "PDT");
                }
                // PDT -> PST (DST ends, clock is turned backward 1 hour).
                {
                    const time::point b{2016, 11, 6, 0, 59, 59, la}; // Before
                    const time::point a{2016, 11, 6, 3, 0, 0, la};   // After
                    snn_require(b == time::point{2016, 11, 6, 7, 59, 59});
                    snn_require(a == time::point{2016, 11, 6, 11, 0, 0});
                    snn_require(b.offset(la).abbr() == "PDT");
                    snn_require(a.offset(la).abbr() == "PST");
                }

                // GMT (UTC), BST (UTC+1).
                // GMT -> BST (DST starts, clock is turned forward 1 hour).
                {
                    const time::point b{2016, 3, 27, 0, 59, 59, london}; // Before
                    const time::point a{2016, 3, 27, 3, 0, 0, london};   // After
                    snn_require(b == time::point{2016, 3, 27, 0, 59, 59});
                    snn_require(a == time::point{2016, 3, 27, 2, 0, 0});
                    snn_require(b.offset(london).abbr() == "GMT");
                    snn_require(a.offset(london).abbr() == "BST");
                }
                // BST -> GMT (DST ends, clock is turned backward 1 hour).
                {
                    const time::point b{2016, 10, 30, 0, 59, 59, london}; // Before
                    const time::point a{2016, 10, 30, 3, 0, 0, london};   // After
                    snn_require(b == time::point{2016, 10, 29, 23, 59, 59});
                    snn_require(a == time::point{2016, 10, 30, 3, 0, 0});
                    snn_require(b.offset(london).abbr() == "BST");
                    snn_require(a.offset(london).abbr() == "GMT");
                }

                // CET (UTC+1), CEST (UTC+2).
                // CET -> CEST (DST starts, clock is turned forward 1 hour).
                {
                    const time::point b{2016, 3, 27, 1, 59, 59, sthlm}; // Before
                    const time::point a{2016, 3, 27, 4, 0, 0, sthlm};   // After
                    snn_require(b == time::point{2016, 3, 27, 0, 59, 59});
                    snn_require(a == time::point{2016, 3, 27, 2, 0, 0});
                    snn_require(b.offset(sthlm).abbr() == "CET");
                    snn_require(a.offset(sthlm).abbr() == "CEST");
                }
                // CEST -> CET (DST ends, clock is turned backward 1 hour).
                {
                    const time::point b{2016, 10, 30, 1, 59, 59, sthlm}; // Before
                    const time::point a{2016, 10, 30, 4, 0, 0, sthlm};   // After
                    snn_require(b == time::point{2016, 10, 29, 23, 59, 59});
                    snn_require(a == time::point{2016, 10, 30, 3, 0, 0});
                    snn_require(b.offset(sthlm).abbr() == "CEST");
                    snn_require(a.offset(sthlm).abbr() == "CET");
                }
            }

            // format()
            // str << point
            {
                time::point p{2014, 3, 30, 0, 39, 15};
                snn_require(p.unix() == 1396139955);

                snn_require(p.format("f t OOO") == "2014-03-30 00:39:15 UTC");

                snn_require(p.format(time::format_string::date) == "2014-03-30");
                snn_require(p.format(time::format_string::local) == "2014-03-30 00:39:15");
                snn_require(p.format(time::format_string::quick) == "2014-03-30 00:39:15 +0000");
                snn_require(p.format(time::format_string::iso8601) == "2014-03-30T00:39:15+00:00");
                snn_require(p.format(time::format_string::rfc3339) == "2014-03-30T00:39:15+00:00");

                using namespace time::format_string;

                snn_require(p.format(rfc2822) == "Sun, 30 Mar 2014 00:39:15 +0000");
                snn_require(p.format(iso8601_z) == "2014-03-30T00:39:15Z");
                snn_require(p.format(iso8601_milli_z) == "2014-03-30T00:39:15.000Z");
                snn_require(p.format(iso8601_nano_z) == "2014-03-30T00:39:15.000000000Z");
                snn_require(p.format(rfc1123) == "Sun, 30 Mar 2014 00:39:15 GMT");

                snn_require(p.format(date, sthlm) == "2014-03-30");
                snn_require(p.format(local, sthlm) == "2014-03-30 01:39:15");
                snn_require(p.format(quick, sthlm) == "2014-03-30 01:39:15 +0100");
                snn_require(p.format(iso8601, sthlm) == "2014-03-30T01:39:15+01:00");
                snn_require(p.format(rfc3339, sthlm) == "2014-03-30T01:39:15+01:00");
                snn_require(p.format(rfc2822, sthlm) == "Sun, 30 Mar 2014 01:39:15 +0100");

                snn_require(p.format(date, la) == "2014-03-29");
                snn_require(p.format(local, la) == "2014-03-29 17:39:15");
                snn_require(p.format(quick, la) == "2014-03-29 17:39:15 -0700");
                snn_require(p.format(iso8601, la) == "2014-03-29T17:39:15-07:00");
                snn_require(p.format(rfc3339, la) == "2014-03-29T17:39:15-07:00");
                snn_require(p.format(rfc2822, la) == "Sat, 29 Mar 2014 17:39:15 -0700");

                // Weekday
                snn_require(p.format("eee (e)", sthlm) == "Sun (0)");
                snn_require(p.format("eee (e)", la) == "Sat (6)");

                // Various
                snn_require(p.format("yyyy-mm-dd", sthlm) == "2014-03-30");
                snn_require(p.format("f", sthlm) == "2014-03-30");
                snn_require(p.format("hh:ii:ss", sthlm) == "01:39:15");
                snn_require(p.format("t", sthlm) == "01:39:15");
                snn_require(p.format("yyyy", sthlm) == "2014");
                snn_require(p.format("", sthlm) == "");
                snn_require(p.format(".", sthlm) == ".");
                snn_require(p.format("..", sthlm) == "..");
                snn_require(p.format("-", sthlm) == "-");
                snn_require(p.format("--", sthlm) == "--");
                snn_require(p.format(R"(yyyy-mm-dd hh:ii:ss\)", sthlm) == "2014-03-30 01:39:15");
                snn_require(p.format(R"(yyyy-mm-dd hh:ii:ss\\)", sthlm) ==
                            R"(2014-03-30 01:39:15\)");
                snn_require(p.format(R"(\)", sthlm) == "");
                snn_require(p.format(R"(\\)", sthlm) == R"(\)");
                snn_require(p.format(R"(\n)", sthlm) == "n");
                snn_require(p.format(R"(\yyyyy-\mmm-\ddd)", sthlm) == "y2014-m03-d30");

                snn_require_throws_code(p.format("x"), time::error::unescaped_alpha_character);
                snn_require_throws_code(p.format("zzzz"), time::error::invalid_format_string);

                strbuf s{"One"};
                s << p;
                s << "Two";
                snn_require(s == "One2014-03-30 00:39:15 +0000Two");
                p.format(R"(\T\h\r\e\e)", s);
                snn_require(s == "One2014-03-30 00:39:15 +0000TwoThree");
                p.format(time::format_string::rfc2822, sthlm, s);
                snn_require(s ==
                            "One2014-03-30 00:39:15 +0000TwoThreeSun, 30 Mar 2014 01:39:15 +0100");
            }
            // Fraction
            {
                // Non-zero fraction.
                time::point nz{2014, 3, 30, 0, 39, 15, 450'075'200, 0};
                snn_require(nz.unix() == 1396139955);
                snn_require(nz.nanosecond() == 450'075'200);

                // Zero fraction.
                time::point zo{2014, 3, 30, 0, 39, 15};
                snn_require(zo.unix() == 1396139955);
                snn_require(zo.nanosecond() == 0);

                using namespace time::format_string;

                snn_require(nz.format(local) == "2014-03-30 00:39:15");
                snn_require(zo.format(local) == "2014-03-30 00:39:15");

                snn_require(nz.format(quick) == "2014-03-30 00:39:15.4500752 +0000");
                snn_require(zo.format(quick) == "2014-03-30 00:39:15 +0000");

                snn_require(nz.format(iso8601_milli_z) == "2014-03-30T00:39:15.450Z");
                snn_require(zo.format(iso8601_milli_z) == "2014-03-30T00:39:15.000Z");

                snn_require(nz.format(iso8601_nano_z) == "2014-03-30T00:39:15.450075200Z");
                snn_require(zo.format(iso8601_nano_z) == "2014-03-30T00:39:15.000000000Z");

                // Fixed fraction (truncated, not rounded).
                snn_require(nz.format("t.n") == "00:39:15.4");
                snn_require(zo.format("t.n") == "00:39:15.0");
                snn_require(nz.format("t.nn") == "00:39:15.45");
                snn_require(zo.format("t.nn") == "00:39:15.00");
                snn_require(nz.format("t.nnn") == "00:39:15.450");
                snn_require(zo.format("t.nnn") == "00:39:15.000");
                snn_require(nz.format("t.nnnn") == "00:39:15.4500");
                snn_require(zo.format("t.nnnn") == "00:39:15.0000");
                snn_require(nz.format("t.nnnnn") == "00:39:15.45007");
                snn_require(zo.format("t.nnnnn") == "00:39:15.00000");
                snn_require(nz.format("t.nnnnnn") == "00:39:15.450075");
                snn_require(zo.format("t.nnnnnn") == "00:39:15.000000");
                snn_require(nz.format("t.nnnnnnn") == "00:39:15.4500752");
                snn_require(zo.format("t.nnnnnnn") == "00:39:15.0000000");
                snn_require(nz.format("t.nnnnnnnn") == "00:39:15.45007520");
                snn_require(zo.format("t.nnnnnnnn") == "00:39:15.00000000");
                snn_require(nz.format("t.nnnnnnnnn") == "00:39:15.450075200");
                snn_require(zo.format("t.nnnnnnnnn") == "00:39:15.000000000");

                // Trimmed fraction (truncated, not rounded).
                snn_require(nz.format("t.N") == "00:39:15.4");
                snn_require(zo.format("t.N") == "00:39:15");
                snn_require(nz.format("t.NN") == "00:39:15.45");
                snn_require(zo.format("t.NN") == "00:39:15");
                snn_require(nz.format("t.NNN") == "00:39:15.45");
                snn_require(zo.format("t.NNN") == "00:39:15");
                snn_require(nz.format("t.NNNN") == "00:39:15.45");
                snn_require(zo.format("t.NNNN") == "00:39:15");
                snn_require(nz.format("t.NNNNN") == "00:39:15.45007");
                snn_require(zo.format("t.NNNNN") == "00:39:15");
                snn_require(nz.format("t.NNNNNN") == "00:39:15.450075");
                snn_require(zo.format("t.NNNNNN") == "00:39:15");
                snn_require(nz.format("t.NNNNNNN") == "00:39:15.4500752");
                snn_require(zo.format("t.NNNNNNN") == "00:39:15");
                snn_require(nz.format("t.NNNNNNNN") == "00:39:15.4500752");
                snn_require(zo.format("t.NNNNNNNN") == "00:39:15");
                snn_require(nz.format("t.NNNNNNNNN") == "00:39:15.4500752");
                snn_require(zo.format("t.NNNNNNNNN") == "00:39:15");
            }
            // 12-hour clock.
            {
                time::point am{2014, 3, 30, 0, 39, 15};
                snn_require(am.unix() == 1396139955);

                snn_require(am.format("g:ii aa AA") == "12:39 am AM");
                snn_require(am.format("gg:ii aa AA") == "12:39 am AM");

                time::point pm{2014, 3, 30, 17, 39, 15};
                snn_require(pm.unix() == 1396201155);

                snn_require(pm.format("g:ii aa AA") == "5:39 pm PM");
                snn_require(pm.format("gg:ii aa AA") == "05:39 pm PM");
            }
            // Epoch
            {
                time::point p{};
                snn_require(p.unix() == 0);

                using namespace time::format_string;

                snn_require(p.format(local) == "1970-01-01 00:00:00");
                snn_require(p.format(quick) == "1970-01-01 00:00:00 +0000");
                snn_require(p.format(iso8601) == "1970-01-01T00:00:00+00:00");
                snn_require(p.format(rfc3339) == "1970-01-01T00:00:00+00:00");
                snn_require(p.format(rfc2822) == "Thu, 01 Jan 1970 00:00:00 +0000");

                snn_require(p.format(iso8601_z) == "1970-01-01T00:00:00Z");
                snn_require(p.format(rfc1123) == "Thu, 01 Jan 1970 00:00:00 GMT");
            }
            {
                // CET -> CEST (DST starts, clock is turned forward 1 hour).
                const time::point b{2016, 3, 27, 1, 59, 59, sthlm}; // Before
                const time::point a{2016, 3, 27, 4, 0, 0, sthlm};   // After
                constexpr cstrview format_string = "f t OOO oooo ooooo";
                snn_require(b.format(format_string, sthlm) ==
                            "2016-03-27 01:59:59 CET +0100 +01:00");
                snn_require(a.format(format_string, sthlm) ==
                            "2016-03-27 04:00:00 CEST +0200 +02:00");

                // Daylight saving time and offset in seconds.
                snn_require(b.format("OOO v o", sthlm) == "CET 0 3600");
                snn_require(a.format("OOO v o", sthlm) == "CEST 1 7200");
            }
            {
                // CEST -> CET (DST ends, clock is turned backward 1 hour).
                const time::point b{2016, 10, 30, 1, 59, 59, sthlm}; // Before
                const time::point a{2016, 10, 30, 4, 0, 0, sthlm};   // After
                constexpr cstrview format_string = "f t OOO oooo ooooo";
                snn_require(b.format(format_string, sthlm) ==
                            "2016-10-30 01:59:59 CEST +0200 +02:00");
                snn_require(a.format(format_string, sthlm) ==
                            "2016-10-30 04:00:00 CET +0100 +01:00");

                // Daylight saving time and offset in seconds.
                snn_require(b.format("OOO v o", sthlm) == "CEST 1 7200");
                snn_require(a.format("OOO v o", sthlm) == "CET 0 3600");
            }
            {
                // Odd time offset & non-alpha abbreviation (Australian Central Western Standard
                // Time).
                const time::point p{2001, 2, 3, 4, 5, 6, eucla};
                constexpr cstrview format_string = "f t OOO oooo ooooo";
                snn_require(p.format(format_string, eucla) ==
                            "2001-02-03 04:05:06 +0845 +0845 +08:45");
            }
            {
                time::point p{-2014, 3, 30, 0, 39, 15};
                snn_require(p.unix() == -125715280845);

                using namespace time::format_string;
                snn_require(p.format(iso8601) == "-2014-03-30T00:39:15+00:00");
                snn_require(p.format(iso8601_z) == "-2014-03-30T00:39:15Z");
                snn_require(p.format(local, utc) == "-2014-03-30 00:39:15");
                snn_require(p.format(rfc1123) == "Sun, 30 Mar -2014 00:39:15 GMT");
                snn_require(p.format(rfc2822) == "Sun, 30 Mar -2014 00:39:15 +0000");
            }
            {
                time::point p{1, 2, 3, 4, 5, 6};
                snn_require(p.unix() == -62132730894);

                using namespace time::format_string;
                snn_require(p.format(iso8601) == "0001-02-03T04:05:06+00:00");
                snn_require(p.format(iso8601_z) == "0001-02-03T04:05:06Z");
                snn_require(p.format(local, utc) == "0001-02-03 04:05:06");
                snn_require(p.format(rfc1123) == "Sat, 03 Feb 0001 04:05:06 GMT");
                snn_require(p.format(rfc2822) == "Sat, 03 Feb 0001 04:05:06 +0000");
            }

            // Space padded day of month.
            {
                time::point p{-2014, 3, 30, 0, 39, 15};
                snn_require(p.unix() == -125715280845);
                snn_require(p.format("DD") == "30");
            }
            {
                time::point p{1, 2, 3, 4, 5, 6};
                snn_require(p.unix() == -62132730894);
                snn_require(p.format("DD") == " 3");
            }

            // year(), month(), day(), hour(), minute(), second()
            // day_of_week(), day_of_year(), clock(), date()
            {
                time::point p{2016, 2, 29, 13, 14, 15}; // Leap day.
                snn_require(p.year() == 2016);
                snn_require(p.month() == time::february);
                snn_require(p.day() == 29);
                snn_require(p.hour() == 13);
                snn_require(p.minute() == 14);
                snn_require(p.second() == 15);
                snn_require(p.day_of_week() == time::monday);
                snn_require(p.day_of_year() == 60);
                snn_require(p.date() == time::year_month_day{2016, 2, 29});
                snn_require(p.time() == time::hour_minute_second{13, 14, 15});
            }

            // year(location), month(location), day(location), hour(location), minute(location),
            // second(location) day_of_week(location), day_of_year(location), clock(location),
            // date(location) week(location), offset(location)
            {
                time::point p{2006, 12, 31, 22, 14, 15, la};
                snn_require(p.unix() == 1167632055);

                // Los Angeles
                snn_require(p.year(la) == 2006);
                snn_require(p.month(la) == time::december);
                snn_require(p.day(la) == 31);
                snn_require(p.hour(la) == 22);
                snn_require(p.minute(la) == 14);
                snn_require(p.second(la) == 15);
                snn_require(p.day_of_week(la) == time::sunday);
                snn_require(p.day_of_year(la) == 365);
                snn_require(p.date(la) == time::year_month_day{2006, 12, 31});
                snn_require(p.time(la) == time::hour_minute_second{22, 14, 15});
                snn_require(p.week(la) == time::year_week{2006, 52});
                snn_require(p.offset(la).abbr() == "PST");

                // UTC
                snn_require(p.year() == 2007);
                snn_require(p.month() == time::january);
                snn_require(p.day() == 1);
                snn_require(p.hour() == 6);
                snn_require(p.minute() == 14);
                snn_require(p.second() == 15);
                snn_require(p.day_of_week() == time::monday);
                snn_require(p.day_of_year() == 1);
                snn_require(p.date() == time::year_month_day{2007, 1, 1});
                snn_require(p.time() == time::hour_minute_second{6, 14, 15});
                snn_require(p.week() == time::year_week{2007, 1});
            }

            // Various
            {
                const time::point p{2006, 12, 31, 22, 14, 15, la};
                const auto ymd = p.date(la);

                // Start of day (00:00:00).
                time::point start{ymd.year(), ymd.month(), ymd.day(), la};
                snn_require(start.date(la) == time::year_month_day{2006, 12, 31});
                snn_require(start.time(la) == time::hour_minute_second{0, 0, 0});

                // Day after (00:00:00).
                time::point day_after{ymd.year(), ymd.month(), ymd.day() + 1, la};
                snn_require(day_after.date(la) == time::year_month_day{2007, 1, 1});
                snn_require(day_after.time(la) == time::hour_minute_second{0, 0, 0});

                // 14 days later at 13:00:00.
                time::point later{ymd.year(), ymd.month(), ymd.day() + 14, 13, 0, 0, la};
                snn_require(later.date(la) == time::year_month_day{2007, 1, 14});
                snn_require(later.time(la) == time::hour_minute_second{13, 0, 0});
            }

            // week()
            {
                // Generated by: detail/iso.week.test.gen.go or detail/iso.week.test.gen.php

                snn_require(time::point{1990, 1, 1}.week() == time::year_week{1990, 1});
                snn_require(time::point{1990, 1, 2}.week() == time::year_week{1990, 1});
                snn_require(time::point{1990, 1, 3}.week() == time::year_week{1990, 1});
                snn_require(time::point{1990, 1, 4}.week() == time::year_week{1990, 1});
                snn_require(time::point{1990, 12, 28}.week() == time::year_week{1990, 52});
                snn_require(time::point{1990, 12, 29}.week() == time::year_week{1990, 52});
                snn_require(time::point{1990, 12, 30}.week() == time::year_week{1990, 52});
                snn_require(time::point{1990, 12, 31}.week() == time::year_week{1991, 1});
                snn_require(time::point{1991, 1, 1}.week() == time::year_week{1991, 1});
                snn_require(time::point{1991, 1, 2}.week() == time::year_week{1991, 1});
                snn_require(time::point{1991, 1, 3}.week() == time::year_week{1991, 1});
                snn_require(time::point{1991, 1, 4}.week() == time::year_week{1991, 1});
                snn_require(time::point{1991, 12, 28}.week() == time::year_week{1991, 52});
                snn_require(time::point{1991, 12, 29}.week() == time::year_week{1991, 52});
                snn_require(time::point{1991, 12, 30}.week() == time::year_week{1992, 1});
                snn_require(time::point{1991, 12, 31}.week() == time::year_week{1992, 1});
                snn_require(time::point{1992, 1, 1}.week() == time::year_week{1992, 1});
                snn_require(time::point{1992, 1, 2}.week() == time::year_week{1992, 1});
                snn_require(time::point{1992, 1, 3}.week() == time::year_week{1992, 1});
                snn_require(time::point{1992, 1, 4}.week() == time::year_week{1992, 1});
                snn_require(time::point{1992, 12, 28}.week() == time::year_week{1992, 53});
                snn_require(time::point{1992, 12, 29}.week() == time::year_week{1992, 53});
                snn_require(time::point{1992, 12, 30}.week() == time::year_week{1992, 53});
                snn_require(time::point{1992, 12, 31}.week() == time::year_week{1992, 53});
                snn_require(time::point{1993, 1, 1}.week() == time::year_week{1992, 53});
                snn_require(time::point{1993, 1, 2}.week() == time::year_week{1992, 53});
                snn_require(time::point{1993, 1, 3}.week() == time::year_week{1992, 53});
                snn_require(time::point{1993, 1, 4}.week() == time::year_week{1993, 1});
                snn_require(time::point{1993, 12, 28}.week() == time::year_week{1993, 52});
                snn_require(time::point{1993, 12, 29}.week() == time::year_week{1993, 52});
                snn_require(time::point{1993, 12, 30}.week() == time::year_week{1993, 52});
                snn_require(time::point{1993, 12, 31}.week() == time::year_week{1993, 52});
                snn_require(time::point{1994, 1, 1}.week() == time::year_week{1993, 52});
                snn_require(time::point{1994, 1, 2}.week() == time::year_week{1993, 52});
                snn_require(time::point{1994, 1, 3}.week() == time::year_week{1994, 1});
                snn_require(time::point{1994, 1, 4}.week() == time::year_week{1994, 1});
                snn_require(time::point{1994, 12, 28}.week() == time::year_week{1994, 52});
                snn_require(time::point{1994, 12, 29}.week() == time::year_week{1994, 52});
                snn_require(time::point{1994, 12, 30}.week() == time::year_week{1994, 52});
                snn_require(time::point{1994, 12, 31}.week() == time::year_week{1994, 52});
                snn_require(time::point{1995, 1, 1}.week() == time::year_week{1994, 52});
                snn_require(time::point{1995, 1, 2}.week() == time::year_week{1995, 1});
                snn_require(time::point{1995, 1, 3}.week() == time::year_week{1995, 1});
                snn_require(time::point{1995, 1, 4}.week() == time::year_week{1995, 1});
                snn_require(time::point{1995, 12, 28}.week() == time::year_week{1995, 52});
                snn_require(time::point{1995, 12, 29}.week() == time::year_week{1995, 52});
                snn_require(time::point{1995, 12, 30}.week() == time::year_week{1995, 52});
                snn_require(time::point{1995, 12, 31}.week() == time::year_week{1995, 52});
                snn_require(time::point{1996, 1, 1}.week() == time::year_week{1996, 1});
                snn_require(time::point{1996, 1, 2}.week() == time::year_week{1996, 1});
                snn_require(time::point{1996, 1, 3}.week() == time::year_week{1996, 1});
                snn_require(time::point{1996, 1, 4}.week() == time::year_week{1996, 1});
                snn_require(time::point{1996, 12, 28}.week() == time::year_week{1996, 52});
                snn_require(time::point{1996, 12, 29}.week() == time::year_week{1996, 52});
                snn_require(time::point{1996, 12, 30}.week() == time::year_week{1997, 1});
                snn_require(time::point{1996, 12, 31}.week() == time::year_week{1997, 1});
                snn_require(time::point{1997, 1, 1}.week() == time::year_week{1997, 1});
                snn_require(time::point{1997, 1, 2}.week() == time::year_week{1997, 1});
                snn_require(time::point{1997, 1, 3}.week() == time::year_week{1997, 1});
                snn_require(time::point{1997, 1, 4}.week() == time::year_week{1997, 1});
                snn_require(time::point{1997, 12, 28}.week() == time::year_week{1997, 52});
                snn_require(time::point{1997, 12, 29}.week() == time::year_week{1998, 1});
                snn_require(time::point{1997, 12, 30}.week() == time::year_week{1998, 1});
                snn_require(time::point{1997, 12, 31}.week() == time::year_week{1998, 1});
                snn_require(time::point{1998, 1, 1}.week() == time::year_week{1998, 1});
                snn_require(time::point{1998, 1, 2}.week() == time::year_week{1998, 1});
                snn_require(time::point{1998, 1, 3}.week() == time::year_week{1998, 1});
                snn_require(time::point{1998, 1, 4}.week() == time::year_week{1998, 1});
                snn_require(time::point{1998, 12, 28}.week() == time::year_week{1998, 53});
                snn_require(time::point{1998, 12, 29}.week() == time::year_week{1998, 53});
                snn_require(time::point{1998, 12, 30}.week() == time::year_week{1998, 53});
                snn_require(time::point{1998, 12, 31}.week() == time::year_week{1998, 53});
                snn_require(time::point{1999, 1, 1}.week() == time::year_week{1998, 53});
                snn_require(time::point{1999, 1, 2}.week() == time::year_week{1998, 53});
                snn_require(time::point{1999, 1, 3}.week() == time::year_week{1998, 53});
                snn_require(time::point{1999, 1, 4}.week() == time::year_week{1999, 1});
                snn_require(time::point{1999, 12, 28}.week() == time::year_week{1999, 52});
                snn_require(time::point{1999, 12, 29}.week() == time::year_week{1999, 52});
                snn_require(time::point{1999, 12, 30}.week() == time::year_week{1999, 52});
                snn_require(time::point{1999, 12, 31}.week() == time::year_week{1999, 52});
                snn_require(time::point{2000, 1, 1}.week() == time::year_week{1999, 52});
                snn_require(time::point{2000, 1, 2}.week() == time::year_week{1999, 52});
                snn_require(time::point{2000, 1, 3}.week() == time::year_week{2000, 1});
                snn_require(time::point{2000, 1, 4}.week() == time::year_week{2000, 1});
                snn_require(time::point{2000, 12, 28}.week() == time::year_week{2000, 52});
                snn_require(time::point{2000, 12, 29}.week() == time::year_week{2000, 52});
                snn_require(time::point{2000, 12, 30}.week() == time::year_week{2000, 52});
                snn_require(time::point{2000, 12, 31}.week() == time::year_week{2000, 52});
                snn_require(time::point{2001, 1, 1}.week() == time::year_week{2001, 1});
                snn_require(time::point{2001, 1, 2}.week() == time::year_week{2001, 1});
                snn_require(time::point{2001, 1, 3}.week() == time::year_week{2001, 1});
                snn_require(time::point{2001, 1, 4}.week() == time::year_week{2001, 1});
                snn_require(time::point{2001, 12, 28}.week() == time::year_week{2001, 52});
                snn_require(time::point{2001, 12, 29}.week() == time::year_week{2001, 52});
                snn_require(time::point{2001, 12, 30}.week() == time::year_week{2001, 52});
                snn_require(time::point{2001, 12, 31}.week() == time::year_week{2002, 1});
                snn_require(time::point{2002, 1, 1}.week() == time::year_week{2002, 1});
                snn_require(time::point{2002, 1, 2}.week() == time::year_week{2002, 1});
                snn_require(time::point{2002, 1, 3}.week() == time::year_week{2002, 1});
                snn_require(time::point{2002, 1, 4}.week() == time::year_week{2002, 1});
                snn_require(time::point{2002, 12, 28}.week() == time::year_week{2002, 52});
                snn_require(time::point{2002, 12, 29}.week() == time::year_week{2002, 52});
                snn_require(time::point{2002, 12, 30}.week() == time::year_week{2003, 1});
                snn_require(time::point{2002, 12, 31}.week() == time::year_week{2003, 1});
                snn_require(time::point{2003, 1, 1}.week() == time::year_week{2003, 1});
                snn_require(time::point{2003, 1, 2}.week() == time::year_week{2003, 1});
                snn_require(time::point{2003, 1, 3}.week() == time::year_week{2003, 1});
                snn_require(time::point{2003, 1, 4}.week() == time::year_week{2003, 1});
                snn_require(time::point{2003, 12, 28}.week() == time::year_week{2003, 52});
                snn_require(time::point{2003, 12, 29}.week() == time::year_week{2004, 1});
                snn_require(time::point{2003, 12, 30}.week() == time::year_week{2004, 1});
                snn_require(time::point{2003, 12, 31}.week() == time::year_week{2004, 1});
                snn_require(time::point{2004, 1, 1}.week() == time::year_week{2004, 1});
                snn_require(time::point{2004, 1, 2}.week() == time::year_week{2004, 1});
                snn_require(time::point{2004, 1, 3}.week() == time::year_week{2004, 1});
                snn_require(time::point{2004, 1, 4}.week() == time::year_week{2004, 1});
                snn_require(time::point{2004, 12, 28}.week() == time::year_week{2004, 53});
                snn_require(time::point{2004, 12, 29}.week() == time::year_week{2004, 53});
                snn_require(time::point{2004, 12, 30}.week() == time::year_week{2004, 53});
                snn_require(time::point{2004, 12, 31}.week() == time::year_week{2004, 53});
                snn_require(time::point{2005, 1, 1}.week() == time::year_week{2004, 53});
                snn_require(time::point{2005, 1, 2}.week() == time::year_week{2004, 53});
                snn_require(time::point{2005, 1, 3}.week() == time::year_week{2005, 1});
                snn_require(time::point{2005, 1, 4}.week() == time::year_week{2005, 1});
                snn_require(time::point{2005, 12, 28}.week() == time::year_week{2005, 52});
                snn_require(time::point{2005, 12, 29}.week() == time::year_week{2005, 52});
                snn_require(time::point{2005, 12, 30}.week() == time::year_week{2005, 52});
                snn_require(time::point{2005, 12, 31}.week() == time::year_week{2005, 52});
                snn_require(time::point{2006, 1, 1}.week() == time::year_week{2005, 52});
                snn_require(time::point{2006, 1, 2}.week() == time::year_week{2006, 1});
                snn_require(time::point{2006, 1, 3}.week() == time::year_week{2006, 1});
                snn_require(time::point{2006, 1, 4}.week() == time::year_week{2006, 1});
                snn_require(time::point{2006, 12, 28}.week() == time::year_week{2006, 52});
                snn_require(time::point{2006, 12, 29}.week() == time::year_week{2006, 52});
                snn_require(time::point{2006, 12, 30}.week() == time::year_week{2006, 52});
                snn_require(time::point{2006, 12, 31}.week() == time::year_week{2006, 52});
                snn_require(time::point{2007, 1, 1}.week() == time::year_week{2007, 1});
                snn_require(time::point{2007, 1, 2}.week() == time::year_week{2007, 1});
                snn_require(time::point{2007, 1, 3}.week() == time::year_week{2007, 1});
                snn_require(time::point{2007, 1, 4}.week() == time::year_week{2007, 1});
                snn_require(time::point{2007, 12, 28}.week() == time::year_week{2007, 52});
                snn_require(time::point{2007, 12, 29}.week() == time::year_week{2007, 52});
                snn_require(time::point{2007, 12, 30}.week() == time::year_week{2007, 52});
                snn_require(time::point{2007, 12, 31}.week() == time::year_week{2008, 1});
                snn_require(time::point{2008, 1, 1}.week() == time::year_week{2008, 1});
                snn_require(time::point{2008, 1, 2}.week() == time::year_week{2008, 1});
                snn_require(time::point{2008, 1, 3}.week() == time::year_week{2008, 1});
                snn_require(time::point{2008, 1, 4}.week() == time::year_week{2008, 1});
                snn_require(time::point{2008, 12, 28}.week() == time::year_week{2008, 52});
                snn_require(time::point{2008, 12, 29}.week() == time::year_week{2009, 1});
                snn_require(time::point{2008, 12, 30}.week() == time::year_week{2009, 1});
                snn_require(time::point{2008, 12, 31}.week() == time::year_week{2009, 1});
                snn_require(time::point{2009, 1, 1}.week() == time::year_week{2009, 1});
                snn_require(time::point{2009, 1, 2}.week() == time::year_week{2009, 1});
                snn_require(time::point{2009, 1, 3}.week() == time::year_week{2009, 1});
                snn_require(time::point{2009, 1, 4}.week() == time::year_week{2009, 1});
                snn_require(time::point{2009, 12, 28}.week() == time::year_week{2009, 53});
                snn_require(time::point{2009, 12, 29}.week() == time::year_week{2009, 53});
                snn_require(time::point{2009, 12, 30}.week() == time::year_week{2009, 53});
                snn_require(time::point{2009, 12, 31}.week() == time::year_week{2009, 53});
                snn_require(time::point{2010, 1, 1}.week() == time::year_week{2009, 53});
                snn_require(time::point{2010, 1, 2}.week() == time::year_week{2009, 53});
                snn_require(time::point{2010, 1, 3}.week() == time::year_week{2009, 53});
                snn_require(time::point{2010, 1, 4}.week() == time::year_week{2010, 1});
                snn_require(time::point{2010, 12, 28}.week() == time::year_week{2010, 52});
                snn_require(time::point{2010, 12, 29}.week() == time::year_week{2010, 52});
                snn_require(time::point{2010, 12, 30}.week() == time::year_week{2010, 52});
                snn_require(time::point{2010, 12, 31}.week() == time::year_week{2010, 52});
                snn_require(time::point{2011, 1, 1}.week() == time::year_week{2010, 52});
                snn_require(time::point{2011, 1, 2}.week() == time::year_week{2010, 52});
                snn_require(time::point{2011, 1, 3}.week() == time::year_week{2011, 1});
                snn_require(time::point{2011, 1, 4}.week() == time::year_week{2011, 1});
                snn_require(time::point{2011, 12, 28}.week() == time::year_week{2011, 52});
                snn_require(time::point{2011, 12, 29}.week() == time::year_week{2011, 52});
                snn_require(time::point{2011, 12, 30}.week() == time::year_week{2011, 52});
                snn_require(time::point{2011, 12, 31}.week() == time::year_week{2011, 52});
                snn_require(time::point{2012, 1, 1}.week() == time::year_week{2011, 52});
                snn_require(time::point{2012, 1, 2}.week() == time::year_week{2012, 1});
                snn_require(time::point{2012, 1, 3}.week() == time::year_week{2012, 1});
                snn_require(time::point{2012, 1, 4}.week() == time::year_week{2012, 1});
                snn_require(time::point{2012, 12, 28}.week() == time::year_week{2012, 52});
                snn_require(time::point{2012, 12, 29}.week() == time::year_week{2012, 52});
                snn_require(time::point{2012, 12, 30}.week() == time::year_week{2012, 52});
                snn_require(time::point{2012, 12, 31}.week() == time::year_week{2013, 1});
                snn_require(time::point{2013, 1, 1}.week() == time::year_week{2013, 1});
                snn_require(time::point{2013, 1, 2}.week() == time::year_week{2013, 1});
                snn_require(time::point{2013, 1, 3}.week() == time::year_week{2013, 1});
                snn_require(time::point{2013, 1, 4}.week() == time::year_week{2013, 1});
                snn_require(time::point{2013, 12, 28}.week() == time::year_week{2013, 52});
                snn_require(time::point{2013, 12, 29}.week() == time::year_week{2013, 52});
                snn_require(time::point{2013, 12, 30}.week() == time::year_week{2014, 1});
                snn_require(time::point{2013, 12, 31}.week() == time::year_week{2014, 1});
                snn_require(time::point{2014, 1, 1}.week() == time::year_week{2014, 1});
                snn_require(time::point{2014, 1, 2}.week() == time::year_week{2014, 1});
                snn_require(time::point{2014, 1, 3}.week() == time::year_week{2014, 1});
                snn_require(time::point{2014, 1, 4}.week() == time::year_week{2014, 1});
                snn_require(time::point{2014, 12, 28}.week() == time::year_week{2014, 52});
                snn_require(time::point{2014, 12, 29}.week() == time::year_week{2015, 1});
                snn_require(time::point{2014, 12, 30}.week() == time::year_week{2015, 1});
                snn_require(time::point{2014, 12, 31}.week() == time::year_week{2015, 1});
                snn_require(time::point{2015, 1, 1}.week() == time::year_week{2015, 1});
                snn_require(time::point{2015, 1, 2}.week() == time::year_week{2015, 1});
                snn_require(time::point{2015, 1, 3}.week() == time::year_week{2015, 1});
                snn_require(time::point{2015, 1, 4}.week() == time::year_week{2015, 1});
                snn_require(time::point{2015, 12, 28}.week() == time::year_week{2015, 53});
                snn_require(time::point{2015, 12, 29}.week() == time::year_week{2015, 53});
                snn_require(time::point{2015, 12, 30}.week() == time::year_week{2015, 53});
                snn_require(time::point{2015, 12, 31}.week() == time::year_week{2015, 53});
                snn_require(time::point{2016, 1, 1}.week() == time::year_week{2015, 53});
                snn_require(time::point{2016, 1, 2}.week() == time::year_week{2015, 53});
                snn_require(time::point{2016, 1, 3}.week() == time::year_week{2015, 53});
                snn_require(time::point{2016, 1, 4}.week() == time::year_week{2016, 1});
                snn_require(time::point{2016, 12, 28}.week() == time::year_week{2016, 52});
                snn_require(time::point{2016, 12, 29}.week() == time::year_week{2016, 52});
                snn_require(time::point{2016, 12, 30}.week() == time::year_week{2016, 52});
                snn_require(time::point{2016, 12, 31}.week() == time::year_week{2016, 52});
                snn_require(time::point{2017, 1, 1}.week() == time::year_week{2016, 52});
                snn_require(time::point{2017, 1, 2}.week() == time::year_week{2017, 1});
                snn_require(time::point{2017, 1, 3}.week() == time::year_week{2017, 1});
                snn_require(time::point{2017, 1, 4}.week() == time::year_week{2017, 1});
                snn_require(time::point{2017, 12, 28}.week() == time::year_week{2017, 52});
                snn_require(time::point{2017, 12, 29}.week() == time::year_week{2017, 52});
                snn_require(time::point{2017, 12, 30}.week() == time::year_week{2017, 52});
                snn_require(time::point{2017, 12, 31}.week() == time::year_week{2017, 52});
                snn_require(time::point{2018, 1, 1}.week() == time::year_week{2018, 1});
                snn_require(time::point{2018, 1, 2}.week() == time::year_week{2018, 1});
                snn_require(time::point{2018, 1, 3}.week() == time::year_week{2018, 1});
                snn_require(time::point{2018, 1, 4}.week() == time::year_week{2018, 1});
                snn_require(time::point{2018, 12, 28}.week() == time::year_week{2018, 52});
                snn_require(time::point{2018, 12, 29}.week() == time::year_week{2018, 52});
                snn_require(time::point{2018, 12, 30}.week() == time::year_week{2018, 52});
                snn_require(time::point{2018, 12, 31}.week() == time::year_week{2019, 1});
                snn_require(time::point{2019, 1, 1}.week() == time::year_week{2019, 1});
                snn_require(time::point{2019, 1, 2}.week() == time::year_week{2019, 1});
                snn_require(time::point{2019, 1, 3}.week() == time::year_week{2019, 1});
                snn_require(time::point{2019, 1, 4}.week() == time::year_week{2019, 1});
                snn_require(time::point{2019, 12, 28}.week() == time::year_week{2019, 52});
                snn_require(time::point{2019, 12, 29}.week() == time::year_week{2019, 52});
                snn_require(time::point{2019, 12, 30}.week() == time::year_week{2020, 1});
                snn_require(time::point{2019, 12, 31}.week() == time::year_week{2020, 1});
                snn_require(time::point{2020, 1, 1}.week() == time::year_week{2020, 1});
                snn_require(time::point{2020, 1, 2}.week() == time::year_week{2020, 1});
                snn_require(time::point{2020, 1, 3}.week() == time::year_week{2020, 1});
                snn_require(time::point{2020, 1, 4}.week() == time::year_week{2020, 1});
                snn_require(time::point{2020, 12, 28}.week() == time::year_week{2020, 53});
                snn_require(time::point{2020, 12, 29}.week() == time::year_week{2020, 53});
                snn_require(time::point{2020, 12, 30}.week() == time::year_week{2020, 53});
                snn_require(time::point{2020, 12, 31}.week() == time::year_week{2020, 53});
                snn_require(time::point{2021, 1, 1}.week() == time::year_week{2020, 53});
                snn_require(time::point{2021, 1, 2}.week() == time::year_week{2020, 53});
                snn_require(time::point{2021, 1, 3}.week() == time::year_week{2020, 53});
                snn_require(time::point{2021, 1, 4}.week() == time::year_week{2021, 1});
                snn_require(time::point{2021, 12, 28}.week() == time::year_week{2021, 52});
                snn_require(time::point{2021, 12, 29}.week() == time::year_week{2021, 52});
                snn_require(time::point{2021, 12, 30}.week() == time::year_week{2021, 52});
                snn_require(time::point{2021, 12, 31}.week() == time::year_week{2021, 52});
                snn_require(time::point{2022, 1, 1}.week() == time::year_week{2021, 52});
                snn_require(time::point{2022, 1, 2}.week() == time::year_week{2021, 52});
                snn_require(time::point{2022, 1, 3}.week() == time::year_week{2022, 1});
                snn_require(time::point{2022, 1, 4}.week() == time::year_week{2022, 1});
                snn_require(time::point{2022, 12, 28}.week() == time::year_week{2022, 52});
                snn_require(time::point{2022, 12, 29}.week() == time::year_week{2022, 52});
                snn_require(time::point{2022, 12, 30}.week() == time::year_week{2022, 52});
                snn_require(time::point{2022, 12, 31}.week() == time::year_week{2022, 52});
                snn_require(time::point{2023, 1, 1}.week() == time::year_week{2022, 52});
                snn_require(time::point{2023, 1, 2}.week() == time::year_week{2023, 1});
                snn_require(time::point{2023, 1, 3}.week() == time::year_week{2023, 1});
                snn_require(time::point{2023, 1, 4}.week() == time::year_week{2023, 1});
                snn_require(time::point{2023, 12, 28}.week() == time::year_week{2023, 52});
                snn_require(time::point{2023, 12, 29}.week() == time::year_week{2023, 52});
                snn_require(time::point{2023, 12, 30}.week() == time::year_week{2023, 52});
                snn_require(time::point{2023, 12, 31}.week() == time::year_week{2023, 52});
                snn_require(time::point{2024, 1, 1}.week() == time::year_week{2024, 1});
                snn_require(time::point{2024, 1, 2}.week() == time::year_week{2024, 1});
                snn_require(time::point{2024, 1, 3}.week() == time::year_week{2024, 1});
                snn_require(time::point{2024, 1, 4}.week() == time::year_week{2024, 1});
                snn_require(time::point{2024, 12, 28}.week() == time::year_week{2024, 52});
                snn_require(time::point{2024, 12, 29}.week() == time::year_week{2024, 52});
                snn_require(time::point{2024, 12, 30}.week() == time::year_week{2025, 1});
                snn_require(time::point{2024, 12, 31}.week() == time::year_week{2025, 1});
                snn_require(time::point{2025, 1, 1}.week() == time::year_week{2025, 1});
                snn_require(time::point{2025, 1, 2}.week() == time::year_week{2025, 1});
                snn_require(time::point{2025, 1, 3}.week() == time::year_week{2025, 1});
                snn_require(time::point{2025, 1, 4}.week() == time::year_week{2025, 1});
                snn_require(time::point{2025, 12, 28}.week() == time::year_week{2025, 52});
                snn_require(time::point{2025, 12, 29}.week() == time::year_week{2026, 1});
                snn_require(time::point{2025, 12, 30}.week() == time::year_week{2026, 1});
                snn_require(time::point{2025, 12, 31}.week() == time::year_week{2026, 1});
                snn_require(time::point{2026, 1, 1}.week() == time::year_week{2026, 1});
                snn_require(time::point{2026, 1, 2}.week() == time::year_week{2026, 1});
                snn_require(time::point{2026, 1, 3}.week() == time::year_week{2026, 1});
                snn_require(time::point{2026, 1, 4}.week() == time::year_week{2026, 1});
                snn_require(time::point{2026, 12, 28}.week() == time::year_week{2026, 53});
                snn_require(time::point{2026, 12, 29}.week() == time::year_week{2026, 53});
                snn_require(time::point{2026, 12, 30}.week() == time::year_week{2026, 53});
                snn_require(time::point{2026, 12, 31}.week() == time::year_week{2026, 53});
                snn_require(time::point{2027, 1, 1}.week() == time::year_week{2026, 53});
                snn_require(time::point{2027, 1, 2}.week() == time::year_week{2026, 53});
                snn_require(time::point{2027, 1, 3}.week() == time::year_week{2026, 53});
                snn_require(time::point{2027, 1, 4}.week() == time::year_week{2027, 1});
                snn_require(time::point{2027, 12, 28}.week() == time::year_week{2027, 52});
                snn_require(time::point{2027, 12, 29}.week() == time::year_week{2027, 52});
                snn_require(time::point{2027, 12, 30}.week() == time::year_week{2027, 52});
                snn_require(time::point{2027, 12, 31}.week() == time::year_week{2027, 52});
                snn_require(time::point{2028, 1, 1}.week() == time::year_week{2027, 52});
                snn_require(time::point{2028, 1, 2}.week() == time::year_week{2027, 52});
                snn_require(time::point{2028, 1, 3}.week() == time::year_week{2028, 1});
                snn_require(time::point{2028, 1, 4}.week() == time::year_week{2028, 1});
                snn_require(time::point{2028, 12, 28}.week() == time::year_week{2028, 52});
                snn_require(time::point{2028, 12, 29}.week() == time::year_week{2028, 52});
                snn_require(time::point{2028, 12, 30}.week() == time::year_week{2028, 52});
                snn_require(time::point{2028, 12, 31}.week() == time::year_week{2028, 52});
                snn_require(time::point{2029, 1, 1}.week() == time::year_week{2029, 1});
                snn_require(time::point{2029, 1, 2}.week() == time::year_week{2029, 1});
                snn_require(time::point{2029, 1, 3}.week() == time::year_week{2029, 1});
                snn_require(time::point{2029, 1, 4}.week() == time::year_week{2029, 1});
                snn_require(time::point{2029, 12, 28}.week() == time::year_week{2029, 52});
                snn_require(time::point{2029, 12, 29}.week() == time::year_week{2029, 52});
                snn_require(time::point{2029, 12, 30}.week() == time::year_week{2029, 52});
                snn_require(time::point{2029, 12, 31}.week() == time::year_week{2030, 1});
            }

            // Comparison

            {
                time::point p1{2014, 3, 30, 0, 39, 15};
                time::point p2{2014, 3, 30, 0, 39, 15};
                time::point p3{2014, 3, 30, 0, 39, 16};

                snn_require(p1 == p2);
                snn_require(p2 != p3);
                snn_require(p1 < p3);
                snn_require(p3 > p2);
            }
            {
                time::point p1{time::duration{4, 500'000'000}};
                time::point p2{time::duration{4, 500'000'000}};
                time::point p3{time::duration{4, 600'000'000}};

                snn_require(p1 == p2);
                snn_require(p2 != p3);
                snn_require(p1 < p3);
                snn_require(p3 > p2);
            }

            // Hash
            {
                // The hash is currently the unsigned unix time in nanoseconds, but this might
                // change in the future. The value should be treated as a unique hash, nothing else.

                snn_require(time::point{1396139955}.hash() == 1396139955000000000u);
                snn_require(time::point{1396139955}.add(time::nanoseconds{999'999'999}).hash() ==
                            1396139955999999999u);

                snn_require(time::point{-1396139955}.hash() == 17050604118709551616u);
                snn_require(time::point{-1396139955}.add(time::nanoseconds{999'999'999}).hash() ==
                            17050604119709551615u);

                snn_require(std::hash<time::point>{}(time::point{1396139955}) ==
                            1396139955000000000u);
            }

            // Addition

            snn_require(time::point{1396139955}.add(time::hours{1}).unix() == 1396143555);
            snn_require(time::point{1396139955}.add(time::duration{3600}).unix() == 1396143555);

            {
                time::point p{time::duration{1396139955, 300'000'000}};
                p.add(time::hours{1}).add(time::duration{5});
                snn_require(p.unix() == 1396143560);
                snn_require(p.nanosecond() == 300'000'000);

                p += time::days{2};
                snn_require(p.unix() == 1396316360);
                snn_require(p.nanosecond() == 300'000'000);

                p += time::duration{3, 400'000'000};
                snn_require(p.unix() == 1396316363);
                snn_require(p.nanosecond() == 700'000'000);

                p += time::milliseconds{400};
                snn_require(p.unix() == 1396316364);
                snn_require(p.nanosecond() == 100'000'000);

                time::point p2 = p + time::duration{10, 200'000'000};
                snn_require(p.unix() == 1396316364);
                snn_require(p.nanosecond() == 100'000'000);
                snn_require(p2.unix() == 1396316374);
                snn_require(p2.nanosecond() == 300'000'000);

                time::point p3 = p + time::microseconds{5'250'000};
                snn_require(p.unix() == 1396316364);
                snn_require(p.nanosecond() == 100'000'000);
                snn_require(p3.unix() == 1396316369);
                snn_require(p3.nanosecond() == 350'000'000);
            }

            // Subtraction

            snn_require(time::point{1396143555}.subtract(time::hours{1}).unix() == 1396139955);
            snn_require(time::point{1396143555}.subtract(time::duration{3600}).unix() ==
                        1396139955);

            {
                time::point p{time::duration{1396143555, 300'000'000}};
                p.subtract(time::hours{1}).subtract(time::duration{5});
                snn_require(p.unix() == 1396139950);
                snn_require(p.nanosecond() == 300'000'000);

                p -= time::days{2};
                snn_require(p.unix() == 1395967150);
                snn_require(p.nanosecond() == 300'000'000);

                p -= time::duration{3, 400'000'000};
                snn_require(p.unix() == 1395967146);
                snn_require(p.nanosecond() == 900'000'000);

                p -= time::milliseconds{400};
                snn_require(p.unix() == 1395967146);
                snn_require(p.nanosecond() == 500'000'000);

                time::point p2 = p - time::duration{10, 200'000'000};
                snn_require(p.unix() == 1395967146);
                snn_require(p.nanosecond() == 500'000'000);
                snn_require(p2.unix() == 1395967136);
                snn_require(p2.nanosecond() == 300'000'000);

                time::point p3 = p - time::microseconds{5'250'000};
                snn_require(p.unix() == 1395967146);
                snn_require(p.nanosecond() == 500'000'000);
                snn_require(p3.unix() == 1395967141);
                snn_require(p3.nanosecond() == 250'000'000);
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());
        snn_static_require(app::test_point());

        // now()
        {
            const time::point p = time::now();
            snn_require(p.year() >= 2022);
        }
        {
            const time::duration d = time::wall::since_epoch();
            time::point p1         = time::now();
            time::point p2         = time::now();
            snn_require(p1.unix() == d.seconds() || p1.unix() == (d.seconds() + 1));
            snn_require(p2 > p1);
        }

        // fmt::format
        {
            constexpr time::point p{2014, 3, 30, 0, 39, 15};
            static_assert(p.unix() == 1396139955);

            snn_require(fmt::format("{}", p) == "2014-03-30 00:39:15 +0000");
            snn_require(fmt::format("{:yyyy-mm-dd}", p) == "2014-03-30");
        }

        // point{y, mon, d, h, min, s}
        {
            for (loop::count lc{1000}; lc--;)
            {
                const auto y   = random::number<i16>(1905, 5001);
                const auto mon = random::number<i16>(-36, 37);
                const auto d   = random::number<i16>(-99, 100);
                const auto h   = random::number<i16>(-99, 100);
                const auto min = random::number<i16>(-999, 1000);
                const auto s   = random::number<i16>(-999, 1000);

                time::point p{y, mon, d, h, min, s};
                const auto u1 = p.unix();

                struct tm parts = {};
                parts.tm_year   = y - 1900;
                parts.tm_mon    = mon - 1;
                parts.tm_mday   = d;
                parts.tm_hour   = h;
                parts.tm_min    = min;
                parts.tm_sec    = s;
                const time_t u2 = ::timegm(&parts);

                if (u1 != u2)
                {
                    fmt::print_error("{}-{}-{} {}:{}:{}\n", y, mon, d, h, min, s);
                }

                snn_require(u1 == u2);
            }
        }

        // year(), month(), day(), hour(), minute(), second()
        // day_of_week(), day_of_year(), clock(), date()
        {
            for (loop::count lc{1000}; lc--;)
            {
                constexpr i64 min_incl = -1096225401600; // -32768-1-1 0:0:0
                constexpr i64 max_excl = 971890963199;   // 32767-12-31 23:59:59
                const auto unixtime    = random::number<i64>(min_incl, max_excl);

                time::point p{unixtime};

                struct tm parts = {};

                if (::gmtime_r(&unixtime, &parts) == nullptr)
                {
                    fmt::print_error("{}\n", unixtime);
                    snn_require(false);
                }

                const auto year  = static_cast<i16>(parts.tm_year + 1900);
                const auto month = static_cast<u8>(parts.tm_mon + 1);
                const auto day   = static_cast<u8>(parts.tm_mday);

                const auto hour = static_cast<u8>(parts.tm_hour);
                const auto min  = static_cast<u8>(parts.tm_min);
                const auto sec  = static_cast<u8>(parts.tm_sec);

                const auto day_of_week = static_cast<u8>(parts.tm_wday);
                const auto day_of_year = static_cast<u16>(parts.tm_yday + 1);

                const time::year_month_day ymd{year, month, day};
                const time::hour_minute_second hms{hour, min, sec};

                if (p.year() != year || p.month() != month || p.day() != day || p.hour() != hour ||
                    p.minute() != min || p.second() != sec || p.day_of_week() != day_of_week ||
                    p.day_of_year() != day_of_year || p.date() != ymd || p.time() != hms)
                {
                    fmt::print_error("{}\n", unixtime);
                    snn_require(false);
                }
            }
        }
    }
}
