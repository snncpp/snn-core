// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/find_greater_than.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::find_greater_than(cstrrng{"abcdefghi"}, '0', promise::is_sorted)
                            .value_or_npos() == 0);

            snn_require(algo::find_greater_than(cstrrng{"abcdefghi"}, 'a', promise::is_sorted)
                            .value_or_npos() == 1);

            snn_require(algo::find_greater_than(cstrrng{"aaaabbbbb"}, 'a', promise::is_sorted)
                            .value_or_npos() == 4);

            snn_require(algo::find_greater_than(cstrrng{"abcdefghi"}, 'f', promise::is_sorted)
                            .value_or_npos() == 6);

            snn_require(algo::find_greater_than(cstrrng{"abcdefghi"}, 'h', promise::is_sorted)
                            .value_or_npos() == 8);

            snn_require(algo::find_greater_than(cstrrng{"abcdefghi"}, 'i', promise::is_sorted)
                            .value_or_npos() == constant::npos);

            snn_require(algo::find_greater_than(cstrrng{"abcdefghi"}, 'x', promise::is_sorted)
                            .value_or_npos() == constant::npos);

            snn_require(
                algo::find_greater_than(cstrrng{""}, 'a', promise::is_sorted).value_or_npos() ==
                constant::npos);

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
