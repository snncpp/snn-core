// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/is_something.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        snn_require(file::is_something("is_something.test.cc"));
        snn_require(file::is_something("/tmp"));
        snn_require(file::is_something("/dev/null"));
        snn_require(!file::is_something("/tmp/non_existing_389287329"));
    }
}
