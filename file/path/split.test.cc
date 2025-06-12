// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/path/split.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool splits_to(const cstrview path, const cstrview dir, const cstrview base,
                                 const cstrview ext)
        {
            const auto [parts, err] = file::path::split<cstrview>(path).unpack();
            if (!err)
            {
                if (parts.directory() == dir && parts.base() == base && parts.extension() == ext)
                {
                    return parts.combined() == path;
                }
            }
            return false;
        }
    }
}

namespace snn
{
    void unittest()
    {
        static_assert(file::path::split<cstrview>("ab/cd.ef").value().directory() == "ab/");
        static_assert(file::path::split<cstrview>("ab/cd.ef").value().base() == "cd");
        static_assert(file::path::split<cstrview>("ab/cd.ef").value().extension() == ".ef");

        // clang-format off
        //                           Path,                   Directory,      Base,       Extension
        static_assert(app::splits_to("",                     "",             "",         ""));
        static_assert(app::splits_to(".",                    "",             ".",        ""));
        static_assert(app::splits_to("..",                   "",             ".",        "."));
        static_assert(app::splits_to("...",                  "",             "..",       "."));
        static_assert(app::splits_to("a",                    "",             "a",        ""));
        static_assert(app::splits_to(".a",                   "",             ".a",       ""));
        static_assert(app::splits_to("a.",                   "",             "a",        "."));
        static_assert(app::splits_to("a.b",                  "",             "a",        ".b"));
        static_assert(app::splits_to("/",                    "/",            "",         ""));
        static_assert(app::splits_to("/.",                   "/",            ".",        ""));
        static_assert(app::splits_to("/..",                  "/",            ".",        "."));
        static_assert(app::splits_to("/.a",                  "/",            ".a",       ""));
        static_assert(app::splits_to("/.a.",                 "/",            ".a",       "."));
        static_assert(app::splits_to("/.a.b",                "/",            ".a",       ".b"));
        static_assert(app::splits_to("/a.b",                 "/",            "a",        ".b"));
        static_assert(app::splits_to("a/b/c",                "a/b/",         "c",        ""));
        static_assert(app::splits_to("a/b/.c",               "a/b/",         ".c",       ""));
        static_assert(app::splits_to("one",                  "",             "one",      ""));
        static_assert(app::splits_to("one/two/three.txt",    "one/two/",     "three",    ".txt"));
        static_assert(app::splits_to("/three.txt",           "/",            "three",    ".txt"));
        static_assert(app::splits_to("../../three.txt",      "../../",       "three",    ".txt"));
        static_assert(app::splits_to("å/ä/ö.åä",             "å/ä/",         "ö",        ".åä"));
        // clang-format on

        static_assert(app::splits_to("å/ä/ö.åä", "\xc3\xa5/\xc3\xa4/", "\xc3\xb6",
                                     ".\xc3\xa5\xc3\xa4"));

        static_assert(!app::splits_to("/a.b", "/", "a.", ".b"));

        const str invalid{init::fill, PATH_MAX, 'a'};
        snn_require(!file::path::split<cstrview>(invalid));
        const str valid{init::fill, PATH_MAX - 1, 'a'};
        snn_require(file::path::split<cstrview>(valid));

        snn_require(file::path::split<cstrview>("/one/two/three"));
        snn_require(!file::path::split<cstrview>("/one/two\0three"));
        snn_require(!file::path::split<cstrview>("/one/two/three\0"));

        snn_require_throws_code(file::path::split<cstrview>("abc\0").value(),
                                generic::error::unexpected_null_character);

        static_assert(sizeof(file::path::parts<cstrview>) == 24);
        static_assert(sizeof(file::path::parts<str>) == 32);

        static_assert(is_trivially_relocatable_v<file::path::parts<cstrview>>);
        static_assert(is_trivially_relocatable_v<file::path::parts<str>>);

        static_assert(std::tuple_size_v<file::path::parts<cstrview>> == 3);
        static_assert(std::tuple_size_v<file::path::parts<str>> == 3);

        static_assert(
            std::is_same_v<std::tuple_element_t<0, file::path::parts<cstrview>>, cstrview>);
        static_assert(
            std::is_same_v<std::tuple_element_t<1, file::path::parts<cstrview>>, cstrview>);
        static_assert(
            std::is_same_v<std::tuple_element_t<2, file::path::parts<cstrview>>, cstrview>);
        static_assert(std::is_same_v<std::tuple_element_t<0, file::path::parts<str>>, cstrview>);
        static_assert(std::is_same_v<std::tuple_element_t<1, file::path::parts<str>>, cstrview>);
        static_assert(std::is_same_v<std::tuple_element_t<2, file::path::parts<str>>, cstrview>);

        // Structured binding.
        {
            const auto [dir, base, ext] = file::path::split<cstrview>("ab/cd.ef").value();
            snn_require(dir == "ab/");
            snn_require(base == "cd");
            snn_require(ext == ".ef");
        }

        // get<>()
        {
            constexpr auto parts = file::path::split<cstrview>("ab/cd.ef").value();
            static_assert(std::is_same_v<decltype(parts), const file::path::parts<cstrview>>);
            static_assert(parts.directory() == "ab/");
            static_assert(parts.base() == "cd");
            static_assert(parts.extension() == ".ef");
            static_assert(parts.get<0>() == "ab/");
            static_assert(parts.get<1>() == "cd");
            static_assert(parts.get<2>() == ".ef");
            static_assert(get<0>(parts) == "ab/");
            static_assert(get<1>(parts) == "cd");
            static_assert(get<2>(parts) == ".ef");
        }

        // Allocation
        {
            auto parts = file::path::split("a/longer/path/goes/on.the.heap").value();
            static_assert(std::is_same_v<decltype(parts), file::path::parts<str>>);
            snn_require(parts.directory() == "a/longer/path/goes/");
            snn_require(parts.base() == "on.the");
            snn_require(parts.extension() == ".heap");
            snn_require(parts.combined() == "a/longer/path/goes/on.the.heap");

            file::path::parts<str> other{std::move(parts)};
            snn_require(other.directory() == "a/longer/path/goes/");
            snn_require(other.base() == "on.the");
            snn_require(other.extension() == ".heap");
            snn_require(other.combined() == "a/longer/path/goes/on.the.heap");

            snn_require(parts.directory() == "");
            snn_require(parts.base() == "");
            snn_require(parts.extension() == "");
            snn_require(parts.combined() == "");
        }
    }
}
