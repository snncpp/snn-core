// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/path/looks_canonical.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        {
            static_assert(file::path::looks_canonical("/"));
            static_assert(file::path::looks_canonical("/home"));
            static_assert(file::path::looks_canonical("/home/"));
            static_assert(file::path::looks_canonical("/home/user"));
            static_assert(file::path::looks_canonical("/home/user/"));
            static_assert(file::path::looks_canonical("/home/user/file.txt"));

            static_assert(file::path::looks_canonical("/home/åäö"));
            static_assert(file::path::looks_canonical("/home/åäö/"));
            static_assert(file::path::looks_canonical("/home/one two three"));
            static_assert(file::path::looks_canonical("/home/one two three/"));

            static_assert(file::path::looks_canonical("/a"));
            static_assert(file::path::looks_canonical("/a/"));
            static_assert(file::path::looks_canonical("/a/b"));
            static_assert(file::path::looks_canonical("/a/b/"));
            static_assert(file::path::looks_canonical("/a/b/..."));
            static_assert(file::path::looks_canonical("/a/b/.c"));
            static_assert(file::path::looks_canonical("/a/b/c.d"));
            static_assert(file::path::looks_canonical("/a/.../b/"));

            static_assert(!file::path::looks_canonical(""));
            static_assert(!file::path::looks_canonical(" "));
            static_assert(!file::path::looks_canonical("a"));
            static_assert(!file::path::looks_canonical("a/"));
            static_assert(!file::path::looks_canonical("a/b"));
            static_assert(!file::path::looks_canonical("a/b/"));
            static_assert(!file::path::looks_canonical("."));
            static_assert(!file::path::looks_canonical("/."));
            static_assert(!file::path::looks_canonical("/./"));
            static_assert(!file::path::looks_canonical(".."));
            static_assert(!file::path::looks_canonical("/.."));
            static_assert(!file::path::looks_canonical("/../"));
            static_assert(!file::path::looks_canonical("/a/b/."));
            static_assert(!file::path::looks_canonical("/a/b/.."));
            static_assert(!file::path::looks_canonical("/a/./b/"));
            static_assert(!file::path::looks_canonical("/a/././"));
            static_assert(!file::path::looks_canonical("/a/././."));
            static_assert(!file::path::looks_canonical("/a/../b/"));
            static_assert(!file::path::looks_canonical("/a/../../"));
            static_assert(!file::path::looks_canonical("/a/../../.."));
            static_assert(!file::path::looks_canonical("//"));
            static_assert(!file::path::looks_canonical("//a"));
            static_assert(!file::path::looks_canonical("//a/"));
            static_assert(!file::path::looks_canonical("//a/b"));
            static_assert(!file::path::looks_canonical("/a//b/"));
            static_assert(!file::path::looks_canonical("/a///b/"));
            static_assert(!file::path::looks_canonical("/a/b//"));
            static_assert(!file::path::looks_canonical("/a//b//"));
            static_assert(!file::path::looks_canonical("\0"));
            static_assert(!file::path::looks_canonical("/\0"));
            static_assert(!file::path::looks_canonical("/\0/"));
            static_assert(!file::path::looks_canonical("/a\0"));
            static_assert(!file::path::looks_canonical("/a\0/"));
            static_assert(!file::path::looks_canonical("/a\0b"));
            static_assert(!file::path::looks_canonical("/a\0b/"));
            static_assert(!file::path::looks_canonical("/\0/."));

            // Test `PATH_MAX`.
            str path{"/"};
            snn_require(file::path::looks_canonical(path));
            path.append_uninitialized(99).fill('a');
            snn_require(file::path::looks_canonical(path));
            path.append_uninitialized(99'999).fill('a');
            snn_require(!file::path::looks_canonical(path));
        }
    }
}
