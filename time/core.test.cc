// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/core.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(time::january == 1);
            snn_require(time::december == 12);

            snn_require(time::sunday == 0);
            snn_require(time::saturday == 6);

            snn_require(time::seconds_per_day<i32> == 86'400);
            snn_require(time::milliseconds_per_hour<i32> == 3'600'000);

            snn_require(time::is_leap_year(2016));
            snn_require(!time::is_leap_year(1999));

            snn_require(time::days_in_month(2016, 2).value_or_default() == 29);
            snn_require(time::days_in_month(2017, 2).value_or_default() == 28);

            snn_require(time::normalize<60, i32>(10, 90) == time::normalized<i32>{11, 30});

            // Structs

            // These are only for storage, use `time::point` for arithmetic.

            // hour_minute_second (alias: hms)
            const time::hour_minute_second hms{12, 39, 58};
            snn_require(hms.is_valid());
            snn_require(hms.is_pm());
            snn_require(!hms.is_am());
            snn_require(hms.hour() == 12);
            snn_require(hms.h == 12);
            snn_require(hms.minute() == 39);
            snn_require(hms.m == 39);
            snn_require(hms.second() == 58);
            snn_require(hms.s == 58);

            // year_month_day (alias: ymd)
            const time::year_month_day ymd{2017, 3, 21};
            snn_require(ymd.is_valid());
            snn_require(ymd.year() == 2017);
            snn_require(ymd.y == 2017);
            snn_require(ymd.month() == 3);
            snn_require(ymd.m == 3);
            snn_require(ymd.day() == 21);
            snn_require(ymd.d == 21);

            // year_week
            const time::year_week yw{2017, 52};
            snn_require(yw.is_valid());
            snn_require(yw.year() == 2017);
            snn_require(yw.y == 2017);
            snn_require(yw.week() == 52);
            snn_require(yw.w == 52);

            // year_month_day_hour_minute_second (alias: ymdhms)
            const time::ymdhms ymdhms{2017, 3, 21, 14, 15, 16};
            snn_require(ymdhms.is_valid());
            snn_require(ymdhms.date() == time::year_month_day{2017, 3, 21});
            snn_require(ymdhms.ymd == time::year_month_day{2017, 3, 21});
            snn_require(ymdhms.year() == 2017);
            snn_require(ymdhms.month() == 3);
            snn_require(ymdhms.day() == 21);
            snn_require(ymdhms.time() == time::hour_minute_second{14, 15, 16});
            snn_require(ymdhms.hms == time::hour_minute_second{14, 15, 16});
            snn_require(ymdhms.hour() == 14);
            snn_require(ymdhms.minute() == 15);
            snn_require(ymdhms.second() == 16);

            // year_month_day_hour_minute_second_nanosecond_offset (alias: ymdhmsno)
            constexpr time::ymdhmsno ymdhmsno{2017, 3, 21, 14, 15, 16, 500'000'000, -3600};
            snn_require(ymdhmsno.is_valid());
            snn_require(ymdhmsno.date() == time::year_month_day{2017, 3, 21});
            snn_require(ymdhmsno.ymd == time::year_month_day{2017, 3, 21});
            snn_require(ymdhmsno.year() == 2017);
            snn_require(ymdhmsno.month() == 3);
            snn_require(ymdhmsno.day() == 21);
            snn_require(ymdhmsno.time() == time::hour_minute_second{14, 15, 16});
            snn_require(ymdhmsno.hms == time::hour_minute_second{14, 15, 16});
            snn_require(ymdhmsno.hour() == 14);
            snn_require(ymdhmsno.minute() == 15);
            snn_require(ymdhmsno.second() == 16);
            snn_require(ymdhmsno.nanosecond() == 500'000'000);
            snn_require(ymdhmsno.n == 500'000'000);
            snn_require(ymdhmsno.offset() == -3600);
            snn_require(ymdhmsno.o == -3600);

            return true;
        }

        constexpr bool test_constants()
        {
            snn_require(time::january == 1);
            snn_require(time::february == 2);
            snn_require(time::march == 3);
            snn_require(time::april == 4);
            snn_require(time::may == 5);
            snn_require(time::june == 6);
            snn_require(time::july == 7);
            snn_require(time::august == 8);
            snn_require(time::september == 9);
            snn_require(time::october == 10);
            snn_require(time::november == 11);
            snn_require(time::december == 12);

            snn_require(time::sunday == 0);
            snn_require(time::monday == 1);
            snn_require(time::tuesday == 2);
            snn_require(time::wednesday == 3);
            snn_require(time::thursday == 4);
            snn_require(time::friday == 5);
            snn_require(time::saturday == 6);

            snn_require(time::seconds_per_minute<i8> == 60);
            snn_require(time::seconds_per_minute<u8> == 60);

            snn_require(time::seconds_per_hour<i16> == 3'600);
            snn_require(time::seconds_per_hour<u16> == 3'600);

            snn_require(time::seconds_per_minute<i32> == 60);
            snn_require(time::seconds_per_hour<i32> == 3'600);
            snn_require(time::seconds_per_day<i32> == 86'400);
            snn_require(time::seconds_per_week<i32> == 604'800);

            snn_require(time::seconds_per_minute<u32> == 60);
            snn_require(time::seconds_per_hour<u32> == 3'600);
            snn_require(time::seconds_per_day<u32> == 86'400);
            snn_require(time::seconds_per_week<u32> == 604'800);

            snn_require(time::milliseconds_per_second<i16> == 1'000);
            snn_require(time::milliseconds_per_second<u16> == 1'000);

            snn_require(time::milliseconds_per_second<i32> == 1'000);
            snn_require(time::milliseconds_per_minute<i32> == 60'000);
            snn_require(time::milliseconds_per_hour<i32> == 3'600'000);
            snn_require(time::milliseconds_per_day<i32> == 86'400'000);

            snn_require(time::milliseconds_per_second<u32> == 1'000);
            snn_require(time::milliseconds_per_minute<u32> == 60'000);
            snn_require(time::milliseconds_per_hour<u32> == 3'600'000);
            snn_require(time::milliseconds_per_day<u32> == 86'400'000);

            return true;
        }

        constexpr bool test_days_before_month()
        {
            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            using time::detail::days_before_month;
            snn_require(days_before_month[0] == 0);
            snn_require(days_before_month[1] == days_before_month[0] + 31);   // January
            snn_require(days_before_month[2] == days_before_month[1] + 28);   // February
            snn_require(days_before_month[3] == days_before_month[2] + 31);   // March
            snn_require(days_before_month[4] == days_before_month[3] + 30);   // April
            snn_require(days_before_month[5] == days_before_month[4] + 31);   // May
            snn_require(days_before_month[6] == days_before_month[5] + 30);   // June
            snn_require(days_before_month[7] == days_before_month[6] + 31);   // July
            snn_require(days_before_month[8] == days_before_month[7] + 31);   // August
            snn_require(days_before_month[9] == days_before_month[8] + 30);   // September
            snn_require(days_before_month[10] == days_before_month[9] + 31);  // October
            snn_require(days_before_month[11] == days_before_month[10] + 30); // November
            snn_require(days_before_month[12] == days_before_month[11] + 31); // December

            SNN_DIAGNOSTIC_POP

            return true;
        }

        constexpr bool test_is_leap_year()
        {
            snn_require(time::is_leap_year(2016));
            snn_require(time::is_leap_year(1600));
            snn_require(time::is_leap_year(2000));

            snn_require(!time::is_leap_year(1999));
            snn_require(!time::is_leap_year(1900));
            snn_require(!time::is_leap_year(2100));

            return true;
        }

        constexpr bool test_days_in_month()
        {
            // Leap year.
            snn_require(time::days_in_month(2016, 0).value_or_default() == 0); // Invalid month.
            snn_require(time::days_in_month(2016, 1).value_or_default() == 31);
            snn_require(time::days_in_month(2016, 2).value_or_default() == 29);
            snn_require(time::days_in_month(2016, 3).value_or_default() == 31);
            snn_require(time::days_in_month(2016, 4).value_or_default() == 30);
            snn_require(time::days_in_month(2016, 5).value_or_default() == 31);
            snn_require(time::days_in_month(2016, 6).value_or_default() == 30);
            snn_require(time::days_in_month(2016, 7).value_or_default() == 31);
            snn_require(time::days_in_month(2016, 8).value_or_default() == 31);
            snn_require(time::days_in_month(2016, 9).value_or_default() == 30);
            snn_require(time::days_in_month(2016, 10).value_or_default() == 31);
            snn_require(time::days_in_month(2016, 11).value_or_default() == 30);
            snn_require(time::days_in_month(2016, 12).value_or_default() == 31);
            snn_require(time::days_in_month(2016, 13).value_or_default() == 0);  // Invalid month.
            snn_require(time::days_in_month(2016, 255).value_or_default() == 0); // Invalid month.

            // Non-leap year.
            snn_require(time::days_in_month(2017, 0).value_or_default() == 0); // Invalid month.
            snn_require(time::days_in_month(2017, 1).value_or_default() == 31);
            snn_require(time::days_in_month(2017, 2).value_or_default() == 28);
            snn_require(time::days_in_month(2017, 3).value_or_default() == 31);
            snn_require(time::days_in_month(2017, 4).value_or_default() == 30);
            snn_require(time::days_in_month(2017, 5).value_or_default() == 31);
            snn_require(time::days_in_month(2017, 6).value_or_default() == 30);
            snn_require(time::days_in_month(2017, 7).value_or_default() == 31);
            snn_require(time::days_in_month(2017, 8).value_or_default() == 31);
            snn_require(time::days_in_month(2017, 9).value_or_default() == 30);
            snn_require(time::days_in_month(2017, 10).value_or_default() == 31);
            snn_require(time::days_in_month(2017, 11).value_or_default() == 30);
            snn_require(time::days_in_month(2017, 12).value_or_default() == 31);
            snn_require(time::days_in_month(2017, 13).value_or_default() == 0);  // Invalid month.
            snn_require(time::days_in_month(2017, 255).value_or_default() == 0); // Invalid month.

            return true;
        }

        constexpr bool test_normalize()
        {
            snn_require(time::normalize<60, i32>(14, 0) == time::normalized<i32>{14, 0});
            snn_require(time::normalize<60, i32>(14, -1) == time::normalized<i32>{13, 59});

            snn_require(time::normalize<60, i32>(14, 59) == time::normalized<i32>{14, 59});
            snn_require(time::normalize<60, i32>(14, 60) == time::normalized<i32>{15, 0});
            snn_require(time::normalize<60, i32>(14, 61) == time::normalized<i32>{15, 1});

            snn_require(time::normalize<60, i32>(10, -130) == time::normalized<i32>{7, 50});
            snn_require(time::normalize<60, i32>(10, -120) == time::normalized<i32>{8, 0});
            snn_require(time::normalize<60, i32>(10, -110) == time::normalized<i32>{8, 10});
            snn_require(time::normalize<60, i32>(10, -100) == time::normalized<i32>{8, 20});
            snn_require(time::normalize<60, i32>(10, -90) == time::normalized<i32>{8, 30});
            snn_require(time::normalize<60, i32>(10, -80) == time::normalized<i32>{8, 40});
            snn_require(time::normalize<60, i32>(10, -70) == time::normalized<i32>{8, 50});
            snn_require(time::normalize<60, i32>(10, -60) == time::normalized<i32>{9, 0});
            snn_require(time::normalize<60, i32>(10, -50) == time::normalized<i32>{9, 10});
            snn_require(time::normalize<60, i32>(10, -40) == time::normalized<i32>{9, 20});
            snn_require(time::normalize<60, i32>(10, -30) == time::normalized<i32>{9, 30});
            snn_require(time::normalize<60, i32>(10, -20) == time::normalized<i32>{9, 40});
            snn_require(time::normalize<60, i32>(10, -10) == time::normalized<i32>{9, 50});
            snn_require(time::normalize<60, i32>(10, 0) == time::normalized<i32>{10, 0});
            snn_require(time::normalize<60, i32>(10, 10) == time::normalized<i32>{10, 10});
            snn_require(time::normalize<60, i32>(10, 20) == time::normalized<i32>{10, 20});
            snn_require(time::normalize<60, i32>(10, 30) == time::normalized<i32>{10, 30});
            snn_require(time::normalize<60, i32>(10, 40) == time::normalized<i32>{10, 40});
            snn_require(time::normalize<60, i32>(10, 50) == time::normalized<i32>{10, 50});
            snn_require(time::normalize<60, i32>(10, 60) == time::normalized<i32>{11, 0});
            snn_require(time::normalize<60, i32>(10, 70) == time::normalized<i32>{11, 10});
            snn_require(time::normalize<60, i32>(10, 80) == time::normalized<i32>{11, 20});
            snn_require(time::normalize<60, i32>(10, 90) == time::normalized<i32>{11, 30});
            snn_require(time::normalize<60, i32>(10, 100) == time::normalized<i32>{11, 40});
            snn_require(time::normalize<60, i32>(10, 110) == time::normalized<i32>{11, 50});
            snn_require(time::normalize<60, i32>(10, 120) == time::normalized<i32>{12, 0});
            snn_require(time::normalize<60, i32>(10, 130) == time::normalized<i32>{12, 10});

            snn_require(time::normalize<60, i64>(0, constant::limit<i64>::min) ==
                        time::normalized<i64>{-153722867280912931, 52});

            snn_require(time::normalize<60, i64>(0, constant::limit<i64>::max) ==
                        time::normalized<i64>{153722867280912930, 7});

            return true;
        }

        constexpr bool test_hms()
        {
            static_assert(std::is_same_v<time::hour_minute_second, time::hms>);
            static_assert(sizeof(time::hour_minute_second) == 4);

            {
                constexpr time::hour_minute_second hms{12, 39, 58};
                snn_require(hms.is_valid());
                snn_require(hms.hour() == 12);
                snn_require(hms.h == 12);
                snn_require(hms.minute() == 39);
                snn_require(hms.m == 39);
                snn_require(hms.second() == 58);
                snn_require(hms.s == 58);
            }
            {
                constexpr time::hour_minute_second hms;
                snn_require(hms.is_valid());
                snn_require(hms.hour() == 0);
                snn_require(hms.h == 0);
                snn_require(hms.minute() == 0);
                snn_require(hms.m == 0);
                snn_require(hms.second() == 0);
                snn_require(hms.s == 0);
            }

            snn_require(time::hour_minute_second{0, 0, 0}.is_valid());
            snn_require(time::hour_minute_second{23, 59, 59}.is_valid());

            snn_require(!time::hour_minute_second{24, 59, 59}.is_valid());
            snn_require(!time::hour_minute_second{23, 60, 59}.is_valid());
            snn_require(!time::hour_minute_second{23, 59, 60}.is_valid());

            snn_require(time::hms{24, 59, 59}.validate().error_code() == time::error::invalid_hour);
            snn_require(time::hms{23, 60, 59}.validate().error_code() ==
                        time::error::invalid_minute);
            snn_require(time::hms{23, 59, 60}.validate().error_code() ==
                        time::error::invalid_second);

            snn_require(time::hms{0, 0, 0} == time::hms{0, 0, 0});
            snn_require(time::hms{1, 2, 3} == time::hms{1, 2, 3});
            snn_require(time::hms{23, 59, 59} == time::hms{23, 59, 59});

            snn_require(time::hms{23, 59, 59} != time::hms{22, 59, 59});
            snn_require(time::hms{23, 59, 59} != time::hms{23, 58, 59});
            snn_require(time::hms{23, 59, 59} != time::hms{23, 59, 58});

            snn_require(time::hms{14, 23, 59} < time::hms{15, 0, 0});
            snn_require(time::hms{0, 4, 59} < time::hms{0, 5, 0});
            snn_require(time::hms{0, 0, 58} < time::hms{0, 0, 59});

            // 12-hour clock.

            snn_require(time::hms{0, 0, 0}.is_am()); // 12 a.m.
            snn_require(time::hms{1, 0, 0}.is_am()); // 1 a.m.
            snn_require(time::hms{2, 0, 0}.is_am());
            snn_require(time::hms{3, 0, 0}.is_am());
            snn_require(time::hms{4, 0, 0}.is_am());
            snn_require(time::hms{5, 0, 0}.is_am());
            snn_require(time::hms{6, 0, 0}.is_am());
            snn_require(time::hms{7, 0, 0}.is_am());
            snn_require(time::hms{8, 0, 0}.is_am());
            snn_require(time::hms{9, 0, 0}.is_am());
            snn_require(time::hms{10, 0, 0}.is_am());
            snn_require(time::hms{11, 0, 0}.is_am());  // 11 a.m.
            snn_require(!time::hms{12, 0, 0}.is_am()); // 12 p.m.
            snn_require(!time::hms{13, 0, 0}.is_am()); // 1 p.m.
            snn_require(!time::hms{14, 0, 0}.is_am());
            snn_require(!time::hms{15, 0, 0}.is_am());
            snn_require(!time::hms{16, 0, 0}.is_am());
            snn_require(!time::hms{17, 0, 0}.is_am());
            snn_require(!time::hms{18, 0, 0}.is_am());
            snn_require(!time::hms{19, 0, 0}.is_am());
            snn_require(!time::hms{20, 0, 0}.is_am());
            snn_require(!time::hms{21, 0, 0}.is_am());
            snn_require(!time::hms{22, 0, 0}.is_am());
            snn_require(!time::hms{23, 0, 0}.is_am()); // 11 p.m.
            snn_require(!time::hms{64, 0, 0}.is_am()); // When invalid.

            snn_require(!time::hms{0, 0, 0}.is_pm()); // 12 a.m.
            snn_require(!time::hms{1, 0, 0}.is_pm()); // 1 a.m.
            snn_require(!time::hms{2, 0, 0}.is_pm());
            snn_require(!time::hms{3, 0, 0}.is_pm());
            snn_require(!time::hms{4, 0, 0}.is_pm());
            snn_require(!time::hms{5, 0, 0}.is_pm());
            snn_require(!time::hms{6, 0, 0}.is_pm());
            snn_require(!time::hms{7, 0, 0}.is_pm());
            snn_require(!time::hms{8, 0, 0}.is_pm());
            snn_require(!time::hms{9, 0, 0}.is_pm());
            snn_require(!time::hms{10, 0, 0}.is_pm());
            snn_require(!time::hms{11, 0, 0}.is_pm()); // 11 a.m.
            snn_require(time::hms{12, 0, 0}.is_pm());  // 12 p.m.
            snn_require(time::hms{13, 0, 0}.is_pm());  // 1 p.m.
            snn_require(time::hms{14, 0, 0}.is_pm());
            snn_require(time::hms{15, 0, 0}.is_pm());
            snn_require(time::hms{16, 0, 0}.is_pm());
            snn_require(time::hms{17, 0, 0}.is_pm());
            snn_require(time::hms{18, 0, 0}.is_pm());
            snn_require(time::hms{19, 0, 0}.is_pm());
            snn_require(time::hms{20, 0, 0}.is_pm());
            snn_require(time::hms{21, 0, 0}.is_pm());
            snn_require(time::hms{22, 0, 0}.is_pm());
            snn_require(time::hms{23, 0, 0}.is_pm()); // 11 p.m.
            snn_require(time::hms{64, 0, 0}.is_pm()); // When invalid.

            snn_require(time::hms{0, 0, 0}.hour_12() == 12); // 12 a.m.
            snn_require(time::hms{1, 0, 0}.hour_12() == 1);  // 1 a.m.
            snn_require(time::hms{2, 0, 0}.hour_12() == 2);
            snn_require(time::hms{3, 0, 0}.hour_12() == 3);
            snn_require(time::hms{4, 0, 0}.hour_12() == 4);
            snn_require(time::hms{5, 0, 0}.hour_12() == 5);
            snn_require(time::hms{6, 0, 0}.hour_12() == 6);
            snn_require(time::hms{7, 0, 0}.hour_12() == 7);
            snn_require(time::hms{8, 0, 0}.hour_12() == 8);
            snn_require(time::hms{9, 0, 0}.hour_12() == 9);
            snn_require(time::hms{10, 0, 0}.hour_12() == 10);
            snn_require(time::hms{11, 0, 0}.hour_12() == 11); // 11 a.m.
            snn_require(time::hms{12, 0, 0}.hour_12() == 12); // 12 p.m.
            snn_require(time::hms{13, 0, 0}.hour_12() == 1);  // 1 p.m.
            snn_require(time::hms{14, 0, 0}.hour_12() == 2);
            snn_require(time::hms{15, 0, 0}.hour_12() == 3);
            snn_require(time::hms{16, 0, 0}.hour_12() == 4);
            snn_require(time::hms{17, 0, 0}.hour_12() == 5);
            snn_require(time::hms{18, 0, 0}.hour_12() == 6);
            snn_require(time::hms{19, 0, 0}.hour_12() == 7);
            snn_require(time::hms{20, 0, 0}.hour_12() == 8);
            snn_require(time::hms{21, 0, 0}.hour_12() == 9);
            snn_require(time::hms{22, 0, 0}.hour_12() == 10);
            snn_require(time::hms{23, 0, 0}.hour_12() == 11); // 11 p.m.
            snn_require(time::hms{64, 0, 0}.hour_12() == 52); // When invalid.

            return true;
        }

        constexpr bool test_ymd()
        {
            static_assert(std::is_same_v<time::year_month_day, time::ymd>);
            static_assert(sizeof(time::year_month_day) == 8);

            {
                constexpr time::year_month_day ymd{2017, 3, 21};
                snn_require(ymd.is_valid());
                snn_require(ymd.year() == 2017);
                snn_require(ymd.y == 2017);
                snn_require(ymd.month() == 3);
                snn_require(ymd.m == 3);
                snn_require(ymd.day() == 21);
                snn_require(ymd.d == 21);
            }
            {
                constexpr time::year_month_day ymd; // Defaults to unix epoch.
                snn_require(ymd.is_valid());
                snn_require(ymd.year() == 1970);
                snn_require(ymd.y == 1970);
                snn_require(ymd.month() == 1);
                snn_require(ymd.m == 1);
                snn_require(ymd.day() == 1);
                snn_require(ymd.d == 1);
            }

            snn_require(time::year_month_day{2001, 1, 1}.is_valid());
            snn_require(time::year_month_day{2001, 12, 31}.is_valid());
            snn_require(time::year_month_day{2016, 2, 29}.is_valid()); // Leap year.
            snn_require(time::year_month_day{2017, 2, 28}.is_valid()); // Non-leap year.
            snn_require(time::year_month_day{-9999, 1, 1}.is_valid());
            snn_require(time::year_month_day{9999, 1, 1}.is_valid());

            snn_require(!time::year_month_day{-10000, 1, 1}.is_valid());
            snn_require(!time::year_month_day{10000, 1, 1}.is_valid());
            snn_require(!time::year_month_day{2001, 0, 1}.is_valid());
            snn_require(!time::year_month_day{2001, 1, 0}.is_valid());
            snn_require(!time::year_month_day{2001, 13, 0}.is_valid());
            snn_require(!time::year_month_day{2001, 13, 1}.is_valid());
            snn_require(!time::year_month_day{2001, 13, 31}.is_valid());
            snn_require(!time::year_month_day{2001, 12, 0}.is_valid());
            snn_require(!time::year_month_day{2001, 12, 32}.is_valid());
            snn_require(!time::year_month_day{2016, 2, 30}.is_valid());
            snn_require(!time::year_month_day{2017, 2, 29}.is_valid());

            snn_require(time::ymd{-10000, 1, 1}.validate().error_code() ==
                        time::error::invalid_year);
            snn_require(time::ymd{2017, 0, 1}.validate().error_code() ==
                        time::error::invalid_month);
            snn_require(time::ymd{2017, 1, 0}.validate().error_code() == time::error::invalid_day);

            snn_require(time::ymd{0, 0, 0} == time::ymd{0, 0, 0});
            snn_require(time::ymd{2001, 12, 31} == time::ymd{2001, 12, 31});
            snn_require(time::ymd{-1234, 5, 6} == time::ymd{-1234, 5, 6});

            snn_require(time::ymd{2001, 12, 31} != time::ymd{2000, 12, 31});
            snn_require(time::ymd{2001, 12, 31} != time::ymd{2001, 11, 31});
            snn_require(time::ymd{2001, 12, 31} != time::ymd{2001, 12, 30});

            snn_require(time::ymd{2001, 3, 15} < time::ymd{2002, 3, 15});
            snn_require(time::ymd{2001, 3, 15} < time::ymd{2001, 4, 15});
            snn_require(time::ymd{2001, 3, 15} < time::ymd{2001, 3, 16});

            return true;
        }

        constexpr bool test_year_week()
        {
            static_assert(sizeof(time::year_week) == 8);

            {
                constexpr time::year_week yw{2017, 52};
                snn_require(yw.is_valid());
                snn_require(yw.year() == 2017);
                snn_require(yw.y == 2017);
                snn_require(yw.week() == 52);
                snn_require(yw.w == 52);
            }
            {
                constexpr time::year_week yw; // Defaults to week of unix epoch.
                snn_require(yw.is_valid());
                snn_require(yw.year() == 1970);
                snn_require(yw.y == 1970);
                snn_require(yw.week() == 1);
                snn_require(yw.w == 1);
            }

            snn_require(time::year_week{2001, 1}.is_valid());
            snn_require(time::year_week{2001, 53}.is_valid());
            snn_require(time::year_week{-9999, 1}.is_valid());
            snn_require(time::year_week{-9999, 53}.is_valid());
            snn_require(time::year_week{9999, 1}.is_valid());
            snn_require(time::year_week{9999, 53}.is_valid());

            snn_require(!time::year_week{-10000, 1}.is_valid());
            snn_require(!time::year_week{10000, 1}.is_valid());
            snn_require(!time::year_week{2001, 0}.is_valid());
            snn_require(!time::year_week{2001, 54}.is_valid());

            snn_require(time::year_week{-10000, 1}.validate().error_code() ==
                        time::error::invalid_year);
            snn_require(time::year_week{2017, 0}.validate().error_code() ==
                        time::error::invalid_week);

            snn_require(time::year_week{2001, 12} == time::year_week{2001, 12});
            snn_require(time::year_week{-1234, 53} == time::year_week{-1234, 53});

            snn_require(time::year_week{2001, 1} != time::year_week{2000, 1});
            snn_require(time::year_week{2001, 1} != time::year_week{2001, 53});

            snn_require(time::year_week{2001, 1} < time::year_week{2002, 1});
            snn_require(time::year_week{2001, 32} < time::year_week{2001, 33});

            return true;
        }

        constexpr bool test_ymdhms()
        {
            static_assert(std::is_same_v<time::year_month_day_hour_minute_second, time::ymdhms>);
            static_assert(sizeof(time::year_month_day_hour_minute_second) == 12);

            {
                constexpr time::ymdhms val{2017, 3, 21, 14, 15, 16};

                snn_require(val.is_valid());

                snn_require(val.date() == time::year_month_day{2017, 3, 21});
                snn_require(val.ymd == time::year_month_day{2017, 3, 21});
                snn_require(val.year() == 2017);
                snn_require(val.month() == 3);
                snn_require(val.day() == 21);

                snn_require(val.time() == time::hour_minute_second{14, 15, 16});
                snn_require(val.hms == time::hour_minute_second{14, 15, 16});
                snn_require(val.hour() == 14);
                snn_require(val.minute() == 15);
                snn_require(val.second() == 16);
            }
            {
                constexpr time::ymdhms val; // Defaults to unix epoch.

                snn_require(val.is_valid());

                snn_require(val.date() == time::year_month_day{1970, 1, 1});
                snn_require(val.ymd == time::year_month_day{1970, 1, 1});
                snn_require(val.year() == 1970);
                snn_require(val.month() == 1);
                snn_require(val.day() == 1);

                snn_require(val.time() == time::hour_minute_second{0, 0, 0});
                snn_require(val.hms == time::hour_minute_second{0, 0, 0});
                snn_require(val.hour() == 0);
                snn_require(val.minute() == 0);
                snn_require(val.second() == 0);
            }

            snn_require(time::ymdhms{2001, 1, 1}.is_valid());
            snn_require(time::ymdhms{2001, 12, 31}.is_valid());
            snn_require(time::ymdhms{2016, 2, 29}.is_valid()); // Leap year.
            snn_require(time::ymdhms{2017, 2, 28}.is_valid()); // Non-leap year.
            snn_require(time::ymdhms{-9999, 1, 1}.is_valid());
            snn_require(time::ymdhms{9999, 1, 1}.is_valid());

            snn_require(!time::ymdhms{-10000, 1, 1}.is_valid());
            snn_require(!time::ymdhms{10000, 1, 1}.is_valid());
            snn_require(!time::ymdhms{2001, 0, 1}.is_valid());
            snn_require(!time::ymdhms{2001, 1, 0}.is_valid());
            snn_require(!time::ymdhms{2001, 13, 0}.is_valid());
            snn_require(!time::ymdhms{2001, 13, 1}.is_valid());
            snn_require(!time::ymdhms{2001, 13, 31}.is_valid());
            snn_require(!time::ymdhms{2001, 12, 0}.is_valid());
            snn_require(!time::ymdhms{2001, 12, 32}.is_valid());
            snn_require(!time::ymdhms{2016, 2, 30}.is_valid());
            snn_require(!time::ymdhms{2017, 2, 29}.is_valid());

            snn_require(time::ymdhms{2001, 1, 1, 14, 15, 16}.is_valid());
            snn_require(!time::ymdhms{2001, 1, 1, 24, 15, 16}.is_valid());
            snn_require(!time::ymdhms{2001, 1, 1, 14, 60, 16}.is_valid());
            snn_require(!time::ymdhms{2001, 1, 1, 14, 15, 60}.is_valid());

            snn_require(time::ymdhms{-10000, 1, 1}.validate().error_code() ==
                        time::error::invalid_year);
            snn_require(time::ymdhms{2017, 0, 1}.validate().error_code() ==
                        time::error::invalid_month);
            snn_require(time::ymdhms{2017, 1, 0}.validate().error_code() ==
                        time::error::invalid_day);

            snn_require(time::ymdhms{2017, 1, 1, 24, 0, 0}.validate().error_code() ==
                        time::error::invalid_hour);
            snn_require(time::ymdhms{2017, 1, 1, 14, 60, 0}.validate().error_code() ==
                        time::error::invalid_minute);
            snn_require(time::ymdhms{2017, 1, 1, 14, 15, 60}.validate().error_code() ==
                        time::error::invalid_second);

            snn_require(time::ymdhms{2017, 3, 21, 14, 15, 16} ==
                        time::ymdhms{2017, 3, 21, 14, 15, 16});

            snn_require(time::ymdhms{2017, 3, 21, 14, 15, 16} !=
                        time::ymdhms{2017, 3, 21, 14, 15, 17});

            snn_require(time::ymdhms{2017, 3, 21, 14, 15, 16} <
                        time::ymdhms{2017, 3, 21, 14, 15, 17});
            snn_require(!(time::ymdhms{2017, 4, 21, 14, 15, 16} <
                          time::ymdhms{2017, 3, 21, 14, 15, 16}));

            return true;
        }

        constexpr bool test_ymdhmsno()
        {
            static_assert(std::is_same_v<time::year_month_day_hour_minute_second_nanosecond_offset,
                                         time::ymdhmsno>);
            static_assert(sizeof(time::year_month_day_hour_minute_second_nanosecond_offset) == 20);

            {
                constexpr time::ymdhmsno val{2017, 3, 21, 14, 15, 16, 500'000'000, -3600};

                snn_require(val.is_valid());

                snn_require(val.date() == time::year_month_day{2017, 3, 21});
                snn_require(val.ymd == time::year_month_day{2017, 3, 21});
                snn_require(val.year() == 2017);
                snn_require(val.month() == 3);
                snn_require(val.day() == 21);

                snn_require(val.time() == time::hour_minute_second{14, 15, 16});
                snn_require(val.hms == time::hour_minute_second{14, 15, 16});
                snn_require(val.hour() == 14);
                snn_require(val.minute() == 15);
                snn_require(val.second() == 16);

                snn_require(val.nanosecond() == 500'000'000);
                snn_require(val.n == 500'000'000);

                snn_require(val.offset() == -3600);
                snn_require(val.o == -3600);
            }
            {
                constexpr time::ymdhmsno val; // Defaults to unix epoch.

                snn_require(val.is_valid());

                snn_require(val.date() == time::year_month_day{1970, 1, 1});
                snn_require(val.ymd == time::year_month_day{1970, 1, 1});
                snn_require(val.year() == 1970);
                snn_require(val.month() == 1);
                snn_require(val.day() == 1);

                snn_require(val.time() == time::hour_minute_second{0, 0, 0});
                snn_require(val.hms == time::hour_minute_second{0, 0, 0});
                snn_require(val.hour() == 0);
                snn_require(val.minute() == 0);
                snn_require(val.second() == 0);

                snn_require(val.nanosecond() == 0);
                snn_require(val.n == 0);

                snn_require(val.offset() == 0);
                snn_require(val.o == 0);
            }

            snn_require(time::ymdhmsno{2001, 1, 1}.is_valid());
            snn_require(time::ymdhmsno{2001, 12, 31}.is_valid());
            snn_require(time::ymdhmsno{2016, 2, 29}.is_valid()); // Leap year.
            snn_require(time::ymdhmsno{2017, 2, 28}.is_valid()); // Non-leap year.
            snn_require(time::ymdhmsno{-9999, 1, 1}.is_valid());
            snn_require(time::ymdhmsno{9999, 1, 1}.is_valid());

            snn_require(!time::ymdhmsno{-10000, 1, 1}.is_valid());
            snn_require(!time::ymdhmsno{10000, 1, 1}.is_valid());
            snn_require(!time::ymdhmsno{2001, 0, 1}.is_valid());
            snn_require(!time::ymdhmsno{2001, 1, 0}.is_valid());
            snn_require(!time::ymdhmsno{2001, 13, 0}.is_valid());
            snn_require(!time::ymdhmsno{2001, 13, 1}.is_valid());
            snn_require(!time::ymdhmsno{2001, 13, 31}.is_valid());
            snn_require(!time::ymdhmsno{2001, 12, 0}.is_valid());
            snn_require(!time::ymdhmsno{2001, 12, 32}.is_valid());
            snn_require(!time::ymdhmsno{2016, 2, 30}.is_valid());
            snn_require(!time::ymdhmsno{2017, 2, 29}.is_valid());

            snn_require(time::ymdhmsno{2001, 1, 1, 14, 15, 16}.is_valid());
            snn_require(!time::ymdhmsno{2001, 1, 1, 24, 15, 16}.is_valid());
            snn_require(!time::ymdhmsno{2001, 1, 1, 14, 60, 16}.is_valid());
            snn_require(!time::ymdhmsno{2001, 1, 1, 14, 15, 60}.is_valid());

            snn_require(time::ymdhmsno{2001, 1, 1, 14, 15, 16, 999'999'999, -3600}.is_valid());
            snn_require(!time::ymdhmsno{2001, 1, 1, 14, 15, 16, 1000'000'000, -3600}.is_valid());
            snn_require(!time::ymdhmsno{2001, 1, 1, 14, 15, 16, 999'999'999, -99999}.is_valid());

            snn_require(time::ymdhmsno{-10000, 1, 1}.validate().error_code() ==
                        time::error::invalid_year);
            snn_require(time::ymdhmsno{2017, 0, 1}.validate().error_code() ==
                        time::error::invalid_month);
            snn_require(time::ymdhmsno{2017, 1, 0}.validate().error_code() ==
                        time::error::invalid_day);

            snn_require(time::ymdhmsno{2017, 1, 1, 24, 0, 0}.validate().error_code() ==
                        time::error::invalid_hour);
            snn_require(time::ymdhmsno{2017, 1, 1, 14, 60, 0}.validate().error_code() ==
                        time::error::invalid_minute);
            snn_require(time::ymdhmsno{2017, 1, 1, 14, 15, 60}.validate().error_code() ==
                        time::error::invalid_second);
            snn_require(
                time::ymdhmsno{2017, 1, 1, 0, 0, 0, 1'000'000'000, 0}.validate().error_code() ==
                time::error::invalid_nanosecond);
            snn_require(time::ymdhmsno{2017, 1, 1, 0, 0, 0, 0, -99999}.validate().error_code() ==
                        time::error::invalid_offset);

            snn_require(time::ymdhmsno{2017, 3, 21, 14, 15, 16, 500'000'000, -3600} ==
                        time::ymdhmsno{2017, 3, 21, 14, 15, 16, 500'000'000, -3600});

            snn_require(time::ymdhmsno{2017, 3, 21, 14, 15, 16, 500'000'000, -3600} !=
                        time::ymdhmsno{2017, 3, 21, 14, 15, 17, 500'000'000, -3600});

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_constants());
        snn_static_require(app::test_days_before_month());
        snn_static_require(app::test_is_leap_year());
        snn_static_require(app::test_days_in_month());
        snn_static_require(app::test_normalize());
        snn_static_require(app::test_hms());
        snn_static_require(app::test_ymd());
        snn_static_require(app::test_year_week());
        snn_static_require(app::test_ymdhms());
        snn_static_require(app::test_ymdhmsno());
    }
}
