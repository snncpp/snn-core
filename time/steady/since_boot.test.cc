// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/steady/since_boot.hh"

#include "snn-core/unittest.hh"
#include <chrono> // duration_cast, seconds, steady_clock

namespace snn::app
{
    namespace
    {
        bool example()
        {
            const time::duration d = time::steady::since_boot();
            snn_require(d.seconds() > 0);

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

        const i64 sec = app::std_chrono_steady_seconds();

        const time::duration d1 = time::steady::since_boot();
        const time::duration d2 = time::steady::since_boot();

        snn_require(d2 > d1);
        snn_require(d1.seconds() == sec || d1.seconds() == (sec + 1));
    }
}
