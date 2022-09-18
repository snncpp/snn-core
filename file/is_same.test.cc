// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/is_same.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        // inline bool is_same(const null_terminated path1, const null_terminated path2)
        {
            snn_require(file::is_same("is_same.test.cc", "is_same.test.cc"));
            snn_require(file::is_same("/tmp", "/tmp"));

            snn_require(!file::is_same("is_same.test.cc", "/tmp"));
            snn_require(!file::is_same("/tmp", "is_same.test.cc"));
            snn_require(!file::is_same("/tmp/non-existing-3271", "/tmp/non-existing-3271"));
            snn_require(!file::is_same("is_same.test.cc", "/tmp/non-existing-3271"));
            snn_require(!file::is_same("/tmp/non-existing-3271", "is_same.test.cc"));
        }

        // inline bool is_same(const file::info& info1, const file::info& info2)
        {
        }
    }
}
