// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/io/common.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/io/blocking.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            static_assert(io::readable<file::io::blocking>);
            static_assert(!io::readable<int>);
            static_assert(!io::readable<str>);

            static_assert(io::writable<file::io::blocking>);
            static_assert(!io::writable<int>);
            static_assert(!io::writable<str>);

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
