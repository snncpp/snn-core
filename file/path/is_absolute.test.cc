// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/path/is_absolute.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        {
            static_assert(file::path::is_absolute("/"));
            static_assert(file::path::is_absolute("//"));
            static_assert(file::path::is_absolute("/a"));
            static_assert(file::path::is_absolute("/a/"));
            static_assert(file::path::is_absolute("/a/b"));
            static_assert(file::path::is_absolute("/a/b/"));

            static_assert(!file::path::is_absolute(""));
            static_assert(!file::path::is_absolute(" "));
            static_assert(!file::path::is_absolute("a"));
            static_assert(!file::path::is_absolute("a/"));
            static_assert(!file::path::is_absolute("a/b"));
            static_assert(!file::path::is_absolute("a/b/"));
        }
    }
}
