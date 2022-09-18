// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/skip.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // All valid, entire range is skipped.
            snn_require(utf8::skip(cstrrng{"åäö"}).count() == 0);

            // Skips up to the invalid UTF-8.
            const cstrrng rng = utf8::skip(cstrrng{"abcåäö\xF0\x82\x82\xACxyz"});
            snn_require(rng.view() == "\xF0\x82\x82\xACxyz");

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        static_assert(utf8::skip(cstrrng{""}).count() == 0);
        static_assert(utf8::skip(cstrrng{"abc"}).count() == 0);
        static_assert(utf8::skip(cstrrng{"abc\n123"}).count() == 0);
        static_assert(utf8::skip(cstrrng{"abcdefghijklmnopqrstuvwxyz"}).count() == 0);
        static_assert(utf8::skip(cstrrng{"åäö"}).count() == 0);
        static_assert(utf8::skip(cstrrng{"abcdefghijklmnopqrstuvwxyzåäö"}).count() == 0);
        static_assert(utf8::skip(cstrrng{"abcådefghijklmnopqrstuvwxyz"}).count() == 0);

        static_assert(utf8::skip(cstrrng{"abc\x80"}).count() == 1);
        static_assert(utf8::skip(cstrrng{"abc\xd6_23"}).count() == 4);
        static_assert(utf8::skip(cstrrng{"\xf0\x82\x82\xac"}).count() == 4);
        static_assert(utf8::skip(cstrrng{"abcåäö\xf0\x82\x82\xac"}).count() == 4);
    }
}
