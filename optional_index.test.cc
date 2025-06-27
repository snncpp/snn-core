// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/optional_index.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                optional_index idx = 123;
                snn_require(idx); // Has value.
                snn_require(idx.has_value());
                snn_require(idx.value() == 123);
                snn_require(idx.value(assume::has_value) == 123);
                snn_require(idx.value_or(99) == 123);
                snn_require(idx.value_or_npos() == 123);
            }
            {
                optional_index idx = constant::npos;
                snn_require(!idx);
                snn_require(!idx.has_value());
                snn_require_throws_code(idx.value(), generic::error::no_value);
                snn_require(idx.value_or(99) == 99);
                snn_require(idx.value_or_npos() == constant::npos);
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

        static_assert(std::is_copy_constructible_v<optional_index>);
        static_assert(std::is_copy_assignable_v<optional_index>);
        static_assert(sane<optional_index>);

        {
            optional_index idx = 123;

            // idx = 456;              // This will not compile.
            idx = optional_index{456}; // Copy assignment.

            snn_require(idx);
            snn_require(idx.has_value());
            snn_require(idx.value() == 456);
            snn_require(idx.value(assume::has_value) == 456);
            snn_require(idx.value_or(99) == 456);
            snn_require(idx.value_or_npos() == 456);
        }
    }
}
