// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/path/extension.hh"

#include "snn-core/unittest.hh"
#include "snn-core/system/error.hh"
#include <cerrno>  // ENAMETOOLONG
#include <climits> // PATH_MAX

namespace snn
{
    void unittest()
    {
        static_assert(file::path::extension<cstrview>("").value() == "");
        static_assert(file::path::extension<cstrview>(".").value() == "");
        static_assert(file::path::extension<cstrview>("..").value() == ".");
        static_assert(file::path::extension<cstrview>("...").value() == ".");
        static_assert(file::path::extension<cstrview>("/").value() == "");
        static_assert(file::path::extension<cstrview>("/.").value() == "");
        static_assert(file::path::extension<cstrview>("/..").value() == ".");
        static_assert(file::path::extension<cstrview>(".e").value() == "");
        static_assert(file::path::extension<cstrview>("b.e").value() == ".e");
        static_assert(file::path::extension<cstrview>("p/.e").value() == "");
        static_assert(file::path::extension<cstrview>("p/b.e").value() == ".e");
        static_assert(file::path::extension<cstrview>("p/b.eee").value() == ".eee");
        static_assert(file::path::extension<cstrview>("p/bbb.eee").value() == ".eee");
        static_assert(
            file::path::extension<cstrview>("ppp/bbb.abcdefghijklmnopqrstuvwxyz").value() ==
            ".abcdefghijklmnopqrstuvwxyz");

        snn_require(file::path::extension("").value() == "");
        snn_require(file::path::extension(".").value() == "");
        snn_require(file::path::extension("..").value() == ".");
        snn_require(file::path::extension("...").value() == ".");
        snn_require(file::path::extension("/").value() == "");
        snn_require(file::path::extension("/.").value() == "");
        snn_require(file::path::extension("/..").value() == ".");
        snn_require(file::path::extension(".e").value() == "");
        snn_require(file::path::extension("b.e").value() == ".e");
        snn_require(file::path::extension("p/.e").value() == "");
        snn_require(file::path::extension("p/b.e").value() == ".e");
        snn_require(file::path::extension("p/b.eee").value() == ".eee");
        snn_require(file::path::extension("p/bbb.eee").value() == ".eee");
        snn_require(file::path::extension("ppp/bbb.abcdefghijklmnopqrstuvwxyz").value() ==
                    ".abcdefghijklmnopqrstuvwxyz");

        const str invalid{container::fill, PATH_MAX, 'a'};
        auto res = file::path::extension(invalid);
        static_assert(std::is_same_v<decltype(res), result<str>>);
        snn_require(!res);
        snn_require(res.error_code() == make_error_code(ENAMETOOLONG, system::error_category));
    }
}
