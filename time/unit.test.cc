// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/unit.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                time::seconds sec{123};
                snn_require(sec);
                snn_require(sec.has_value());
                snn_require(sec.value() == 123);
                snn_require(sec.value(assume::has_value) == 123);
                snn_require(sec.value_or(-1) == 123);
            }
            {
                time::seconds sec;
                snn_require(!sec);
                snn_require(sec.has_value());
                snn_require(sec.value() == 0);
                snn_require(sec.value(assume::has_value) == 0);
                snn_require(sec.value_or(-1) == 0);
            }
            {
                time::minutes min{10};
                min.add(time::hours{3}).add(time::seconds{300});
                snn_require(min.has_value());
                snn_require(min.value() == 195);
            }
            {
                snn_require(time::nanoseconds{86'409'876'543'210}.to<time::minutes>() == 1'440);
                snn_require(time::nanoseconds{86'409'876'543'210}.duration() ==
                            time::duration{86'409, 876'543'210});
            }
            {
                static_assert(time::seconds::is_smaller_than<time::minutes>());
                snn_require(time::minutes{2} > time::seconds{90});
                snn_require(time::minutes{2} == time::seconds{120});
            }
            {
                // No overflow.
                snn_require(time::nanoseconds{time::days{106'751}}.has_value());

                // Overflow.
                time::nanoseconds ns{time::days{106'752}};
                snn_require(!ns);
                snn_require(!ns.has_value());
                snn_require(ns.value_or(-1) == -1);
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        static_assert(sizeof(time::seconds) == 16);

        // Type traits.

        static_assert(time::is_unit_v<time::nanoseconds>);
        static_assert(time::is_unit_v<time::hours>);
        static_assert(!time::is_unit_v<i64>);
        static_assert(!time::is_unit_v<num::safe<i64>>);

        // Conversions

        // No rounding takes place (integer division).

        static_assert(time::nanoseconds{86409876543210}.to<time::nanoseconds>() == 86409876543210);
        static_assert(time::nanoseconds{86409876543210}.to<time::microseconds>() == 86409876543);
        static_assert(time::nanoseconds{86409876543210}.to<time::milliseconds>() == 86409876);
        static_assert(time::nanoseconds{86409876543210}.to<time::seconds>() == 86409);
        static_assert(time::nanoseconds{86409876543210}.to<time::minutes>() == 1440);
        static_assert(time::nanoseconds{86409876543210}.to<time::hours>() == 24);
        static_assert(time::nanoseconds{86409876543210}.to<time::days>() == 1);

        static_assert(time::nanoseconds{-86409876543210}.to<time::nanoseconds>() ==
                      -86409876543210);
        static_assert(time::nanoseconds{-86409876543210}.to<time::microseconds>() == -86409876543);
        static_assert(time::nanoseconds{-86409876543210}.to<time::milliseconds>() == -86409876);
        static_assert(time::nanoseconds{-86409876543210}.to<time::seconds>() == -86409);
        static_assert(time::nanoseconds{-86409876543210}.to<time::minutes>() == -1440);
        static_assert(time::nanoseconds{-86409876543210}.to<time::hours>() == -24);
        static_assert(time::nanoseconds{-86409876543210}.to<time::days>() == -1);

        static_assert(time::nanoseconds{86'409'876'543'210}.duration() ==
                      time::duration{86'409, 876'543'210});
        static_assert(time::nanoseconds{1'700'000'000}.duration() ==
                      time::duration{1, 700'000'000});
        static_assert(time::nanoseconds{-86'409'876'543'210}.duration() ==
                      time::duration{-86'410, 123'456'790});
        static_assert(time::nanoseconds{-1'700'000'000}.duration() ==
                      time::duration{-2, 300'000'000});

        static_assert(time::microseconds{86409876543}.to<time::nanoseconds>() == 86409876543000);
        static_assert(time::microseconds{86409876543}.to<time::microseconds>() == 86409876543);
        static_assert(time::microseconds{86409876543}.to<time::milliseconds>() == 86409876);
        static_assert(time::microseconds{86409876543}.to<time::seconds>() == 86409);
        static_assert(time::microseconds{86409876543}.to<time::minutes>() == 1440);
        static_assert(time::microseconds{86409876543}.to<time::hours>() == 24);
        static_assert(time::microseconds{86409876543}.to<time::days>() == 1);

        static_assert(time::microseconds{-86409876543}.to<time::nanoseconds>() == -86409876543000);
        static_assert(time::microseconds{-86409876543}.to<time::microseconds>() == -86409876543);
        static_assert(time::microseconds{-86409876543}.to<time::milliseconds>() == -86409876);
        static_assert(time::microseconds{-86409876543}.to<time::seconds>() == -86409);
        static_assert(time::microseconds{-86409876543}.to<time::minutes>() == -1440);
        static_assert(time::microseconds{-86409876543}.to<time::hours>() == -24);
        static_assert(time::microseconds{-86409876543}.to<time::days>() == -1);

        static_assert(time::microseconds{86'409'876'543}.duration() ==
                      time::duration{86'409, 876'543'000});
        static_assert(time::microseconds{1'700'000}.duration() == time::duration{1, 700'000'000});
        static_assert(time::microseconds{-86'409'876'543}.duration() ==
                      time::duration{-86'410, 123'457'000});
        static_assert(time::microseconds{-1'700'000}.duration() == time::duration{-2, 300'000'000});

        static_assert(time::milliseconds{86409876}.to<time::nanoseconds>() == 86409876000000);
        static_assert(time::milliseconds{86409876}.to<time::microseconds>() == 86409876000);
        static_assert(time::milliseconds{86409876}.to<time::milliseconds>() == 86409876);
        static_assert(time::milliseconds{86409876}.to<time::seconds>() == 86409);
        static_assert(time::milliseconds{86409876}.to<time::minutes>() == 1440);
        static_assert(time::milliseconds{86409876}.to<time::hours>() == 24);
        static_assert(time::milliseconds{86409876}.to<time::days>() == 1);

        static_assert(time::milliseconds{-86409876}.to<time::nanoseconds>() == -86409876000000);
        static_assert(time::milliseconds{-86409876}.to<time::microseconds>() == -86409876000);
        static_assert(time::milliseconds{-86409876}.to<time::milliseconds>() == -86409876);
        static_assert(time::milliseconds{-86409876}.to<time::seconds>() == -86409);
        static_assert(time::milliseconds{-86409876}.to<time::minutes>() == -1440);
        static_assert(time::milliseconds{-86409876}.to<time::hours>() == -24);
        static_assert(time::milliseconds{-86409876}.to<time::days>() == -1);

        static_assert(time::milliseconds{86'409'876}.duration() ==
                      time::duration{86'409, 876'000'000});
        static_assert(time::milliseconds{1'700}.duration() == time::duration{1, 700'000'000});
        static_assert(time::milliseconds{-86'409'876}.duration() ==
                      time::duration{-86'410, 124'000'000});
        static_assert(time::milliseconds{-1'700}.duration() == time::duration{-2, 300'000'000});

        static_assert(time::seconds{86409}.to<time::nanoseconds>() == 86409000000000);
        static_assert(time::seconds{86409}.to<time::microseconds>() == 86409000000);
        static_assert(time::seconds{86409}.to<time::milliseconds>() == 86409000);
        static_assert(time::seconds{86409}.to<time::seconds>() == 86409);
        static_assert(time::seconds{86409}.to<time::minutes>() == 1440);
        static_assert(time::seconds{86409}.to<time::hours>() == 24);
        static_assert(time::seconds{86409}.to<time::days>() == 1);

        static_assert(time::seconds{-86409}.to<time::nanoseconds>() == -86409000000000);
        static_assert(time::seconds{-86409}.to<time::microseconds>() == -86409000000);
        static_assert(time::seconds{-86409}.to<time::milliseconds>() == -86409000);
        static_assert(time::seconds{-86409}.to<time::seconds>() == -86409);
        static_assert(time::seconds{-86409}.to<time::minutes>() == -1440);
        static_assert(time::seconds{-86409}.to<time::hours>() == -24);
        static_assert(time::seconds{-86409}.to<time::days>() == -1);

        static_assert(time::seconds{86409}.duration() == time::duration{86409, 0});
        static_assert(time::seconds{-86409}.duration() == time::duration{-86409, 0});

        static_assert(time::minutes{1440}.to<time::nanoseconds>() == 86400000000000);
        static_assert(time::minutes{1440}.to<time::microseconds>() == 86400000000);
        static_assert(time::minutes{1440}.to<time::milliseconds>() == 86400000);
        static_assert(time::minutes{1440}.to<time::seconds>() == 86400);
        static_assert(time::minutes{1440}.to<time::minutes>() == 1440);
        static_assert(time::minutes{1440}.to<time::hours>() == 24);
        static_assert(time::minutes{1440}.to<time::days>() == 1);

        static_assert(time::minutes{-1440}.to<time::nanoseconds>() == -86400000000000);
        static_assert(time::minutes{-1440}.to<time::microseconds>() == -86400000000);
        static_assert(time::minutes{-1440}.to<time::milliseconds>() == -86400000);
        static_assert(time::minutes{-1440}.to<time::seconds>() == -86400);
        static_assert(time::minutes{-1440}.to<time::minutes>() == -1440);
        static_assert(time::minutes{-1440}.to<time::hours>() == -24);
        static_assert(time::minutes{-1440}.to<time::days>() == -1);

        static_assert(time::minutes{1440}.duration() == time::duration{86400, 0});
        static_assert(time::minutes{-1440}.duration() == time::duration{-86400, 0});

        static_assert(time::hours{24}.to<time::nanoseconds>() == 86400000000000);
        static_assert(time::hours{24}.to<time::microseconds>() == 86400000000);
        static_assert(time::hours{24}.to<time::milliseconds>() == 86400000);
        static_assert(time::hours{24}.to<time::seconds>() == 86400);
        static_assert(time::hours{24}.to<time::minutes>() == 1440);
        static_assert(time::hours{24}.to<time::hours>() == 24);
        static_assert(time::hours{24}.to<time::days>() == 1);

        static_assert(time::hours{-24}.to<time::nanoseconds>() == -86400000000000);
        static_assert(time::hours{-24}.to<time::microseconds>() == -86400000000);
        static_assert(time::hours{-24}.to<time::milliseconds>() == -86400000);
        static_assert(time::hours{-24}.to<time::seconds>() == -86400);
        static_assert(time::hours{-24}.to<time::minutes>() == -1440);
        static_assert(time::hours{-24}.to<time::hours>() == -24);
        static_assert(time::hours{-24}.to<time::days>() == -1);

        static_assert(time::hours{24}.duration() == time::duration{86400, 0});
        static_assert(time::hours{-24}.duration() == time::duration{-86400, 0});

        static_assert(time::days{1}.to<time::nanoseconds>() == 86400000000000);
        static_assert(time::days{1}.to<time::microseconds>() == 86400000000);
        static_assert(time::days{1}.to<time::milliseconds>() == 86400000);
        static_assert(time::days{1}.to<time::seconds>() == 86400);
        static_assert(time::days{1}.to<time::minutes>() == 1440);
        static_assert(time::days{1}.to<time::hours>() == 24);
        static_assert(time::days{1}.to<time::days>() == 1);

        static_assert(time::days{-1}.to<time::nanoseconds>() == -86400000000000);
        static_assert(time::days{-1}.to<time::microseconds>() == -86400000000);
        static_assert(time::days{-1}.to<time::milliseconds>() == -86400000);
        static_assert(time::days{-1}.to<time::seconds>() == -86400);
        static_assert(time::days{-1}.to<time::minutes>() == -1440);
        static_assert(time::days{-1}.to<time::hours>() == -24);
        static_assert(time::days{-1}.to<time::days>() == -1);

        static_assert(time::days{1}.duration() == time::duration{86400, 0});
        static_assert(time::days{-1}.duration() == time::duration{-86400, 0});

        {
            constexpr auto max = constant::limit<i64>::max;
            static_assert(time::nanoseconds{max}.to<time::nanoseconds>() == max);
            static_assert(time::nanoseconds{max}.to<time::microseconds>() == 9223372036854775);
            static_assert(time::nanoseconds{max}.to<time::milliseconds>() == 9223372036854);
            static_assert(time::nanoseconds{max}.to<time::seconds>() == 9223372036);
            static_assert(time::nanoseconds{max}.to<time::minutes>() == 153722867);
            static_assert(time::nanoseconds{max}.to<time::hours>() == 2562047);
            static_assert(time::nanoseconds{max}.to<time::days>() == 106751);

            static_assert(time::nanoseconds{max}.duration() ==
                          time::duration{9'223'372'036, 854'775'807});

            constexpr auto min = constant::limit<i64>::min;
            static_assert(time::nanoseconds{min}.to<time::nanoseconds>() == min);
            static_assert(time::nanoseconds{min}.to<time::microseconds>() == -9223372036854775);
            static_assert(time::nanoseconds{min}.to<time::milliseconds>() == -9223372036854);
            static_assert(time::nanoseconds{min}.to<time::seconds>() == -9223372036);
            static_assert(time::nanoseconds{min}.to<time::minutes>() == -153722867);
            static_assert(time::nanoseconds{min}.to<time::hours>() == -2562047);
            static_assert(time::nanoseconds{min}.to<time::days>() == -106751);

            static_assert(time::nanoseconds{min}.duration() ==
                          time::duration{-9'223'372'037, 145'224'192});
        }

        // Implicit conversion.
        {
            constexpr time::minutes m = time::hours{2};
            static_assert(m);
            static_assert(m.has_value());
            static_assert(m.value() == 120);
            static_assert(m == 120);

            constexpr time::hours h = m;
            static_assert(h);
            static_assert(h.has_value());
            static_assert(h.value() == 2);
            static_assert(h == 2);
        }

        // Implicit conversion (overflow).
        {
            constexpr time::nanoseconds ns = time::hours{2'562'048};
            static_assert(!ns);
            static_assert(!ns.has_value());

            constexpr time::hours h = ns;
            static_assert(!h);
            static_assert(!h.has_value());
            static_assert(!(h == 2'562'048));
        }

        // Conversion to duration with overflow.
        {
            constexpr time::nanoseconds ns = time::hours{2'562'048};
            static_assert(!ns);
            static_assert(!ns.has_value());

            constexpr optional<time::duration> opt = ns.duration();
            static_assert(!opt);
            static_assert(!opt.has_value());
        }

        // Conversion from duration.
        {
            constexpr time::minutes m{time::duration{120, 500'000'000}};
            static_assert(m);
            static_assert(m.has_value());
            static_assert(m.value() == 2);
            static_assert(m == 2);
        }
        {
            constexpr time::microseconds us{time::duration{120, 500'000'000}};
            static_assert(us);
            static_assert(us.has_value());
            static_assert(us.value() == 120'500'000);
            static_assert(us == 120'500'000);
        }
        {
            constexpr time::microseconds us{time::duration{-120, 300'000'000}};
            static_assert(us);
            static_assert(us.has_value());
            static_assert(us.value() == -119'700'000);
            static_assert(us == -119'700'000);
        }

        // Addition
        {
            static_assert(time::minutes{10}.add(time::hours{3}).add(time::seconds{300}) == 195);

            time::seconds sec;
            snn_require(sec.value() == 0);

            sec.add(4);
            snn_require(sec.value() == 4);

            sec.add(time::minutes{3});
            snn_require(sec.value() == 184);

            sec.add(time::seconds{5});
            snn_require(sec.value() == 189);

            sec.add(time::milliseconds{999});
            snn_require(sec.value() == 189);

            sec.add(time::milliseconds{9999});
            snn_require(sec.value() == 198);

            sec += 2;
            snn_require(sec.value() == 200);

            sec += time::hours{1};
            snn_require(sec.value() == 3800);
        }
        {
            decltype(auto) u = time::milliseconds{4} + 10;
            static_assert(std::is_same_v<decltype(u), time::milliseconds>);
            snn_require(u.value() == 14);
        }
        {
            decltype(auto) u = time::milliseconds{4} + time::milliseconds{10};
            static_assert(std::is_same_v<decltype(u), time::milliseconds>);
            snn_require(u.value() == 14);
        }
        {
            decltype(auto) u = time::milliseconds{4} + time::minutes{2};
            static_assert(std::is_same_v<decltype(u), time::milliseconds>); // Smallest unit.
            snn_require(u.value() == 120'004);
        }
        {
            decltype(auto) u = time::minutes{2} + time::milliseconds{4};
            static_assert(std::is_same_v<decltype(u), time::milliseconds>); // Smallest unit.
            snn_require(u.value() == 120'004);
        }
        {
            time::seconds sec;
            snn_require(sec.value() == 0);

            decltype(auto) s = ++sec;
            static_assert(std::is_same_v<decltype(s), time::seconds&>);
            snn_require(sec.value() == 1);
            snn_require(s.value() == 1);
        }
        {
            time::seconds sec;
            snn_require(sec.value() == 0);

            decltype(auto) s = sec++;
            static_assert(std::is_same_v<decltype(s), time::seconds>);
            snn_require(sec.value() == 1);
            snn_require(s.value() == 0);
        }

        // Subtraction
        {
            static_assert(
                time::minutes{200}.subtract(time::hours{3}).subtract(time::seconds{300}) == 15);

            time::seconds sec{200};
            snn_require(sec.value() == 200);

            sec.subtract(20);
            snn_require(sec.value() == 180);

            sec.subtract(time::minutes{2});
            snn_require(sec.value() == 60);

            sec.subtract(time::seconds{5});
            snn_require(sec.value() == 55);

            sec.subtract(time::milliseconds{999});
            snn_require(sec.value() == 55);

            sec.subtract(time::milliseconds{9999});
            snn_require(sec.value() == 46);

            sec -= 2;
            snn_require(sec.value() == 44);

            sec -= time::hours{1};
            snn_require(sec.value() == -3556);
        }
        {
            decltype(auto) u = time::milliseconds{10} - 4;
            static_assert(std::is_same_v<decltype(u), time::milliseconds>);
            snn_require(u.value() == 6);
        }
        {
            decltype(auto) u = time::milliseconds{10} - time::milliseconds{4};
            static_assert(std::is_same_v<decltype(u), time::milliseconds>);
            snn_require(u.value() == 6);
        }
        {
            decltype(auto) u = time::seconds{200} - time::minutes{2};
            static_assert(std::is_same_v<decltype(u), time::seconds>); // Smallest unit.
            snn_require(u.value() == 80);
        }
        {
            decltype(auto) u = time::minutes{2} - time::seconds{180};
            static_assert(std::is_same_v<decltype(u), time::seconds>); // Smallest unit.
            snn_require(u.value() == -60);
        }
        {
            time::seconds sec;
            snn_require(sec.value() == 0);

            decltype(auto) s = --sec;
            static_assert(std::is_same_v<decltype(s), time::seconds&>);
            snn_require(sec.value() == -1);
            snn_require(s.value() == -1);
        }
        {
            time::seconds sec;
            snn_require(sec.value() == 0);

            decltype(auto) s = sec--;
            static_assert(std::is_same_v<decltype(s), time::seconds>);
            snn_require(sec.value() == -1);
            snn_require(s.value() == 0);
        }

        // Multiplication
        {
            static_assert(time::minutes{20}.multiply(time::hours{1}).multiply(2) == 2400);

            time::seconds sec{2};
            snn_require(sec.value() == 2);

            sec.multiply(2);
            snn_require(sec.value() == 4);

            sec.multiply(time::minutes{1});
            snn_require(sec.value() == 240);

            sec.multiply(time::seconds{1});
            snn_require(sec.value() == 240);

            sec.multiply(time::milliseconds{1999});
            snn_require(sec.value() == 240);

            sec *= 2;
            snn_require(sec.value() == 480);

            sec *= time::minutes{2};
            snn_require(sec.value() == 57'600);
        }
        {
            decltype(auto) u = time::milliseconds{10} * 4;
            static_assert(std::is_same_v<decltype(u), time::milliseconds>);
            snn_require(u.value() == 40);
        }
        {
            decltype(auto) u = time::milliseconds{10} * time::milliseconds{4};
            static_assert(std::is_same_v<decltype(u), time::milliseconds>);
            snn_require(u.value() == 40);
        }
        {
            decltype(auto) u = time::seconds{3} * time::minutes{2};
            static_assert(std::is_same_v<decltype(u), time::seconds>); // Smallest unit.
            snn_require(u.value() == 360);
        }
        {
            decltype(auto) u = time::minutes{2} * time::seconds{3};
            static_assert(std::is_same_v<decltype(u), time::seconds>); // Smallest unit.
            snn_require(u.value() == 360);
        }

        // Division
        {
            static_assert(time::minutes{480}.divide(time::hours{1}).divide(2) == 4);

            time::seconds sec{600};
            snn_require(sec.value() == 600);

            sec.divide(2);
            snn_require(sec.value() == 300);

            sec.divide(time::minutes{1});
            snn_require(sec.value() == 5);

            sec.divide(time::seconds{1});
            snn_require(sec.value() == 5);

            sec.divide(time::milliseconds{1999});
            snn_require(sec.value() == 5);

            sec /= 2;
            snn_require(sec.value() == 2);

            sec /= time::seconds{2};
            snn_require(sec.value() == 1);
        }
        {
            decltype(auto) u = time::milliseconds{10} / 2;
            static_assert(std::is_same_v<decltype(u), time::milliseconds>);
            snn_require(u.value() == 5);
        }
        {
            decltype(auto) u = time::milliseconds{10} / time::milliseconds{2};
            static_assert(std::is_same_v<decltype(u), time::milliseconds>);
            snn_require(u.value() == 5);
        }
        {
            decltype(auto) u = time::seconds{360} / time::minutes{2};
            static_assert(std::is_same_v<decltype(u), time::seconds>); // Smallest unit.
            snn_require(u.value() == 3);
        }
        {
            decltype(auto) u = time::minutes{2} / time::seconds{30};
            static_assert(std::is_same_v<decltype(u), time::seconds>); // Smallest unit.
            snn_require(u.value() == 4);
        }

        // Modulo
        {
            static_assert(time::minutes{500}.modulo(time::hours{1}) == 20);
            static_assert(time::minutes{500}.modulo(time::hours{1}).modulo(30) == 20);

            time::seconds sec{500};
            snn_require(sec.value() == 500);

            sec.modulo(600);
            snn_require(sec.value() == 500);

            sec.modulo(time::minutes{1});
            snn_require(sec.value() == 20);

            sec.modulo(time::seconds{3});
            snn_require(sec.value() == 2);

            sec.modulo(time::milliseconds{3999});
            snn_require(sec.value() == 2);

            sec %= 3;
            snn_require(sec.value() == 2);

            sec %= time::seconds{2};
            snn_require(sec.value() == 0);
        }
        {
            decltype(auto) u = time::milliseconds{10} % 3;
            static_assert(std::is_same_v<decltype(u), time::milliseconds>);
            snn_require(u.value() == 1);
        }
        {
            decltype(auto) u = time::milliseconds{10} % time::milliseconds{3};
            static_assert(std::is_same_v<decltype(u), time::milliseconds>);
            snn_require(u.value() == 1);
        }
        {
            decltype(auto) u = time::seconds{400} % time::minutes{2};
            static_assert(std::is_same_v<decltype(u), time::seconds>); // Smallest unit.
            snn_require(u.value() == 40);
        }
        {
            decltype(auto) u = time::minutes{2} % time::seconds{50};
            static_assert(std::is_same_v<decltype(u), time::seconds>); // Smallest unit.
            snn_require(u.value() == 20);
        }

        // Comparison

        static_assert(!time::nanoseconds::is_smaller_than<time::nanoseconds>());
        static_assert(time::nanoseconds::is_smaller_than<time::microseconds>());
        static_assert(time::nanoseconds::is_smaller_than<time::milliseconds>());
        static_assert(time::nanoseconds::is_smaller_than<time::seconds>());
        static_assert(time::nanoseconds::is_smaller_than<time::minutes>());
        static_assert(time::nanoseconds::is_smaller_than<time::hours>());
        static_assert(time::nanoseconds::is_smaller_than<time::days>());

        static_assert(!time::microseconds::is_smaller_than<time::nanoseconds>());
        static_assert(!time::microseconds::is_smaller_than<time::microseconds>());
        static_assert(time::microseconds::is_smaller_than<time::milliseconds>());
        static_assert(time::microseconds::is_smaller_than<time::seconds>());
        static_assert(time::microseconds::is_smaller_than<time::minutes>());
        static_assert(time::microseconds::is_smaller_than<time::hours>());
        static_assert(time::microseconds::is_smaller_than<time::days>());

        static_assert(!time::milliseconds::is_smaller_than<time::nanoseconds>());
        static_assert(!time::milliseconds::is_smaller_than<time::microseconds>());
        static_assert(!time::milliseconds::is_smaller_than<time::milliseconds>());
        static_assert(time::milliseconds::is_smaller_than<time::seconds>());
        static_assert(time::milliseconds::is_smaller_than<time::minutes>());
        static_assert(time::milliseconds::is_smaller_than<time::hours>());
        static_assert(time::milliseconds::is_smaller_than<time::days>());

        static_assert(!time::seconds::is_smaller_than<time::nanoseconds>());
        static_assert(!time::seconds::is_smaller_than<time::microseconds>());
        static_assert(!time::seconds::is_smaller_than<time::milliseconds>());
        static_assert(!time::seconds::is_smaller_than<time::seconds>());
        static_assert(time::seconds::is_smaller_than<time::minutes>());
        static_assert(time::seconds::is_smaller_than<time::hours>());
        static_assert(time::seconds::is_smaller_than<time::days>());

        static_assert(!time::minutes::is_smaller_than<time::nanoseconds>());
        static_assert(!time::minutes::is_smaller_than<time::microseconds>());
        static_assert(!time::minutes::is_smaller_than<time::milliseconds>());
        static_assert(!time::minutes::is_smaller_than<time::seconds>());
        static_assert(!time::minutes::is_smaller_than<time::minutes>());
        static_assert(time::minutes::is_smaller_than<time::hours>());
        static_assert(time::minutes::is_smaller_than<time::days>());

        static_assert(!time::hours::is_smaller_than<time::nanoseconds>());
        static_assert(!time::hours::is_smaller_than<time::microseconds>());
        static_assert(!time::hours::is_smaller_than<time::milliseconds>());
        static_assert(!time::hours::is_smaller_than<time::seconds>());
        static_assert(!time::hours::is_smaller_than<time::minutes>());
        static_assert(!time::hours::is_smaller_than<time::hours>());
        static_assert(time::hours::is_smaller_than<time::days>());

        static_assert(!time::days::is_smaller_than<time::nanoseconds>());
        static_assert(!time::days::is_smaller_than<time::microseconds>());
        static_assert(!time::days::is_smaller_than<time::milliseconds>());
        static_assert(!time::days::is_smaller_than<time::seconds>());
        static_assert(!time::days::is_smaller_than<time::minutes>());
        static_assert(!time::days::is_smaller_than<time::hours>());
        static_assert(!time::days::is_smaller_than<time::days>());

        {
            static_assert(time::seconds{120} == time::seconds{120});
            static_assert(!(time::seconds{120} == time::seconds{121}));
            static_assert(time::seconds{119} < time::seconds{120});
            static_assert(time::seconds{120} > time::seconds{119});
            static_assert(!(time::seconds{120} < time::seconds{120}));
            static_assert(!(time::seconds{120} > time::seconds{120}));
            static_assert(time::seconds{120} <= time::seconds{121});
            static_assert(time::seconds{120} <= time::seconds{120});
            static_assert(time::seconds{120} >= time::seconds{120});
            static_assert(time::seconds{120} >= time::seconds{119});
            static_assert(!(time::seconds{121} <= time::seconds{120}));
            static_assert(!(time::seconds{120} >= time::seconds{121}));

            static_assert(time::seconds{120} == 120);
            static_assert(time::seconds{120} >= 120);
            static_assert(time::seconds{120} <= 120);
            static_assert(time::seconds{120} > 119);
            static_assert(time::seconds{120} < 121);
            static_assert(!(time::seconds{120} == 121));
            static_assert(!(time::seconds{120} >= 121));
            static_assert(!(time::seconds{120} <= 119));
            static_assert(!(time::seconds{120} > 120));
            static_assert(!(time::seconds{120} < 120));

            static_assert(time::minutes{2} == time::seconds{120});
            static_assert(!(time::minutes{2} == time::seconds{121}));
            static_assert(time::seconds{120} == time::minutes{2});
            static_assert(!(time::seconds{121} == time::minutes{2}));

            static_assert(time::minutes{2} <= time::seconds{120});
            static_assert(time::minutes{2} >= time::seconds{120});
            static_assert(time::minutes{2} < time::seconds{121});
            static_assert(time::minutes{2} > time::seconds{119});

            static_assert(time::seconds{120} <= time::minutes{2});
            static_assert(time::seconds{120} >= time::minutes{2});
            static_assert(time::seconds{119} < time::minutes{2});
            static_assert(time::seconds{121} > time::minutes{2});

            static_assert(!(time::seconds{121} <= time::minutes{2}));
            static_assert(!(time::seconds{119} >= time::minutes{2}));
            static_assert(!(time::seconds{120} < time::minutes{2}));
            static_assert(!(time::seconds{120} > time::minutes{2}));

            constexpr auto min = constant::limit<i64>::min;

            constexpr time::nanoseconds overflowed{time::days{106'752}};
            static_assert(!overflowed);
            static_assert(!overflowed.has_value());
            static_assert(overflowed != 0);
            static_assert(overflowed != min);
            static_assert(overflowed == overflowed);
            static_assert(!(overflowed >= min));
            static_assert(overflowed <= min);
            static_assert(!(overflowed > min));
            static_assert(overflowed < min);

            static_assert(overflowed < time::nanoseconds{min});
            static_assert(overflowed <= time::nanoseconds{min});
            static_assert(overflowed != time::nanoseconds{min});
            static_assert(overflowed < time::minutes{min});
            static_assert(overflowed <= time::minutes{min});
            static_assert(overflowed != time::minutes{min});
            static_assert(!(overflowed > time::minutes{min}));
            static_assert(!(overflowed >= time::minutes{min}));
            static_assert(time::nanoseconds{min} > overflowed);
            static_assert(time::nanoseconds{min} >= overflowed);
            static_assert(!(time::nanoseconds{min} == overflowed));

            // Correct results when conversions would overflow.
            static_assert(time::nanoseconds{1} < time::days{106'752});
            static_assert(time::days{106'752} > time::nanoseconds{1});
        }
    }
}
