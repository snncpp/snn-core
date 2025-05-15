// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/offset.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/reader_writer.hh"

namespace snn
{
    void unittest()
    {
        file::reader_writer r;

        snn_require(r.open_for_reading("offset.test.cc"));
        snn_require(r);

        const int fd = r.descriptor().value();

        const auto offset_before = file::offset(fd);
        snn_require(offset_before);
        snn_require(offset_before.value() == 0);

        str s;
        strview dest = s.resize_for_overwrite(12);

        snn_require(r.read_some(dest).value() == 12);
        snn_require(s == "// Copyright");

        const usize offset_after = file::offset(fd).value_or_default();
        snn_require(offset_after == 12);

        auto res = file::offset(-1);
        snn_require(!res);
        snn_require(res.error_code() == make_error_code(EBADF, system::error_category));
    }
}
