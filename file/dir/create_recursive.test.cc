// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/dir/create_recursive.hh"

#include "snn-core/unittest.hh"

#include "snn-core/file/is_directory.hh"
#include "snn-core/file/is_something.hh"
#include "snn-core/file/status.hh"
#include "snn-core/file/dir/create_temporary.hh"
#include "snn-core/file/dir/remove.hh"
#include "snn-core/file/path/join.hh"
#include "snn-core/file/permission/mask.hh"

namespace snn::app
{
    namespace
    {
        bool test_create_recursive()
        {
            const str tmp_dir = file::dir::create_temporary("snn-unittest-").value();

            const str first  = file::path::join(tmp_dir, "one");
            const str second = file::path::join(tmp_dir, "one/two");
            const str third  = file::path::join(tmp_dir, "one/two/three");

            snn_require(!file::is_something(first));

            using file::perm;

            // Disable permission mask so we can verify that the permissions are set correctly.
            const auto original_mask = file::permission::mask(perm::none);

            snn_require(file::dir::create_recursive(third, perm::owner_all, perm::all_all));

            snn_require(file::is_directory(third));

            snn_require(file::status(first).value().permission() == perm::all_all);
            snn_require(file::status(second).value().permission() == perm::all_all);
            snn_require(file::status(third).value().permission() == perm::owner_all);

            auto res = file::dir::create_recursive(third);
            snn_require(!res);
            snn_require(res.error_code() == make_error_code(EEXIST, system::error_category));

            snn_require(file::dir::remove(third));
            snn_require(file::dir::remove(second));
            snn_require(file::dir::remove(first));

            snn_require(file::dir::remove(tmp_dir));

            file::permission::mask(original_mask);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::test_create_recursive());
    }
}
