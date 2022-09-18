// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/copy.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/is_something.hh"
#include "snn-core/file/read.hh"
#include "snn-core/file/remove.hh"
#include "snn-core/file/touch.hh"
#include "snn-core/file/write.hh"
#include "snn-core/file/dir/create_temporary.hh"
#include "snn-core/file/dir/remove.hh"
#include "snn-core/file/path/join.hh"
#include "snn-core/random/string.hh"

namespace snn::app
{
    namespace
    {
        bool test_copy()
        {
            const str tmp_dir = file::dir::create_temporary("snn-unittest-").value();

            const str first  = file::path::join(tmp_dir, "first");
            const str second = file::path::join(tmp_dir, "second");

            snn_require(!file::copy(first, first));    // Source doesn't exist.
            snn_require(!file::copy(first, second));   // Source doesn't exist.
            snn_require(!file::copy(tmp_dir, second)); // Source is not a regular file.

            snn_require(!file::is_something(first));
            snn_require(!file::is_something(second));

            // Use a size that is several times larger than the current smallest buffer size (4096),
            // but not an exact multiple.
            const auto contents = random::string<strbuf>(14'000);

            snn_require(file::write(first, contents));

            snn_require(file::copy(first, first)); // Copy to same (does nothing).
            snn_require(file::read(first).value() == contents);

            // Can't append to same.
            snn_require(!file::copy(first, first, file::option::create_or_append));
            snn_require(file::read(first).value() == contents);

            snn_require(!file::copy(first, first, file::option::create_or_fail)); // File exists.
            snn_require(file::read(first).value() == contents);

            snn_require(file::copy(first, second)); // Creates second.
            snn_require(file::read(second).value() == contents);

            snn_require(file::copy(first, second)); // Overwrites second.
            snn_require(file::read(second).value() == contents);

            // Truncate and append doesn't make sense here, but it must work.
            snn_require(file::copy(first, second,
                                   file::option::create_or_truncate | file::option::append));
            snn_require(file::read(second).value() == contents);

            snn_require(file::remove(second));

            // Use a small buffer size to ensure multiple read/write calls. `file::copy()` will
            // currently not use a buffer smaller than 4096 bytes (if the file is larger than that).
            constexpr usize use_smallest_buffer_size = 0;
            snn_require(file::copy(first, second, file::option::create_or_truncate,
                                   file::perm::regular_default, use_smallest_buffer_size));
            snn_require(file::read(second).value() == contents);

            snn_require(file::remove(second));

            snn_require(file::copy(first, second, file::option::create_or_fail));
            snn_require(file::read(second).value() == contents);

            snn_require(!file::copy(first, second, file::option::create_or_fail)); // File exists.
            snn_require(file::read(second).value() == contents);

            snn_require(file::copy(first, second, file::option::create_or_append)); // Append
            snn_require(file::read(second).value() == concat(contents, contents));

            snn_require(!file::copy(first, "/dev/null")); // Destination is not a regular file.

            snn_require(file::remove(first));
            snn_require(file::touch(first)); // Create empty file.

            snn_require(file::copy(first, second)); // Overwrite with empty file.
            snn_require(file::read(second).value().is_empty());

            snn_require(file::remove(second));

            snn_require(file::copy(first, second));
            snn_require(file::read(second).value().is_empty());

            snn_require(file::remove(first));
            snn_require(file::remove(second));

            snn_require(file::dir::remove(tmp_dir));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::test_copy());
    }
}
