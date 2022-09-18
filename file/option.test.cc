// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/option.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        static_assert(std::is_same_v<std::underlying_type_t<file::option>, int>);

        static_assert(to_underlying(file::option::none) == 0);
        static_assert(to_underlying(file::option::append) == O_APPEND);
        static_assert(to_underlying(file::option::exclusive) == O_EXCL);
        static_assert(to_underlying(~file::option::exclusive) == ~O_EXCL);

        static_assert(to_underlying(file::option::read_only) == (O_RDONLY | O_CLOEXEC));
        static_assert(to_underlying(file::option::read_only & ~file::option::close_on_exec) ==
                      O_RDONLY);

        static_assert(to_underlying(file::option::none | file::option::exclusive) == O_EXCL);
        static_assert(to_underlying(file::option::append | file::option::exclusive) ==
                      (O_APPEND | O_EXCL));

        static_assert((file::option::append & file::option::exclusive) == file::option::none);
        static_assert(((file::option::append | file::option::exclusive) & file::option::append) ==
                      file::option::append);
        static_assert(((file::option::append | file::option::exclusive) &
                       file::option::exclusive) == file::option::exclusive);
    }
}
