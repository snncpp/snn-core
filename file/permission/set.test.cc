// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/permission/set.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/is_regular.hh"
#include "snn-core/file/is_something.hh"
#include "snn-core/file/remove.hh"
#include "snn-core/file/status.hh"
#include "snn-core/file/touch.hh"
#include "snn-core/file/dir/create_temporary.hh"
#include "snn-core/file/dir/remove.hh"
#include "snn-core/file/path/join.hh"

namespace snn::app
{
    namespace
    {
        bool test_set()
        {
            const str tmp_dir  = file::dir::create_temporary("snn-unittest-").value();
            const str tmp_file = file::path::join(tmp_dir, "permission-test");

            // The file doesn't exist yet.
            snn_require(!file::is_something(tmp_file));
            snn_require(!file::permission::set(tmp_file, file::perm::owner_all));

            snn_require(file::touch(tmp_file));

            snn_require(file::is_regular(tmp_file));
            snn_require(file::status(tmp_file).value().permission() != file::perm::owner_all);

            snn_require(file::permission::set(tmp_file, file::perm::owner_all));
            snn_require(file::status(tmp_file).value().permission() == file::perm::owner_all);

            snn_require(file::remove(tmp_file));
            snn_require(file::dir::remove(tmp_dir));

            return true;
        }

    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::test_set());
    }
}
