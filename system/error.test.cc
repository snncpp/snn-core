// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/system/error.hh"

#include "snn-core/unittest.hh"
#include <cerrno>

namespace snn::app
{
    namespace
    {
        bool example()
        {
            snn_require(system::error_category.message<cstrview>(EPERM) ==
                        "Operation not permitted");

            return true;
        }

        bool test_error_category()
        {
            {
                constexpr error_code ec{EPERM, system::error_category};
                static_assert(ec.value() == 1);
                static_assert(ec.category() == system::error_category);
                snn_require(ec.message<cstrview>() == "Operation not permitted");
            }
            {
                constexpr error_code ec{-123, system::error_category};
                static_assert(ec.value() == -123);
                static_assert(ec.category() == system::error_category);
                snn_require(ec.message<cstrview>() == "");
            }
            {
                constexpr error_code ec{999'999'999, system::error_category};
                static_assert(ec.value() == 999'999'999);
                static_assert(ec.category() == system::error_category);
                snn_require(ec.message<cstrview>() == "");
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());
        snn_require(app::test_error_category());
    }
}
