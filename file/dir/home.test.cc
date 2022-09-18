// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/dir/home.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        {
            const auto path = file::dir::home<str>().value();
            snn_require(path.has_front('/'));
            snn_require(path.contains("/home/"));
            snn_require(path.size() > string_size("/home/"));
        }
        {
            const auto path = file::dir::home<cstrview>().value();
            snn_require(path.has_front('/'));
            snn_require(path.contains("/home/"));
            snn_require(path.size() > string_size("/home/"));
        }
        {
            const auto path = file::dir::home<null_term<const char*>>().value();
            snn_require(path.to<cstrview>().contains("/home/"));
        }

        static_assert(noexcept(file::dir::home<cstrview>()));
        static_assert(noexcept(file::dir::home<null_term<const char*>>()));
        static_assert(!noexcept(file::dir::home<str>()));
    }
}
