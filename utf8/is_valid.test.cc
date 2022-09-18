// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/is_valid.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/read.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(utf8::is_valid(""));
            snn_require(utf8::is_valid("a"));
            snn_require(utf8::is_valid("123"));
            snn_require(utf8::is_valid("abcdefghijklmnopqrstuvwxyz"));
            snn_require(utf8::is_valid("Å"));
            snn_require(utf8::is_valid("åäö"));
            snn_require(utf8::is_valid("ÅÄÖ\0åäö"));

            snn_require(!utf8::is_valid("\xE5"));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        static_assert(utf8::is_valid("\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd"));
        static_assert(utf8::is_valid("\xEF\xBF\xBD\uFFFD"));
        static_assert(utf8::is_valid("\xcc\x8a"));
        static_assert(utf8::is_valid("\xe2\x82\xac"));
        static_assert(utf8::is_valid("\xf0\x9f\x8d\x8a"));
        static_assert(!utf8::is_valid("\xd6"));
        static_assert(!utf8::is_valid("\xd6\xc3\x85\xd6"));
        static_assert(!utf8::is_valid("\xf0\x82\x82\xac")); // Overlong
        static_assert(!utf8::is_valid("\xcc"));             // Short
        static_assert(!utf8::is_valid("\xe2\x82"));         // Short
        static_assert(!utf8::is_valid("\xe2"));             // Short
        static_assert(!utf8::is_valid("\xf0\x9f\x8d"));     // Short
        static_assert(!utf8::is_valid("\xf0\x9f"));         // Short
        static_assert(!utf8::is_valid("\xf0"));             // Short

        const auto stress_test = file::read<strbuf>("test.data/stress-test.txt").value();
        snn_require(stress_test.size() == 22781);

        const auto stress_test_corrected =
            file::read<strbuf>("test.data/stress-test-corrected.txt").value();
        snn_require(stress_test_corrected.size() == 23541);

        const auto stress_test_stripped =
            file::read<strbuf>("test.data/stress-test-stripped.txt").value();
        snn_require(stress_test_stripped.size() == 22401);

        snn_require(!utf8::is_valid(stress_test));
        snn_require(utf8::is_valid(stress_test_corrected));
        snn_require(utf8::is_valid(stress_test_stripped));
    }
}
