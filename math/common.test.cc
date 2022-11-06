// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/math/common.hh"

#include "snn-core/unittest.hh"
#include "snn-core/random/number.hh"
#include "snn-core/range/step.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(math::max(5, 4) == 5);
            snn_require(math::max(1, 3, 2) == 3);

            snn_require(math::abs(-5) == 5);
            snn_require(math::abs(constant::limit<i32>::min) == u32{2147483648});

            snn_require(math::add_with_overflow(constant::limit<u32>::max, 0) ==
                        constant::limit<u32>::max);
            snn_require(math::add_with_overflow(constant::limit<u32>::max, 1) == 0);

            snn_require(math::add_with_saturation(constant::limit<u32>::max, 0) ==
                        constant::limit<u32>::max);
            snn_require(math::add_with_saturation(constant::limit<u32>::max, 1) ==
                        constant::limit<u32>::max);

            snn_require(math::count_digits<math::base::decimal>(u32{83922}).get() == 5);

            snn_require(math::is_power_of_two(2u));
            snn_require(!math::is_power_of_two(3u));

            snn_require(math::round_up_to_multiple<8>(17).get() == 24);

            snn_require(math::is_within_bounds<u32>(0));
            snn_require(!math::is_within_bounds<u32>(-1));

            snn_require(math::within_bounds_or<usize>(0, 99) == 0);
            snn_require(math::within_bounds_or<usize>(1, 99) == 1);
            snn_require(math::within_bounds_or<usize>(-1, 99) == 99);

            return true;
        }

        constexpr usize count_digits_ref(u64 n) noexcept
        {
            usize count = 0;
            do
            {
                ++count;
                n /= 10;
            } while (n);
            return count;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        static_assert(math::add_with_overflow(0u, 0) == 0);
        static_assert(math::add_with_overflow(3u, 4) == 7);
        static_assert(math::add_with_overflow(constant::limit<u32>::max, 0) ==
                      constant::limit<u32>::max);
        static_assert(math::add_with_overflow(constant::limit<u32>::max, 1) == 0);
        static_assert(math::add_with_overflow(constant::limit<u32>::max, 2) == 1);

        static_assert(math::multiply_with_overflow(0u, 0) == 0);
        static_assert(math::multiply_with_overflow(5u, 3) == 15);
        static_assert(math::multiply_with_overflow(constant::limit<u16>::max, 3) == 65533);

        static_assert(math::negate_with_overflow(0u) == 0);
        static_assert(math::negate_with_overflow(u32{1}) == constant::limit<u32>::max);
        static_assert(math::negate_with_overflow(constant::limit<u32>::max) == 1);

        static_assert(math::subtract_with_overflow(0u, 0) == 0);
        static_assert(math::subtract_with_overflow(22u, 7) == 15);
        static_assert(math::subtract_with_overflow(u32{}, 1) == constant::limit<u32>::max);
        static_assert(math::subtract_with_overflow(u32{0}, constant::limit<u32>::max) == 1);
        static_assert(math::subtract_with_overflow(u32{1}, constant::limit<u32>::max) == 2);

        static_assert(math::add_with_saturation(0u, 0) == 0);
        static_assert(math::add_with_saturation(3u, 4) == 7);

        static_assert(math::add_with_saturation(u8{100}, 99) == 199);
        static_assert(math::add_with_saturation(u8{100}, 170) == constant::limit<u8>::max);

        static_assert(math::add_with_saturation(u32{4'000'000'000}, 99) == 4'000'000'099);
        static_assert(math::add_with_saturation(u32{4'000'000'000}, 300'000'000) ==
                      constant::limit<u32>::max);

        static_assert(math::add_with_saturation(u64{1'234'567'890'000}, 99) == 1'234'567'890'099);
        static_assert(math::add_with_saturation(constant::limit<u64>::max, 99) ==
                      constant::limit<u64>::max);

        static_assert(math::multiply_with_saturation(0u, 0) == 0);
        static_assert(math::multiply_with_saturation(3u, 4) == 12);

        static_assert(math::multiply_with_saturation(u8{105}, 0) == 0);
        static_assert(math::multiply_with_saturation(u8{105}, 1) == 105);
        static_assert(math::multiply_with_saturation(u8{105}, 2) == 210);
        static_assert(math::multiply_with_saturation(u8{105}, 3) == constant::limit<u8>::max);

        static_assert(math::multiply_with_saturation(u32{90'000}, 3) == 270'000);
        static_assert(math::multiply_with_saturation(u32{90'000}, 90'000) ==
                      constant::limit<u32>::max);

        static_assert(math::multiply_with_saturation(u64{9'000'000'000'000'000}, 3) ==
                      27'000'000'000'000'000);
        static_assert(math::multiply_with_saturation(u64{9'000'000'000'000'000}, 3'000) ==
                      constant::limit<u64>::max);

        static_assert(math::subtract_with_saturation(0u, 0) == 0);
        static_assert(math::subtract_with_saturation(8u, 5) == 3);

        static_assert(math::subtract_with_saturation(u8{100}, 75) == 25);
        static_assert(math::subtract_with_saturation(u8{100}, 100) == 0);
        static_assert(math::subtract_with_saturation(u8{100}, 105) == 0);
        static_assert(math::subtract_with_saturation(u8{0}, constant::limit<u8>::max) == 0);
        static_assert(math::subtract_with_saturation(u8{99}, constant::limit<u8>::max) == 0);

        static_assert(math::subtract_with_saturation(u32{90'000}, 70'000) == 20'000);
        static_assert(math::subtract_with_saturation(u32{0}, constant::limit<u32>::max) == 0);
        static_assert(math::subtract_with_saturation(u32{99}, constant::limit<u32>::max) == 0);

        static_assert(math::subtract_with_saturation(u64{1'234'567'890'000}, 1'234'567'000'000) ==
                      890'000);
        static_assert(math::subtract_with_saturation(u64{0}, constant::limit<u64>::max) == 0);
        static_assert(math::subtract_with_saturation(u64{99}, constant::limit<u64>::max) == 0);

        static_assert(math::abs(i32{}) == 0);
        static_assert(math::abs(u32{}) == 0);

        static_assert(math::abs(i32{-123}) == 123);
        static_assert(math::abs(u32{123}) == 123);

        static_assert(std::is_same_v<decltype(math::abs(i8{})), u8>);
        static_assert(std::is_same_v<decltype(math::abs(i16{})), u16>);
        static_assert(std::is_same_v<decltype(math::abs(i32{})), u32>);
        static_assert(std::is_same_v<decltype(math::abs(i64{})), u64>);

        static_assert(std::is_same_v<decltype(math::abs(u8{})), u8>);
        static_assert(std::is_same_v<decltype(math::abs(u16{})), u16>);
        static_assert(std::is_same_v<decltype(math::abs(u32{})), u32>);
        static_assert(std::is_same_v<decltype(math::abs(u64{})), u64>);

        static_assert(math::abs<i8>(0) == 0);
        static_assert(math::abs<i8>(37) == 37u);
        static_assert(math::abs<i8>(-42) == 42u);
        static_assert(math::abs(constant::limit<i8>::max) == 127u);
        static_assert(math::abs(constant::limit<i8>::min) == 128u);
        static_assert(math::abs(constant::limit<u8>::max) == constant::limit<u8>::max);

        static_assert(math::abs<i16>(0) == 0);
        static_assert(math::abs<i16>(379) == 379u);
        static_assert(math::abs<i16>(-19381) == 19381u);
        static_assert(math::abs(constant::limit<i16>::max) == 32767u);
        static_assert(math::abs(constant::limit<i16>::min) == 32768u);
        static_assert(math::abs(constant::limit<u16>::max) == constant::limit<u16>::max);

        static_assert(math::abs<i32>(0) == 0);
        static_assert(math::abs<i32>(3793829) == 3793829u);
        static_assert(math::abs<i32>(-1938170182) == 1938170182u);
        static_assert(math::abs(constant::limit<i32>::max) == 2147483647u);
        static_assert(math::abs(constant::limit<i32>::min) == 2147483648u);
        static_assert(math::abs(constant::limit<u32>::max) == constant::limit<u32>::max);

        static_assert(math::abs<i64>(0) == 0);
        static_assert(math::abs<i64>(379382989233) == 379382989233u);
        static_assert(math::abs<i64>(-493817018283926103) == 493817018283926103u);
        static_assert(math::abs(constant::limit<i64>::max) == 9223372036854775807u);
        static_assert(math::abs(constant::limit<i64>::min) == 9223372036854775808u);
        static_assert(math::abs(constant::limit<u64>::max) == constant::limit<u64>::max);

        static_assert(math::abs_diff(0, 0) == 0);
        static_assert(math::abs_diff(5, 5) == 0);
        static_assert(math::abs_diff(-5, -5) == 0);
        static_assert(math::abs_diff(-5, 5) == 10);
        static_assert(math::abs_diff(5, -5) == 10);
        static_assert(math::abs_diff(0, 10) == 10);
        static_assert(math::abs_diff(10, 0) == 10);
        static_assert(math::abs_diff(20u, 10u) == 10);
        static_assert(math::abs_diff(10u, 20u) == 10);
        static_assert(math::abs_diff(-10, 0) == 10);
        static_assert(math::abs_diff(0, -10) == 10);
        static_assert(math::abs_diff(-10, -20) == 10);
        static_assert(math::abs_diff(-20, -10) == 10);

        static_assert(math::abs_diff(constant::limit<i8>::min, constant::limit<i8>::max) ==
                      constant::limit<u8>::max);
        static_assert(math::abs_diff(constant::limit<i8>::max, constant::limit<i8>::min) ==
                      constant::limit<u8>::max);

        static_assert(math::abs_diff(constant::limit<i64>::min, constant::limit<i64>::max) ==
                      constant::limit<u64>::max);
        static_assert(math::abs_diff(constant::limit<i64>::max, constant::limit<i64>::min) ==
                      constant::limit<u64>::max);

        static_assert(math::abs_diff(constant::limit<i64>::min, constant::limit<i64>::min) == 0);
        static_assert(math::abs_diff(constant::limit<i64>::max, constant::limit<i64>::max) == 0);

        static_assert(math::abs_diff(constant::limit<u32>::min, constant::limit<u32>::max) ==
                      constant::limit<u32>::max);
        static_assert(math::abs_diff(constant::limit<u32>::max, constant::limit<u32>::min) ==
                      constant::limit<u32>::max);

        static_assert(math::abs_diff(constant::limit<u32>::min, constant::limit<u32>::min) == 0);
        static_assert(math::abs_diff(constant::limit<u32>::max, constant::limit<u32>::max) == 0);

        static_assert(math::clamp(99, 1, 123) == 99);
        static_assert(math::clamp(1, 1, 123) == 1);
        static_assert(math::clamp(123, 1, 123) == 123);

        static_assert(math::clamp(0, 1, 123) == 1);
        static_assert(math::clamp(-99, 1, 123) == 1);

        static_assert(math::clamp(124, 1, 123) == 123);
        static_assert(math::clamp(999, 1, 123) == 123);

        static_assert(math::clamp<u8>(-1) == 0); // Clamped to min.
        static_assert(math::clamp<u8>(0) == 0);
        static_assert(math::clamp<u8>(123) == 123);
        static_assert(math::clamp<u8>(255) == 255);
        static_assert(math::clamp<u8>(256) == 255); // Clamped to max.
        static_assert(std::is_same_v<decltype(math::clamp<u8>(123)), u8>);

        static_assert(math::clamp<usize>(123) == 123);
        static_assert(math::clamp<usize>(-123) == 0);
        static_assert(std::is_same_v<decltype(math::clamp<usize>(-123)), usize>);

        static_assert(math::clamp<usize>(constant::limit<i32>::min) == constant::limit<usize>::min);
        static_assert(math::clamp<usize>(constant::limit<i32>::max) == constant::limit<i32>::max);
        static_assert(math::clamp<usize>(constant::limit<u32>::min) == constant::limit<u32>::min);
        static_assert(math::clamp<usize>(constant::limit<u32>::max) == constant::limit<u32>::max);

        static_assert(math::clamp<usize>(constant::limit<i64>::min) == constant::limit<usize>::min);
        static_assert(math::clamp<usize>(constant::limit<i64>::max) == constant::limit<i64>::max);
        static_assert(math::clamp<usize>(constant::limit<u64>::min) == constant::limit<u64>::min);
        static_assert(math::clamp<usize>(constant::limit<u64>::max) == constant::limit<u64>::max);

        static_assert(math::clamp<isize>(constant::limit<i32>::min) == constant::limit<i32>::min);
        static_assert(math::clamp<isize>(constant::limit<i32>::max) == constant::limit<i32>::max);
        static_assert(math::clamp<isize>(constant::limit<u32>::min) == constant::limit<u32>::min);
        static_assert(math::clamp<isize>(constant::limit<u32>::max) == constant::limit<u32>::max);

        static_assert(math::clamp<isize>(constant::limit<i64>::min) == constant::limit<i64>::min);
        static_assert(math::clamp<isize>(constant::limit<i64>::max) == constant::limit<i64>::max);
        static_assert(math::clamp<isize>(constant::limit<u64>::min) == constant::limit<u64>::min);
        static_assert(math::clamp<isize>(constant::limit<u64>::max) == constant::limit<isize>::max);

        static_assert(math::clamp<u32>(constant::limit<i32>::min) == constant::limit<u32>::min);
        static_assert(math::clamp<u32>(constant::limit<i32>::max) == constant::limit<i32>::max);
        static_assert(math::clamp<u32>(constant::limit<u32>::min) == constant::limit<u32>::min);
        static_assert(math::clamp<u32>(constant::limit<u32>::max) == constant::limit<u32>::max);

        static_assert(math::clamp<u32>(constant::limit<i64>::min) == constant::limit<u32>::min);
        static_assert(math::clamp<u32>(constant::limit<i64>::max) == constant::limit<u32>::max);
        static_assert(math::clamp<u32>(constant::limit<u64>::min) == constant::limit<u64>::min);
        static_assert(math::clamp<u32>(constant::limit<u64>::max) == constant::limit<u32>::max);

        static_assert(math::clamp<i32>(constant::limit<i32>::min) == constant::limit<i32>::min);
        static_assert(math::clamp<i32>(constant::limit<i32>::max) == constant::limit<i32>::max);
        static_assert(math::clamp<i32>(constant::limit<u32>::min) == constant::limit<u32>::min);
        static_assert(math::clamp<i32>(constant::limit<u32>::max) == constant::limit<i32>::max);

        static_assert(math::clamp<i32>(constant::limit<i64>::min) == constant::limit<i32>::min);
        static_assert(math::clamp<i32>(constant::limit<i64>::max) == constant::limit<i32>::max);
        static_assert(math::clamp<i32>(constant::limit<u64>::min) == constant::limit<u64>::min);
        static_assert(math::clamp<i32>(constant::limit<u64>::max) == constant::limit<i32>::max);

#if SNN_INT128_BOOL
        static_assert(math::clamp<u32>(constant::limit<i128>::min) == constant::limit<u32>::min);
        static_assert(math::clamp<u32>(constant::limit<i128>::max) == constant::limit<u32>::max);
        static_assert(math::clamp<u32>(constant::limit<u128>::min) == constant::limit<u128>::min);
        static_assert(math::clamp<u32>(constant::limit<u128>::max) == constant::limit<u32>::max);

        static_assert(math::clamp<i32>(constant::limit<i128>::min) == constant::limit<i32>::min);
        static_assert(math::clamp<i32>(constant::limit<i128>::max) == constant::limit<i32>::max);
        static_assert(math::clamp<i32>(constant::limit<u128>::min) == constant::limit<u128>::min);
        static_assert(math::clamp<i32>(constant::limit<u128>::max) == constant::limit<i32>::max);
#endif

        using math::base;

        static_assert(math::count_digits<base::decimal, u8>(0).get() == 1);
        static_assert(math::count_digits<base::decimal, u8>(1).get() == 1);
        static_assert(math::count_digits<base::decimal, u8>(9).get() == 1);
        static_assert(math::count_digits<base::decimal, u8>(10).get() == 2);
        static_assert(math::count_digits<base::decimal, u8>(99).get() == 2);
        static_assert(math::count_digits<base::decimal, u8>(100).get() == 3);
        static_assert(math::count_digits<base::decimal, u8>(255).get() == 3);

        static_assert(math::count_digits<base::decimal, u16>(0).get() == 1);
        static_assert(math::count_digits<base::decimal, u16>(1).get() == 1);
        static_assert(math::count_digits<base::decimal, u16>(9).get() == 1);
        static_assert(math::count_digits<base::decimal, u16>(10).get() == 2);
        static_assert(math::count_digits<base::decimal, u16>(99).get() == 2);
        static_assert(math::count_digits<base::decimal, u16>(100).get() == 3);
        static_assert(math::count_digits<base::decimal, u16>(999).get() == 3);
        static_assert(math::count_digits<base::decimal, u16>(1000).get() == 4);
        static_assert(math::count_digits<base::decimal, u16>(9999).get() == 4);
        static_assert(math::count_digits<base::decimal, u16>(10000).get() == 5);
        static_assert(math::count_digits<base::decimal, u16>(65535).get() == 5);

        static_assert(math::count_digits<base::decimal, u32>(0).get() == 1);
        static_assert(math::count_digits<base::decimal, u32>(1).get() == 1);
        static_assert(math::count_digits<base::decimal, u32>(10).get() == 2);
        static_assert(math::count_digits<base::decimal, u32>(100).get() == 3);
        static_assert(math::count_digits<base::decimal, u32>(1000).get() == 4);
        static_assert(math::count_digits<base::decimal, u32>(10000).get() == 5);
        static_assert(math::count_digits<base::decimal, u32>(100000).get() == 6);
        static_assert(math::count_digits<base::decimal, u32>(1000000).get() == 7);
        static_assert(math::count_digits<base::decimal, u32>(10000000).get() == 8);
        static_assert(math::count_digits<base::decimal, u32>(100000000).get() == 9);
        static_assert(math::count_digits<base::decimal, u32>(1000000000).get() == 10);

        static_assert(math::count_digits<base::decimal, u32>(9).get() == 1);
        static_assert(math::count_digits<base::decimal, u32>(99).get() == 2);
        static_assert(math::count_digits<base::decimal, u32>(999).get() == 3);
        static_assert(math::count_digits<base::decimal, u32>(9999).get() == 4);
        static_assert(math::count_digits<base::decimal, u32>(99999).get() == 5);
        static_assert(math::count_digits<base::decimal, u32>(999999).get() == 6);
        static_assert(math::count_digits<base::decimal, u32>(9999999).get() == 7);
        static_assert(math::count_digits<base::decimal, u32>(99999999).get() == 8);
        static_assert(math::count_digits<base::decimal, u32>(999999999).get() == 9);
        static_assert(math::count_digits<base::decimal, u32>(4294967295).get() == 10);

        static_assert(math::count_digits<base::decimal, u32>(3).get() == 1);
        static_assert(math::count_digits<base::decimal, u32>(15).get() == 2);
        static_assert(math::count_digits<base::decimal, u32>(672).get() == 3);
        static_assert(math::count_digits<base::decimal, u32>(2145).get() == 4);
        static_assert(math::count_digits<base::decimal, u32>(68431).get() == 5);
        static_assert(math::count_digits<base::decimal, u32>(737120).get() == 6);
        static_assert(math::count_digits<base::decimal, u32>(3756843).get() == 7);
        static_assert(math::count_digits<base::decimal, u32>(36705021).get() == 8);
        static_assert(math::count_digits<base::decimal, u32>(571667464).get() == 9);

        static_assert(math::count_digits<base::decimal, u64>(0).get() == 1);
        static_assert(math::count_digits<base::decimal, u64>(1).get() == 1);
        static_assert(math::count_digits<base::decimal, u64>(10).get() == 2);
        static_assert(math::count_digits<base::decimal, u64>(100).get() == 3);
        static_assert(math::count_digits<base::decimal, u64>(1000).get() == 4);
        static_assert(math::count_digits<base::decimal, u64>(10000).get() == 5);
        static_assert(math::count_digits<base::decimal, u64>(100000).get() == 6);
        static_assert(math::count_digits<base::decimal, u64>(1000000).get() == 7);
        static_assert(math::count_digits<base::decimal, u64>(10000000).get() == 8);
        static_assert(math::count_digits<base::decimal, u64>(100000000).get() == 9);
        static_assert(math::count_digits<base::decimal, u64>(1000000000).get() == 10);
        static_assert(math::count_digits<base::decimal, u64>(10000000000).get() == 11);
        static_assert(math::count_digits<base::decimal, u64>(100000000000).get() == 12);
        static_assert(math::count_digits<base::decimal, u64>(1000000000000).get() == 13);
        static_assert(math::count_digits<base::decimal, u64>(10000000000000).get() == 14);
        static_assert(math::count_digits<base::decimal, u64>(100000000000000).get() == 15);
        static_assert(math::count_digits<base::decimal, u64>(1000000000000000).get() == 16);
        static_assert(math::count_digits<base::decimal, u64>(10000000000000000).get() == 17);
        static_assert(math::count_digits<base::decimal, u64>(100000000000000000).get() == 18);
        static_assert(math::count_digits<base::decimal, u64>(1000000000000000000).get() == 19);
        static_assert(math::count_digits<base::decimal, u64>(10000000000000000000u).get() == 20);

        static_assert(math::count_digits<base::decimal, u64>(9).get() == 1);
        static_assert(math::count_digits<base::decimal, u64>(99).get() == 2);
        static_assert(math::count_digits<base::decimal, u64>(999).get() == 3);
        static_assert(math::count_digits<base::decimal, u64>(9999).get() == 4);
        static_assert(math::count_digits<base::decimal, u64>(99999).get() == 5);
        static_assert(math::count_digits<base::decimal, u64>(999999).get() == 6);
        static_assert(math::count_digits<base::decimal, u64>(9999999).get() == 7);
        static_assert(math::count_digits<base::decimal, u64>(99999999).get() == 8);
        static_assert(math::count_digits<base::decimal, u64>(999999999).get() == 9);
        static_assert(math::count_digits<base::decimal, u64>(9999999999).get() == 10);
        static_assert(math::count_digits<base::decimal, u64>(99999999999).get() == 11);
        static_assert(math::count_digits<base::decimal, u64>(999999999999).get() == 12);
        static_assert(math::count_digits<base::decimal, u64>(9999999999999).get() == 13);
        static_assert(math::count_digits<base::decimal, u64>(99999999999999).get() == 14);
        static_assert(math::count_digits<base::decimal, u64>(999999999999999).get() == 15);
        static_assert(math::count_digits<base::decimal, u64>(9999999999999999).get() == 16);
        static_assert(math::count_digits<base::decimal, u64>(99999999999999999).get() == 17);
        static_assert(math::count_digits<base::decimal, u64>(999999999999999999).get() == 18);
        static_assert(math::count_digits<base::decimal, u64>(9999999999999999999u).get() == 19);
        static_assert(math::count_digits<base::decimal, u64>(18446744073709551615u).get() == 20);

        static_assert(math::count_digits<base::decimal, u64>(3).get() == 1);
        static_assert(math::count_digits<base::decimal, u64>(15).get() == 2);
        static_assert(math::count_digits<base::decimal, u64>(672).get() == 3);
        static_assert(math::count_digits<base::decimal, u64>(2145).get() == 4);
        static_assert(math::count_digits<base::decimal, u64>(68431).get() == 5);
        static_assert(math::count_digits<base::decimal, u64>(737120).get() == 6);
        static_assert(math::count_digits<base::decimal, u64>(3756843).get() == 7);
        static_assert(math::count_digits<base::decimal, u64>(36705021).get() == 8);
        static_assert(math::count_digits<base::decimal, u64>(571667464).get() == 9);
        static_assert(math::count_digits<base::decimal, u64>(6623513324).get() == 10);
        static_assert(math::count_digits<base::decimal, u64>(25647755784).get() == 11);
        static_assert(math::count_digits<base::decimal, u64>(376802208140).get() == 12);
        static_assert(math::count_digits<base::decimal, u64>(1181523785732).get() == 13);
        static_assert(math::count_digits<base::decimal, u64>(82185827155483).get() == 14);
        static_assert(math::count_digits<base::decimal, u64>(441272465483060).get() == 15);
        static_assert(math::count_digits<base::decimal, u64>(7450688310168308).get() == 16);
        static_assert(math::count_digits<base::decimal, u64>(28733662153401566).get() == 17);
        static_assert(math::count_digits<base::decimal, u64>(777043303305485181).get() == 18);
        static_assert(math::count_digits<base::decimal, u64>(4273103853458024124).get() == 19);
        static_assert(math::count_digits<base::decimal, u64>(15308084750314874172u).get() == 20);

        // Random
        for (loop::count lc{100}; lc--;)
        {
            const auto rnd_num = random::number<u64>();
            for (const auto shift_n : range::step<u32>{0, 64, 8})
            {
                u64 i = rnd_num;
                i >>= shift_n;
                snn_require(math::count_digits<base::decimal>(i).get() == app::count_digits_ref(i));
            }
        }

#if SNN_INT128_BOOL
        static_assert(math::count_digits<base::decimal, u128>(0).get() == 1);
        static_assert(math::count_digits<base::decimal, u128>(1).get() == 1);
        static_assert(math::count_digits<base::decimal, u128>(10).get() == 2);
        static_assert(math::count_digits<base::decimal, u128>(100).get() == 3);
        static_assert(math::count_digits<base::decimal, u128>(1000).get() == 4);
        static_assert(math::count_digits<base::decimal, u128>(10000).get() == 5);
        static_assert(math::count_digits<base::decimal, u128>(100000).get() == 6);
        static_assert(math::count_digits<base::decimal, u128>(1000000).get() == 7);
        static_assert(math::count_digits<base::decimal, u128>(10000000).get() == 8);
        static_assert(math::count_digits<base::decimal, u128>(100000000).get() == 9);
        static_assert(math::count_digits<base::decimal, u128>(1000000000).get() == 10);
        static_assert(math::count_digits<base::decimal, u128>(10000000000).get() == 11);
        static_assert(math::count_digits<base::decimal, u128>(100000000000).get() == 12);
        static_assert(math::count_digits<base::decimal, u128>(1000000000000).get() == 13);
        static_assert(math::count_digits<base::decimal, u128>(10000000000000).get() == 14);
        static_assert(math::count_digits<base::decimal, u128>(100000000000000).get() == 15);
        static_assert(math::count_digits<base::decimal, u128>(1000000000000000).get() == 16);
        static_assert(math::count_digits<base::decimal, u128>(10000000000000000).get() == 17);
        static_assert(math::count_digits<base::decimal, u128>(100000000000000000).get() == 18);
        static_assert(math::count_digits<base::decimal, u128>(1000000000000000000).get() == 19);
        static_assert(math::count_digits<base::decimal, u128>(10000000000000000000u).get() == 20);

        static_assert(math::count_digits<base::decimal, u128>(9).get() == 1);
        static_assert(math::count_digits<base::decimal, u128>(99).get() == 2);
        static_assert(math::count_digits<base::decimal, u128>(999).get() == 3);
        static_assert(math::count_digits<base::decimal, u128>(9999).get() == 4);
        static_assert(math::count_digits<base::decimal, u128>(99999).get() == 5);
        static_assert(math::count_digits<base::decimal, u128>(999999).get() == 6);
        static_assert(math::count_digits<base::decimal, u128>(9999999).get() == 7);
        static_assert(math::count_digits<base::decimal, u128>(99999999).get() == 8);
        static_assert(math::count_digits<base::decimal, u128>(999999999).get() == 9);
        static_assert(math::count_digits<base::decimal, u128>(9999999999).get() == 10);
        static_assert(math::count_digits<base::decimal, u128>(99999999999).get() == 11);
        static_assert(math::count_digits<base::decimal, u128>(999999999999).get() == 12);
        static_assert(math::count_digits<base::decimal, u128>(9999999999999).get() == 13);
        static_assert(math::count_digits<base::decimal, u128>(99999999999999).get() == 14);
        static_assert(math::count_digits<base::decimal, u128>(999999999999999).get() == 15);
        static_assert(math::count_digits<base::decimal, u128>(9999999999999999).get() == 16);
        static_assert(math::count_digits<base::decimal, u128>(99999999999999999).get() == 17);
        static_assert(math::count_digits<base::decimal, u128>(999999999999999999).get() == 18);
        static_assert(math::count_digits<base::decimal, u128>(9999999999999999999u).get() == 19);
        static_assert(math::count_digits<base::decimal, u128>(constant::limit<u128>::max).get() ==
                      39);

        static_assert(math::count_digits<base::binary>(constant::limit<u128>::max).get() == 128);
        static_assert(math::count_digits<base::hex>(constant::limit<u128>::max).get() == 32);
#endif

        static_assert(math::count_digits<base::binary>(0u).get() == 1);
        static_assert(math::count_digits<base::binary>(1u).get() == 1);
        static_assert(math::count_digits<base::binary>(10u).get() == 4);
        static_assert(math::count_digits<base::binary>(100u).get() == 7);
        static_assert(math::count_digits<base::binary>(255u).get() == 8);
        static_assert(math::count_digits<base::binary>(256u).get() == 9);

        static_assert(math::count_digits<base::octal>(0u).get() == 1);
        static_assert(math::count_digits<base::octal>(1u).get() == 1);
        static_assert(math::count_digits<base::octal>(7u).get() == 1);
        static_assert(math::count_digits<base::octal>(8u).get() == 2);
        static_assert(math::count_digits<base::octal>(63u).get() == 2);
        static_assert(math::count_digits<base::octal>(64u).get() == 3);
        static_assert(math::count_digits<base::octal>(511u).get() == 3);
        static_assert(math::count_digits<base::octal>(512u).get() == 4);

        static_assert(math::count_digits<base::hex>(0u).get() == 1);
        static_assert(math::count_digits<base::hex>(1u).get() == 1);
        static_assert(math::count_digits<base::hex>(10u).get() == 1);
        static_assert(math::count_digits<base::hex>(100u).get() == 2);
        static_assert(math::count_digits<base::hex>(255u).get() == 2);
        static_assert(math::count_digits<base::hex>(256u).get() == 3);

        static_assert(math::max_digits<i8, base::binary>().get() == 8);
        static_assert(math::max_digits<u8, base::binary>().get() == 8);
        static_assert(math::max_digits<i16, base::binary>().get() == 16);
        static_assert(math::max_digits<u16, base::binary>().get() == 16);
        static_assert(math::max_digits<i32, base::binary>().get() == 32);
        static_assert(math::max_digits<u32, base::binary>().get() == 32);
        static_assert(math::max_digits<i64, base::binary>().get() == 64);
        static_assert(math::max_digits<u64, base::binary>().get() == 64);

        static_assert(math::max_digits<i8, base::decimal>().get() == 3);
        static_assert(math::max_digits<u8, base::decimal>().get() == 3);
        static_assert(math::max_digits<i16, base::decimal>().get() == 5);
        static_assert(math::max_digits<u16, base::decimal>().get() == 5);
        static_assert(math::max_digits<i32, base::decimal>().get() == 10);
        static_assert(math::max_digits<u32, base::decimal>().get() == 10);
        static_assert(math::max_digits<i64, base::decimal>().get() == 19);
        static_assert(math::max_digits<u64, base::decimal>().get() == 20);

        static_assert(math::max_digits<i8, base::hex>().get() == 2);
        static_assert(math::max_digits<u8, base::hex>().get() == 2);
        static_assert(math::max_digits<i16, base::hex>().get() == 4);
        static_assert(math::max_digits<u16, base::hex>().get() == 4);
        static_assert(math::max_digits<i32, base::hex>().get() == 8);
        static_assert(math::max_digits<u32, base::hex>().get() == 8);
        static_assert(math::max_digits<i64, base::hex>().get() == 16);
        static_assert(math::max_digits<u64, base::hex>().get() == 16);

        static_assert(math::max_digits<i8, base::octal>().get() == 3);
        static_assert(math::max_digits<u8, base::octal>().get() == 3);
        static_assert(math::max_digits<i16, base::octal>().get() == 6);
        static_assert(math::max_digits<u16, base::octal>().get() == 6);
        static_assert(math::max_digits<i32, base::octal>().get() == 11);
        static_assert(math::max_digits<u32, base::octal>().get() == 11);
        static_assert(math::max_digits<i64, base::octal>().get() == 22);
        static_assert(math::max_digits<u64, base::octal>().get() == 22);

#if SNN_INT128_BOOL
        static_assert(math::max_digits<i128, base::binary>().get() == 128);
        static_assert(math::max_digits<u128, base::binary>().get() == 128);

        static_assert(math::max_digits<i128, base::decimal>().get() == 39);
        static_assert(math::max_digits<u128, base::decimal>().get() == 39);

        static_assert(math::max_digits<i128, base::hex>().get() == 32);
        static_assert(math::max_digits<u128, base::hex>().get() == 32);

        static_assert(math::max_digits<i128, base::octal>().get() == 43);
        static_assert(math::max_digits<u128, base::octal>().get() == 43);
#endif

        static_assert(math::is_power_of_two(1u));
        static_assert(math::is_power_of_two(2u));
        static_assert(math::is_power_of_two(4u));
        static_assert(math::is_power_of_two(8u));
        static_assert(math::is_power_of_two(16u));
        static_assert(math::is_power_of_two(32u));
        static_assert(math::is_power_of_two(64u));
        static_assert(math::is_power_of_two(128u));
        static_assert(math::is_power_of_two(256u));
        static_assert(math::is_power_of_two(512u));
        static_assert(math::is_power_of_two(1024u));
        static_assert(math::is_power_of_two(2048u));
        static_assert(math::is_power_of_two(4096u));
        static_assert(math::is_power_of_two(8192u));
        static_assert(math::is_power_of_two(16384u));
        static_assert(math::is_power_of_two(32768u));
        static_assert(math::is_power_of_two(65536u));
        static_assert(math::is_power_of_two(131072u));
        static_assert(math::is_power_of_two(262144u));

        static_assert(!math::is_power_of_two(0u));
        static_assert(!math::is_power_of_two(3u));
        static_assert(!math::is_power_of_two(5u));
        static_assert(!math::is_power_of_two(6u));
        static_assert(!math::is_power_of_two(7u));
        static_assert(!math::is_power_of_two(9u));
        static_assert(!math::is_power_of_two(10u));
        static_assert(!math::is_power_of_two(11u));
        static_assert(!math::is_power_of_two(12u));
        static_assert(!math::is_power_of_two(13u));
        static_assert(!math::is_power_of_two(14u));
        static_assert(!math::is_power_of_two(15u));
        static_assert(!math::is_power_of_two(44u));
        static_assert(!math::is_power_of_two(1234u));
        static_assert(!math::is_power_of_two(3000u));
        static_assert(!math::is_power_of_two(64000u));

        static_assert(math::is_within_bounds<usize>(0));
        static_assert(math::is_within_bounds<usize>(123));
        static_assert(math::is_within_bounds<usize>(isize{}));
        static_assert(math::is_within_bounds<usize>(constant::limit<i8>::max));
        static_assert(math::is_within_bounds<usize>(constant::limit<i16>::max));
        static_assert(math::is_within_bounds<usize>(constant::limit<i32>::max));
        static_assert(math::is_within_bounds<usize>(constant::limit<i64>::max));
        static_assert(math::is_within_bounds<usize>(constant::limit<u8>::min));
        static_assert(math::is_within_bounds<usize>(constant::limit<u16>::min));
        static_assert(math::is_within_bounds<usize>(constant::limit<u32>::min));
        static_assert(math::is_within_bounds<usize>(constant::limit<u64>::min));
        static_assert(math::is_within_bounds<usize>(constant::limit<u8>::max));
        static_assert(math::is_within_bounds<usize>(constant::limit<u16>::max));
        static_assert(math::is_within_bounds<usize>(constant::limit<u32>::max));
        static_assert(math::is_within_bounds<usize>(constant::limit<u64>::max));
        static_assert(!math::is_within_bounds<usize>(-1));
        static_assert(!math::is_within_bounds<usize>(constant::limit<i8>::min));
        static_assert(!math::is_within_bounds<usize>(constant::limit<i16>::min));
        static_assert(!math::is_within_bounds<usize>(constant::limit<i32>::min));
        static_assert(!math::is_within_bounds<usize>(constant::limit<i64>::min));
#if SNN_INT128_BOOL
        static_assert(math::is_within_bounds<usize>(constant::limit<u128>::min));
        static_assert(!math::is_within_bounds<usize>(constant::limit<u128>::max));
        static_assert(!math::is_within_bounds<usize>(constant::limit<i128>::min));
        static_assert(!math::is_within_bounds<usize>(constant::limit<i128>::max));

        static_assert(math::is_within_bounds<u128>(constant::limit<i64>::max));
        static_assert(math::is_within_bounds<u128>(constant::limit<u64>::min));
        static_assert(math::is_within_bounds<u128>(constant::limit<u64>::max));
        static_assert(math::is_within_bounds<u128>(constant::limit<u128>::min));
        static_assert(math::is_within_bounds<u128>(constant::limit<u128>::max));
        static_assert(math::is_within_bounds<u128>(constant::limit<i128>::max));
        static_assert(!math::is_within_bounds<u128>(constant::limit<i64>::min));
        static_assert(!math::is_within_bounds<u128>(constant::limit<i128>::min));
#endif

        static_assert(math::is_within_bounds<isize>(-1));
        static_assert(math::is_within_bounds<isize>(0u));
        static_assert(math::is_within_bounds<isize>(123u));
        static_assert(math::is_within_bounds<isize>(constant::limit<i8>::min));
        static_assert(math::is_within_bounds<isize>(constant::limit<i16>::min));
        static_assert(math::is_within_bounds<isize>(constant::limit<i32>::min));
        static_assert(math::is_within_bounds<isize>(constant::limit<i64>::min));
        static_assert(math::is_within_bounds<isize>(constant::limit<i8>::max));
        static_assert(math::is_within_bounds<isize>(constant::limit<i16>::max));
        static_assert(math::is_within_bounds<isize>(constant::limit<i32>::max));
        static_assert(math::is_within_bounds<isize>(constant::limit<i64>::max));
        static_assert(math::is_within_bounds<isize>(constant::limit<u8>::min));
        static_assert(math::is_within_bounds<isize>(constant::limit<u16>::min));
        static_assert(math::is_within_bounds<isize>(constant::limit<u32>::min));
        static_assert(math::is_within_bounds<isize>(constant::limit<u64>::min));
        static_assert(!math::is_within_bounds<isize>(constant::limit<usize>::max));
#if SNN_INT128_BOOL
        static_assert(math::is_within_bounds<isize>(constant::limit<u128>::min));
        static_assert(!math::is_within_bounds<isize>(constant::limit<u128>::max));
        static_assert(!math::is_within_bounds<isize>(constant::limit<i128>::min));
        static_assert(!math::is_within_bounds<isize>(constant::limit<i128>::max));

        static_assert(math::is_within_bounds<i128>(constant::limit<i64>::min));
        static_assert(math::is_within_bounds<i128>(constant::limit<i64>::max));
        static_assert(math::is_within_bounds<i128>(constant::limit<u64>::min));
        static_assert(math::is_within_bounds<i128>(constant::limit<u64>::max));
        static_assert(math::is_within_bounds<i128>(constant::limit<i128>::max));
        static_assert(math::is_within_bounds<i128>(constant::limit<i128>::min));
        static_assert(math::is_within_bounds<i128>(constant::limit<u128>::min));
        static_assert(!math::is_within_bounds<i128>(constant::limit<u128>::max));
#endif

        static_assert(math::is_within_bounds<u8>(0));
        static_assert(math::is_within_bounds<u8>(123));
        static_assert(math::is_within_bounds<u8>(255));
        static_assert(math::is_within_bounds<u8>(constant::limit<i8>::max));
        static_assert(!math::is_within_bounds<u8>(-1));
        static_assert(!math::is_within_bounds<u8>(256));
        static_assert(!math::is_within_bounds<u8>(constant::limit<i8>::min));
        static_assert(!math::is_within_bounds<u8>(constant::limit<i16>::min));
        static_assert(!math::is_within_bounds<u8>(constant::limit<i32>::min));
        static_assert(!math::is_within_bounds<u8>(constant::limit<i64>::min));
        static_assert(!math::is_within_bounds<u8>(constant::limit<i16>::max));
        static_assert(!math::is_within_bounds<u8>(constant::limit<i32>::max));
        static_assert(!math::is_within_bounds<u8>(constant::limit<i64>::max));
        static_assert(!math::is_within_bounds<u8>(constant::limit<u16>::max));
        static_assert(!math::is_within_bounds<u8>(constant::limit<u32>::max));
        static_assert(!math::is_within_bounds<u8>(constant::limit<u64>::max));

        static_assert(math::is_within_bounds<i8>(0u));
        static_assert(math::is_within_bounds<i8>(127u));
        static_assert(math::is_within_bounds<i8>(-128));
        static_assert(math::is_within_bounds<i8>(constant::limit<u8>::min));
        static_assert(math::is_within_bounds<i8>(constant::limit<u16>::min));
        static_assert(math::is_within_bounds<i8>(constant::limit<u32>::min));
        static_assert(math::is_within_bounds<i8>(constant::limit<u64>::min));
        static_assert(!math::is_within_bounds<i8>(128u));
        static_assert(!math::is_within_bounds<i8>(-129));
        static_assert(!math::is_within_bounds<i8>(constant::limit<i16>::min));
        static_assert(!math::is_within_bounds<i8>(constant::limit<i32>::min));
        static_assert(!math::is_within_bounds<i8>(constant::limit<i64>::min));
        static_assert(!math::is_within_bounds<i8>(constant::limit<i16>::max));
        static_assert(!math::is_within_bounds<i8>(constant::limit<i32>::max));
        static_assert(!math::is_within_bounds<i8>(constant::limit<i64>::max));
        static_assert(!math::is_within_bounds<i8>(constant::limit<u8>::max));
        static_assert(!math::is_within_bounds<i8>(constant::limit<u16>::max));
        static_assert(!math::is_within_bounds<i8>(constant::limit<u32>::max));
        static_assert(!math::is_within_bounds<i8>(constant::limit<u64>::max));

        static_assert(math::max(1) == 1);
        static_assert(math::max(15) == 15);

        static_assert(math::max(0, 0) == 0);
        static_assert(math::max(1, 0) == 1);
        static_assert(math::max(0, 1) == 1);
        static_assert(math::max(1, 1) == 1);
        static_assert(math::max(15, 3) == 15);
        static_assert(math::max(147, 148) == 148);
        static_assert(math::max(-15, 3) == 3);
        static_assert(math::max(147, -148) == 147);

        static_assert(math::max(1, 2, 3) == 3);
        static_assert(math::max(1, 3, 2) == 3);
        static_assert(math::max(2, 1, 3) == 3);
        static_assert(math::max(3, 1, 2) == 3);
        static_assert(math::max(2, 3, 1) == 3);
        static_assert(math::max(3, 2, 1) == 3);

        static_assert(math::max(1, 2, 3, 4) == 4);
        static_assert(math::max(1, 4, 2, 3) == 4);
        static_assert(math::max(1, 3, 4, 2) == 4);
        static_assert(math::max(2, 1, 3, 4) == 4);
        static_assert(math::max(4, 1, 2, 3) == 4);
        static_assert(math::max(3, 1, 4, 2) == 4);
        static_assert(math::max(2, 3, 1, 4) == 4);
        static_assert(math::max(4, 2, 1, 3) == 4);
        static_assert(math::max(3, 4, 1, 2) == 4);
        static_assert(math::max(2, 3, 4, 1) == 4);
        static_assert(math::max(4, 2, 3, 1) == 4);
        static_assert(math::max(3, 4, 2, 1) == 4);

        static_assert(std::is_same_v<decltype(math::max(123, 1)), int>);
        static_assert(std::is_same_v<decltype(math::max(usize{123}, 1)), usize>);

        static_assert(math::min(1) == 1);
        static_assert(math::min(15) == 15);

        static_assert(math::min(0, 0) == 0);
        static_assert(math::min(1, 0) == 0);
        static_assert(math::min(0, 1) == 0);
        static_assert(math::min(1, 1) == 1);
        static_assert(math::min(15, 3) == 3);
        static_assert(math::min(147, 148) == 147);
        static_assert(math::min(-15, 3) == -15);
        static_assert(math::min(147, -148) == -148);

        static_assert(math::min(1, 2, 3) == 1);
        static_assert(math::min(1, 3, 2) == 1);
        static_assert(math::min(2, 1, 3) == 1);
        static_assert(math::min(3, 1, 2) == 1);
        static_assert(math::min(2, 3, 1) == 1);
        static_assert(math::min(3, 2, 1) == 1);

        static_assert(math::min(1, 2, 3, 4) == 1);
        static_assert(math::min(1, 4, 2, 3) == 1);
        static_assert(math::min(1, 3, 4, 2) == 1);
        static_assert(math::min(2, 1, 3, 4) == 1);
        static_assert(math::min(4, 1, 2, 3) == 1);
        static_assert(math::min(3, 1, 4, 2) == 1);
        static_assert(math::min(2, 3, 1, 4) == 1);
        static_assert(math::min(4, 2, 1, 3) == 1);
        static_assert(math::min(3, 4, 1, 2) == 1);
        static_assert(math::min(2, 3, 4, 1) == 1);
        static_assert(math::min(4, 2, 3, 1) == 1);
        static_assert(math::min(3, 4, 2, 1) == 1);

        static_assert(std::is_same_v<decltype(math::min(123, 1)), int>);
        static_assert(std::is_same_v<decltype(math::min(usize{123}, 1)), usize>);

        static_assert(math::pow10(0).get() == 1u);
        static_assert(math::pow10(1).get() == 10u);
        static_assert(math::pow10(2).get() == 100u);
        static_assert(math::pow10(3).get() == 1000u);
        static_assert(math::pow10(4).get() == 10000u);
        static_assert(math::pow10(5).get() == 100000u);
        static_assert(math::pow10(6).get() == 1000000u);
        static_assert(math::pow10(7).get() == 10000000u);
        static_assert(math::pow10(8).get() == 100000000u);
        static_assert(math::pow10(9).get() == 1000000000u);
        static_assert(math::pow10(10).get() == 10000000000u);
        static_assert(math::pow10(11).get() == 100000000000u);
        static_assert(math::pow10(12).get() == 1000000000000u);
        static_assert(math::pow10(13).get() == 10000000000000u);
        static_assert(math::pow10(14).get() == 100000000000000u);
        static_assert(math::pow10(15).get() == 1000000000000000u);
        static_assert(math::pow10(16).get() == 10000000000000000u);
        static_assert(math::pow10(17).get() == 100000000000000000u);
        static_assert(math::pow10(18).get() == 1000000000000000000u);
        static_assert(math::pow10(19).get() == 10000000000000000000u);
        snn_require_throws_code(math::pow10(20), num::error::out_of_range);

        static_assert(math::round_up_to_multiple<1>(0).get() == 1);
        static_assert(math::round_up_to_multiple<1>(1).get() == 1);
        static_assert(math::round_up_to_multiple<1>(2).get() == 2);
        static_assert(math::round_up_to_multiple<1>(3).get() == 3);

        static_assert(math::round_up_to_multiple<2>(0).get() == 2);
        static_assert(math::round_up_to_multiple<2>(1).get() == 2);
        static_assert(math::round_up_to_multiple<2>(2).get() == 2);
        static_assert(math::round_up_to_multiple<2>(3).get() == 4);

        static_assert(math::round_up_to_multiple<8>(0).get() == 8);
        static_assert(math::round_up_to_multiple<8>(1).get() == 8);
        static_assert(math::round_up_to_multiple<8>(7).get() == 8);
        static_assert(math::round_up_to_multiple<8>(8).get() == 8);
        static_assert(math::round_up_to_multiple<8>(9).get() == 16);
        static_assert(math::round_up_to_multiple<8>(15).get() == 16);
        static_assert(math::round_up_to_multiple<8>(16).get() == 16);
        static_assert(math::round_up_to_multiple<8>(17).get() == 24);

        static_assert(math::round_up_to_multiple<4096>(0).get() == 4096);
        static_assert(math::round_up_to_multiple<4096>(1).get() == 4096);
        static_assert(math::round_up_to_multiple<4096>(1234).get() == 4096);
        static_assert(math::round_up_to_multiple<4096>(4096).get() == 4096);
        static_assert(math::round_up_to_multiple<4096>(14000).get() == 16384);

        static_assert(math::round_up_to_multiple<4096, u32>(0).get() == 4096);
        static_assert(math::round_up_to_multiple<4096, u32>(1).get() == 4096);
        static_assert(math::round_up_to_multiple<4096, u32>(1234).get() == 4096);
        static_assert(math::round_up_to_multiple<4096, u32>(4096).get() == 4096);
        static_assert(math::round_up_to_multiple<4096, u32>(14000).get() == 16384);

        static_assert(math::round_up_to_multiple<4096, u16>(0).get() == 4096);
        static_assert(math::round_up_to_multiple<4096, u16>(1).get() == 4096);
        static_assert(math::round_up_to_multiple<4096, u16>(1234).get() == 4096);
        static_assert(math::round_up_to_multiple<4096, u16>(4096).get() == 4096);
        static_assert(math::round_up_to_multiple<4096, u16>(14000).get() == 16384);
        static_assert(math::round_up_to_multiple<4096, u16>(65535).get() == 65535);

        static_assert(
            std::is_same_v<decltype(math::round_up_to_multiple<4096>(1)), not_zero<usize>>);
        static_assert(
            std::is_same_v<decltype(math::round_up_to_multiple<4096, u32>(1)), not_zero<u32>>);

        // Overflow, value is returned unchanged.
        static_assert(math::round_up_to_multiple<4096>(constant::limit<usize>::max).get() ==
                      constant::limit<usize>::max);

        static_assert(math::within_bounds_or<usize>(0, 99) == 0);
        static_assert(math::within_bounds_or<usize>(2, 99) == 2);
        static_assert(math::within_bounds_or<usize>(constant::limit<usize>::max, 99) ==
                      constant::limit<usize>::max);

        static_assert(math::within_bounds_or<usize>(-1, 99) == 99);
        static_assert(math::within_bounds_or<usize>(-1, 0) == 0);

        static_assert(math::within_bounds_or<i8>(-128, 99) == -128);
        static_assert(math::within_bounds_or<i8>(127, 99) == 127);
        static_assert(math::within_bounds_or<i8>(127u, 99) == 127);

        static_assert(math::within_bounds_or<i8>(-129, 99) == 99);
        static_assert(math::within_bounds_or<i8>(-129, 0) == 0);
        static_assert(math::within_bounds_or<i8>(128, 99) == 99);
        static_assert(math::within_bounds_or<i8>(128, 0) == 0);
        static_assert(math::within_bounds_or<i8>(128u, 0) == 0);

        static_assert(std::is_same_v<decltype(math::within_bounds_or<usize>(5, 0)), usize>);
        static_assert(std::is_same_v<decltype(math::within_bounds_or<i8>(5, 0)), i8>);
    }
}
