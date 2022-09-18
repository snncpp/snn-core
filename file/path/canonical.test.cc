// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/path/canonical.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/dir/current.hh"
#include "snn-core/file/path/join.hh"
#include <cerrno> // ENOENT

namespace snn
{
    void unittest()
    {
        {
            const str cwd = file::dir::current().value();
            snn_require(cwd.has_back("/file/path"));

            snn_require(file::path::canonical(cwd).value() == cwd);
            snn_require(file::path::canonical(".").value() == cwd);
            snn_require(file::path::canonical("./canonical.test.cc").value() ==
                        file::path::join(cwd, "canonical.test.cc"));
        }

        {
            decltype(auto) res = file::path::canonical("/tmp/non_existing_389287329");
            static_assert(std::is_same_v<decltype(res), result<str>>);
            snn_require(!res);
            snn_require(!res.has_value());
            snn_require(res.error_code() == make_error_code(ENOENT, system::error_category));
        }
    }
}
