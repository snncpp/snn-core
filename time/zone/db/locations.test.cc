// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/zone/db/locations.hh"

#include "snn-core/unittest.hh"
#include "snn-core/ascii/fn/less_than_icase.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(time::zone::db::locations.count() > 300);

            snn_require(time::zone::db::locations.contains("Europe/Stockholm"));
            snn_require(!time::zone::db::locations.contains("Foo/Bar"));

            snn_require(time::zone::db::locations.is_sorted());
            snn_require(time::zone::db::locations.is_sorted(ascii::fn::less_than_icase{}));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
    }
}
