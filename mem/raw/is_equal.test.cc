// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/mem/raw/is_equal.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        constexpr auto a = not_null{"abcdefghijklmnopqrstuvwxyz"};

        static_assert(mem::raw::is_equal(a, not_null{""}, byte_size{0}));
        static_assert(mem::raw::is_equal(a, not_null{"a"}, byte_size{1}));
        static_assert(mem::raw::is_equal(a, not_null{"ab"}, byte_size{2}));
        static_assert(!mem::raw::is_equal(a, not_null{"A"}, byte_size{1}));
        static_assert(!mem::raw::is_equal(a, not_null{"aB"}, byte_size{2}));
        static_assert(!mem::raw::is_equal(a, not_null{"Ab"}, byte_size{2}));

        static_assert(mem::raw::is_equal<0>(a, ""));
        static_assert(mem::raw::is_equal<1>(a, "a"));
        static_assert(!mem::raw::is_equal<1>(a, "A"));
        static_assert(mem::raw::is_equal<2>(a, "ab"));
        static_assert(!mem::raw::is_equal<2>(a, "aB"));
        static_assert(!mem::raw::is_equal<2>(a, "Ab"));
        static_assert(mem::raw::is_equal<3>(a, "abc"));
        static_assert(!mem::raw::is_equal<3>(a, "abC"));
        static_assert(!mem::raw::is_equal<3>(a, "Abc"));
    }
}
