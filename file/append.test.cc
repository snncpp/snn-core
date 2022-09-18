// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/append.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/is_something.hh"
#include "snn-core/file/read.hh"
#include "snn-core/file/remove.hh"
#include "snn-core/file/dir/create_temporary.hh"
#include "snn-core/file/dir/remove.hh"
#include "snn-core/file/path/join.hh"

namespace snn
{
    void unittest()
    {
        const str tmp_dir  = file::dir::create_temporary("snn-unittest-").value();
        const str tmp_file = file::path::join(tmp_dir, "append-test");

        constexpr cstrview contents{"abcdefghijklmnopqrstuvwxyz 12345"
                                    "67890 ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

        snn_require(!file::is_something(tmp_file));

        snn_require(file::append(tmp_file, contents));

        snn_require(file::is_something(tmp_file));

        snn_require(file::read(tmp_file).value() == contents);

        snn_require(file::append(tmp_file, contents));

        const auto s = file::read(tmp_file).value_or_default();
        snn_require(s.size() == (contents.size() * 2));
        snn_require(s.has_front(contents));
        snn_require(s.has_back(contents));

        snn_require(file::remove(tmp_file));

        snn_require(!file::is_something(tmp_file));

        snn_require(file::dir::remove(tmp_dir));
    }
}
