// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/unchecked/count.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/read.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(utf8::unchecked::count("") == 0);
            snn_require(utf8::unchecked::count("a") == 1);
            snn_require(utf8::unchecked::count("abc") == 3);
            snn_require(utf8::unchecked::count("abcdefghijklmnopqrstuvwxyz") == 26);
            snn_require(utf8::unchecked::count("åäö") == 3);
            snn_require(utf8::unchecked::count("abcåäö") == 6);
            snn_require(utf8::unchecked::count("����") == 4);
            snn_require(utf8::unchecked::count("ÅÄÖ\0åäö") == 7);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        static_assert(utf8::unchecked::count("Å\0äö\r\nabc\n\xF0\x9F\x8D\x83\x61") == 12);

        static_assert(utf8::unchecked::count("\x41") == 1);
        static_assert(utf8::unchecked::count("\xcc\x8a") == 1);
        static_assert(utf8::unchecked::count("\xe2\x82\xac") == 1);
        static_assert(utf8::unchecked::count("\xf0\x9f\x8d\x8a") == 1);

        // Invalid
        static_assert(utf8::unchecked::count("\xf0\x82\x82\xac") == 4);

        const auto stress_test = file::read<strbuf>("../test.data/stress-test.txt").value();
        snn_require(stress_test.size() == 22781);

        const auto stress_test_corrected =
            file::read<strbuf>("../test.data/stress-test-corrected.txt").value();
        snn_require(stress_test_corrected.size() == 23541);

        const auto stress_test_stripped =
            file::read<strbuf>("../test.data/stress-test-stripped.txt").value();
        snn_require(stress_test_stripped.size() == 22401);

        snn_require(utf8::unchecked::count(stress_test) == 22593);
        snn_require(utf8::unchecked::count(stress_test_corrected) == 22593);
        snn_require(utf8::unchecked::count(stress_test_stripped) == 22213);
    }
}
