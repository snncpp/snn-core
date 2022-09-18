// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/zone/location.hh"

#include "snn-core/unittest.hh"
#include "snn-core/time/zone/db/utc.hh"
#include "snn-core/time/zone/db/america/los_angeles.hh"
#include "snn-core/time/zone/db/europe/stockholm.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            time::zone::location sthlm = time::zone::db::europe::stockholm.location();

            snn_require(sthlm.name() == "Europe/Stockholm");
            snn_require(sthlm.country_codes().contains(country::code{"SE"}));

            const time::zone::offset offset = sthlm.offset(1234567890);
            snn_require(offset.abbreviation().view() == "CET");
            snn_require(offset.seconds() == 3600);
            snn_require(!offset.is_daylight_saving_time());

            return true;
        }

        constexpr bool test_location()
        {
            // UTC
            {
                time::zone::location utc = time::zone::db::utc.location();

                snn_require(utc.name() == "UTC");

                // 2016-02-06 11:40:31 +0000 UTC
                {
                    const time::zone::offset offset = utc.offset(1454758831);
                    snn_require(offset.abbreviation().view() == "UTC");
                    snn_require(offset.seconds() == 0);
                    snn_require(!offset.is_daylight_saving_time());
                }

                snn_require(utc.offset(0).seconds() == 0);
                snn_require(utc.offset(constant::limit<i64>::max).seconds() == 0);
                snn_require(utc.offset(0).seconds() == 0);
                snn_require(utc.offset(constant::limit<i64>::min).seconds() == 0);
            }

            // "Europe/Stockholm"
            {
                time::zone::location sthlm = time::zone::db::europe::stockholm.location();

                snn_require(sthlm.name() == "Europe/Stockholm");
                snn_require(sthlm.country_codes().contains(country::code{"SE"}));

                // 2016-02-06 12:40:31 +0100 CET
                {
                    const time::zone::offset offset = sthlm.offset(1454758831);
                    snn_require(offset.abbreviation().view() == "CET");
                    snn_require(offset.seconds() == 3600);
                    snn_require(!offset.is_daylight_saving_time());
                }

                // 2015-03-29 01:59:59 +0100 CET
                {
                    const time::zone::offset offset = sthlm.offset(1427590799);
                    snn_require(offset.abbr().view() == "CET");
                    snn_require(offset.seconds() == 3600);
                    snn_require(!offset.is_dst());
                }

                // 2015-03-29 03:00:00 +0200 CEST
                {
                    const time::zone::offset offset = sthlm.offset(1427590800);
                    snn_require(offset.abbr().view() == "CEST");
                    snn_require(offset.seconds() == 7200);
                    snn_require(offset.is_dst());
                }
            }

            // America/Los_Angeles
            {
                time::zone::location la = time::zone::db::america::los_angeles.location();

                snn_require(la.name() == "America/Los_Angeles");
                snn_require(la.country_codes().contains(country::code{"US"}));

                // 2016-02-06 03:40:31 -0800 PST
                {
                    const time::zone::offset offset = la.offset(1454758831);
                    snn_require(offset.abbreviation().view() == "PST");
                    snn_require(offset.seconds() == -28800);
                    snn_require(!offset.is_daylight_saving_time());
                }

                // 2015-03-08 01:59:59 -0800 PST
                {
                    const time::zone::offset offset = la.offset(1425808799);
                    snn_require(offset.abbr().view() == "PST");
                    snn_require(offset.seconds() == -28800);
                    snn_require(!offset.is_dst());
                }

                // 2015-03-08 03:00:00 -0700 PDT
                {
                    const time::zone::offset offset = la.offset(1425808800);
                    snn_require(offset.abbr().view() == "PDT");
                    snn_require(offset.seconds() == -25200);
                    snn_require(offset.is_dst());
                }
            }

            {
                array<time::zone::offset, 4> offsets{{
                    {"ABCD", 7200 + 0, true}, // + X to make seconds unique for this test.
                    {"ABC", 3600 + 1, false}, // + X to make seconds unique for this test.
                    {"ABCD", 7200 + 2, true}, // + X to make seconds unique for this test.
                    {"ABC", 3600 + 3, false}, // + X to make seconds unique for this test.
                }};

                array<time::zone::transition, 4> transitions{{
                    {0, -1692496800},
                    {1, -1680483600},
                    {2, 323830800},
                    {3, 338950800},
                }};

                time::zone::db::entry test{"Test/Test", "A comment", "", {}, offsets, transitions};

                time::zone::location loc{test};

                // < first transition.
                snn_require(loc.offset(-1792496799).seconds() == 7200); // Fallback (first).
                snn_require(loc.offset(-1692496801).seconds() == 7200); // Fallback (first).
                snn_require(loc.last_offset_from() == constant::limit<i64>::min);
                snn_require(loc.last_offset_to() == -1692496800);

                snn_require(loc.offset(constant::limit<i64>::min).seconds() == 7200); // Fallback
                snn_require(loc.last_offset_from() == constant::limit<i64>::min);
                snn_require(loc.last_offset_to() == -1692496800);

                // == first transition.
                snn_require(loc.offset(-1692496800).seconds() == 7200);
                snn_require(loc.last_offset_from() == -1692496800);
                snn_require(loc.last_offset_to() == -1680483600);

                // < second transition.
                snn_require(loc.offset(-1680483601).seconds() == 7200);
                snn_require(loc.last_offset_from() == -1692496800);
                snn_require(loc.last_offset_to() == -1680483600);

                // == second transition.
                snn_require(loc.offset(-1680483600).seconds() == 3601);
                snn_require(loc.last_offset_from() == -1680483600);
                snn_require(loc.last_offset_to() == 323830800);

                // < third transition.
                snn_require(loc.offset(-123).seconds() == 3601);
                snn_require(loc.offset(0).seconds() == 3601);
                snn_require(loc.offset(123).seconds() == 3601);
                snn_require(loc.offset(323830799).seconds() == 3601);
                snn_require(loc.last_offset_from() == -1680483600);
                snn_require(loc.last_offset_to() == 323830800);

                // == third transition.
                snn_require(loc.offset(323830800).seconds() == 7202);
                snn_require(loc.last_offset_from() == 323830800);
                snn_require(loc.last_offset_to() == 338950800);

                // < fourth transition.
                snn_require(loc.offset(333810100).seconds() == 7202);
                snn_require(loc.offset(338950799).seconds() == 7202);
                snn_require(loc.last_offset_from() == 323830800);
                snn_require(loc.last_offset_to() == 338950800);

                // == fourth transition.
                snn_require(loc.offset(338950800).seconds() == 3603);
                snn_require(loc.last_offset_from() == 338950800);
                snn_require(loc.last_offset_to() == constant::limit<i64>::max);

                // > fourth transition.
                snn_require(loc.offset(338950801).seconds() == 3603);
                snn_require(loc.offset(454675600).seconds() == 3603);
                snn_require(loc.last_offset_from() == 338950800);
                snn_require(loc.last_offset_to() == constant::limit<i64>::max);

                snn_require(loc.offset(constant::limit<i64>::max).seconds() == 3603);
                snn_require(loc.last_offset_from() == 338950800);
                snn_require(loc.last_offset_to() == constant::limit<i64>::max);

                // Back to second.
                snn_require(loc.offset(-1680483600).seconds() == 3601);
                snn_require(loc.last_offset_from() == -1680483600);
                snn_require(loc.last_offset_to() == 323830800);
            }

            {
                snn_require(time::zone::location::is_valid_name("Africa/Porto-Novo"));
                snn_require(time::zone::location::is_valid_name("America/Argentina/Buenos_Aires"));
                snn_require(time::zone::location::is_valid_name("CET"));
                snn_require(time::zone::location::is_valid_name("Europe/Stockholm"));
                snn_require(time::zone::location::is_valid_name("A/B"));

                snn_require(!time::zone::location::is_valid_name(""));
                snn_require(!time::zone::location::is_valid_name("8"));
                snn_require(!time::zone::location::is_valid_name("."));
                snn_require(!time::zone::location::is_valid_name("/"));
                snn_require(!time::zone::location::is_valid_name("A/_"));
                snn_require(!time::zone::location::is_valid_name("_/B"));
                snn_require(!time::zone::location::is_valid_name("-/-"));
                snn_require(!time::zone::location::is_valid_name("A/B-"));
                snn_require(!time::zone::location::is_valid_name("A-/B"));
                snn_require(!time::zone::location::is_valid_name("Africa/Porto--Novo"));
                snn_require(!time::zone::location::is_valid_name("Africa/-Porto-Novo"));
                snn_require(!time::zone::location::is_valid_name("Africa/Porto-Novo-"));
                snn_require(!time::zone::location::is_valid_name("Europe/5tockholm"));
                snn_require(!time::zone::location::is_valid_name("Europe//Stockholm"));
                snn_require(!time::zone::location::is_valid_name("/Europe/Stockholm"));
                snn_require(!time::zone::location::is_valid_name("Europe/Stockholm/"));
                snn_require(!time::zone::location::is_valid_name("Europe/Stockholm_"));
                snn_require(!time::zone::location::is_valid_name("__/Europe/Stockholm"));
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
        snn_static_require(app::test_location());

        static_assert(sizeof(time::zone::location) == 32);
        static_assert(is_trivially_relocatable_v<time::zone::location>);
    }
}
