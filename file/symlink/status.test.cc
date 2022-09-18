// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/symlink/status.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
#if defined(__FreeBSD__)
        {
            // On FreeBSD /home is a symlink to /usr/home.
            const auto info = file::symlink::status("/home").value();
            snn_require(info.is_symlink());
            snn_require(!info.is_directory());
            snn_require(info.type() == file::type::symlink);
        }
#endif

        {
            const auto info = file::symlink::status("/tmp").value();
            snn_require(info.is_directory());
            snn_require(!info.is_regular());
            snn_require(info.type() == file::type::directory);
        }

        {
            file::info info{container::do_not_initialize};
            snn_require(file::symlink::status("/tmp", info));
            snn_require(info.is_directory());
            snn_require(!info.is_regular());
            snn_require(info.type() == file::type::directory);
        }

        {
            snn_require(!file::symlink::status("/tmp/non_existing_389287329"));
            snn_require_throws_code(file::symlink::status("/tmp/non_existing_389287329").value(),
                                    make_error_code(ENOENT, system::error_category));
        }
    }
}
