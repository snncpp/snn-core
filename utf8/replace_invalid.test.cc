// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/replace_invalid.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/read.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                // Valid UTF-8 (nothing to replace).
                snn_require(utf8::replace_invalid("") == "");
                snn_require(utf8::replace_invalid("a") == "a");
                snn_require(utf8::replace_invalid("aåäö") == "aåäö");
            }
            {
                // Mixed
                const str s = utf8::replace_invalid("\xD6Å\xD6");
                snn_require(s == "\xEF\xBF\xBD\xC3\x85\xEF\xBF\xBD");
                snn_require(s == "�Å�");
            }
            {
                // Invalid
                const str s = utf8::replace_invalid("\xD6");
                snn_require(s == "\xEF\xBF\xBD");
                snn_require(s == "�");
            }
            {
                // Invalid (overlong encoding).
                const str s = utf8::replace_invalid("\xF0\x82\x82\xAC");
                snn_require(s == "\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD\xEF\xBF\xBD");
                snn_require(s == "����");
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

        const auto stress_test = file::read<strbuf>("test.data/stress-test.txt").value();
        snn_require(stress_test.size() == 22781);

        const auto stress_test_corrected =
            file::read<strbuf>("test.data/stress-test-corrected.txt").value();
        snn_require(stress_test_corrected.size() == 23541);

        const auto stress_test_stripped =
            file::read<strbuf>("test.data/stress-test-stripped.txt").value();
        snn_require(stress_test_stripped.size() == 22401);

        snn_require(utf8::replace_invalid<strbuf>(stress_test) == stress_test_corrected);
        snn_require(utf8::replace_invalid<strbuf>(stress_test_corrected) == stress_test_corrected);
        snn_require(utf8::replace_invalid<strbuf>(stress_test_stripped) == stress_test_stripped);
    }
}
