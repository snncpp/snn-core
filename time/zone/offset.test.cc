// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/zone/offset.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                time::zone::offset cet{"CET", 3600, false};
                snn_require(!cet.is_daylight_saving_time());
                snn_require(!cet.is_dst());
                snn_require(cet.seconds() == 3600);
                snn_require(cet.abbreviation() == "CET");
                snn_require(cet.abbr() == "CET");
            }
            {
                time::zone::offset cest = {"CEST", 7200, true};
                snn_require(cest.is_daylight_saving_time());
                snn_require(cest.is_dst());
                snn_require(cest.seconds() == 7200);
                snn_require(cest.abbreviation() == "CEST");
                snn_require(cest.abbr() == "CEST");
            }

            return true;
        }

        constexpr bool test_offset()
        {
            static_assert(sizeof(time::zone::offset) == 12);
            static_assert(is_trivially_relocatable_v<time::zone::offset>);

            {
                cstrview s{"+0200"};
                time::zone::offset offs{time::zone::abbr{s}, 7200, true};
                snn_require(offs.is_daylight_saving_time());
                snn_require(offs.is_dst());
                snn_require(offs.seconds() == 7200);
                snn_require(offs.abbreviation() == "+0200");
                snn_require(offs.abbr() == "+0200");
            }

            {
                cstrview s{"-01"};
                time::zone::offset offs = {time::zone::abbr{s}, -3600, false};
                snn_require(!offs.is_daylight_saving_time());
                snn_require(!offs.is_dst());
                snn_require(offs.seconds() == -3600);
                snn_require(offs.abbreviation() == "-01");
                snn_require(offs.abbr() == "-01");
            }

            {
                auto utc = time::zone::offset::utc();
                snn_require(!utc.is_daylight_saving_time());
                snn_require(!utc.is_dst());
                snn_require(utc.seconds() == 0);
                snn_require(utc.abbreviation() == "UTC");
                snn_require(utc.abbr() == "UTC");
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
        snn_static_require(app::test_offset());
    }
}
