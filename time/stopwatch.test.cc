// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/stopwatch.hh"

#include "snn-core/unittest.hh"
#include "snn-core/thread/sleep_for.hh"
#include "snn-core/time/unit.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            time::stopwatch stopwatch; // Starts running.

            thread::sleep_for(time::milliseconds{2}.duration().value()).or_throw();

            const time::duration d1 = stopwatch.duration();
            snn_require(d1.seconds() == 0);
            snn_require(d1.nanoseconds() >= 2'000'000 && d1.nanoseconds() < 2'900'000);

            // The stopwatch keeps running.

            const time::duration d2 = stopwatch.duration();
            snn_require(d2 > d1);

            stopwatch.reset();

            thread::sleep_for(time::milliseconds{1100}.duration().value()).or_throw();

            // Note that the stopwatch keeps running after each of these calls.
            const i64 ns = stopwatch.nanoseconds();
            const i64 us = stopwatch.microseconds();
            const i64 ms = stopwatch.milliseconds();

            snn_require(ns >= 1'100'000'000 && ns < 1'200'000'000); // ~1.1 seconds.
            snn_require(us >= 1'100'000 && us < 1'200'000);         // ~1.1 seconds.
            snn_require(ms >= 1'100 && ms < 1'200);                 // ~1.1 seconds.

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());
    }
}
