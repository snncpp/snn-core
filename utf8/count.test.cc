// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/count.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/read.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(utf8::count("").value() == 0);
            snn_require(utf8::count("a").value() == 1);
            snn_require(utf8::count("abc").value() == 3);
            snn_require(utf8::count("abcdefghijklmnopqrstuvwxyz").value() == 26);
            snn_require(utf8::count("åäö").value() == 3);
            snn_require(utf8::count("abcåäö").value() == 6);
            snn_require(utf8::count("����").value() == 4);
            snn_require(utf8::count("ÅÄÖ\0åäö").value() == 7);
            snn_require(utf8::count("Å\0äö\r\nabc\n\U0001F419a").value() == 12);

            // Invalid UTF-8.
            snn_require(!utf8::count("\xE5"));
            snn_require_throws_code(utf8::count("\xE5").value(),
                                    make_error_code(utf8::error::invalid));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        static_assert(utf8::count("\x41").value() == 1);
        static_assert(utf8::count("\xcc\x8a").value() == 1);
        static_assert(utf8::count("\xe2\x82\xac").value() == 1);
        static_assert(utf8::count("\xf0\x9f\x8d\x8a").value() == 1);

        snn_require_throws_code(utf8::count("\xf0\x82\x82\xac").value(),
                                make_error_code(utf8::error::invalid));

        const auto stress_test = file::read<strbuf>("test.data/stress-test.txt").value();
        snn_require(stress_test.size() == 22781);

        const auto stress_test_corrected =
            file::read<strbuf>("test.data/stress-test-corrected.txt").value();
        snn_require(stress_test_corrected.size() == 23541);

        const auto stress_test_stripped =
            file::read<strbuf>("test.data/stress-test-stripped.txt").value();
        snn_require(stress_test_stripped.size() == 22401);

        snn_require_throws_code(utf8::count(stress_test).value(), utf8::error::invalid);
        snn_require(utf8::count(stress_test_corrected).value() == 22593);
        snn_require(utf8::count(stress_test_stripped).value() == 22213);
    }
}
