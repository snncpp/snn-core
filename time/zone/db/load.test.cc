// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/zone/db/load.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            {
                auto opt = time::zone::db::load("europe/stockholm");
                snn_require(opt);
                const auto& e = opt.value();
                snn_require(e.name == "Europe/Stockholm");
                snn_require(e.country_codes.contains("SE"));
            }
            {
                auto opt = time::zone::db::load("aMERICa/LOS_angeles");
                snn_require(opt);
                decltype(auto) e = opt.value();
                static_assert(std::is_same_v<decltype(e), const time::zone::db::entry&>);
                snn_require(e.name == "America/Los_Angeles");
                snn_require(e.country_codes.contains("US"));
            }
            {
                snn_require(time::zone::db::load("Europe/Stockholm"));
                snn_require(!time::zone::db::load("Europe"));
                snn_require(!time::zone::db::load("Stockholm"));
                snn_require(!time::zone::db::load("Foo/Bar"));
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());
    }
}
