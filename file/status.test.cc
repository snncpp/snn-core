// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/status.hh"

#include "snn-core/unittest.hh"
#include <fcntl.h>  // open
#include <unistd.h> // close

namespace snn
{
    void unittest()
    {
        {
            file::info info{init::do_not_initialize};
            snn_require(file::status("/tmp", info));
            snn_require(info.is_directory());
            snn_require(!info.is_regular());
            snn_require(info.type() == file::type::directory);
        }

        {
            const str path{"/tmp"};
            const auto info = file::status(path).value();
            snn_require(info.is_directory());
            snn_require(!info.is_regular());
            snn_require(info.type() == file::type::directory);
            snn_require((info.permission() & file::perm::all_all) == file::perm::all_all);
            snn_require((info.permission() & file::perm::sticky) == file::perm::sticky);
            snn_require((info.permission() & file::perm::setuid) == file::perm::none);
            snn_require((info.permission() & file::perm::setgid) == file::perm::none);
        }

        {
            const str path{"/dev/urandom"};
            auto info = file::status(path).value();
            snn_require(info.is_character());
            snn_require(!info.is_directory());
            snn_require(!info.is_regular());
            snn_require(info.size() == 0);
            snn_require(info.type() == file::type::character);
        }

        {
            const auto info = file::status("status.test.cc").value();
            snn_require(!info.is_directory());
            snn_require(info.is_regular());
            snn_require(info.size() > 1'000);
            snn_require(info.type() == file::type::regular);
        }

        {
            const int fd = ::open("status.test.cc", O_RDONLY | O_CLOEXEC);
            snn_require(fd != -1);
            const auto [info, err] = file::status(fd).unpack();
            snn_require(!err);
            snn_require(!info.is_directory());
            snn_require(info.is_regular());
            snn_require(info.size() > 1'000);
            snn_require(info.type() == file::type::regular);
            ::close(fd);
        }

        {
            snn_require(!file::status("/tmp/non_existing_389287329"));
            snn_require_throws_code(file::status("/tmp/non_existing_389287329").value(),
                                    make_error_code(ENOENT, system::error_category));
        }
    }
}
