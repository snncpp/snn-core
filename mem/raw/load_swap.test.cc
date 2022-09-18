// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/mem/raw/load_swap.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        static_assert(mem::raw::load_swap<u16>("\0\0\0\0\0\0\0\0") == 0);
        static_assert(mem::raw::load_swap<u32>("\0\0\0\0\0\0\0\0") == 0);
        static_assert(mem::raw::load_swap<u64>("\0\0\0\0\0\0\0\0") == 0);

        snn_require(mem::raw::load_swap<u16>("\0\0\0\0\0\0\0\0") == 0);
        snn_require(mem::raw::load_swap<u32>("\0\0\0\0\0\0\0\0") == 0);
        snn_require(mem::raw::load_swap<u64>("\0\0\0\0\0\0\0\0") == 0);

        static_assert(mem::raw::load_swap<u16>("abcdefgh") == 0x6162);
        static_assert(mem::raw::load_swap<u32>("abcdefgh") == 0x61626364);
        static_assert(mem::raw::load_swap<u64>("abcdefgh") == 0x6162636465666768);

        snn_require(mem::raw::load_swap<u16>("abcdefgh") == 0x6162);
        snn_require(mem::raw::load_swap<u32>("abcdefgh") == 0x61626364);
        snn_require(mem::raw::load_swap<u64>("abcdefgh") == 0x6162636465666768);
    }
}
