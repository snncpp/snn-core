// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/seek.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/offset.hh"
#include "snn-core/file/reader_writer.hh"

namespace snn
{
    void unittest()
    {
        file::reader_writer r;

        snn_require(r.open_for_reading("seek.test.cc"));
        snn_require(r);

        const int fd = r.descriptor().value();

        snn_require(file::offset(fd).value_or(123) == 0);

        snn_require(file::seek(fd, 1));
        snn_require(file::offset(fd).value_or_default() == 1);

        snn_require(file::seek(fd, usize{5}));
        snn_require(file::offset(fd).value_or_default() == 5);

        snn_require(file::seek(fd, -2, file::whence::current));
        snn_require(file::offset(fd).value_or_default() == 3);

        str s;

        snn_require(r.read_some(s.resize_for_overwrite(4)).value() == 4);
        snn_require(s == "Copy");

        snn_require(file::offset(fd).value_or_default() == 7);
        snn_require(file::seek(fd, 0));
        snn_require(file::offset(fd).value_or(123) == 0);

        snn_require(r.read_some(s.resize_for_overwrite(12)).value() == 12);
        snn_require(s == "// Copyright");
        snn_require(file::offset(fd).value_or_default() == 12);

        snn_require(file::seek(fd, 0, file::whence::end));
        snn_require(file::offset(fd).value_or_default() > 1'000);

        {
            auto res = file::seek(fd, -5'000, file::whence::current);
            snn_require(!res);
            snn_require(res.error_code() == make_error_code(EINVAL, system::error_category));
        }

        {
            auto res = file::seek(fd, constant::limit<usize>::max);
            snn_require(!res);
            snn_require(res.error_code() == make_error_code(EINVAL, system::error_category));
        }

        {
            auto res = file::seek(-1, 123);
            snn_require(!res);
            snn_require(res.error_code() == make_error_code(EBADF, system::error_category));
        }
    }
}
