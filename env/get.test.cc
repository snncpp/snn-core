// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/env/get.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            snn_require(env::get<cstrview>("HOME").value().has_front("/home/"));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        snn_require(env::get<cstrview>("HOME"));
        snn_require(!env::get<cstrview>("THIS_SHOULD_NOT_EXIST_382932"));

        const auto pwd = env::get<cstrview>("PWD");
        snn_require(pwd);
        snn_require(pwd.value().contains('/'));

        const auto home = env::get<str>("HOME");
        snn_require(home);
        snn_require(home.value().contains("/home/"));

        const auto path = env::get<null_term<const char*>>("PATH");
        snn_require(path);
        snn_require(path.value().to<cstrview>().contains(':'));

        static_assert(std::is_nothrow_constructible_v<cstrview, not_null<const char*>,
                                                      assume::null_terminated_t>);
        static_assert(!std::is_nothrow_constructible_v<str, not_null<const char*>,
                                                       assume::null_terminated_t>);
        static_assert(std::is_nothrow_constructible_v<null_term<const char*>, not_null<const char*>,
                                                      assume::null_terminated_t>);
    }
}
