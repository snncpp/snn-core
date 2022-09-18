// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/mem/raw/compare.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        static_assert(mem::raw::compare(not_null{""}, not_null{""}, byte_size{0}) == 0);

        static_assert(mem::raw::compare(not_null{"a"}, not_null{"a"}, byte_size{1}) == 0);
        static_assert(mem::raw::compare(not_null{"a"}, not_null{"b"}, byte_size{1}) < 0);
        static_assert(mem::raw::compare(not_null{"b"}, not_null{"a"}, byte_size{1}) > 0);

        static_assert(mem::raw::compare(not_null{"abc"}, not_null{"abc"}, byte_size{3}) == 0);
        static_assert(mem::raw::compare(not_null{"abc"}, not_null{"abx"}, byte_size{2}) == 0);

        static_assert(mem::raw::compare(not_null{"abc"}, not_null{"abx"}, byte_size{3}) < 0);
        static_assert(mem::raw::compare(not_null{"abx"}, not_null{"abc"}, byte_size{3}) > 0);

        static_assert(mem::raw::compare<3>(not_null{"abc"}, not_null{"abc"}) == 0);
        static_assert(mem::raw::compare<2>(not_null{"abc"}, not_null{"abx"}) == 0);

        static_assert(mem::raw::compare<3>(not_null{"abc"}, not_null{"abx"}) < 0);
        static_assert(mem::raw::compare<3>(not_null{"abx"}, not_null{"abc"}) > 0);
    }
}
