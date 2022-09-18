// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/strip_invalid.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/read.hh"
#include "snn-core/utf8/is_valid.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // Valid UTF-8 (nothing to strip).
            snn_require(utf8::strip_invalid("") == "");
            snn_require(utf8::strip_invalid("a") == "a");
            snn_require(utf8::strip_invalid("Å") == "Å");
            snn_require(utf8::strip_invalid("Å\0äö\r\nabc\na") == "Å\0äö\r\nabc\na");

            // Invalid
            snn_require(utf8::strip_invalid("\xD6") == "");

            // Invalid (overlong encoding).
            snn_require(utf8::strip_invalid("\xF0\x82\x82\xAC") == "");

            // Mixed
            snn_require(utf8::strip_invalid("123\xD6Å\xF0\x82\x82\xACÄÖ") == "123ÅÄÖ");

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        const auto stress_test = file::read<strbuf>("test.data/stress-test.txt").value();
        snn_require(stress_test.size() == 22781);

        const auto stress_test_corrected =
            file::read<strbuf>("test.data/stress-test-corrected.txt").value();
        snn_require(stress_test_corrected.size() == 23541);

        const auto stress_test_stripped =
            file::read<strbuf>("test.data/stress-test-stripped.txt").value();
        snn_require(stress_test_stripped.size() == 22401);

        snn_require(utf8::strip_invalid<strbuf>(stress_test) == stress_test_stripped);
        snn_require(utf8::strip_invalid<strbuf>(stress_test_corrected) == stress_test_corrected);
        snn_require(utf8::strip_invalid<strbuf>(stress_test_stripped) == stress_test_stripped);
    }
}
