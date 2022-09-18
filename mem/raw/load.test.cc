// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/mem/raw/load.hh"

#include "snn-core/unittest.hh"
#include <cstring> // memcpy

namespace snn::app
{
    namespace
    {
        template <typename Int>
        Int load_ref(const char*) noexcept
        {
            static_assert(meta::always_false<Int>, "Not implemented.");
            return 0;
        }

        template <>
        u16 load_ref<u16>(const char* const data) noexcept
        {
            u16 i = 0;
            std::memcpy(&i, data, sizeof(i));
            return i;
        }

        template <>
        u32 load_ref<u32>(const char* const data) noexcept
        {
            u32 i = 0;
            std::memcpy(&i, data, sizeof(i));
            return i;
        }

        template <>
        u64 load_ref<u64>(const char* const data) noexcept
        {
            u64 i = 0;
            std::memcpy(&i, data, sizeof(i));
            return i;
        }
    }
}

namespace snn
{
    void unittest()
    {
        static_assert(mem::raw::load<u16>("\0\0\0\0\0\0\0\0") == 0);
        static_assert(mem::raw::load<u32>("\0\0\0\0\0\0\0\0") == 0);
        static_assert(mem::raw::load<u64>("\0\0\0\0\0\0\0\0") == 0);

        snn_require(mem::raw::load<u16>("\0\0\0\0\0\0\0\0") == 0);
        snn_require(mem::raw::load<u32>("\0\0\0\0\0\0\0\0") == 0);
        snn_require(mem::raw::load<u64>("\0\0\0\0\0\0\0\0") == 0);

        snn_require(app::load_ref<u16>("\0\0\0\0\0\0\0\0") == 0);
        snn_require(app::load_ref<u32>("\0\0\0\0\0\0\0\0") == 0);
        snn_require(app::load_ref<u64>("\0\0\0\0\0\0\0\0") == 0);

        static_assert(mem::raw::load<u16>("abcdefgh") == 0x6261);
        static_assert(mem::raw::load<u32>("abcdefgh") == 0x64636261);
        static_assert(mem::raw::load<u64>("abcdefgh") == 0x6867666564636261);

        snn_require(mem::raw::load<u16>("abcdefgh") == 0x6261);
        snn_require(mem::raw::load<u32>("abcdefgh") == 0x64636261);
        snn_require(mem::raw::load<u64>("abcdefgh") == 0x6867666564636261);

        snn_require(app::load_ref<u16>("abcdefgh") == 0x6261);
        snn_require(app::load_ref<u32>("abcdefgh") == 0x64636261);
        snn_require(app::load_ref<u64>("abcdefgh") == 0x6867666564636261);
    }
}
