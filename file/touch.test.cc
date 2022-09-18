// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/touch.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/is_regular.hh"
#include "snn-core/file/is_something.hh"
#include "snn-core/file/read.hh"
#include "snn-core/file/remove.hh"
#include "snn-core/file/status.hh"
#include "snn-core/file/write.hh"
#include "snn-core/file/dir/create_temporary.hh"
#include "snn-core/file/dir/remove.hh"
#include "snn-core/file/path/join.hh"

namespace snn
{
    void unittest()
    {
        const str tmp_dir  = file::dir::create_temporary("snn-unittest-").value();
        const str tmp_file = file::path::join(tmp_dir, "touch-test");

        snn_require(!file::is_something(tmp_file));

        snn_require(file::touch(tmp_file));
        snn_require(file::is_regular(tmp_file));
        snn_require(file::status(tmp_file).value().size() == 0);

        snn_require(file::remove(tmp_file));
        snn_require(!file::is_something(tmp_file));

        // Can't touch a non-existing file without `file::option::create`.
        snn_require(!file::touch(tmp_file, file::option::none));

        snn_require(file::write(tmp_file, "abcdefghijklmnopqrstuvwxyz"));

        snn_require(file::touch(tmp_file)); // Must not change the contents of the file in any way.

        snn_require(file::read(tmp_file).value() == "abcdefghijklmnopqrstuvwxyz");

        snn_require(file::remove(tmp_file));
        snn_require(!file::is_something(tmp_file));

        // Can't touch a directory.
        snn_require(!file::touch(tmp_dir));

        // Can't touch a character device.
        snn_require(!file::touch("/dev/null"));

        snn_require(file::dir::remove(tmp_dir));
    }
}
