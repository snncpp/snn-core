// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/path/join.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool test_join()
        {
            snn_require(file::path::join() == "");
            snn_require(file::path::join("") == "");
            snn_require(file::path::join("", "") == "");

            snn_require(file::path::join("one", str{"two"}, "three") == "one/two/three");
            snn_require(file::path::join("/one", "two", cstrview{"three"}) == "/one/two/three");
            snn_require(file::path::join("/one", "/two", "three") == "/two/three");
            snn_require(file::path::join("/one", "/two", "/three") == "/three");

            str def = "def";
            snn_require(file::path::join("abc", def) == "abc/def");
            snn_require(file::path::join("/abc", def) == "/abc/def");
            snn_require(file::path::join("/abc/", def) == "/abc/def");

            str slash_def = "/def";
            snn_require(file::path::join("/abc", slash_def) == "/def");
            snn_require(file::path::join("/abc/", slash_def) == "/def");

            snn_require(file::path::join("abc", as_num(123)) == "abc/123");
            snn_require(file::path::join("abc/", as_num(123)) == "abc/123");
            snn_require(file::path::join("/abc", as_num(123)) == "/abc/123");
            snn_require(file::path::join("/abc/", as_num(123)) == "/abc/123");

            snn_require(file::path::join("abc", as_num(123), "def") == "abc/123/def");
            snn_require(file::path::join("abc/", as_num(123), "def") == "abc/123/def");
            snn_require(file::path::join("/abc", as_num(123), "def") == "/abc/123/def");
            snn_require(file::path::join("/abc/", as_num(123), "def") == "/abc/123/def");

            snn_require(file::path::join("abc", as_num(123), "/def") == "/def");
            snn_require(file::path::join("abc/", as_num(123), "/def") == "/def");
            snn_require(file::path::join("/abc", as_num(123), "/def") == "/def");
            snn_require(file::path::join("/abc/", as_num(123), "/def") == "/def");

            snn_require(file::path::join("abc", "", "def") == "abc/def");
            snn_require(file::path::join("abc/", "", "def") == "abc/def");
            snn_require(file::path::join("/abc", "", "def") == "/abc/def");
            snn_require(file::path::join("/abc/", "", "def") == "/abc/def");

            snn_require(file::path::join("//abc", "def") == "//abc/def");
            snn_require(file::path::join("//abc/", "def") == "//abc/def");
            snn_require(file::path::join("//abc//", "def") == "//abc//def");

            snn_require(file::path::join("abc", "") == "abc/");
            snn_require(file::path::join("abc", '/') == "/");
            snn_require(file::path::join("abc", 'd') == "abc/d");

            snn_require(file::path::join("abc", "", "") == "abc/");
            snn_require(file::path::join("abc", '/', "") == "/");
            snn_require(file::path::join("abc", 'd', "") == "abc/d/");

            snn_require(file::path::join("abc", "", "xyz") == "abc/xyz");
            snn_require(file::path::join("abc", '/', "xyz") == "/xyz");
            snn_require(file::path::join("abc", '\\', "xyz") == "abc/\\/xyz");
            snn_require(file::path::join("abc", 'd', "xyz") == "abc/d/xyz");

            snn_require(file::path::join("", "") == "");
            snn_require(file::path::join("", 'a') == "a");
            snn_require(file::path::join("", '/') == "/");
            snn_require(file::path::join("", "a") == "a");
            snn_require(file::path::join("", "/") == "/");
            snn_require(file::path::join("", "abc") == "abc");
            snn_require(file::path::join("", "/abc") == "/abc");
            snn_require(file::path::join("", as_num(123)) == "123");

            snn_require(file::path::join("x", "") == "x/");
            snn_require(file::path::join("x", 'a') == "x/a");
            snn_require(file::path::join("x", '/') == "/");
            snn_require(file::path::join("x", "a") == "x/a");
            snn_require(file::path::join("x", "/") == "/");
            snn_require(file::path::join("x", "abc") == "x/abc");
            snn_require(file::path::join("x", "/abc") == "/abc");
            snn_require(file::path::join("x", as_num(123)) == "x/123");

            snn_require(file::path::join("/", "") == "/");
            snn_require(file::path::join("/", 'a') == "/a");
            snn_require(file::path::join("/", '/') == "/");
            snn_require(file::path::join("/", "a") == "/a");
            snn_require(file::path::join("/", "/") == "/");
            snn_require(file::path::join("/", "abc") == "/abc");
            snn_require(file::path::join("/", "/abc") == "/abc");
            snn_require(file::path::join("/", as_num(123)) == "/123");

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::test_join());
    }
}
