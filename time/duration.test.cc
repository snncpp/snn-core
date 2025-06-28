// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/duration.hh"

#include "snn-core/unittest.hh"
#include "snn-core/math/fp/common.hh"
#include "snn-core/time/unit.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            time::duration d{8, 1'999'999'999};

            snn_require(d);                                   // Not zero.
            snn_require(d.seconds_part() == 9);               // Normalized
            snn_require(d.nanoseconds_part() == 999'999'999); // Normalized

            d += time::duration{3, 6};
            snn_require(d.seconds_part() == 13);
            snn_require(d.nanoseconds_part() == 5);

            snn_require(d.to<time::microseconds>() == 13'000'000);
            snn_require(d.to<time::nanoseconds>() == 13'000'000'005);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        {
            constexpr time::duration d;
            static_assert(!d);
            static_assert(d.seconds_part() == 0);
            static_assert(d.nanoseconds_part() == 0);
        }
        {
            constexpr time::duration d{8};
            static_assert(d);
            static_assert(d.seconds_part() == 8);
            static_assert(d.nanoseconds_part() == 0);
        }
        {
            constexpr time::duration d{8, 0};
            static_assert(d);
            static_assert(d.seconds_part() == 8);
            static_assert(d.nanoseconds_part() == 0);
        }
        {
            constexpr time::duration d{0, 999'999'999};
            static_assert(d);
            static_assert(d.seconds_part() == 0);
            static_assert(d.nanoseconds_part() == 999'999'999);
        }
        {
            constexpr time::duration d{8, 999'999'999};
            static_assert(d);
            static_assert(d.seconds_part() == 8);
            static_assert(d.nanoseconds_part() == 999'999'999);
        }
        {
            constexpr time::duration d{8, 999'999'999, assume::is_valid};
            static_assert(d);
            static_assert(d.seconds_part() == 8);
            static_assert(d.nanoseconds_part() == 999'999'999);
        }
        {
            constexpr time::duration d{8, 2'000'000'000};
            static_assert(d);
            static_assert(d.seconds_part() == 10);
            static_assert(d.nanoseconds_part() == 0);
        }
        {
            constexpr time::duration d{8, 1'999'999'999};
            static_assert(d);
            static_assert(d.seconds_part() == 9);
            static_assert(d.nanoseconds_part() == 999'999'999);
        }
        {
            constexpr time::duration d{8, -2'000'000'000};
            static_assert(d);
            static_assert(d.seconds_part() == 6);
            static_assert(d.nanoseconds_part() == 0);
        }
        {
            constexpr time::duration d{8, -1'999'999'999};
            static_assert(d);
            static_assert(d.seconds_part() == 6);
            static_assert(d.nanoseconds_part() == 1);
        }

        // Addition

        static_assert(time::duration{8, 3}.add(time::duration{8, 3}) == time::duration{16, 6});

        {
            time::duration d{1, 999'999'999};
            snn_require(d.seconds_part() == 1);
            snn_require(d.nanoseconds_part() == 999'999'999);

            d += time::duration{2};
            snn_require(d.seconds_part() == 3);
            snn_require(d.nanoseconds_part() == 999'999'999);

            d += time::duration{3, 1};
            snn_require(d.seconds_part() == 7);
            snn_require(d.nanoseconds_part() == 0);
        }
        {
            time::duration d = time::duration{7, 999'999'999} + time::duration{-5, 999'999'999};
            snn_require(d.seconds_part() == 3);
            snn_require(d.nanoseconds_part() == 999'999'998);
        }
        {
            constexpr time::duration d =
                time::duration{-4, 300'000'000} + time::duration{-6, 800'000'000};
            static_assert(d.seconds_part() == -9);
            static_assert(d.nanoseconds_part() == 100'000'000);
        }

        // Subtraction

        static_assert(time::duration{8, 3}.subtract(time::duration{3, 2}) == time::duration{5, 1});

        {
            time::duration d{1, 8};
            snn_require(d.seconds_part() == 1);
            snn_require(d.nanoseconds_part() == 8);

            d -= time::duration{2};
            snn_require(d.seconds_part() == -1);
            snn_require(d.nanoseconds_part() == 8);

            d -= time::duration{3, 9};
            snn_require(d.seconds_part() == -5);
            snn_require(d.nanoseconds_part() == 999'999'999);

            d -= time::duration{0, 999'999'999};
            snn_require(d.seconds_part() == -5);
            snn_require(d.nanoseconds_part() == 0);
        }
        {
            time::duration d = time::duration{1, 999'999'999} - time::duration{-3, 999'999'999};
            snn_require(d.seconds_part() == 4);
            snn_require(d.nanoseconds_part() == 0);
        }
        {
            constexpr time::duration d =
                time::duration{-4, 300'000'000} - time::duration{-6, 800'000'000};
            static_assert(d.seconds_part() == 1);
            static_assert(d.nanoseconds_part() == 500'000'000);
        }

        // Conversion

        static_assert(time::duration{0, 500'000'000}.to<time::nanoseconds>() == 500'000'000);

        static_assert(time::duration{1, 500'000'000}.to<time::nanoseconds>() == 1'500'000'000);
        static_assert(time::duration{-1, 500'000'000}.to<time::nanoseconds>() == -500'000'000);

        static_assert(time::duration{2}.to<time::nanoseconds>() == 2'000'000'000);
        static_assert(time::duration{-2}.to<time::nanoseconds>() == -2'000'000'000);
        static_assert(time::duration{2, 1}.to<time::nanoseconds>() == 2'000'000'001);
        static_assert(time::duration{-2, 1}.to<time::nanoseconds>() == -1'999'999'999);
        static_assert(time::duration{2, 999'999'999}.to<time::nanoseconds>() == 2'999'999'999);
        static_assert(time::duration{-2, 999'999'999}.to<time::nanoseconds>() == -1'000'000'001);

        static_assert(time::duration{2}.to<time::microseconds>() == 2'000'000);
        static_assert(time::duration{-2}.to<time::microseconds>() == -2'000'000);
        static_assert(time::duration{2, 1}.to<time::microseconds>() == 2'000'000);
        static_assert(time::duration{-2, 1}.to<time::microseconds>() == -1'999'999);
        static_assert(time::duration{2, 999'999'999}.to<time::microseconds>() == 2'999'999);
        static_assert(time::duration{-2, 999'999'999}.to<time::microseconds>() == -1'000'000);

        static_assert(time::duration{2}.to<time::milliseconds>() == 2'000);
        static_assert(time::duration{-2}.to<time::milliseconds>() == -2'000);
        static_assert(time::duration{2, 1}.to<time::milliseconds>() == 2'000);
        static_assert(time::duration{-2, 1}.to<time::milliseconds>() == -1'999);
        static_assert(time::duration{2, 999'999'999}.to<time::milliseconds>() == 2'999);
        static_assert(time::duration{-2, 999'999'999}.to<time::milliseconds>() == -1'000);

        static_assert(time::duration{2}.to<time::seconds>() == 2);
        static_assert(time::duration{-2}.to<time::seconds>() == -2);
        static_assert(time::duration{2, 1}.to<time::seconds>() == 2);
        static_assert(time::duration{-2, 1}.to<time::seconds>() == -1);
        static_assert(time::duration{2, 999'999'999}.to<time::seconds>() == 2);
        static_assert(time::duration{-2, 999'999'999}.to<time::seconds>() == -1);

        static_assert(time::duration{3'678}.to<time::seconds>() == 3'678);
        static_assert(time::duration{3'678}.to<time::minutes>() == 61);
        static_assert(time::duration{3'678}.to<time::hours>() == 1);
        static_assert(time::duration{3'678, 1}.to<time::seconds>() == 3'678);
        static_assert(time::duration{3'678, 1}.to<time::minutes>() == 61);
        static_assert(time::duration{3'678, 1}.to<time::hours>() == 1);
        static_assert(time::duration{3'678, 999'999'999}.to<time::seconds>() == 3'678);
        static_assert(time::duration{3'678, 999'999'999}.to<time::minutes>() == 61);
        static_assert(time::duration{3'678, 999'999'999}.to<time::hours>() == 1);

        static_assert(time::duration{-3'678}.to<time::seconds>() == -3'678);
        static_assert(time::duration{-3'678}.to<time::minutes>() == -61);
        static_assert(time::duration{-3'678}.to<time::hours>() == -1);
        static_assert(time::duration{-3'678, 1}.to<time::seconds>() == -3'677);
        static_assert(time::duration{-3'678, 1}.to<time::minutes>() == -61);
        static_assert(time::duration{-3'678, 1}.to<time::hours>() == -1);
        static_assert(time::duration{-3'678, 999'999'999}.to<time::seconds>() == -3'677);
        static_assert(time::duration{-3'678, 999'999'999}.to<time::minutes>() == -61);
        static_assert(time::duration{-3'678, 999'999'999}.to<time::hours>() == -1);

        static_assert(time::duration{129'600}.to<time::seconds>() == 129'600);
        static_assert(time::duration{129'600}.to<time::minutes>() == 2'160);
        static_assert(time::duration{129'600}.to<time::hours>() == 36);
        static_assert(time::duration{129'600}.to<time::days>() == 1);
        static_assert(time::duration{129'600, 1}.to<time::seconds>() == 129'600);
        static_assert(time::duration{129'600, 1}.to<time::minutes>() == 2'160);
        static_assert(time::duration{129'600, 1}.to<time::hours>() == 36);
        static_assert(time::duration{129'600, 1}.to<time::days>() == 1);
        static_assert(time::duration{129'600, 999'999'999}.to<time::seconds>() == 129'600);
        static_assert(time::duration{129'600, 999'999'999}.to<time::minutes>() == 2'160);
        static_assert(time::duration{129'600, 999'999'999}.to<time::hours>() == 36);
        static_assert(time::duration{129'600, 999'999'999}.to<time::days>() == 1);

        static_assert(time::duration{-129'600}.to<time::seconds>() == -129'600);
        static_assert(time::duration{-129'600}.to<time::minutes>() == -2'160);
        static_assert(time::duration{-129'600}.to<time::hours>() == -36);
        static_assert(time::duration{-129'600}.to<time::days>() == -1);
        static_assert(time::duration{-129'600, 1}.to<time::seconds>() == -129'599);
        static_assert(time::duration{-129'600, 1}.to<time::minutes>() == -2'159);
        static_assert(time::duration{-129'600, 1}.to<time::hours>() == -35);
        static_assert(time::duration{-129'600, 1}.to<time::days>() == -1);
        static_assert(time::duration{-129'600, 999'999'999}.to<time::seconds>() == -129'599);
        static_assert(time::duration{-129'600, 999'999'999}.to<time::minutes>() == -2'159);
        static_assert(time::duration{-129'600, 999'999'999}.to<time::hours>() == -35);
        static_assert(time::duration{-129'600, 999'999'999}.to<time::days>() == -1);

        static_assert(time::duration{0, 500'000'000}.to_nanoseconds<i64>() == 500'000'000);

        static_assert(time::duration{1, 500'000'000}.to_nanoseconds<i64>() == 1'500'000'000);
        static_assert(time::duration{-1, 500'000'000}.to_nanoseconds<i64>() == -500'000'000);

        static_assert(time::duration{2}.to_nanoseconds<i64>() == 2'000'000'000);
        static_assert(time::duration{-2}.to_nanoseconds<i64>() == -2'000'000'000);
        static_assert(time::duration{2, 1}.to_nanoseconds<i64>() == 2'000'000'001);
        static_assert(time::duration{-2, 1}.to_nanoseconds<i64>() == -1'999'999'999);
        static_assert(time::duration{2, 999'999'999}.to_nanoseconds<i64>() == 2'999'999'999);
        static_assert(time::duration{-2, 999'999'999}.to_nanoseconds<i64>() == -1'000'000'001);

        static_assert(time::duration{2}.to_microseconds<i64>() == 2'000'000);
        static_assert(time::duration{-2}.to_microseconds<i64>() == -2'000'000);
        static_assert(time::duration{2, 1}.to_microseconds<i64>() == 2'000'000);
        static_assert(time::duration{-2, 1}.to_microseconds<i64>() == -1'999'999);
        static_assert(time::duration{2, 999'999'999}.to_microseconds<i64>() == 2'999'999);
        static_assert(time::duration{-2, 999'999'999}.to_microseconds<i64>() == -1'000'000);

        static_assert(time::duration{2}.to_milliseconds<i64>() == 2'000);
        static_assert(time::duration{-2}.to_milliseconds<i64>() == -2'000);
        static_assert(time::duration{2, 1}.to_milliseconds<i64>() == 2'000);
        static_assert(time::duration{-2, 1}.to_milliseconds<i64>() == -1'999);
        static_assert(time::duration{2, 999'999'999}.to_milliseconds<i64>() == 2'999);
        static_assert(time::duration{-2, 999'999'999}.to_milliseconds<i64>() == -1'000);

        static_assert(time::duration{2}.to_seconds<i64>() == 2);
        static_assert(time::duration{-2}.to_seconds<i64>() == -2);
        static_assert(time::duration{2, 1}.to_seconds<i64>() == 2);
        static_assert(time::duration{-2, 1}.to_seconds<i64>() == -1);
        static_assert(time::duration{2, 999'999'999}.to_seconds<i64>() == 2);
        static_assert(time::duration{-2, 999'999'999}.to_seconds<i64>() == -1);

        static_assert(time::duration{2, 999'999'999}.to_nanoseconds<i64>(assume::not_negative) ==
                      2'999'999'999);
        static_assert(time::duration{2, 999'999'999}.to_microseconds<i64>(assume::not_negative) ==
                      2'999'999);
        static_assert(time::duration{2, 999'999'999}.to_milliseconds<i64>(assume::not_negative) ==
                      2'999);
        static_assert(time::duration{2, 999'999'999}.to_seconds<i64>(assume::not_negative) == 2);

        static_assert(time::duration{3'678}.to_seconds<i64>() == 3'678);
        static_assert(time::duration{3'678}.to_minutes<i64>() == 61);
        static_assert(time::duration{3'678}.to_hours<i64>() == 1);
        static_assert(time::duration{3'678, 1}.to_seconds<i64>() == 3'678);
        static_assert(time::duration{3'678, 1}.to_minutes<i64>() == 61);
        static_assert(time::duration{3'678, 1}.to_hours<i64>() == 1);
        static_assert(time::duration{3'678, 999'999'999}.to_seconds<i64>() == 3'678);
        static_assert(time::duration{3'678, 999'999'999}.to_minutes<i64>() == 61);
        static_assert(time::duration{3'678, 999'999'999}.to_hours<i64>() == 1);

        static_assert(time::duration{-3'678}.to_seconds<i64>() == -3'678);
        static_assert(time::duration{-3'678}.to_minutes<i64>() == -61);
        static_assert(time::duration{-3'678}.to_hours<i64>() == -1);
        static_assert(time::duration{-3'678, 1}.to_seconds<i64>() == -3'677);
        static_assert(time::duration{-3'678, 1}.to_minutes<i64>() == -61);
        static_assert(time::duration{-3'678, 1}.to_hours<i64>() == -1);
        static_assert(time::duration{-3'678, 999'999'999}.to_seconds<i64>() == -3'677);
        static_assert(time::duration{-3'678, 999'999'999}.to_minutes<i64>() == -61);
        static_assert(time::duration{-3'678, 999'999'999}.to_hours<i64>() == -1);

        static_assert(time::duration{129'600}.to_seconds<i64>() == 129'600);
        static_assert(time::duration{129'600}.to_minutes<i64>() == 2'160);
        static_assert(time::duration{129'600}.to_hours<i64>() == 36);
        static_assert(time::duration{129'600}.to_days<i64>() == 1);
        static_assert(time::duration{129'600, 1}.to_seconds<i64>() == 129'600);
        static_assert(time::duration{129'600, 1}.to_minutes<i64>() == 2'160);
        static_assert(time::duration{129'600, 1}.to_hours<i64>() == 36);
        static_assert(time::duration{129'600, 1}.to_days<i64>() == 1);
        static_assert(time::duration{129'600, 999'999'999}.to_seconds<i64>() == 129'600);
        static_assert(time::duration{129'600, 999'999'999}.to_minutes<i64>() == 2'160);
        static_assert(time::duration{129'600, 999'999'999}.to_hours<i64>() == 36);
        static_assert(time::duration{129'600, 999'999'999}.to_days<i64>() == 1);

        static_assert(time::duration{129'600}.to_seconds<i64>(assume::not_negative) == 129'600);
        static_assert(time::duration{129'600}.to_minutes<i64>(assume::not_negative) == 2'160);
        static_assert(time::duration{129'600}.to_hours<i64>(assume::not_negative) == 36);
        static_assert(time::duration{129'600}.to_days<i64>(assume::not_negative) == 1);

        static_assert(time::duration{-129'600}.to_seconds<i64>() == -129'600);
        static_assert(time::duration{-129'600}.to_minutes<i64>() == -2'160);
        static_assert(time::duration{-129'600}.to_hours<i64>() == -36);
        static_assert(time::duration{-129'600}.to_days<i64>() == -1);
        static_assert(time::duration{-129'600, 1}.to_seconds<i64>() == -129'599);
        static_assert(time::duration{-129'600, 1}.to_minutes<i64>() == -2'159);
        static_assert(time::duration{-129'600, 1}.to_hours<i64>() == -35);
        static_assert(time::duration{-129'600, 1}.to_days<i64>() == -1);
        static_assert(time::duration{-129'600, 999'999'999}.to_seconds<i64>() == -129'599);
        static_assert(time::duration{-129'600, 999'999'999}.to_minutes<i64>() == -2'159);
        static_assert(time::duration{-129'600, 999'999'999}.to_hours<i64>() == -35);
        static_assert(time::duration{-129'600, 999'999'999}.to_days<i64>() == -1);

#if SNN_INT128_ENABLED
        static_assert(
            time::duration{constant::limit<i64>::max, 999'999'999}.to_nanoseconds<i128>() ==
            cstrview{"9223372036854775807999999999"}.to<i128>().value());
        static_assert(
            time::duration{constant::limit<i64>::min, 999'999'999}.to_nanoseconds<i128>() ==
            cstrview{"-9223372036854775807000000001"}.to<i128>().value());

        static_assert(
            time::duration{constant::limit<i64>::max, 999'999'999}.to_microseconds<i128>() ==
            cstrview{"9223372036854775807999999"}.to<i128>().value());
        static_assert(
            time::duration{constant::limit<i64>::min, 999'999'999}.to_microseconds<i128>() ==
            cstrview{"-9223372036854775807000000"}.to<i128>().value());

        static_assert(
            time::duration{constant::limit<i64>::max, 999'999'999}.to_milliseconds<i128>() ==
            cstrview{"9223372036854775807999"}.to<i128>().value());
        static_assert(
            time::duration{constant::limit<i64>::min, 999'999'999}.to_milliseconds<i128>() ==
            cstrview{"-9223372036854775807000"}.to<i128>().value());

        static_assert(time::duration{constant::limit<i64>::max, 999'999'999}.to_seconds<i128>() ==
                      i128{constant::limit<i64>::max});
        static_assert(time::duration{constant::limit<i64>::min, 999'999'999}.to_seconds<i128>() ==
                      i128{constant::limit<i64>::min + 1});
#endif

        namespace fp = math::fp;

        static_assert(fp::is_almost_equal(time::duration{0, 500'000'000}.to_nanoseconds<double>(),
                                          0.5e9, 1e-10));

        static_assert(fp::is_almost_equal(time::duration{1, 500'000'000}.to_nanoseconds<double>(),
                                          1.5e9, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-1, 500'000'000}.to_nanoseconds<double>(),
                                          -0.5e9, 1e-10));

        static_assert(fp::is_almost_equal(time::duration{2}.to_nanoseconds<double>(), 2e9, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-2}.to_nanoseconds<double>(), -2e9,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{2, 1}.to_nanoseconds<double>(),
                                          2'000'000'001.0, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-2, 1}.to_nanoseconds<double>(),
                                          -1'999'999'999.0, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{2, 999'999'999}.to_nanoseconds<double>(),
                                          2'999'999'999.0, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-2, 999'999'999}.to_nanoseconds<double>(),
                                          -1'000'000'001.0, 1e-10));

        static_assert(fp::is_almost_equal(time::duration{2}.to_microseconds<double>(), 2e6, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-2}.to_microseconds<double>(), -2e6,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{2, 1}.to_microseconds<double>(),
                                          2'000'000.001, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-2, 1}.to_microseconds<double>(),
                                          -1'999'999.999, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{2, 999'999'999}.to_microseconds<double>(),
                                          2'999'999.999, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-2, 999'999'999}.to_microseconds<double>(),
                                          -1'000'000.001, 1e-10));

        static_assert(fp::is_almost_equal(time::duration{2}.to_milliseconds<double>(), 2'000.0,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{-2}.to_milliseconds<double>(), -2'000.0,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{2, 1}.to_milliseconds<double>(),
                                          2'000.000001, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-2, 1}.to_milliseconds<double>(),
                                          -1'999.999999, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{2, 999'999'999}.to_milliseconds<double>(),
                                          2'999.999999, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-2, 999'999'999}.to_milliseconds<double>(),
                                          -1'000.000001, 1e-10));

        static_assert(fp::is_almost_equal(time::duration{2}.to_seconds<double>(), 2.0, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-2}.to_seconds<double>(), -2.0, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{2, 1}.to_seconds<double>(), 2.000000001,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{-2, 1}.to_seconds<double>(), -1.999999999,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{2, 999'999'999}.to_seconds<double>(),
                                          2.999999999, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-2, 999'999'999}.to_seconds<double>(),
                                          -1.000000001, 1e-10));

        static_assert(fp::is_almost_equal(time::duration{3'678}.to_seconds<double>(), 3'678.0,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{3'678}.to_minutes<double>(), 61.3, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{3'678}.to_hours<double>(), 1.021666666,
                                          1e-9));
        static_assert(fp::is_almost_equal(time::duration{3'678, 1}.to_seconds<double>(),
                                          3'678.000000001, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{3'678, 1}.to_minutes<double>(), 61.3,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{3'678, 1}.to_hours<double>(), 1.021666666,
                                          1e-9));
        static_assert(fp::is_almost_equal(time::duration{3'678, 999'999'999}.to_seconds<double>(),
                                          3'678.999999999, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{3'678, 999'999'999}.to_minutes<double>(),
                                          61.316666666, 1e-9));
        static_assert(fp::is_almost_equal(time::duration{3'678, 999'999'999}.to_hours<double>(),
                                          1.021944444, 1e-9));

        static_assert(fp::is_almost_equal(time::duration{-3'678}.to_seconds<double>(), -3'678.0,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{-3'678}.to_minutes<double>(), -61.3,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{-3'678}.to_hours<double>(), -1.021666666,
                                          1e-9));
        static_assert(fp::is_almost_equal(time::duration{-3'678, 1}.to_seconds<double>(),
                                          -3'677.999999999, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-3'678, 1}.to_minutes<double>(),
                                          -61.299999999, 1e-9));
        static_assert(fp::is_almost_equal(time::duration{-3'678, 1}.to_hours<double>(),
                                          -1.021666666, 1e-9));
        static_assert(fp::is_almost_equal(time::duration{-3'678, 999'999'999}.to_seconds<double>(),
                                          -3'677.000000001, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-3'678, 999'999'999}.to_minutes<double>(),
                                          -61.283333333, 1e-9));
        static_assert(fp::is_almost_equal(time::duration{-3'678, 999'999'999}.to_hours<double>(),
                                          -1.021388888, 1e-9));

        static_assert(fp::is_almost_equal(time::duration{129'600}.to_seconds<double>(), 129'600.0,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{129'600}.to_minutes<double>(), 2'160.0,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{129'600}.to_hours<double>(), 36.0, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{129'600}.to_days<double>(), 1.5, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{129'600, 1}.to_seconds<double>(),
                                          129'600.000000001, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{129'600, 1}.to_minutes<double>(), 2'160.0,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{129'600, 1}.to_hours<double>(), 36.0,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{129'600, 1}.to_days<double>(), 1.5,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{129'600, 999'999'999}.to_seconds<double>(),
                                          129'600.999999999, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{129'600, 999'999'999}.to_minutes<double>(),
                                          2'160.016666666, 1e-9));
        static_assert(fp::is_almost_equal(time::duration{129'600, 999'999'999}.to_hours<double>(),
                                          36.000277777, 1e-9));
        static_assert(fp::is_almost_equal(time::duration{129'600, 999'999'999}.to_days<double>(),
                                          1.5000115741, 1e-9));

        static_assert(fp::is_almost_equal(time::duration{-129'600}.to_seconds<double>(), -129'600.0,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{-129'600}.to_minutes<double>(), -2'160.0,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{-129'600}.to_hours<double>(), -36.0,
                                          1e-10));
        static_assert(fp::is_almost_equal(time::duration{-129'600}.to_days<double>(), -1.5, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-129'600, 1}.to_seconds<double>(),
                                          -129'599.999999999, 1e-10));
        static_assert(fp::is_almost_equal(time::duration{-129'600, 1}.to_minutes<double>(),
                                          -2'159.999999999, 1e-9));
        static_assert(fp::is_almost_equal(time::duration{-129'600, 1}.to_hours<double>(),
                                          -35.999999999, 1e-9));
        static_assert(fp::is_almost_equal(time::duration{-129'600, 1}.to_days<double>(),
                                          -1.499999999, 1e-9));
        static_assert(fp::is_almost_equal(
            time::duration{-129'600, 999'999'999}.to_seconds<double>(), -129'599.000000001, 1e-10));
        static_assert(fp::is_almost_equal(
            time::duration{-129'600, 999'999'999}.to_minutes<double>(), -2'159.983333333, 1e-9));
        static_assert(fp::is_almost_equal(time::duration{-129'600, 999'999'999}.to_hours<double>(),
                                          -35.999722222, 1e-9));
        static_assert(fp::is_almost_equal(time::duration{-129'600, 999'999'999}.to_days<double>(),
                                          -1.499988426, 1e-9));

        // Comparison

        static_assert(time::duration{8, 3} == time::duration{8, 3});
        static_assert(!(time::duration{8, 3} == time::duration{7, 3}));
        static_assert(!(time::duration{8, 3} == time::duration{8, 4}));

        static_assert(time::duration{2, 99} < time::duration{3, 99});
        static_assert(!(time::duration{3, 99} < time::duration{3, 99}));
        static_assert(!(time::duration{4, 99} < time::duration{3, 99}));
        static_assert(time::duration{3, 98} < time::duration{3, 99});
        static_assert(!(time::duration{3, 99} < time::duration{3, 99}));
        static_assert(!(time::duration{3, 99} < time::duration{3, 98}));

        // -1'999'999'999 ns < -1'000'000'001 ns.
        static_assert(time::duration{-2, 1} < time::duration{-2, 999'999'999});
    }
}
