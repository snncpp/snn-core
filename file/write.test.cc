// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/write.hh"

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
        const str tmp_file = file::path::join(tmp_dir, "write-test");

        constexpr cstrview contents{"abcdefghijklmnopqrstuvwxyz 12345"
                                    "67890 ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

        snn_require(!file::is_something(tmp_file));

        snn_require(file::write(tmp_file, contents));

        snn_require(file::is_something(tmp_file));

        // File exists, can't open in exclusive mode.
        auto res = file::write(tmp_file, contents, file::option::create_or_fail);
        snn_require(!res);
        snn_require(res.error_code() == error_code{EEXIST, system::error_category});

        snn_require(file::read(tmp_file).value() == contents);

        // Overwrite
        snn_require(file::write(tmp_file, contents));

        snn_require(file::read(tmp_file).value() == contents);

        // Append
        snn_require(file::write(tmp_file, contents, file::option::create_or_append));

        snn_require(file::read(tmp_file).value().size() == (contents.size() * 2));

        snn_require(file::remove(tmp_file));

        // File doesn't exist, can open in exclusive mode.
        snn_require(file::write(tmp_file, contents, file::option::create_or_fail));

        snn_require(file::read(tmp_file).value() == contents);

        snn_require(file::remove(tmp_file));

        // Can't write to a directory.
        snn_require(!file::write(tmp_dir, contents));

        snn_require(file::dir::remove(tmp_dir));
    }
}
