// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/ascii/to_integral.hh"

#include "snn-core/unittest.hh"
#include "snn-core/range/step.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            auto opt = ascii::to_integral<i64>(cstrrng{"123"});
            static_assert(std::is_same_v<decltype(opt), optional<i64>>);
            snn_require(opt.has_value());
            snn_require(opt.value() == 123);

            snn_require(ascii::to_integral<i64>(cstrrng{"0"}).value() == 0);
            snn_require(ascii::to_integral<i64>(cstrrng{"-0"}).value() == 0);
            snn_require(ascii::to_integral<i64>(cstrrng{"7"}).value() == 7);
            snn_require(ascii::to_integral<i64>(cstrrng{"-3"}).value() == -3);
            snn_require(ascii::to_integral<i64>(cstrrng{"-38272"}).value() == -38272);

            // Max
            snn_require(ascii::to_integral<i64>(cstrrng{"9223372036854775807"}).value() ==
                        constant::limit<i64>::max);

            // Min
            snn_require(ascii::to_integral<i64>(cstrrng{"-9223372036854775808"}).value() ==
                        constant::limit<i64>::min);

            // Invalid
            snn_require(!ascii::to_integral<i64>(cstrrng{""}));
            snn_require(!ascii::to_integral<i64>(cstrrng{"-"}));
            snn_require(!ascii::to_integral<i64>(cstrrng{"abc"}));
            snn_require(!ascii::to_integral<i64>(cstrrng{"a1"}));
            snn_require(!ascii::to_integral<i64>(cstrrng{"\0"}));
            snn_require(!ascii::to_integral<i64>(cstrrng{"-\0"}));
            snn_require(!ascii::to_integral<i64>(cstrrng{"922337203x238"}));
            snn_require(!ascii::to_integral<i64>(cstrrng{"-922337203x238"}));
            snn_require(!ascii::to_integral<i64>(cstrrng{"-7a"}));
            snn_require(!ascii::to_integral<i64>(cstrrng{"3foo"}));
            snn_require(!ascii::to_integral<i64>(cstrrng{"3\0"}));

            // Invalid - Overflow
            snn_require(!ascii::to_integral<i64>(cstrrng{"9223372036854775808"}));
            snn_require(!ascii::to_integral<i64>(cstrrng{"-9223372036854775809"}));

            // Invalid - Zero prefix.
            snn_require(!ascii::to_integral<i64>(cstrrng{"-0922337203"}));
            snn_require(!ascii::to_integral<i64>(cstrrng{"0922337203"}));

            return true;
        }

        constexpr bool test_to_integral()
        {
            // u64

            static_assert(ascii::to_integral<u64>(cstrrng{"0"}));
            static_assert(ascii::to_integral<u64>(cstrrng{"0"}).value() == 0);

            static_assert(ascii::to_integral<u64>(cstrrng{"123"}));
            static_assert(ascii::to_integral<u64>(cstrrng{"123"}).value() == 123);

            static_assert(ascii::to_integral<u64>(cstrrng{"832392"}));
            static_assert(ascii::to_integral<u64>(cstrrng{"832392"}).value() == 832392);

            // Max
            static_assert(ascii::to_integral<u64>(cstrrng{"18446744073709551615"}));
            static_assert(ascii::to_integral<u64>(cstrrng{"18446744073709551615"}).value() ==
                          constant::limit<u64>::max);

            // Invalid
            static_assert(!ascii::to_integral<u64>(cstrrng{""}));
            static_assert(!ascii::to_integral<u64>(cstrrng{"922337203x238"}));
            static_assert(!ascii::to_integral<u64>(cstrrng{"3foo"}));
            static_assert(!ascii::to_integral<u64>(cstrrng{"3\0"}));
            static_assert(!ascii::to_integral<u64>(cstrrng{"-"}));
            static_assert(!ascii::to_integral<u64>(cstrrng{"-0"}));
            static_assert(!ascii::to_integral<u64>(cstrrng{"-3"}));
            static_assert(!ascii::to_integral<u64>(cstrrng{"-184467440"}));
            static_assert(!ascii::to_integral<u64>(cstrrng{"abc"}));
            static_assert(!ascii::to_integral<u64>(cstrrng{"a1"}));
            static_assert(!ascii::to_integral<u64>(cstrrng{"\0"}));
            static_assert(!ascii::to_integral<u64>(cstrrng{"-\0"}));

            // Invalid - Overflow
            static_assert(!ascii::to_integral<u64>(cstrrng{"18446744073709551616"}));
            static_assert(!ascii::to_integral<u64>(cstrrng{"99999999999999999999"}));

            // Invalid - Zero prefix.
            static_assert(!ascii::to_integral<u64>(cstrrng{"0184467440"}));

            // u32

            // Max
            static_assert(ascii::to_integral<u32>(cstrrng{"4294967295"}));
            static_assert(ascii::to_integral<u32>(cstrrng{"4294967295"}).value() ==
                          constant::limit<u32>::max);

            // Invalid - Overflow
            static_assert(!ascii::to_integral<u32>(cstrrng{"4294967296"}));
            static_assert(!ascii::to_integral<u32>(cstrrng{"18446744073709551616"}));

            // i32

            // Max
            static_assert(ascii::to_integral<i32>(cstrrng{"2147483647"}));
            static_assert(ascii::to_integral<i32>(cstrrng{"2147483647"}).value() ==
                          constant::limit<i32>::max);

            // Min
            static_assert(ascii::to_integral<i32>(cstrrng{"-2147483648"}));
            static_assert(ascii::to_integral<i32>(cstrrng{"-2147483648"}).value() ==
                          constant::limit<i32>::min);

            // Overflow
            static_assert(!ascii::to_integral<i32>(cstrrng{"2147483648"}));
            static_assert(!ascii::to_integral<i32>(cstrrng{"-2147483649"}));
            static_assert(!ascii::to_integral<i32>(cstrrng{"18446744073709551616"}));
            static_assert(!ascii::to_integral<i32>(cstrrng{"-9223372036854775809"}));

            // u16

            // Max
            static_assert(ascii::to_integral<u16>(cstrrng{"65535"}));
            static_assert(ascii::to_integral<u16>(cstrrng{"65535"}).value() ==
                          constant::limit<u16>::max);

            // Overflow
            static_assert(!ascii::to_integral<u16>(cstrrng{"65536"}));

            // i16

            // Max
            static_assert(ascii::to_integral<i16>(cstrrng{"32767"}));
            static_assert(ascii::to_integral<i16>(cstrrng{"32767"}).value() ==
                          constant::limit<i16>::max);

            // Min
            static_assert(ascii::to_integral<i16>(cstrrng{"-32768"}));
            static_assert(ascii::to_integral<i16>(cstrrng{"-32768"}).value() ==
                          constant::limit<i16>::min);

            // Overflow
            static_assert(!ascii::to_integral<i16>(cstrrng{"32768"}));
            static_assert(!ascii::to_integral<i16>(cstrrng{"-32769"}));

            // i16 (hex)

            // Max
            static_assert(ascii::to_integral<i16, math::base::hex>(cstrrng{"7FfF"}));
            static_assert(ascii::to_integral<i16, math::base::hex>(cstrrng{"7FfF"}).value() ==
                          constant::limit<i16>::max);

            // Min
            static_assert(ascii::to_integral<i16, math::base::hex>(cstrrng{"-8000"}));
            static_assert(ascii::to_integral<i16, math::base::hex>(cstrrng{"-8000"}).value() ==
                          constant::limit<i16>::min);

            // Overflow
            static_assert(!ascii::to_integral<i16, math::base::hex>(cstrrng{"8000"}));
            static_assert(!ascii::to_integral<i16, math::base::hex>(cstrrng{"-8001"}));

            // u8

            // Max
            static_assert(ascii::to_integral<u8>(cstrrng{"255"}));
            static_assert(ascii::to_integral<u8>(cstrrng{"255"}).value() ==
                          constant::limit<u8>::max);

            // Overflow
            static_assert(!ascii::to_integral<u8>(cstrrng{"256"}));

            // u8 (binary)

            // Max
            static_assert(ascii::to_integral<u8, math::base::binary>(cstrrng{"11111111"}));
            static_assert(ascii::to_integral<u8, math::base::binary>(cstrrng{"11111111"}).value() ==
                          constant::limit<u8>::max);

            // Overflow
            static_assert(!ascii::to_integral<u8, math::base::binary>(cstrrng{"100000000"}));

            // u8 (octal)

            static_assert(ascii::to_integral<u8, math::base::octal>(cstrrng{"77"}));
            static_assert(ascii::to_integral<u8, math::base::octal>(cstrrng{"77"}).value() == 63);

            // Max
            static_assert(ascii::to_integral<u8, math::base::octal>(cstrrng{"377"}));
            static_assert(ascii::to_integral<u8, math::base::octal>(cstrrng{"377"}).value() ==
                          constant::limit<u8>::max);

            // Overflow
            static_assert(!ascii::to_integral<u8, math::base::octal>(cstrrng{"400"}));

            // Invalid
            static_assert(!ascii::to_integral<u8, math::base::octal>(cstrrng{"778"}));

            // u8 (hex)

            // Max
            static_assert(ascii::to_integral<u8, math::base::hex>(cstrrng{"FF"}));
            static_assert(ascii::to_integral<u8, math::base::hex>(cstrrng{"FF"}).value() ==
                          constant::limit<u8>::max);

            // Overflow
            static_assert(!ascii::to_integral<u8, math::base::hex>(cstrrng{"100"}));

            // i8

            // Max
            static_assert(ascii::to_integral<i8>(cstrrng{"127"}));
            static_assert(ascii::to_integral<i8>(cstrrng{"127"}).value() ==
                          constant::limit<i8>::max);

            // Min
            static_assert(ascii::to_integral<i8>(cstrrng{"-128"}));
            static_assert(ascii::to_integral<i8>(cstrrng{"-128"}).value() ==
                          constant::limit<i8>::min);

            // Overflow
            static_assert(!ascii::to_integral<i8>(cstrrng{"128"}));
            static_assert(!ascii::to_integral<i8>(cstrrng{"-129"}));

            // i8 (binary)

            // Max
            static_assert(ascii::to_integral<i8, math::base::binary>(cstrrng{"1111111"}));
            static_assert(ascii::to_integral<i8, math::base::binary>(cstrrng{"1111111"}).value() ==
                          constant::limit<i8>::max);

            // Min
            static_assert(ascii::to_integral<i8, math::base::binary>(cstrrng{"-10000000"}));
            static_assert(
                ascii::to_integral<i8, math::base::binary>(cstrrng{"-10000000"}).value() ==
                constant::limit<i8>::min);

            // Overflow
            static_assert(!ascii::to_integral<i8, math::base::binary>(cstrrng{"10000000"}));
            static_assert(!ascii::to_integral<i8, math::base::binary>(cstrrng{"-10000001"}));

            // i8 (octal)

            // Max
            static_assert(ascii::to_integral<i8, math::base::octal>(cstrrng{"177"}));
            static_assert(ascii::to_integral<i8, math::base::octal>(cstrrng{"177"}).value() ==
                          constant::limit<i8>::max);

            // Min
            static_assert(ascii::to_integral<i8, math::base::octal>(cstrrng{"-200"}));
            static_assert(ascii::to_integral<i8, math::base::octal>(cstrrng{"-200"}).value() ==
                          constant::limit<i8>::min);

            // Overflow
            static_assert(!ascii::to_integral<i8, math::base::octal>(cstrrng{"200"}));
            static_assert(!ascii::to_integral<i8, math::base::octal>(cstrrng{"-201"}));

            // Non-contiguous range.

            static_assert(ascii::to_integral<i32>(range::step{'1', '4'}).value() == 123);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        static_assert(app::test_to_integral());
    }
}
