// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fmt/type.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 10)
            snn_require(fmt::type_name<i32>() == "int");
            snn_require(fmt::type_name<str>() == "snn::strcore<snn::detail::strcore::sso>");

            snn_require(fmt::type<i32>() == "int");

            str append_to = "cstrview == ";
            fmt::type<cstrview>(append_to);
            snn_require(append_to == "cstrview == snn::array_view<const char>");
#endif

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
    }
}
