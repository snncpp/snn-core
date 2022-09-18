// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/mem/raw/is_overlapping.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        //               Position: 0  1  2  3  4  5  6  7  8  9
        static constexpr array arr{8, 2, 1, 7, 3, 0, 1, 3, 5, 1};

        static_assert(mem::raw::is_overlapping(arr.view(2, 3), arr.view(2, 3)));

        static_assert(mem::raw::is_overlapping(arr.view(0, 5), arr.view(4, 3)));
        static_assert(mem::raw::is_overlapping(arr.view(4, 3), arr.view(0, 5)));

        static_assert(mem::raw::is_overlapping(arr.view(0, 2), arr.view(1, 1)));
        static_assert(mem::raw::is_overlapping(arr.view(1, 1), arr.view(0, 2)));

        static_assert(!mem::raw::is_overlapping(arr.view(0, 5), arr.view(5, 1)));
        static_assert(!mem::raw::is_overlapping(arr.view(5, 1), arr.view(0, 5)));

        static_assert(!mem::raw::is_overlapping(arr.view(1, 2), arr.view(6, 2)));
        static_assert(!mem::raw::is_overlapping(arr.view(6, 2), arr.view(1, 2)));

        static_assert(!mem::raw::is_overlapping(arr.view(0, 0), arr.view(0, 0)));

        static_assert(!mem::raw::is_overlapping(arr.view(0, 1), arr.view(0, 0)));
        static_assert(!mem::raw::is_overlapping(arr.view(0, 0), arr.view(0, 1)));

        static constexpr array arr2{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
        // Comparing two distinct objects is not possible when constant evaluated. Changing this to
        // a `static_assert` will result in a compilation error: "comparison has unspecified value".
        snn_require(!mem::raw::is_overlapping(arr.view(), arr2.view()));
    }
}
