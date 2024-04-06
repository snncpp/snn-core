// Copyright (c) 2024 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/math/bits/common.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(math::bits::reverse_bytes(u16{0x0102}) == 0x0201);
            snn_require(math::bits::reverse_bytes(u32{0x01020304}) == 0x04030201);
            snn_require(math::bits::reverse_bytes(u64{0x0102030405060708}) == 0x0807060504030201);

            return true;
        }

        constexpr bool test_reverse_bytes()
        {
            snn_require(math::bits::reverse_bytes(u16{}) == 0);
            snn_require(math::bits::reverse_bytes(u32{}) == 0);
            snn_require(math::bits::reverse_bytes(u64{}) == 0);

            // With all bits set, the returned value is the same.
            snn_require(math::bits::reverse_bytes(constant::limit<u16>::max) ==
                        constant::limit<u16>::max);
            snn_require(math::bits::reverse_bytes(constant::limit<u32>::max) ==
                        constant::limit<u32>::max);
            snn_require(math::bits::reverse_bytes(constant::limit<u64>::max) ==
                        constant::limit<u64>::max);

            static_assert(std::is_same_v<decltype(math::bits::reverse_bytes(u16{})), u16>);
            static_assert(std::is_same_v<decltype(math::bits::reverse_bytes(u32{})), u32>);
            static_assert(std::is_same_v<decltype(math::bits::reverse_bytes(u64{})), u64>);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_reverse_bytes());
    }
}
