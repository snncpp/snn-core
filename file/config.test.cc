// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/config.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        file::config c{"config.test.conf"};

        snn_require(c);

        snn_require(c.get<str>("file") == "/this/is/a/path");
        snn_require(c.get<cstrview>("file") == "/this/is/a/path");

        snn_require(c.get<u16>("port") == 5432);
        snn_require(c.get<u16>("limit") == 0);      // Invalid
        snn_require(c.get<u16>("limit", 99) == 99); // Invalid, with default.

        snn_require(c.get<str>("duplicate") == "second");
        snn_require(c.get<cstrview>("duplicate") == "second");

        snn_require(c.get<str>("description") ==
                    R"(A description, with extra whitespace: "     ".)");
        snn_require(c.get<cstrview>("description") ==
                    R"(A description, with extra whitespace: "     ".)");

        snn_require(c.get<str>("non-existent") == "");
        snn_require(c.get<cstrview>("non-existent") == "");

        snn_require(c.get<str>("non-existent", "Default value.") == "Default value.");
        snn_require(c.get<cstrview>("non-existent", "Default value.") == "Default value.");

        const auto s = c.to<strbuf>();
        snn_require(s == "file: /this/is/a/path\n"
                         "port: 5432\n"
                         "limit: invalid\n"
                         "duplicate: first\n"
                         "duplicate: second\n"
                         "description: A description, with extra whitespace: \"     \".\n");
    }
}
