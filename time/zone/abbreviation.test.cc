// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/zone/abbreviation.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            time::zone::abbr cet{"CET"};
            snn_require(cet.view() == "CET");
            snn_require(algo::is_equal(cet.range(), {'C', 'E', 'T'}));

            return true;
        }

        constexpr bool test_abbreviation()
        {
            static_assert(sizeof(time::zone::abbreviation) == 7);
            static_assert(is_trivially_relocatable_v<time::zone::abbreviation>);

            // Construction
            snn_require_nothrow(time::zone::abbreviation{cstrview{"CEST"}});
            snn_require_throws_code(time::zone::abbreviation{cstrview{"..."}},
                                    time::error::invalid_zone_abbreviation);

            snn_require(time::zone::abbreviation{"CEST"} == "CEST");
            snn_require(time::zone::abbreviation{"+0945"} == "+0945");
            snn_require(time::zone::abbreviation{"-01"} == "-01");

            // View
            snn_require(time::zone::abbreviation{"CEST"}.view() == "CEST");
            snn_require(time::zone::abbreviation{"+0945"}.view() == "+0945");
            snn_require(time::zone::abbreviation{"-01"}.view() == "-01");
            snn_require(time::zone::abbreviation{cstrview{"CEST"}}.view() == "CEST");
            snn_require(time::zone::abbreviation{cstrview{"+0945"}}.view() == "+0945");
            snn_require(time::zone::abbreviation{cstrview{"-01"}}.view() == "-01");
            snn_require(time::zone::abbreviation{"CEST", assume::is_valid}.view() == "CEST");
            snn_require(time::zone::abbreviation{"+0945", assume::is_valid}.view() == "+0945");
            snn_require(time::zone::abbreviation{"-01", assume::is_valid}.view() == "-01");

            // Alias
            static_assert(std::is_same_v<time::zone::abbr, time::zone::abbreviation>);
            snn_require(time::zone::abbr{"-01"} == "-01");
            snn_require(time::zone::abbr{cstrview{"-01"}} == "-01");

            // Comparison
            snn_require(time::zone::abbr{"-01"} == time::zone::abbr{"-01"});
            snn_require(!(time::zone::abbr{"-0100"} == time::zone::abbr{"+0100"}));
            snn_require(time::zone::abbr{"CET"} < time::zone::abbr{"CST"});
            snn_require(!(time::zone::abbr{"CET"} < time::zone::abbr{"CEST"}));
            snn_require(time::zone::abbr{"-01"} == cstrview{"-01"});
            snn_require(!(time::zone::abbr{"-0100"} == cstrview{"+0100"}));
            snn_require(time::zone::abbr{"CET"} < cstrview{"CST"});
            snn_require(!(time::zone::abbr{"CET"} < cstrview{"CEST"}));

            // UTC
            snn_require(time::zone::abbreviation::utc() == "UTC");
            snn_require(time::zone::abbr::utc() == "UTC");

            return true;
        }

        constexpr bool test_is_valid()
        {
            static_assert(time::zone::abbreviation::is_valid("abc"));
            static_assert(time::zone::abbreviation::is_valid("abcd"));
            static_assert(time::zone::abbreviation::is_valid("abcde"));
            static_assert(time::zone::abbreviation::is_valid("abcdef"));
            static_assert(time::zone::abbreviation::is_valid("ABC"));
            static_assert(time::zone::abbreviation::is_valid("ABCD"));
            static_assert(time::zone::abbreviation::is_valid("ABCDE"));
            static_assert(time::zone::abbreviation::is_valid("ABCDEF"));
            static_assert(time::zone::abbreviation::is_valid("+01"));
            static_assert(time::zone::abbreviation::is_valid("-01"));
            static_assert(time::zone::abbreviation::is_valid("+0145"));
            static_assert(time::zone::abbreviation::is_valid("-9999"));
            static_assert(time::zone::abbreviation::is_valid("-0000"));

            static_assert(!time::zone::abbreviation::is_valid(""));
            static_assert(!time::zone::abbreviation::is_valid("a"));
            static_assert(!time::zone::abbreviation::is_valid("ab"));
            static_assert(!time::zone::abbreviation::is_valid("A"));
            static_assert(!time::zone::abbreviation::is_valid("AB"));
            static_assert(!time::zone::abbreviation::is_valid("+"));
            static_assert(!time::zone::abbreviation::is_valid("-"));
            static_assert(!time::zone::abbreviation::is_valid("+0"));
            static_assert(!time::zone::abbreviation::is_valid("-0"));
            static_assert(!time::zone::abbreviation::is_valid("+000"));
            static_assert(!time::zone::abbreviation::is_valid("-000"));
            static_assert(!time::zone::abbreviation::is_valid("+00000"));
            static_assert(!time::zone::abbreviation::is_valid("-00000"));
            static_assert(!time::zone::abbreviation::is_valid("+000000"));
            static_assert(!time::zone::abbreviation::is_valid("-000000"));
            static_assert(!time::zone::abbreviation::is_valid("+0a"));
            static_assert(!time::zone::abbreviation::is_valid("-0a"));
            static_assert(!time::zone::abbreviation::is_valid("+a0"));
            static_assert(!time::zone::abbreviation::is_valid("-a0"));
            static_assert(!time::zone::abbreviation::is_valid("+aa"));
            static_assert(!time::zone::abbreviation::is_valid("-aa"));
            static_assert(!time::zone::abbreviation::is_valid("abcdefg"));
            static_assert(!time::zone::abbreviation::is_valid("ABCDEFG"));
            static_assert(!time::zone::abbreviation::is_valid("A_c"));
            static_assert(!time::zone::abbreviation::is_valid("A9B"));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_abbreviation());
        snn_static_require(app::test_is_valid());
    }
}
