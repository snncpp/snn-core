// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/reduce.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/fn/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::reduce(array<int, 0>{}.range(), 0, fn::add{}) == 0);
            snn_require(algo::reduce(array{7}.range(), 0, fn::add{}) == 7);
            snn_require(algo::reduce(array{7, 8}.range(), 0, fn::add{}) == 15);
            snn_require(algo::reduce(array{7, 8, 9}.range(), 0, fn::add{}) == 24);
            snn_require(algo::reduce(array{7, 8, 9}.range(), 100, fn::add{}) == 124);

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
