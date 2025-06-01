// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/thread/sleep_for.hh"

#include "snn-core/unittest.hh"
#include "snn-core/fmt/print.hh"
#include "snn-core/time/duration.formatter.hh"
#include "snn-core/time/stopwatch.hh"
#include "snn-core/time/unit.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            time::stopwatch stopwatch;

            constexpr time::duration how_long = time::milliseconds{50}.duration().value();
            thread::sleep_for(how_long).or_throw();

            fmt::print("Slept for: {}\n", stopwatch.duration());

            return true;
        }

        bool test_sleep_for()
        {
            time::stopwatch stopwatch;

            for (loop::count lc{2}; lc--;)
            {
                thread::sleep_for(time::milliseconds{2}.duration().value()).or_throw();

                const i64 ns = stopwatch.nanoseconds();
                snn_require(ns >= 2'000'000 && ns < 2'900'000);

                stopwatch.reset();
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
        snn_require(app::test_sleep_for());
    }
}
