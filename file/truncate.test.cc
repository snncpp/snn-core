// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/truncate.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/remove.hh"
#include "snn-core/file/status.hh"
#include "snn-core/file/write.hh"
#include "snn-core/file/reader_writer.hh"
#include "snn-core/file/dir/create_temporary.hh"
#include "snn-core/file/dir/remove.hh"
#include "snn-core/file/path/join.hh"

namespace snn
{
    void unittest()
    {
        const str tmp_dir = file::dir::create_temporary("snn-unittest-").value();
        const str foo     = file::path::join(tmp_dir, "foo");

        snn_require(file::write(foo, "abcdefgh"));
        snn_require(file::status(foo).value().size() == 8);

        snn_require(file::truncate(foo));
        snn_require(file::status(foo).value().size() == 0);

        snn_require(file::remove(foo));
        snn_require(!file::truncate(foo));

        {
            file::reader_writer w;
            snn_require(w.open_for_writing(foo));

            const int fd = w.descriptor().value();

            snn_require(w.write_all("abcdefgh"));

            snn_require(w.offset().value_or_default() == 8);

            snn_require(file::truncate(fd));

            // Offset is not modified.
            snn_require(w.offset().value_or_default() == 8);

            snn_require(w.seek(0));

            snn_require(w.write_all("ABCD"));

            snn_require(w.offset().value_or_default() == 4);

            snn_require(!file::truncate(-1));
        }

        snn_require(file::status(foo).value().size() == 4);
        snn_require(file::remove(foo));

        snn_require(file::dir::remove(tmp_dir));
    }
}
