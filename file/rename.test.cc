// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/rename.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/is_regular.hh"
#include "snn-core/file/is_something.hh"
#include "snn-core/file/remove.hh"
#include "snn-core/file/touch.hh"

namespace snn
{
    void unittest()
    {
        {
            const str a{"/tmp/nonexisting3189287a"};
            const str b{"/tmp/nonexisting3189287b"};

            snn_require(!file::is_something(a));
            snn_require(!file::is_something(b));

            snn_require(!file::rename(a, b));

            auto res = file::rename(a, b);
            snn_require(!res);
            snn_require(res.error_code() == make_error_code(ENOENT, system::error_category));

            snn_require(file::touch(a));
            snn_require(file::is_regular(a));
            snn_require(!file::is_something(b));

            snn_require(file::rename(a, b));
            snn_require(!file::is_something(a));
            snn_require(file::is_regular(b));

            snn_require(file::remove(b));
        }
    }
}
