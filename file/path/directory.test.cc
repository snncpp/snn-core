// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/path/directory.hh"

#include "snn-core/unittest.hh"
#include "snn-core/system/error.hh"
#include <cerrno>  // ENAMETOOLONG
#include <climits> // PATH_MAX

namespace snn
{
    void unittest()
    {
        static_assert(file::path::directory<cstrview>("").value() == "");
        static_assert(file::path::directory<cstrview>(".").value() == "");
        static_assert(file::path::directory<cstrview>("..").value() == "");
        static_assert(file::path::directory<cstrview>("...").value() == "");
        static_assert(file::path::directory<cstrview>("/").value() == "/");
        static_assert(file::path::directory<cstrview>("/.").value() == "/");
        static_assert(file::path::directory<cstrview>("/..").value() == "/");
        static_assert(file::path::directory<cstrview>(".e").value() == "");
        static_assert(file::path::directory<cstrview>("b.e").value() == "");
        static_assert(file::path::directory<cstrview>("p/.e").value() == "p/");
        static_assert(file::path::directory<cstrview>("p/b.e").value() == "p/");
        static_assert(file::path::directory<cstrview>("p/b.eee").value() == "p/");
        static_assert(file::path::directory<cstrview>("p/bbb.eee").value() == "p/");
        static_assert(
            file::path::directory<cstrview>("abcdefghijklm/nopqrstuvwxyz/bbb.eee").value() ==
            "abcdefghijklm/nopqrstuvwxyz/");

        snn_require(file::path::directory("").value() == "");
        snn_require(file::path::directory(".").value() == "");
        snn_require(file::path::directory("..").value() == "");
        snn_require(file::path::directory("...").value() == "");
        snn_require(file::path::directory("/").value() == "/");
        snn_require(file::path::directory("/.").value() == "/");
        snn_require(file::path::directory("/..").value() == "/");
        snn_require(file::path::directory(".e").value() == "");
        snn_require(file::path::directory("b.e").value() == "");
        snn_require(file::path::directory("p/.e").value() == "p/");
        snn_require(file::path::directory("p/b.e").value() == "p/");
        snn_require(file::path::directory("p/b.eee").value() == "p/");
        snn_require(file::path::directory("p/bbb.eee").value() == "p/");
        snn_require(file::path::directory("abcdefghijklm/nopqrstuvwxyz/bbb.eee").value() ==
                    "abcdefghijklm/nopqrstuvwxyz/");

        const str invalid{container::fill, PATH_MAX, 'a'};
        auto res = file::path::directory(invalid);
        static_assert(std::is_same_v<decltype(res), result<str>>);
        snn_require(!res);
        snn_require(res.error_code() == make_error_code(ENAMETOOLONG, system::error_category));
    }
}
