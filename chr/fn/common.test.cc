// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/chr/fn/common.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // is_base
            {
                auto is_binary = chr::fn::is_base{math::base::binary};
                snn_require(is_binary('0'));
                snn_require(is_binary('1'));
                snn_require(!is_binary('2'));
                snn_require(!is_binary('8'));
                snn_require(!is_binary('9'));
                snn_require(!is_binary('f'));
                snn_require(!is_binary('x'));
            }
            {
                auto is_octal = chr::fn::is_base{math::base::octal};
                snn_require(is_octal('0'));
                snn_require(is_octal('1'));
                snn_require(is_octal('2'));
                snn_require(!is_octal('8'));
                snn_require(!is_octal('9'));
                snn_require(!is_octal('f'));
                snn_require(!is_octal('x'));
            }
            {
                const auto is_decimal = chr::fn::is_base{math::base::decimal};
                snn_require(is_decimal('0'));
                snn_require(is_decimal('1'));
                snn_require(is_decimal('2'));
                snn_require(is_decimal('8'));
                snn_require(is_decimal('9'));
                snn_require(!is_decimal('f'));
                snn_require(!is_decimal('x'));
            }
            {
                const auto is_hex = chr::fn::is_base{math::base::hex};
                snn_require(is_hex('0'));
                snn_require(is_hex('1'));
                snn_require(is_hex('2'));
                snn_require(is_hex('8'));
                snn_require(is_hex('9'));
                snn_require(is_hex('f'));
                snn_require(!is_hex('x'));
            }

            // xor_bits
            {
                snn_require(chr::fn::xor_bits{}('a', '9') == 'X');
                snn_require(chr::fn::xor_bits{}('9', 'a') == 'X');
                snn_require(chr::fn::xor_bits{}('a', 'a') == '\0');
                snn_require(chr::fn::xor_bits{}('9', '9') == '\0');
                snn_require('a' == 0b0110'0001);
                snn_require('9' == 0b0011'1001);
                snn_require('X' == 0b0101'1000);

                const auto xor_bits = chr::fn::xor_bits{};
                decltype(auto) c    = xor_bits(to_char(0b1100'1100), to_char(0b0111'1001));
                static_assert(std::is_same_v<decltype(c), char>);
                snn_require(c == to_char(0b1011'0101));
            }

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
