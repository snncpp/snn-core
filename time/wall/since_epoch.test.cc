// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/wall/since_epoch.hh"

#include "snn-core/unittest.hh"
#include <chrono> // duration_cast, seconds, system_clock

namespace snn::app
{
    namespace
    {
        bool example()
        {
            const time::duration d = time::wall::since_epoch();
            snn_require(d.seconds() > 0);

            return true;
        }

        i64 std_chrono_system_seconds()
        {
            using namespace std::chrono;
            return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        const i64 sec = app::std_chrono_system_seconds();

        const time::duration d1 = time::wall::since_epoch();
        const time::duration d2 = time::wall::since_epoch();

        // These can fail if the wall time was changed between calls:
        snn_require(d2 > d1);
        snn_require(d1.seconds() == sec || d1.seconds() == (sec + 1));
    }
}
