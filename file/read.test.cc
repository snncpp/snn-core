// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/read.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        {
            const auto contents = file::read<strbuf>("read.test.cc").value_or_default();
            snn_require(contents.size() > 1'500);
            snn_require(contents.has_front("// Copyright"));
            snn_require(contents.has_back("}\n}\n"));
            snn_require(contents.contains("size() > "
                                          "1'500"));
        }
        {
            const str path{"read.test.cc"};
            str contents{"File contents here: "};
            snn_require(file::read(path, contents));
            snn_require(contents.has_front("File contents here: // Copyright"));
            snn_require(contents.has_back("}\n}\n"));
        }
        {
            const auto res = file::read("/tmp/snn-unittest-should-not-exist");
            snn_require(!res);
            snn_require(res.error_code() == make_error_code(ENOENT, system::error_category));
        }
        {
            const auto res = file::read("/dev/urandom");
            snn_require(!res); // Not a file.
            snn_require(res.error_code() == make_error_code(EINVAL, system::error_category));
        }
        {
            const auto res = file::read("/tmp");
            snn_require(!res); // Not a file.
            snn_require(res.error_code() == make_error_code(EINVAL, system::error_category));
        }
    }
}
