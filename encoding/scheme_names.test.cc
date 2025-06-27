// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/encoding/scheme_names.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/any.hh"
#include "snn-core/encoding/schemes.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/range/view/pair.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(encoding::scheme_names.contains("Unknown"));
            snn_require(encoding::scheme_names.contains("ISO-8859-1"));
            snn_require(encoding::scheme_names.contains("UTF-8"));

            snn_require(!encoding::scheme_names.contains("utf-8"));

            snn_require(encoding::scheme_names.count() > 30);

            return true;
        }

        constexpr bool test_scheme_names()
        {
            auto schemes      = encoding::schemes.range();
            auto scheme_names = encoding::scheme_names.range();

            snn_require(schemes.count() > 30);
            snn_require(schemes.count() == scheme_names.count());

            range::view::pair combined{schemes, scheme_names};
            snn_require(combined.count() == schemes.count());

            snn_require(combined);
            const auto unknown = combined.front(assume::not_empty);
            snn_require(unknown.first == encoding::scheme::unknown);
            snn_require(unknown.second == "Unknown");

            snn_require(algo::any(combined, [](auto p) {
                return p.first == encoding::scheme::utf8 && p.second == "UTF-8";
            }));

            snn_require(algo::any(combined, [](auto p) {
                return p.first == encoding::scheme::shift_jis && p.second == "Shift_JIS";
            }));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_scheme_names());
    }
}
