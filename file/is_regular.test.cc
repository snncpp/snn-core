// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/is_regular.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        // inline bool is_regular(const null_terminated path)
        {
            snn_require(file::is_regular("is_regular.test.cc"));
            snn_require(!file::is_regular("/tmp"));
            snn_require(!file::is_regular("/tmp/nonexisting3189287"));
        }
    }
}
