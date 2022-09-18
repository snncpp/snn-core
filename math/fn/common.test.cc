// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/math/fn/common.hh"

#include "snn-core/unittest.hh"

namespace snn
{
    void unittest()
    {
        // add_with_overflow
        {
            constexpr auto add = math::fn::add_with_overflow{};
            static_assert(add(0u, 0) == 0);
            static_assert(add(3u, 4) == 7);
            static_assert(add(constant::limit<u32>::max, 0) == constant::limit<u32>::max);
            static_assert(add(constant::limit<u32>::max, 1) == 0);
            static_assert(add(constant::limit<u32>::max, 2) == 1);
        }

        // multiply_with_overflow
        {
            constexpr auto multiply = math::fn::multiply_with_overflow{};
            static_assert(multiply(0u, 0) == 0);
            static_assert(multiply(5u, 3) == 15);
            static_assert(multiply(constant::limit<u16>::max, 3) == 65533);
        }

        // negate_with_overflow
        {
            constexpr auto negate = math::fn::negate_with_overflow{};
            static_assert(negate(0u) == 0);
            static_assert(negate(u32{1}) == constant::limit<u32>::max);
            static_assert(negate(constant::limit<u32>::max) == 1);
        }

        // subtract_with_overflow
        {
            constexpr auto subtract = math::fn::subtract_with_overflow{};
            static_assert(subtract(0u, 0) == 0);
            static_assert(subtract(22u, 7) == 15);
            static_assert(subtract(u32{}, 1) == constant::limit<u32>::max);
            static_assert(subtract(u32{0}, constant::limit<u32>::max) == 1);
            static_assert(subtract(u32{1}, constant::limit<u32>::max) == 2);
        }

        // add_with_saturation
        {
            constexpr auto add = math::fn::add_with_saturation{};

            static_assert(add(0u, 0) == 0);
            static_assert(add(3u, 4) == 7);

            static_assert(add(u8{100}, 99) == 199);
            static_assert(add(constant::limit<u8>::max, 0) == constant::limit<u8>::max);
            static_assert(add(constant::limit<u8>::max, 99) == constant::limit<u8>::max);

            static_assert(add(u32{90'000}, 99) == 90'099);
            static_assert(add(constant::limit<u32>::max, 0) == constant::limit<u32>::max);
            static_assert(add(constant::limit<u32>::max, 99) == constant::limit<u32>::max);

            static_assert(add(u64{1'234'567'890'000}, 99) == 1'234'567'890'099);
            static_assert(add(constant::limit<u64>::max, 0) == constant::limit<u64>::max);
            static_assert(add(constant::limit<u64>::max, 99) == constant::limit<u64>::max);

            static_assert(std::is_same_v<decltype(add(1u, 0)), unsigned int>);
            static_assert(std::is_same_v<decltype(add(u8{}, 0)), u8>);
            static_assert(std::is_same_v<decltype(add(u16{}, 0)), u16>);
            static_assert(std::is_same_v<decltype(add(u32{}, 0)), u32>);
            static_assert(std::is_same_v<decltype(add(u64{}, 0)), u64>);
        }

        // multiply_with_saturation
        {
            constexpr auto multiply = math::fn::multiply_with_saturation{};

            static_assert(multiply(0u, 0) == 0);
            static_assert(multiply(3u, 4) == 12);

            static_assert(multiply(u8{105}, 0) == 0);
            static_assert(multiply(u8{105}, 1) == 105);
            static_assert(multiply(u8{105}, 2) == 210);
            static_assert(multiply(u8{105}, 3) == constant::limit<u8>::max);

            static_assert(multiply(u32{90'000}, 3) == 270'000);
            static_assert(multiply(u32{90'000}, 90'000) == constant::limit<u32>::max);

            static_assert(multiply(u64{9'000'000'000'000'000}, 3) == 27'000'000'000'000'000);
            static_assert(multiply(u64{9'000'000'000'000'000}, 3'000) == constant::limit<u64>::max);

            static_assert(std::is_same_v<decltype(multiply(1u, 0)), unsigned int>);
            static_assert(std::is_same_v<decltype(multiply(u8{}, 0)), u8>);
            static_assert(std::is_same_v<decltype(multiply(u16{}, 0)), u16>);
            static_assert(std::is_same_v<decltype(multiply(u32{}, 0)), u32>);
            static_assert(std::is_same_v<decltype(multiply(u64{}, 0)), u64>);
        }

        // subtract_with_saturation
        {
            constexpr auto subtract = math::fn::subtract_with_saturation{};

            static_assert(subtract(0u, 0) == 0);
            static_assert(subtract(8u, 5) == 3);

            static_assert(subtract(u8{100}, 75) == 25);
            static_assert(subtract(u8{100}, 100) == 0);
            static_assert(subtract(u8{100}, 105) == 0);
            static_assert(subtract(u8{0}, constant::limit<u8>::max) == 0);
            static_assert(subtract(u8{99}, constant::limit<u8>::max) == 0);

            static_assert(subtract(u32{90'000}, 70'000) == 20'000);
            static_assert(subtract(u32{0}, constant::limit<u32>::max) == 0);
            static_assert(subtract(u32{99}, constant::limit<u32>::max) == 0);

            static_assert(subtract(u64{1'234'567'890'000}, 1'234'567'000'000) == 890'000);
            static_assert(subtract(u64{0}, constant::limit<u64>::max) == 0);
            static_assert(subtract(u64{99}, constant::limit<u64>::max) == 0);

            static_assert(std::is_same_v<decltype(subtract(1u, 0)), unsigned int>);
            static_assert(std::is_same_v<decltype(subtract(u8{}, 0)), u8>);
            static_assert(std::is_same_v<decltype(subtract(u16{}, 0)), u16>);
            static_assert(std::is_same_v<decltype(subtract(u32{}, 0)), u32>);
            static_assert(std::is_same_v<decltype(subtract(u64{}, 0)), u64>);
        }

        // equal_to
        {
            constexpr auto equal_to = math::fn::equal_to{};

            static_assert(equal_to(3, 3));
            static_assert(equal_to(-1, -1));
            static_assert(!equal_to(-1, 3));
            static_assert(!equal_to(3, -3));

            // Shorthand
            static_assert(math::fn::eq{}(3, 3));
            static_assert(math::fn::eq{}(-1, -1));
            static_assert(!math::fn::eq{}(-1, 3));
            static_assert(!math::fn::eq{}(3, -3));

            // Same signedness.

            static_assert(equal_to(int{3}, i64{3}));
            static_assert(equal_to(i64{-123}, int{-123}));
            static_assert(!equal_to(int{3}, i64{4}));
            static_assert(!equal_to(constant::limit<i64>::min, constant::limit<int>::min));

            static_assert(equal_to(u8{3}, u64{3}));
            static_assert(equal_to(u32{}, usize{}));
            static_assert(!equal_to(u64{5}, u8{4}));
            static_assert(!equal_to(constant::limit<usize>::max, constant::limit<u32>::max));

            // Signed == unsigned.

            static_assert(equal_to(isize{123}, u32{123}));
            static_assert(equal_to(constant::limit<i16>::max, usize{32767}));
            static_assert(equal_to(isize{}, u32{}));
            static_assert(equal_to(i8{32}, u8{32}));
            static_assert(!equal_to(isize{5}, u32{6}));
            static_assert(!equal_to(i32{-1}, u32{}));
            static_assert(!equal_to(i32{-123}, u32{123}));
            static_assert(!equal_to(isize{-1}, constant::limit<usize>::max));

            // Unsigned == signed.

            static_assert(equal_to(u32{123}, isize{123}));
            static_assert(equal_to(usize{32767}, constant::limit<i16>::max));
            static_assert(equal_to(u32{}, isize{}));
            static_assert(equal_to(u8{32}, i8{32}));
            static_assert(!equal_to(u32{6}, isize{5}));
            static_assert(!equal_to(u32{}, i32{-1}));
            static_assert(!equal_to(u32{123}, i32{-123}));
            static_assert(!equal_to(constant::limit<usize>::max, isize{-1}));
        }

        // not_equal_to
        {
            constexpr auto not_equal_to = math::fn::not_equal_to{};
            static_assert(not_equal_to(-1, 3));
            static_assert(not_equal_to(3, -3));
            static_assert(!not_equal_to(3, 3));
            static_assert(!not_equal_to(-1, -1));

            // Shorthand
            static_assert(math::fn::ne{}(-1, 3));
            static_assert(math::fn::ne{}(3, -3));
            static_assert(!math::fn::ne{}(3, 3));
            static_assert(!math::fn::ne{}(-1, -1));
        }

        // less_than
        {
            constexpr auto less_than = math::fn::less_than{};

            static_assert(less_than(3, 4));
            static_assert(less_than(-3, -2));
            static_assert(!less_than(3, 3));
            static_assert(!less_than(4, 3));

            // Shorthand
            static_assert(math::fn::lt{}(3, 4));
            static_assert(math::fn::lt{}(-3, -2));
            static_assert(!math::fn::lt{}(3, 3));
            static_assert(!math::fn::lt{}(4, 3));

            // Same signedness.

            static_assert(less_than(int{3}, i64{4}));
            static_assert(less_than(i64{-15}, int{-14}));
            static_assert(less_than(constant::limit<i64>::min, constant::limit<int>::min));
            static_assert(!less_than(int{4}, i64{4}));
            static_assert(!less_than(int{4}, i64{3}));
            static_assert(!less_than(constant::limit<int>::min, constant::limit<i64>::min));

            static_assert(less_than(u8{14}, u64{15}));
            static_assert(less_than(u64{4}, u8{5}));
            static_assert(less_than(constant::limit<u32>::max, constant::limit<usize>::max));
            static_assert(!less_than(u32{}, usize{}));
            static_assert(!less_than(u64{6}, u8{5}));
            static_assert(!less_than(constant::limit<usize>::max, constant::limit<u32>::max));

            // Signed < unsigned.

            static_assert(less_than(isize{5}, u32{6}));
            static_assert(less_than(i32{-1}, u32{}));
            static_assert(less_than(i32{}, u32{1}));
            static_assert(less_than(i32{-123}, u32{123}));
            static_assert(less_than(isize{-1}, constant::limit<usize>::max));
            static_assert(!less_than(isize{4}, u32{4}));
            static_assert(!less_than(isize{5}, u32{4}));
            static_assert(!less_than(constant::limit<i16>::max, usize{32767}));
            static_assert(!less_than(isize{}, u32{}));
            static_assert(!less_than(i8{32}, u8{31}));

            // Unsigned < signed.

            static_assert(less_than(u32{}, isize{1}));
            static_assert(less_than(u32{5}, isize{6}));
            static_assert(less_than(u8{31}, i8{32}));
            static_assert(!less_than(constant::limit<usize>::max, isize{-1}));
            static_assert(!less_than(u32{123}, i32{-123}));
            static_assert(!less_than(u32{}, i32{-1}));
            static_assert(!less_than(u32{123}, isize{123}));
            static_assert(!less_than(usize{32767}, constant::limit<i16>::max));
            static_assert(!less_than(u32{}, isize{}));
        }

        // greater_than
        {
            constexpr auto greater_than = math::fn::greater_than{};
            static_assert(greater_than(4, 3));
            static_assert(greater_than(-2, -3));
            static_assert(greater_than(1, 0));
            static_assert(greater_than(0, -1));
            static_assert(!greater_than(3, 3));
            static_assert(!greater_than(3, 4));
            static_assert(!greater_than(-3, -2));
            static_assert(!greater_than(-3, -3));
            static_assert(!greater_than(-1, 0));

            // Shorthand
            static_assert(math::fn::gt{}(4, 3));
            static_assert(math::fn::gt{}(-2, -3));
            static_assert(math::fn::gt{}(1, 0));
            static_assert(math::fn::gt{}(0, -1));
            static_assert(!math::fn::gt{}(3, 3));
            static_assert(!math::fn::gt{}(3, 4));
            static_assert(!math::fn::gt{}(-3, -2));
            static_assert(!math::fn::gt{}(-3, -3));
            static_assert(!math::fn::gt{}(-1, 0));
        }

        // less_than_or_equal_to
        {
            constexpr auto less_than_or_equal_to = math::fn::less_than_or_equal_to{};
            static_assert(less_than_or_equal_to(2, 3));
            static_assert(less_than_or_equal_to(3, 3));
            static_assert(less_than_or_equal_to(0, 0));
            static_assert(less_than_or_equal_to(-1, 0));
            static_assert(less_than_or_equal_to(-1, -1));
            static_assert(less_than_or_equal_to(-2, -1));
            static_assert(!less_than_or_equal_to(4, 3));
            static_assert(!less_than_or_equal_to(1, 0));
            static_assert(!less_than_or_equal_to(0, -1));
            static_assert(!less_than_or_equal_to(-1, -2));

            // Shorthand
            static_assert(math::fn::lte{}(2, 3));
            static_assert(math::fn::lte{}(3, 3));
            static_assert(math::fn::lte{}(0, 0));
            static_assert(math::fn::lte{}(-1, 0));
            static_assert(math::fn::lte{}(-1, -1));
            static_assert(math::fn::lte{}(-2, -1));
            static_assert(!math::fn::lte{}(4, 3));
            static_assert(!math::fn::lte{}(1, 0));
            static_assert(!math::fn::lte{}(0, -1));
            static_assert(!math::fn::lte{}(-1, -2));
        }

        // greater_than_or_equal_to
        {
            constexpr auto greater_than_or_equal_to = math::fn::greater_than_or_equal_to{};
            static_assert(greater_than_or_equal_to(4, 3));
            static_assert(greater_than_or_equal_to(3, 3));
            static_assert(greater_than_or_equal_to(1, 0));
            static_assert(greater_than_or_equal_to(0, 0));
            static_assert(greater_than_or_equal_to(0, -1));
            static_assert(greater_than_or_equal_to(-1, -1));
            static_assert(greater_than_or_equal_to(-1, -2));
            static_assert(!greater_than_or_equal_to(2, 3));
            static_assert(!greater_than_or_equal_to(-1, 0));
            static_assert(!greater_than_or_equal_to(-2, -1));

            // Shorthand
            static_assert(math::fn::gte{}(4, 3));
            static_assert(math::fn::gte{}(3, 3));
            static_assert(math::fn::gte{}(1, 0));
            static_assert(math::fn::gte{}(0, 0));
            static_assert(math::fn::gte{}(0, -1));
            static_assert(math::fn::gte{}(-1, -1));
            static_assert(math::fn::gte{}(-1, -2));
            static_assert(!math::fn::gte{}(2, 3));
            static_assert(!math::fn::gte{}(-1, 0));
            static_assert(!math::fn::gte{}(-2, -1));
        }

        // is_even
        {
            constexpr auto is_even = math::fn::is_even{};

            static_assert(is_even(0));
            static_assert(is_even(2));
            static_assert(is_even(4));

            static_assert(is_even(-2));
            static_assert(is_even(-4));

            static_assert(!is_even(1));
            static_assert(!is_even(3));
            static_assert(!is_even(5));

            static_assert(!is_even(-1));
            static_assert(!is_even(-3));
            static_assert(!is_even(-5));
        }

        // is_odd
        {
            constexpr auto is_odd = math::fn::is_odd{};

            static_assert(is_odd(1));
            static_assert(is_odd(3));
            static_assert(is_odd(5));

            static_assert(is_odd(-1));
            static_assert(is_odd(-3));
            static_assert(is_odd(-5));

            static_assert(!is_odd(0));
            static_assert(!is_odd(2));
            static_assert(!is_odd(4));

            static_assert(!is_odd(-2));
            static_assert(!is_odd(-4));
        }
    }
}
