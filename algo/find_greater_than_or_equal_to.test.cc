// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/find_greater_than_or_equal_to.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::find_greater_than_or_equal_to(cstrrng{"abcdefghi"}, '0',
                                                            promise::is_sorted)
                            .value_or_npos() == 0);

            snn_require(algo::find_greater_than_or_equal_to(cstrrng{"abcdefghi"}, 'a',
                                                            promise::is_sorted)
                            .value_or_npos() == 0);

            snn_require(algo::find_greater_than_or_equal_to(cstrrng{"aaaaaaaaa"}, 'a',
                                                            promise::is_sorted)
                            .value_or_npos() == 0);

            snn_require(algo::find_greater_than_or_equal_to(cstrrng{"abcdefghi"}, 'b',
                                                            promise::is_sorted)
                            .value_or_npos() == 1);

            snn_require(algo::find_greater_than_or_equal_to(cstrrng{"abcdddghi"}, 'd',
                                                            promise::is_sorted)
                            .value_or_npos() == 3);

            snn_require(algo::find_greater_than_or_equal_to(cstrrng{"aaaabbbbb"}, 'b',
                                                            promise::is_sorted)
                            .value_or_npos() == 4);

            snn_require(algo::find_greater_than_or_equal_to(cstrrng{"abcdefghi"}, 'i',
                                                            promise::is_sorted)
                            .value_or_npos() == 8);

            snn_require(algo::find_greater_than_or_equal_to(cstrrng{""}, 'a', promise::is_sorted)
                            .value_or_npos() == constant::npos);

            snn_require(algo::find_greater_than_or_equal_to(cstrrng{"abcdefghi"}, 'j',
                                                            promise::is_sorted)
                            .value_or_npos() == constant::npos);

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
