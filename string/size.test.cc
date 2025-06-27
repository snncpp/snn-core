// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/string/size.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(string::size(cstrview{""}) == 0);
            snn_require(string::size(cstrview{"a"}) == 1);
            snn_require(string::size(cstrview{"abc"}) == 3);
            snn_require(string::size(cstrview{"a\0c"}) == 3);

            snn_require(string::size(array_view{""}) == 0);
            snn_require(string::size(array_view{"a"}) == 1);
            snn_require(string::size(array_view{"abc"}) == 3);
            snn_require(string::size(array_view{"a\0c"}) == 3);

            snn_require(string::size(str{""}) == 0);
            snn_require(string::size(str{"a"}) == 1);
            snn_require(string::size(str{"abc"}) == 3);
            snn_require(string::size(str{"a\0c"}) == 3);

            snn_require(string::size('a') == 1);
            snn_require(string::size('\0') == 1);

            snn_require(string::size(nullptr, assume::null_terminated) == 0);
            snn_require(string::size("", assume::null_terminated) == 0);
            snn_require(string::size("a", assume::null_terminated) == 1);
            snn_require(string::size("ab", assume::null_terminated) == 2);
            snn_require(string::size("abc", assume::null_terminated) == 3);
            snn_require(string::size("Å", assume::null_terminated) == 2);
            snn_require(string::size("ÅÄÖ", assume::null_terminated) == 6);
            snn_require(string::size("abc\0def", assume::null_terminated) == 3);
            snn_require(string::size("\0", assume::null_terminated) == 0);
            snn_require(string::size("\0\0\0", assume::null_terminated) == 0);

            snn_require(string::size(not_null{""}, assume::null_terminated) == 0);
            snn_require(string::size(not_null{"a"}, assume::null_terminated) == 1);
            snn_require(string::size(not_null{"ab"}, assume::null_terminated) == 2);
            snn_require(string::size(not_null{"abc"}, assume::null_terminated) == 3);
            snn_require(string::size(not_null{"Å"}, assume::null_terminated) == 2);
            snn_require(string::size(not_null{"ÅÄÖ"}, assume::null_terminated) == 6);
            snn_require(string::size(not_null{"abc\0def"}, assume::null_terminated) == 3);
            snn_require(string::size(not_null{"\0"}, assume::null_terminated) == 0);
            snn_require(string::size(not_null{"\0\0\0"}, assume::null_terminated) == 0);

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
