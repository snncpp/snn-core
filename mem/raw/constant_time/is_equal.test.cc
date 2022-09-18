// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/mem/raw/constant_time/is_equal.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        using mem::raw::constant_time::is_equal;

        snn_require(is_equal(not_null{"abcdef"}, not_null{"abcDEF"}, byte_size{0}));
        snn_require(is_equal(not_null{"abcdef"}, not_null{"abcDEF"}, byte_size{1}));
        snn_require(is_equal(not_null{"abcdef"}, not_null{"abcDEF"}, byte_size{3}));
        snn_require(!is_equal(not_null{"abcdef"}, not_null{"abcDEF"}, byte_size{4}));
        snn_require(!is_equal(not_null{"abcdef"}, not_null{"abcDEF"}, byte_size{5}));
        snn_require(!is_equal(not_null{"abcdef"}, not_null{"abcDEF"}, byte_size{6}));

        snn_require(is_equal(not_null{"\x00\x00\x32"}, not_null{"\x32\x00\x32"}, byte_size{0}));
        snn_require(is_equal(not_null{"\x00\x00\x00"}, not_null{"\x00\x00\x00"}, byte_size{3}));
        snn_require(is_equal(not_null{"\xff\x00\xff"}, not_null{"\xff\x00\xff"}, byte_size{3}));

        snn_require(!is_equal(not_null{"\x00\x00\x32"}, not_null{"\x32\x00\x32"}, byte_size{3}));
        snn_require(!is_equal(not_null{"\x32\x00\x32"}, not_null{"\x32\x00\x00"}, byte_size{3}));
        snn_require(!is_equal(not_null{"\x32\x00\x32"}, not_null{"\x00\x00\x00"}, byte_size{3}));
        snn_require(!is_equal(not_null{"\x01\x02\x03"}, not_null{"\x01\x02\x04"}, byte_size{3}));
        snn_require(!is_equal(not_null{"\x01\x02\x03"}, not_null{"\x01\x02\x02"}, byte_size{3}));
    }
}
