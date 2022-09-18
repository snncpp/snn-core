// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/random/boolean.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            usize tries = 0;
            while (true)
            {
                const bool b1 = random::boolean();
                const bool b2 = random::boolean();
                if (b1 != b2)
                {
                    break;
                }

                ++tries;
                snn_require(tries < 20);
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
    }
}
