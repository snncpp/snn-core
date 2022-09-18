// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/encoding/schemes.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(encoding::schemes.contains(encoding::scheme::unknown));
            snn_require(encoding::schemes.contains(encoding::scheme::iso8859_1));
            snn_require(encoding::schemes.contains(encoding::scheme::utf8));

            snn_require(encoding::schemes.count() > 30);

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
