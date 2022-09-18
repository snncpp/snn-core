// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/ascii/lower_wrapper.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // {
            //     // This will not compile (in debug mode):
            //     constexpr auto lw = ascii::as_lower("ABC");
            // }
            {
                auto lw = ascii::as_lower("");
                static_assert(std::is_same_v<decltype(lw), ascii::lower_wrapper<cstrview>>);
                snn_require(lw.get().size() == 0);
                snn_require(lw.get() == "");
            }
            {
                auto lw = ascii::as_lower("abcdefghijklmnopqrstuvwxyz");
                static_assert(std::is_same_v<decltype(lw), ascii::lower_wrapper<cstrview>>);
                snn_require(lw.get().size() == 26);
                snn_require(lw.get() == "abcdefghijklmnopqrstuvwxyz");
            }
            {
                // Only ASCII bytes are required to be lowercase.
                auto lw = ascii::as_lower("abcÅÄÖ");
                static_assert(std::is_same_v<decltype(lw), ascii::lower_wrapper<cstrview>>);
                snn_require(lw.get().size() == 9);
                snn_require(lw.get() == "abcÅÄÖ");
            }
            {
                cstrview s{"abc"};
                auto lw = ascii::as_lower(s);
                static_assert(std::is_same_v<decltype(lw), ascii::lower_wrapper<cstrview>>);
                snn_require(lw.get().size() == 3);
                snn_require(lw.get() == "abc");
            }
            {
                str s{"abc"};
                auto lw = ascii::as_lower<cstrview>(s);
                static_assert(std::is_same_v<decltype(lw), ascii::lower_wrapper<cstrview>>);
                snn_require(lw.get().size() == 3);
                snn_require(lw.get() == "abc");
            }
            {
                str s{"abc"};
                auto lw = ascii::as_lower(s.view());
                static_assert(std::is_same_v<decltype(lw), ascii::lower_wrapper<strview>>);
                snn_require(lw.get().size() == 3);
                snn_require(lw.get() == "abc");
            }

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
