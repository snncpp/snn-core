// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/dir/current.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/is_same.hh"

namespace snn
{
    void unittest()
    {
        const str current_path = file::dir::current().value();
        snn_require(current_path.has_front('/')); // Must be absolute.
        snn_require(current_path.has_back("/file/dir"));

        const str current_logical_path = file::dir::current_logical().value();
        snn_require(current_logical_path.has_front('/')); // Must be absolute.
        snn_require(current_logical_path.has_back("/file/dir"));

        snn_require(file::is_same(current_path, current_logical_path));
    }
}
