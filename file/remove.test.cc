// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/remove.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/is_regular.hh"
#include "snn-core/file/status.hh"
#include "snn-core/file/touch.hh"

namespace snn
{
    void unittest()
    {
        const str path{"/tmp/nonexisting3189287"};

        snn_require(!file::status(path));
        snn_require(!file::remove(path));

        snn_require(file::touch(path));
        snn_require(file::is_regular(path));

        snn_require(file::remove(path));
        snn_require(!file::status(path));
        snn_require(!file::remove(path));
    }
}
