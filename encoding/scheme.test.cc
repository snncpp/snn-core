// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/encoding/scheme.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(to_underlying(encoding::scheme::unknown) == 0);
            snn_require(to_underlying(encoding::scheme::utf8) != 0);

            snn_require(encoding::scheme_count > 30);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
    }
}
