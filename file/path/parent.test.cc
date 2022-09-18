// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/path/parent.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        static_assert(file::path::parent<cstrview>("") == "");

        static_assert(file::path::parent<cstrview>("/") == "/");
        static_assert(file::path::parent<cstrview>("//") == "//");
        static_assert(file::path::parent<cstrview>("///") == "///");

        static_assert(file::path::parent<cstrview>("/home") == "/");
        static_assert(file::path::parent<cstrview>("/home/") == "/");
        static_assert(file::path::parent<cstrview>("/home/user") == "/home");
        static_assert(file::path::parent<cstrview>("/home/user/") == "/home");
        static_assert(file::path::parent<cstrview>("/home/user/file") == "/home/user");
        static_assert(file::path::parent<cstrview>("/home/user/file.txt") == "/home/user");
        static_assert(file::path::parent<cstrview>("/home/user/dir") == "/home/user");
        static_assert(file::path::parent<cstrview>("/home/user/dir/") == "/home/user");

        static_assert(file::path::parent<cstrview>("//home") == "//");
        static_assert(file::path::parent<cstrview>("//home//") == "//");
        static_assert(file::path::parent<cstrview>("///home//user//file.txt") == "///home//user");

        static_assert(file::path::parent<cstrview>("home") == "");
        static_assert(file::path::parent<cstrview>("home/") == "");
        static_assert(file::path::parent<cstrview>("home/user") == "home");
        static_assert(file::path::parent<cstrview>("home/user/") == "home");
        static_assert(file::path::parent<cstrview>("home/user/file") == "home/user");
        static_assert(file::path::parent<cstrview>("home/user/file.txt") == "home/user");
        static_assert(file::path::parent<cstrview>("home/user/subdir") == "home/user");
        static_assert(file::path::parent<cstrview>("home/user/subdir/") == "home/user");

        static_assert(file::path::parent<cstrview>("home//") == "");
        static_assert(file::path::parent<cstrview>("home//user//subdir//") == "home//user");

        // Dots have no special meaning.
        static_assert(file::path::parent<cstrview>("home/user/.") == "home/user");
        static_assert(file::path::parent<cstrview>("home/user/./.") == "home/user/.");
        static_assert(file::path::parent<cstrview>("home/user/..") == "home/user");
        static_assert(file::path::parent<cstrview>("home/user/../..") == "home/user/..");
        static_assert(file::path::parent<cstrview>("../../..") == "../..");
        static_assert(file::path::parent<cstrview>("../../../") == "../..");
        static_assert(file::path::parent<cstrview>("../..") == "..");
        static_assert(file::path::parent<cstrview>("../../") == "..");

        static_assert(std::is_same_v<decltype(file::path::parent("")), str>);
        static_assert(std::is_same_v<decltype(file::path::parent<str>("")), str>);
        static_assert(std::is_same_v<decltype(file::path::parent<cstrview>("")), cstrview>);

        static_assert(!noexcept(file::path::parent("")));
        static_assert(!noexcept(file::path::parent<str>("")));
        static_assert(noexcept(file::path::parent<cstrview>("")));
    }
}
