// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/stream/common.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/reader_writer.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // Concepts

            static_assert(stream::readable<file::reader_writer<>>);
            static_assert(!stream::readable<int>);
            static_assert(!stream::readable<str>);

            static_assert(stream::writable<file::reader_writer<>>);
            static_assert(!stream::writable<int>);
            static_assert(!stream::writable<str>);

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
