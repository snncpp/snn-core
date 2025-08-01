// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/steady/duration_since_boot.hh"

#include "snn-core/unittest.hh"
#include <chrono> // duration_cast, seconds, steady_clock

namespace snn::app
{
    namespace
    {
        bool example()
        {
            const time::duration d = time::steady::duration_since_boot();
            snn_require(d.seconds_part() > 0);

            return true;
        }

        i64 std_chrono_steady_seconds()
        {
            using namespace std::chrono;
            return duration_cast<seconds>(steady_clock::now().time_since_epoch()).count();
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        {
            const i64 sec = app::std_chrono_steady_seconds();

            const time::duration d1 = time::steady::duration_since_boot();
            const time::duration d2 = time::steady::duration_since_boot();

            snn_require(d2 > d1);
            snn_require(d1.seconds_part() == sec || d1.seconds_part() == (sec + 1));
        }
        {
            const auto ns = time::steady::nanoseconds_since_boot();
            const auto us = time::steady::microseconds_since_boot();
            const auto ms = time::steady::milliseconds_since_boot();
            const auto s  = time::steady::seconds_since_boot();
            snn_require(us == (ns / 1'000) || us == ((ns / 1'000) + 1));
            snn_require(ms == (us / 1'000) || ms == ((us / 1'000) + 1));
            snn_require(s == (ms / 1'000) || s == ((ms / 1'000) + 1));
        }
    }
}
