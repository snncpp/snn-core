// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/path/base.hh"

#include "snn-core/unittest.hh"
#include "snn-core/system/error.hh"
#include <cerrno>  // ENAMETOOLONG
#include <climits> // PATH_MAX

namespace snn
{
    void unittest()
    {
        static_assert(file::path::base<cstrview>("").value() == "");
        static_assert(file::path::base<cstrview>(".").value() == ".");
        static_assert(file::path::base<cstrview>("..").value() == ".");
        static_assert(file::path::base<cstrview>("...").value() == "..");
        static_assert(file::path::base<cstrview>("/").value() == "");
        static_assert(file::path::base<cstrview>("/.").value() == ".");
        static_assert(file::path::base<cstrview>("/..").value() == ".");
        static_assert(file::path::base<cstrview>(".e").value() == ".e");
        static_assert(file::path::base<cstrview>("b.e").value() == "b");
        static_assert(file::path::base<cstrview>("p/.e").value() == ".e");
        static_assert(file::path::base<cstrview>("p/b.e").value() == "b");
        static_assert(file::path::base<cstrview>("p/b.eee").value() == "b");
        static_assert(file::path::base<cstrview>("p/bbb.eee").value() == "bbb");
        static_assert(file::path::base<cstrview>("ppp/abcdefghijklmnopqrstuvwxyz.eee").value() ==
                      "abcdefghijklmnopqrstuvwxyz");

        snn_require(file::path::base("").value() == "");
        snn_require(file::path::base(".").value() == ".");
        snn_require(file::path::base("..").value() == ".");
        snn_require(file::path::base("...").value() == "..");
        snn_require(file::path::base("/").value() == "");
        snn_require(file::path::base("/.").value() == ".");
        snn_require(file::path::base("/..").value() == ".");
        snn_require(file::path::base(".e").value() == ".e");
        snn_require(file::path::base("b.e").value() == "b");
        snn_require(file::path::base("p/.e").value() == ".e");
        snn_require(file::path::base("p/b.e").value() == "b");
        snn_require(file::path::base("p/b.eee").value() == "b");
        snn_require(file::path::base("p/bbb.eee").value() == "bbb");
        snn_require(file::path::base("ppp/abcdefghijklmnopqrstuvwxyz.eee").value() ==
                    "abcdefghijklmnopqrstuvwxyz");

        const str invalid{container::fill, PATH_MAX, 'a'};
        auto res = file::path::base(invalid);
        static_assert(std::is_same_v<decltype(res), result<str>>);
        snn_require(!res);
        snn_require(res.error_code() == make_error_code(ENAMETOOLONG, system::error_category));
    }
}
