// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/is_directory.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        // inline bool is_directory(const null_terminated path)
        {
            snn_require(file::is_directory("/"));
            snn_require(file::is_directory("/tmp"));
            snn_require(!file::is_directory("is_directory.test.cc"));
            snn_require(!file::is_directory("/tmp/should-not-exist-8372811935"));
        }
    }
}
