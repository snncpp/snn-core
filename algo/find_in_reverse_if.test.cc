// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/find_in_reverse_if.hh"

#include "snn-core/unittest.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/fn/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::find_in_reverse_if(cstrrng{"a"}, chr::is_alpha_lower));
            snn_require(
                algo::find_in_reverse_if(cstrrng{"a"}, chr::is_alpha_lower).value_or_npos() == 0);

            snn_require(algo::find_in_reverse_if(cstrrng{"ab"}, chr::is_alpha_lower));
            snn_require(
                algo::find_in_reverse_if(cstrrng{"ab"}, chr::is_alpha_lower).value_or_npos() == 1);

            snn_require(
                algo::find_in_reverse_if(cstrrng{"abc"}, chr::is_alpha_lower).value_or_npos() == 2);
            snn_require(
                algo::find_in_reverse_if(cstrrng{"abC"}, chr::is_alpha_lower).value_or_npos() == 1);
            snn_require(
                algo::find_in_reverse_if(cstrrng{"aBC"}, chr::is_alpha_lower).value_or_npos() == 0);

            snn_require(!algo::find_in_reverse_if(cstrrng{"ABC"}, chr::is_alpha_lower));
            snn_require(
                algo::find_in_reverse_if(cstrrng{"ABC"}, chr::is_alpha_lower).value_or_npos() ==
                constant::npos);

            return true;
        }

        constexpr bool test_find_in_reverse_if()
        {
            snn_require(!algo::find_in_reverse_if(cstrrng{""}, chr::is_alpha_lower));
            snn_require(
                algo::find_in_reverse_if(cstrrng{""}, chr::is_alpha_lower).value_or_npos() ==
                constant::npos);

            snn_require(!algo::find_in_reverse_if(cstrrng{"A"}, chr::is_alpha_lower));
            snn_require(
                algo::find_in_reverse_if(cstrrng{"A"}, chr::is_alpha_lower).value_or_npos() ==
                constant::npos);

            snn_require(algo::find_in_reverse_if(cstrrng{"abc"}, fn::is{fn::equal_to{}, '\0'})
                            .value_or_npos() == constant::npos);

            snn_require(algo::find_in_reverse_if(cstrrng{"abc"}, fn::is{fn::equal_to{}, 'b'})
                            .value_or_npos() == 1);

            cstrrng rng{"abCD"};
            snn_require(algo::find_in_reverse_if(rng, chr::is_alpha_lower).value_or_npos() == 1);
            snn_require(algo::find_in_reverse_if(rng, chr::is_alpha_upper).value_or_npos() == 3);
            rng.pop_back_n(2);
            snn_require(algo::find_in_reverse_if(rng, chr::is_alpha_lower).value_or_npos() == 1);
            snn_require(algo::find_in_reverse_if(rng, chr::is_alpha_upper).value_or_npos() ==
                        constant::npos);
            rng.pop_back_n(2);
            snn_require(rng.is_empty());
            snn_require(algo::find_in_reverse_if(rng, chr::is_alpha_lower).value_or_npos() ==
                        constant::npos);
            snn_require(algo::find_in_reverse_if(rng, chr::is_alpha_upper).value_or_npos() ==
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
        snn_static_require(app::test_find_in_reverse_if());
    }
}
