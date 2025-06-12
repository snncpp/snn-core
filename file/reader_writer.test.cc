// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/reader_writer.hh"

#include "snn-core/unittest.hh"
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
        static_assert(!std::is_copy_assignable_v<file::reader_writer<>>);
        static_assert(!std::is_copy_constructible_v<file::reader_writer<>>);
        static_assert(std::is_move_assignable_v<file::reader_writer<>>);
        static_assert(std::is_move_constructible_v<file::reader_writer<>>);

        // explicit reader_writer(Io io = Io{})
        // explicit operator bool() const
        // result<void> write_all(const cstrview s)
        {
            const str tmp_dir  = file::dir::create_temporary("snn-unittest-").value();
            const str tmp_file = file::path::join(tmp_dir, "foobar");

            file::reader_writer rw;
            snn_require(!rw);
            snn_require(!rw.is_open());

            // File does not exist.
            snn_require(!file::is_something(tmp_file));

            // Open (will create file but not truncate).
            snn_require(rw.open_for_reading_and_writing(tmp_file));

            snn_require(rw);
            snn_require(rw.is_open());

            snn_require(rw.write_some("abc").value_or_default() == 3);
            snn_require(rw.write_all("defghijklmnopqrstuvwxyz"));

            snn_require(rw.close());
            snn_require(!rw);
            snn_require(!rw.is_open());

            // Re-open (will not truncate).
            snn_require(rw.open_for_reading_and_writing(tmp_file));

            // Overwrite beginning of file.
            snn_require(rw.write_all("ABC"));

            str buf;
            strview dest = buf.resize_for_overwrite(3);
            snn_require(rw.read_some(dest).value_or_default() == 3);
            snn_require(buf == "def");

            snn_require(rw.close());

            snn_require(file::read(tmp_file).value() == "ABCdefghijklmnopqrstuvwxyz");

            // Create or append to file.
            snn_require(rw.open_for_appending(tmp_file));
            rw << "...123456789";

            snn_require(rw.close());

            snn_require(file::read(tmp_file).value() == "ABCdefghijklmnopqrstuvwxyz...123456789");

            snn_require(file::remove(tmp_file));
            snn_require(file::dir::remove(tmp_dir));
        }
        {
            file::reader_writer rw;

            auto res = rw.open_for_writing("/tmp");
            snn_require(!res);
            snn_require(res.error_code() == make_error_code(EISDIR, system::error_category));
            snn_require(!rw);
            snn_require(!rw.is_open());

            // Appending an empty string has no effect.
            snn_require(rw.write_all(""));

            res = rw.write_all("a");
            snn_require(!res);
            snn_require(res.error_code() == make_error_code(EBADF, system::error_category));
        }

        // explicit reader_writer(const null_terminated path, ...)
        {
            const str tmp_dir  = file::dir::create_temporary("snn-unittest-").value();
            const str tmp_file = file::path::join(tmp_dir, "foobar");

            file::reader_writer rw{tmp_file,
                                   file::option::write_only | file::option::create_or_fail};
            snn_require(rw);
            snn_require(rw.is_open());

            snn_require_throws_code(
                (file::reader_writer{tmp_file,
                                     file::option::write_only | file::option::create_or_fail}),
                make_error_code(EEXIST, system::error_category));

            snn_require(file::remove(tmp_file));
            snn_require(file::dir::remove(tmp_dir));
        }

        // explicit reader_writer(file::descriptor)
        {
            file::reader_writer rw;
            snn_require(rw.open_for_reading("reader_writer.test.cc"));
            snn_require(rw);
            snn_require(rw.is_open());

            file::descriptor fd = std::move(rw.descriptor());
            snn_require(fd);

            snn_require(!rw);
            snn_require(!rw.is_open());

            rw = file::reader_writer{std::move(fd)};
            snn_require(rw);
            snn_require(rw.is_open());

            snn_require(!fd);

            snn_require(rw.close());
        }

        {
            file::reader_writer rw;
            snn_require(rw.open_for_reading("reader_writer.test.cc"));
            snn_require(rw.is_open());

            file::reader_writer rw2{std::move(rw)};
            snn_require(rw2.is_open());
            snn_require(!rw.is_open());

            rw = std::move(rw2);
            snn_require(!rw2.is_open());
            snn_require(rw.is_open());

            snn_require(rw.close());
        }

        // offset
        {
            const str tmp_dir  = file::dir::create_temporary("snn-unittest-").value();
            const str tmp_file = file::path::join(tmp_dir, "foobar");

            file::reader_writer rw;

            // Create or truncate file.
            snn_require(rw.open_for_writing(tmp_file));

            snn_require(rw);
            snn_require(rw.is_open());
            snn_require(rw.offset().value() == 0);

            snn_require(rw.write_all("abcdefghijklmnopqrstuvwxyz"));
            snn_require(rw.offset().value() == 26);

            snn_require(rw.seek(15).value() == 15);                       // Absolute
            snn_require(rw.seek(5, file::whence::current).value() == 20); // Relative

            snn_require(rw.write_all("uvwxyz"));
            snn_require(rw.offset().value() == 26);

            snn_require(rw.close());

            const auto stat = file::status(tmp_file).value();
            snn_require(stat.is_regular());
            snn_require(stat.size() == 26);

            snn_require(file::remove(tmp_file));
            snn_require(file::dir::remove(tmp_dir));
        }

        // truncate
        {
            const str tmp_dir  = file::dir::create_temporary("snn-unittest-").value();
            const str tmp_file = file::path::join(tmp_dir, "foobar");

            snn_require(file::write(tmp_file, "abcdefghijklmnopqrstuvwxyz"));

            file::reader_writer rw;
            // Create or append to file.
            snn_require(rw.open_for_appending(tmp_file));

            file::info info{init::do_not_initialize};

            snn_require(rw.status(info));
            snn_require(info.is_regular());
            snn_require(info.size() == 26);

            snn_require(rw.truncate(15));

            snn_require(rw.status(info));
            snn_require(info.size() == 15);

            snn_require(rw.truncate());

            snn_require(rw.status(info));
            snn_require(info.size() == 0);

            snn_require(rw.close());

            snn_require(file::remove(tmp_file));
            snn_require(file::dir::remove(tmp_dir));
        }

        {
            file::reader_writer rw;
            snn_require(!rw);
            snn_require(!rw.is_open());

            snn_require(rw.open_for_reading("reader_writer.test.cc"));
            snn_require(rw);
            snn_require(rw.is_open());

            str s;

            auto res = rw.read_some(s.view()); // Empty buffer.
            snn_require(res);
            snn_require(res.value() == 0);

            strview dest = s.resize_for_overwrite(12);

            res = rw.read_some(dest);
            snn_require(res);
            snn_require(res.value() == 12);
            snn_require(s == "// Copyright");

            res = rw.read_some(s.resize_for_overwrite(4));
            snn_require(res);
            snn_require(res.value() == 4);
            snn_require(s == " (c)");

            {
                auto info = rw.status().value();
                snn_require(info.is_regular());
                snn_require(info.size() > 2'000);
            }
            {
                file::info info{init::do_not_initialize};
                snn_require(rw.status(info));
                snn_require(info.is_regular());
                snn_require(info.size() > 2'000);
            }
        }

        // read_fill
        {
            file::reader_writer rw;

            snn_require(!rw);
            snn_require(!rw.is_open());

            snn_require(rw.open_for_reading("reader_writer.test.cc"));
            snn_require(rw);
            snn_require(rw.is_open());

            strbuf chunk1;
            strbuf chunk2;
            strbuf chunk3;

            // We can read the first 1'000 bytes.
            snn_require(rw.read_fill(chunk1.resize_for_overwrite(1'000)));
            snn_require(chunk1.has_front("// Copyright"));

            // We can read an additional 1'000 bytes.
            snn_require(rw.read_fill(chunk2.resize_for_overwrite(1'000)));
            snn_require(chunk2 != chunk1);

            // We can't read an additional 50'000 bytes.
            auto res = rw.read_fill(chunk3.resize_for_overwrite(50'000));
            snn_require(!res);
            snn_require(res.error_code() == file::error::no_more_data);
        }

        {
            file::reader_writer rw;
            auto res = rw.open_for_reading("/tmp/snn-unittest-should-not-exist");
            snn_require(!res);
            snn_require(res.error_code() == make_error_code(ENOENT, system::error_category));
            snn_require(!rw);
            snn_require(!rw.is_open());
        }

        {
            int fd = -1;
            {
                file::reader_writer rw;
                snn_require(rw.open_for_reading("reader_writer.test.cc"));
                fd = rw.descriptor().value();
                // The file descriptor will be closed here.
            }
            snn_require(fd != -1);
            const auto res = file::status(fd);
            snn_require(!res);
            snn_require(res.error_code() == make_error_code(EBADF, system::error_category));
        }

        {
            file::reader_writer rw;
            snn_require(rw.open_for_reading("/dev/urandom"));
            str s;
            constexpr usize buf_size = 16;
            auto res                 = rw.read_some(s.resize_for_overwrite(buf_size));
            snn_require(res);
            snn_require(res.value() == buf_size);
        }

        {
            file::reader_writer rw;
            // Does not check if path is a file/character device.
            snn_require(rw.open_for_reading("/tmp"));
        }

        {
            file::reader_writer rw{"reader_writer.test.cc", file::option::read_only};
            snn_require(rw);
            snn_require(rw.is_open());

            snn_require_throws_code((file::reader_writer{"/tmp/snn-unittest-should-not-exist",
                                                         file::option::read_only}),
                                    make_error_code(ENOENT, system::error_category));
        }
    }
}
