// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/country/codes.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/any.hh"
#include "snn-core/country/names.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/range/view/pair.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            auto codes = country::codes.range();
            auto names = country::names.range();

            snn_require(codes.count() > 200);
            snn_require(codes.count() == names.count());

            snn_require(codes.contains("SE"));
            snn_require(!codes.contains("se"));

            snn_require(names.contains("Sweden"));
            snn_require(!names.contains("SWEDEN"));

            range::view::pair combined{names, codes};
            snn_require(combined.count() == codes.count());

            // clang-format off
            snn_require(algo::any(combined, [](auto p) {
                return p.first == "Afghanistan" && p.second == "AF";
            }));
            snn_require(algo::any(combined, [](auto p) {
                return p.first == "Iceland" && p.second == "IS";
            }));
            snn_require(algo::any(combined, [](auto p) {
                return p.first == "Sweden" && p.second == "SE";
            }));
            snn_require(algo::any(combined, [](auto p) {
                return p.first == "Zimbabwe" && p.second == "ZW";
            }));
            // clang-format on

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
