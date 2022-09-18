// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/dir/list.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/remove.hh"
#include "snn-core/file/touch.hh"
#include "snn-core/file/dir/create_temporary.hh"
#include "snn-core/file/dir/remove.hh"
#include "snn-core/file/path/join.hh"
#include "snn-core/set/sorted.hh"

namespace snn
{
    void unittest()
    {
        {
            const str tmp_dir = file::dir::create_temporary("snn-unittest-").value();

            snn_require(file::dir::list(tmp_dir).value().is_empty());

            const str first_file  = file::path::join(tmp_dir, "first");
            const str second_file = file::path::join(tmp_dir, "second");

            snn_require(file::touch(first_file));
            snn_require(file::touch(second_file));

            const file::dir::entry first_entry{"first", file::type::regular};
            const file::dir::entry second_entry{"second", file::type::regular};
            const file::dir::entry non_existent_entry{"third", file::type::regular};

            const auto entries = file::dir::list(tmp_dir).value();
            snn_require(entries.count() == 2);
            snn_require(entries.contains(first_entry));
            snn_require(entries.contains(second_entry));
            snn_require(!entries.contains(non_existent_entry));

            set::sorted<file::dir::entry> sorted_entries;
            snn_require(file::dir::list(tmp_dir, sorted_entries));
            snn_require(sorted_entries.count() == 2);
            snn_require(sorted_entries.contains(first_entry));
            snn_require(sorted_entries.contains(second_entry));
            snn_require(!sorted_entries.contains(non_existent_entry));

            snn_require(file::remove(first_file));
            snn_require(file::remove(second_file));
            snn_require(file::dir::remove(tmp_dir));
        }
        {
            const auto r = file::dir::list("list.test.cc");
            snn_require(!r);
            snn_require(r.error_code() == make_error_code(ENOTDIR, system::error_category));
        }
        {
            const auto r = file::dir::list("/tmp/snn-unittest-should-not-exist");
            snn_require(!r);
            snn_require(r.error_code() == make_error_code(ENOENT, system::error_category));
        }

        static_assert(file::dir::detail::skip_name("."));
        static_assert(file::dir::detail::skip_name(".."));
        static_assert(!file::dir::detail::skip_name(""));
        static_assert(!file::dir::detail::skip_name("a"));
        static_assert(!file::dir::detail::skip_name("ab"));
        static_assert(!file::dir::detail::skip_name("abc"));
        static_assert(!file::dir::detail::skip_name(".a"));
        static_assert(!file::dir::detail::skip_name(".ab"));
        static_assert(!file::dir::detail::skip_name(".abc"));
        static_assert(!file::dir::detail::skip_name("..a"));
        static_assert(!file::dir::detail::skip_name("..ab"));
        static_assert(!file::dir::detail::skip_name("..abc"));
        static_assert(!file::dir::detail::skip_name("..."));
    }
}
