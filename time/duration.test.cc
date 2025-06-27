// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/duration.hh"

#include "snn-core/time/unit.hh"
#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            time::duration d{8, 1'999'999'999};

            snn_require(d);                              // Not zero.
            snn_require(d.seconds() == 9);               // Normalized
            snn_require(d.nanoseconds() == 999'999'999); // Normalized

            d += time::duration{3, 6};
            snn_require(d.seconds() == 13);
            snn_require(d.nanoseconds() == 5);

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
            static_assert(d.seconds() == 0);
            static_assert(d.nanoseconds() == 0);
        }
        {
            constexpr time::duration d{8};
            static_assert(d);
            static_assert(d.seconds() == 8);
            static_assert(d.nanoseconds() == 0);
        }
        {
            constexpr time::duration d{8, 0};
            static_assert(d);
            static_assert(d.seconds() == 8);
            static_assert(d.nanoseconds() == 0);
        }
        {
            constexpr time::duration d{0, 999'999'999};
            static_assert(d);
            static_assert(d.seconds() == 0);
            static_assert(d.nanoseconds() == 999'999'999);
        }
        {
            constexpr time::duration d{8, 999'999'999};
            static_assert(d);
            static_assert(d.seconds() == 8);
            static_assert(d.nanoseconds() == 999'999'999);
        }
        {
            constexpr time::duration d{8, 999'999'999, assume::is_valid};
            static_assert(d);
            static_assert(d.seconds() == 8);
            static_assert(d.nanoseconds() == 999'999'999);
        }
        {
            constexpr time::duration d{8, 2'000'000'000};
            static_assert(d);
            static_assert(d.seconds() == 10);
            static_assert(d.nanoseconds() == 0);
        }
        {
            constexpr time::duration d{8, 1'999'999'999};
            static_assert(d);
            static_assert(d.seconds() == 9);
            static_assert(d.nanoseconds() == 999'999'999);
        }
        {
            constexpr time::duration d{8, -2'000'000'000};
            static_assert(d);
            static_assert(d.seconds() == 6);
            static_assert(d.nanoseconds() == 0);
        }
        {
            constexpr time::duration d{8, -1'999'999'999};
            static_assert(d);
            static_assert(d.seconds() == 6);
            static_assert(d.nanoseconds() == 1);
        }

        // Addition

        static_assert(time::duration{8, 3}.add(time::duration{8, 3}) == time::duration{16, 6});

        {
            time::duration d{1, 999'999'999};
            snn_require(d.seconds() == 1);
            snn_require(d.nanoseconds() == 999'999'999);

            d += time::duration{2};
            snn_require(d.seconds() == 3);
            snn_require(d.nanoseconds() == 999'999'999);

            d += time::duration{3, 1};
            snn_require(d.seconds() == 7);
            snn_require(d.nanoseconds() == 0);
        }
        {
            time::duration d = time::duration{7, 999'999'999} + time::duration{-5, 999'999'999};
            snn_require(d.seconds() == 3);
            snn_require(d.nanoseconds() == 999'999'998);
        }
        {
            constexpr time::duration d =
                time::duration{-4, 300'000'000} + time::duration{-6, 800'000'000};
            static_assert(d.seconds() == -9);
            static_assert(d.nanoseconds() == 100'000'000);
        }

        // Subtraction

        static_assert(time::duration{8, 3}.subtract(time::duration{3, 2}) == time::duration{5, 1});

        {
            time::duration d{1, 8};
            snn_require(d.seconds() == 1);
            snn_require(d.nanoseconds() == 8);

            d -= time::duration{2};
            snn_require(d.seconds() == -1);
            snn_require(d.nanoseconds() == 8);

            d -= time::duration{3, 9};
            snn_require(d.seconds() == -5);
            snn_require(d.nanoseconds() == 999'999'999);

            d -= time::duration{0, 999'999'999};
            snn_require(d.seconds() == -5);
            snn_require(d.nanoseconds() == 0);
        }
        {
            time::duration d = time::duration{1, 999'999'999} - time::duration{-3, 999'999'999};
            snn_require(d.seconds() == 4);
            snn_require(d.nanoseconds() == 0);
        }
        {
            constexpr time::duration d =
                time::duration{-4, 300'000'000} - time::duration{-6, 800'000'000};
            static_assert(d.seconds() == 1);
            static_assert(d.nanoseconds() == 500'000'000);
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

        static_assert(time::duration{3678}.to<time::seconds>() == 3678);
        static_assert(time::duration{3678}.to<time::minutes>() == 61);
        static_assert(time::duration{3678}.to<time::hours>() == 1);
        static_assert(time::duration{3678, 1}.to<time::seconds>() == 3678);
        static_assert(time::duration{3678, 1}.to<time::minutes>() == 61);
        static_assert(time::duration{3678, 1}.to<time::hours>() == 1);
        static_assert(time::duration{3678, 999'999'999}.to<time::seconds>() == 3678);
        static_assert(time::duration{3678, 999'999'999}.to<time::minutes>() == 61);
        static_assert(time::duration{3678, 999'999'999}.to<time::hours>() == 1);

        static_assert(time::duration{-3678}.to<time::seconds>() == -3678);
        static_assert(time::duration{-3678}.to<time::minutes>() == -61);
        static_assert(time::duration{-3678}.to<time::hours>() == -1);
        static_assert(time::duration{-3678, 1}.to<time::seconds>() == -3677);
        static_assert(time::duration{-3678, 1}.to<time::minutes>() == -61);
        static_assert(time::duration{-3678, 1}.to<time::hours>() == -1);
        static_assert(time::duration{-3678, 999'999'999}.to<time::seconds>() == -3677);
        static_assert(time::duration{-3678, 999'999'999}.to<time::minutes>() == -61);
        static_assert(time::duration{-3678, 999'999'999}.to<time::hours>() == -1);

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
