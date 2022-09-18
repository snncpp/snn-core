// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/system/page_size.hh"

#include "snn-core/unittest.hh"
#include "snn-core/math/common.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            const not_zero<usize> page_size = system::page_size();
            snn_require(page_size.get() >= 4096);
            snn_require(math::is_power_of_two(page_size.get()));

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
