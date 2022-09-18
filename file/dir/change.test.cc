// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/dir/change.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/dir/current.hh"

namespace snn
{
    void unittest()
    {
        const str before = file::dir::current().value();

        snn_require(file::dir::change("/"));

        const str after = file::dir::current().value();
        snn_require(after == "/");
        snn_require(after != before);

        snn_require(!file::dir::change("/tmp/non_existing_389287329"));

        // Restore
        snn_require(file::dir::change(before));
    }
}
