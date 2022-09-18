// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/dir/remove.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/is_directory.hh"
#include "snn-core/file/status.hh"
#include "snn-core/file/dir/create.hh"

namespace snn
{
    void unittest()
    {
        snn_require(!file::status("/tmp/non_existing_389287329"));
        snn_require(!file::dir::remove("/tmp/non_existing_389287329"));

        snn_require(file::dir::create("/tmp/non_existing_389287329"));
        snn_require(file::is_directory("/tmp/non_existing_389287329"));

        snn_require(file::dir::remove("/tmp/non_existing_389287329"));
        snn_require(!file::status("/tmp/non_existing_389287329"));
    }
}
