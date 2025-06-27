// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/path/append.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool test_append()
        {
            str path;

            file::path::append("", path);
            snn_require(path == "");

            file::path::append('a', path);
            snn_require(path == "a");

            path.clear();

            file::path::append("abc", path);
            snn_require(path == "abc");

            path.clear();

            file::path::append(as_num(123), path);
            snn_require(path == "123");

            path.clear();

            file::path::append(cstrview{"abc"}, path, assume::no_overlap);
            snn_require(path == "abc");

            path.clear();

            file::path::append('a', path);
            snn_require(path == "a");

            file::path::append("abc", path);
            snn_require(path == "a/abc");

            file::path::append(as_num(123), path);
            snn_require(path == "a/abc/123");

            file::path::append(cstrview{"def"}, path, assume::no_overlap);
            snn_require(path == "a/abc/123/def");

            file::path::append("", path);
            snn_require(path == "a/abc/123/def/");

            file::path::append("", path);
            snn_require(path == "a/abc/123/def/");

            file::path::append("ghi", path);
            snn_require(path == "a/abc/123/def/ghi");

            file::path::append('/', path);
            snn_require(path == "/");

            file::path::append("/abc", path);
            snn_require(path == "/abc");

            file::path::append(str{"/def"}, path, assume::no_overlap);
            snn_require(path == "/def");

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::test_append());
    }
}
