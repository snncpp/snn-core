// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fmt/error_code.hh"

#include "snn-core/unittest.hh"
#include "snn-core/generic/error.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(fmt::error_code(make_error_code(generic::error::no_error)) ==
                        "Generic: No error (0)");

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
