// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/steady/cache.hh"

#include "snn-core/unittest.hh"
#include "snn-core/time/unit.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            time::steady::cache cache;

            time::duration d1 = cache.get();
            snn_require(d1.seconds() > 0);

            snn_require(cache.seconds() == d1.seconds());
            snn_require(cache.milliseconds() == d1.to<time::milliseconds>().value());

            time::duration d2 = cache.get();
            snn_require(d2 == d1);

            cache.update();

            time::duration d3 = cache.get();
            snn_require(d3 > d2);

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
