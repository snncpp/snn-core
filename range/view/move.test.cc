// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/move.hh"

#include "snn-core/array.hh"
#include "snn-core/strcore.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/max.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array<strbuf, 4> a{"One", "Two", "Three", "Four"};

            // Without move.

            auto max_ref = algo::max(a.range());
            static_assert(std::is_same_v<decltype(max_ref), optional<strbuf&>>);
            snn_require(max_ref.has_value());
            snn_require(max_ref.value() == "Two");

            // The array has not changed.
            snn_require(a.at(0).value() == "One");
            snn_require(a.at(1).value() == "Two");
            snn_require(a.at(2).value() == "Three");
            snn_require(a.at(3).value() == "Four");

            // With move.

            auto max = algo::max(a.range() | range::v::move{});
            static_assert(std::is_same_v<decltype(max), optional<strbuf>>);
            snn_require(max.has_value());
            snn_require(max.value() == "Two");

            snn_require(a.at(0).value() == "");      // Moved as def. max if it's the only element.
            snn_require(a.at(1).value() == "One");   // Moved as final max (swapped with def. max).
            snn_require(a.at(2).value() == "Three"); // Never moved, only compared.
            snn_require(a.at(3).value() == "Four");  // Never moved, only compared.

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
