// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fmt/exception.hh"

#include "snn-core/unittest.hh"
#include "snn-core/generic/error.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            str buffer = "Error: ";

            fmt::exception(snn::exception{make_error_code(generic::error::no_error)}, buffer);
            snn_require(buffer == "Error: Generic: No error (0)");

            buffer.append('\n');

            fmt::exception(std::runtime_error{"Something something"}, buffer);
            snn_require(buffer == "Error: Generic: No error (0)\nSomething something");

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
