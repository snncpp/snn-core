// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/core.hh"

#include "snn-core/unittest.hh"
#include "snn-core/vec.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/math/fp/common.hh"
#include "snn-core/range/integral.hh"
#include <list>        // list
#include <sys/types.h> // off_t, ssize_t

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            // Macro "functions".

            snn_should(true); // Removed in optimized builds.
            snn_should_if_not_fuzzing(true);
            snn_assert(true);

            // Macro constants.

            static_assert(std::is_same_v<decltype(SNN_ADDRESS_SANITIZER_ENABLED), int>);
            static_assert(std::is_same_v<decltype(SNN_ASSERT_ENABLED), int>);
            static_assert(std::is_same_v<decltype(SNN_SHOULD_ENABLED), int>);
            static_assert(std::is_same_v<decltype(SNN_INT128_ENABLED), int>);

            // Types

            static_assert(std::is_same_v<usize, std::size_t>);
            static_assert(std::is_same_v<isize, std::int64_t>);

            static_assert(std::is_same_v<uptr, std::uintptr_t>);
            static_assert(std::is_same_v<iptr, std::intptr_t>);

            static_assert(std::is_same_v<iptrdiff, std::ptrdiff_t>);

            static_assert(std::is_same_v<u64, std::uint64_t>);
            static_assert(std::is_same_v<u32, std::uint32_t>);
            static_assert(std::is_same_v<u16, std::uint16_t>);
            static_assert(std::is_same_v<u8, std::uint8_t>);

            static_assert(std::is_same_v<u32fast, std::uint_fast32_t>);
            static_assert(std::is_same_v<u16fast, std::uint_fast16_t>);
            static_assert(std::is_same_v<u8fast, std::uint_fast8_t>);

            static_assert(std::is_same_v<i64, std::int64_t>);
            static_assert(std::is_same_v<i32, std::int32_t>);
            static_assert(std::is_same_v<i16, std::int16_t>);
            static_assert(std::is_same_v<i8, std::int8_t>);

            static_assert(std::is_same_v<i32fast, std::int_fast32_t>);
            static_assert(std::is_same_v<i16fast, std::int_fast16_t>);
            static_assert(std::is_same_v<i8fast, std::int_fast8_t>);

            static_assert(std::is_same_v<byte, std::uint8_t>);

#if SNN_INT128_ENABLED
            static_assert(std::is_same_v<u128, __uint128_t>);
            static_assert(std::is_same_v<i128, __int128_t>);
#endif

            // Concepts

            static_assert(arithmetic<int>);
            static_assert(brace_constructible_from<i64, int>);
            static_assert(!brace_constructible_from<int, i64>); // Would narrow.
            static_assert(callable<fn::less_than, int, int>);
            static_assert(constructible_from<int, i64>);
            static_assert(constructible_from_iterators<str>);
            static_assert(convertible_to<int, i64>);
            static_assert(explicitly_default_constructible<promise::has_value_t>);
            static_assert(floating_point<float>);
            static_assert(implicitly_default_constructible<str>);
            static_assert(legacy_iterable<str>);
            static_assert(pointer<char*>);
            static_assert(power_of_two<8>);
            static_assert(predicate<fn::is_zero, int>);
            static_assert(sane<str>);
            static_assert(value_type_or<int, int&>);

            // Has concepts.

            static_assert(has_append_inplace<vec<int>>);
            static_assert(has_at<cstrview>);
            static_assert(has_contiguous_iterator<cstrview>);
            static_assert(has_count<cstrview>);
            static_assert(has_drop_back<cstrrng>);
            static_assert(has_drop_front<cstrrng>);
            static_assert(has_to<cstrview, int>);

            // Integral concepts.

            static_assert(character<char>);
            static_assert(integral<bool>);
            static_assert(same_signedness_as<int, i64>);
            static_assert(signed_integral<int>);
            static_assert(!strict_integral<bool>);
            static_assert(strict_integral_min<u64, 32>);
            static_assert(!strict_integral_min<u16, 32>);
            static_assert(strict_signed_integral<int>);
            static_assert(strict_unsigned_integral<u32>);
            static_assert(unsigned_integral<u32>);

            // Range concepts.

            static_assert(input_range<cstrrng>);
            static_assert(forward_range<cstrrng>);
            static_assert(bidirectional_range<cstrrng>);
            static_assert(random_access_range<cstrrng>);
            static_assert(contiguous_range<cstrrng>);

            // Type traits

            static_assert(is_trivially_relocatable_v<str>);
            static_assert(std::is_same_v<deep_const_t<int*>, const int*>);
            static_assert(std::is_same_v<front_value_t<str>, char>);
            static_assert(std::is_same_v<not_deduced_t<str>, str>);
            static_assert(std::is_same_v<promote_integral_t<u16, 32>, u32>);
            static_assert(std::is_same_v<remove_cv_rvalue_ref_t<int&>, int&>);
            static_assert(std::is_same_v<trivially_relocatable_if_t<i32, str>, i32>);

            // Constants

            static_assert(constant::bits_per_byte == 8);
            static_assert(constant::dynamic_count == std::numeric_limits<usize>::max());
            static_assert(constant::npos == std::numeric_limits<usize>::max());
            static_assert(constant::value_initialized<int> == 0);

            static_assert(constant::exit::failure == 1);
            static_assert(constant::exit::success == 0);

            static_assert(constant::limit<usize>::min == std::numeric_limits<usize>::min());
            static_assert(constant::limit<usize>::max == std::numeric_limits<usize>::max());

            static_assert(constant::fp::limit<double>::min_negative < 0);
            static_assert(constant::fp::limit<double>::min_positive > 0);
            static_assert(constant::fp::limit<double>::max > 0);
            static_assert(constant::fp::limit<double>::infinity > 0);
            static_assert(constant::fp::limit<double>::nan != constant::fp::limit<double>::nan);

            static_assert(constant::size::kilobyte<i16> == 1'000);
            static_assert(constant::size::megabyte<i32> == 1'000'000);
            static_assert(constant::size::gigabyte<i32> == 1'000'000'000);
            static_assert(constant::size::terabyte<i64> == 1'000'000'000'000);
            static_assert(constant::size::petabyte<i64> == 1'000'000'000'000'000);
            static_assert(constant::size::exabyte<i64> == 1'000'000'000'000'000'000);

            static_assert(constant::size::kibibyte<i16> == 1'024);
            static_assert(constant::size::mebibyte<i32> == 1'048'576);
            static_assert(constant::size::gibibyte<i32> == 1'073'741'824);
            static_assert(constant::size::tebibyte<i64> == 1'099'511'627'776);
            static_assert(constant::size::pebibyte<i64> == 1'125'899'906'842'624);
            static_assert(constant::size::exbibyte<i64> == 1'152'921'504'606'846'976);

            // Functions

            static_assert(promote(123) == 123);    // Promote to min 32-bit (with the same sign).
            static_assert(promote<64>(true) == 1); // Promote to min 64-bit (with the same sign).
            static_assert(string_size("One Two") == 7);
            static_assert(string_size("One\0Two") == 7);
            static_assert(to_underlying(generic::error::no_error) == 0);

            // Integral conversion functions (non-destructive bit casts).

            static_assert(force_signed(-1) == -1);
            static_assert(force_signed(123) == 123);
            static_assert(force_signed(123u) == 123);
            static_assert(force_unsigned(-1) == constant::limit<u32>::max);
            static_assert(force_unsigned(123) == 123u);
            static_assert(force_unsigned(123u) == 123u);
            static_assert(to_char(123u) == 123);
            static_assert(to_byte(123) == 123u);
            static_assert(to_i8(123u) == 123);
            static_assert(to_u8(123) == 123u);
            static_assert(to_i16(123u) == 123);
            static_assert(to_u16(123) == 123u);
            static_assert(to_i32(123u) == 123);
            static_assert(to_u32(123) == 123u);
            static_assert(to_i64(123u) == 123);
            static_assert(to_u64(123) == 123u);
            static_assert(to_isize(123u) == 123);
            static_assert(to_usize(123) == 123u);

            // Wrappers (and helper functions).

            byte_size bs{123};
            snn_require(bs.get() == 123);

            not_null nn{&bs};
            snn_require(nn.get() != nullptr);

            not_zero nz{456};
            snn_require(nz.get() == 456);

            auto num = as_num(789);
            static_assert(std::is_same_v<decltype(num), numeric<int>>);
            snn_require(num.get() == 789);

            semi_const<int> i = 123;
            snn_require(i.get() == 123);

            transient<cstrview> s = "abc";
            snn_require(s.get() == "abc");

            return true;
        }

        constexpr bool test_arithmetic()
        {
            static_assert(arithmetic<bool>);
            static_assert(arithmetic<char>);
            static_assert(arithmetic<int>);
            static_assert(arithmetic<usize>);
            static_assert(arithmetic<float>);
            static_assert(arithmetic<const int>);
            static_assert(arithmetic<const float>);
            static_assert(!arithmetic<int&>);
            static_assert(!arithmetic<int*>);
            static_assert(!arithmetic<cstrview>);

            return true;
        }

        constexpr bool test_array_count()
        {
            i32 arr[] = {2};
            static_assert(array_count(arr) == 1);

            usize arr2[] = {123, 456, 789};
            static_assert(array_count(arr2) == 3);

            i32 arr3[2][3] = {{1, 2, 3}, {4, 5, 6}};
            static_assert(array_count(arr3) == 2);

            static_assert(array_count("") == 1);
            static_assert(array_count("a") == 2);
            static_assert(array_count("\0") == 2);
            static_assert(array_count("Å") == 3);

            return true;
        }

        constexpr bool test_brace_constructible_from()
        {
            static_assert(brace_constructible_from<i64, int>);
            static_assert(brace_constructible_from<cstrview, not_null<const char*>, usize>);
            static_assert(!brace_constructible_from<int, i64>); // Would narrow.
            static_assert(!brace_constructible_from<cstrview, const char*>);
            static_assert(!brace_constructible_from<cstrview, const char*, const char*>);

            return true;
        }

        constexpr bool test_byte_size()
        {
            static_assert(byte_size{0}.get() == 0);
            static_assert(byte_size{42}.get() == 42);
            static_assert(byte_size{999'999'999'999}.get() == 999'999'999'999);

            constexpr byte_size a{123};
            constexpr byte_size b{123};
            constexpr byte_size c{92};

            static_assert(std::is_same_v<decltype(a), const byte_size<usize>>);
            static_assert(std::is_same_v<decltype(a.get()), usize>);
            static_assert(sizeof(a) == sizeof(usize));

            static_assert(a == b);
            static_assert(!(a == c));

            static_assert(b != c);
            static_assert(!(a != b));

            static_assert(c < a);
            static_assert(!(a < c));

            static_assert(a > c);
            static_assert(!(c > a));

            // Deduction (will hold a `usize` by default when created with an integer literal).

            static_assert(std::is_same_v<decltype(byte_size{0}), byte_size<usize>>);
            static_assert(std::is_same_v<decltype(byte_size{999'999'999'999}), byte_size<usize>>);
            static_assert(std::is_same_v<decltype(byte_size{u8{}}), byte_size<u8>>);
            static_assert(std::is_same_v<decltype(byte_size{u16{}}), byte_size<u16>>);
            static_assert(std::is_same_v<decltype(byte_size{u32{}}), byte_size<u32>>);
            static_assert(std::is_same_v<decltype(byte_size{u64{}}), byte_size<usize>>);

            // Explicit type.

            constexpr byte_size<u32> bs{456};
            static_assert(bs.get() == 456);
            static_assert(std::is_same_v<decltype(bs), const byte_size<u32>>);
            static_assert(std::is_same_v<decltype(bs.get()), u32>);
            static_assert(sizeof(bs) == sizeof(u32));

            // Conversion

            constexpr byte_size from{constant::limit<u32>::max};
            static_assert(std::is_same_v<decltype(from), const byte_size<u32>>);
            constexpr byte_size<usize> to = from; // Implicit conversion.
            static_assert(to.get() == constant::limit<u32>::max);

            static_assert(std::is_nothrow_convertible_v<byte_size<usize>, byte_size<usize>>);
            static_assert(std::is_nothrow_convertible_v<byte_size<u32>, byte_size<usize>>);
            static_assert(std::is_nothrow_convertible_v<byte_size<u16>, byte_size<u32>>);
            static_assert(std::is_nothrow_convertible_v<byte_size<u8>, byte_size<u16>>);

            // Would narrow.
            static_assert(!std::is_convertible_v<byte_size<usize>, byte_size<u32>>);
            static_assert(!std::is_convertible_v<byte_size<u32>, byte_size<u16>>);

            // Implicit conversion from an integral is not allowed.
            static_assert(!std::is_convertible_v<usize, byte_size<usize>>);

            return true;
        }

        constexpr bool test_character()
        {
            static_assert(character<char>);
            static_assert(character<const char>);

            static_assert(!character<volatile char>);
            static_assert(!character<const volatile char>);
            static_assert(!character<signed char>);
            static_assert(!character<const signed char>);
            static_assert(!character<unsigned char>);
            static_assert(!character<const unsigned char>);
            static_assert(!character<u8>);
            static_assert(!character<const u8>);
            static_assert(!character<i8>);
            static_assert(!character<const i8>);
            static_assert(!character<int>);
            static_assert(!character<bool>);
            static_assert(!character<char&>);
            static_assert(!character<const char&>);
            static_assert(!character<char*>);
            static_assert(!character<const char*>);
            static_assert(!character<cstrview>);

            return true;
        }

        constexpr bool test_constant()
        {
            static_assert(constant::bits_per_byte == 8);
            static_assert(constant::dynamic_count == constant::limit<usize>::max);
            static_assert(constant::npos == constant::limit<usize>::max);

            static_assert(constant::value_initialized<int> == 0);

            return true;
        }

        constexpr bool test_constant_exit()
        {
            static_assert(constant::exit::failure == 1);
            static_assert(constant::exit::success == 0);

            return true;
        }

        bool test_constant_fp()
        {
            snn_require(math::fp::is_bitwise_equal(constant::fp::limit<double>::min_negative,
                                                   std::numeric_limits<double>::lowest()));
            snn_require(math::fp::is_bitwise_equal(constant::fp::limit<double>::min_positive,
                                                   std::numeric_limits<double>::min()));
            snn_require(math::fp::is_bitwise_equal(constant::fp::limit<double>::max,
                                                   std::numeric_limits<double>::max()));
            snn_require(math::fp::is_bitwise_equal(constant::fp::limit<const double>::max,
                                                   std::numeric_limits<double>::max()));
            snn_require(math::fp::is_bitwise_equal(constant::fp::limit<double>::infinity,
                                                   std::numeric_limits<double>::infinity()));
            snn_require(math::fp::is_bitwise_equal(constant::fp::limit<double>::nan,
                                                   std::numeric_limits<double>::quiet_NaN()));

            snn_require(math::fp::is_bitwise_equal(constant::fp::limit<float>::min_negative,
                                                   std::numeric_limits<float>::lowest()));
            snn_require(math::fp::is_bitwise_equal(constant::fp::limit<float>::min_positive,
                                                   std::numeric_limits<float>::min()));
            snn_require(math::fp::is_bitwise_equal(constant::fp::limit<float>::max,
                                                   std::numeric_limits<float>::max()));
            snn_require(math::fp::is_bitwise_equal(constant::fp::limit<float>::infinity,
                                                   std::numeric_limits<float>::infinity()));
            snn_require(math::fp::is_bitwise_equal(constant::fp::limit<float>::nan,
                                                   std::numeric_limits<float>::quiet_NaN()));

            return true;
        }

        constexpr bool test_constant_limit()
        {
            static_assert(constant::limit<bool>::min == 0);
            static_assert(constant::limit<bool>::max == 1);

            static_assert(constant::limit<u8>::min == 0);
            static_assert(constant::limit<u8>::max == 255);
            static_assert(constant::limit<u16>::min == 0);
            static_assert(constant::limit<u16>::max == 65535);
            static_assert(constant::limit<u32>::min == 0);
            static_assert(constant::limit<u32>::max == 4294967295);
            static_assert(constant::limit<const u32>::max == 4294967295);
            static_assert(constant::limit<u64>::min == 0);
            static_assert(constant::limit<u64>::max == 18446744073709551615u);

            static_assert(constant::limit<i8>::min == -128);
            static_assert(constant::limit<i8>::max == 127);
            static_assert(constant::limit<i16>::min == -32768);
            static_assert(constant::limit<i16>::max == 32767);
            static_assert(constant::limit<i32>::min == -2147483648);
            static_assert(constant::limit<i32>::max == 2147483647);
            static_assert(constant::limit<const i32>::max == 2147483647);
            static_assert(constant::limit<i64>::min == (-9223372036854775807 - 1));
            static_assert(constant::limit<i64>::max == 9223372036854775807);

            static_assert(constant::limit<char>::min == std::numeric_limits<char>::min());
            static_assert(constant::limit<i8>::min == std::numeric_limits<i8>::min());
            static_assert(constant::limit<i16>::min == std::numeric_limits<i16>::min());
            static_assert(constant::limit<i32>::min == std::numeric_limits<i32>::min());
            static_assert(constant::limit<i64>::min == std::numeric_limits<i64>::min());
            static_assert(constant::limit<isize>::min == std::numeric_limits<isize>::min());
            static_assert(constant::limit<u8>::min == std::numeric_limits<u8>::min());
            static_assert(constant::limit<u16>::min == std::numeric_limits<u16>::min());
            static_assert(constant::limit<u32>::min == std::numeric_limits<u32>::min());
            static_assert(constant::limit<u64>::min == std::numeric_limits<u64>::min());
            static_assert(constant::limit<usize>::min == std::numeric_limits<usize>::min());

            static_assert(constant::limit<char>::max == std::numeric_limits<char>::max());
            static_assert(constant::limit<i8>::max == std::numeric_limits<i8>::max());
            static_assert(constant::limit<i16>::max == std::numeric_limits<i16>::max());
            static_assert(constant::limit<i32>::max == std::numeric_limits<i32>::max());
            static_assert(constant::limit<i64>::max == std::numeric_limits<i64>::max());
            static_assert(constant::limit<isize>::max == std::numeric_limits<isize>::max());
            static_assert(constant::limit<u8>::max == std::numeric_limits<u8>::max());
            static_assert(constant::limit<u16>::max == std::numeric_limits<u16>::max());
            static_assert(constant::limit<u32>::max == std::numeric_limits<u32>::max());
            static_assert(constant::limit<u64>::max == std::numeric_limits<u64>::max());
            static_assert(constant::limit<usize>::max == std::numeric_limits<usize>::max());

            static_assert(constant::limit<byte>::min == 0);
            static_assert(constant::limit<byte>::max == 255);

            return true;
        }

        constexpr bool test_constant_size()
        {
            // Metric

            static_assert(constant::size::kilobyte<i16> == 1'000);
            static_assert(constant::size::kilobyte<u16> == 1'000);

            static_assert(constant::size::megabyte<i32> == 1'000'000);
            static_assert(constant::size::megabyte<u32> == 1'000'000);
            static_assert(constant::size::gigabyte<i32> == 1'000'000'000);
            static_assert(constant::size::gigabyte<u32> == 1'000'000'000);

            static_assert(constant::size::kilobyte<i64> == 1'000);
            static_assert(constant::size::megabyte<i64> == 1'000'000);
            static_assert(constant::size::gigabyte<i64> == 1'000'000'000);
            static_assert(constant::size::terabyte<i64> == 1'000'000'000'000);
            static_assert(constant::size::petabyte<i64> == 1'000'000'000'000'000);
            static_assert(constant::size::exabyte<i64> == 1'000'000'000'000'000'000);

            static_assert(constant::size::kilobyte<u64> == 1'000);
            static_assert(constant::size::megabyte<u64> == 1'000'000);
            static_assert(constant::size::gigabyte<u64> == 1'000'000'000);
            static_assert(constant::size::terabyte<u64> == 1'000'000'000'000);
            static_assert(constant::size::petabyte<u64> == 1'000'000'000'000'000);
            static_assert(constant::size::exabyte<u64> == 1'000'000'000'000'000'000);

            // IEC

            static_assert(constant::size::kibibyte<i16> == 1'024);
            static_assert(constant::size::kibibyte<u16> == 1'024);

            static_assert(constant::size::mebibyte<i32> == 1'048'576);
            static_assert(constant::size::mebibyte<u32> == 1'048'576);
            static_assert(constant::size::gibibyte<i32> == 1'073'741'824);
            static_assert(constant::size::gibibyte<u32> == 1'073'741'824);

            static_assert(constant::size::kibibyte<i64> == 1'024);
            static_assert(constant::size::mebibyte<i64> == 1'048'576);
            static_assert(constant::size::gibibyte<i64> == 1'073'741'824);
            static_assert(constant::size::tebibyte<i64> == 1'099'511'627'776);
            static_assert(constant::size::pebibyte<i64> == 1'125'899'906'842'624);
            static_assert(constant::size::exbibyte<i64> == 1'152'921'504'606'846'976);

            static_assert(constant::size::kibibyte<u64> == 1'024);
            static_assert(constant::size::mebibyte<u64> == 1'048'576);
            static_assert(constant::size::gibibyte<u64> == 1'073'741'824);
            static_assert(constant::size::tebibyte<u64> == 1'099'511'627'776);
            static_assert(constant::size::pebibyte<u64> == 1'125'899'906'842'624);
            static_assert(constant::size::exbibyte<u64> == 1'152'921'504'606'846'976);

            return true;
        }

        constexpr bool test_constructible_from()
        {
            static_assert(constructible_from<i64, int>);
            static_assert(constructible_from<int, i64>);
            static_assert(constructible_from<cstrview, not_null<const char*>, usize>);
            static_assert(!constructible_from<cstrview, const char*>);
            static_assert(!constructible_from<cstrview, const char*, const char*>);

            return true;
        }

        constexpr bool test_constructible_from_iterators()
        {
            static_assert(constructible_from_iterators<str>);
            static_assert(constructible_from_iterators<cstrrng>);
            static_assert(constructible_from_iterators<vec<int>>);
            static_assert(!constructible_from_iterators<int>);
            static_assert(!constructible_from_iterators<range::integral<int>>);

            return true;
        }

        constexpr bool test_implicitly_default_constructible()
        {
            static_assert(implicitly_default_constructible<int>);
            static_assert(implicitly_default_constructible<str>);
            static_assert(!implicitly_default_constructible<promise::is_valid_t>);

            return true;
        }

        constexpr bool test_explicitly_default_constructible()
        {
            static_assert(explicitly_default_constructible<promise::is_valid_t>);
            static_assert(!explicitly_default_constructible<int>);
            static_assert(!explicitly_default_constructible<str>);

            return true;
        }

        constexpr bool test_convertible_to()
        {
            static_assert(convertible_to<int, int>);
            static_assert(convertible_to<int, i64>);
            static_assert(convertible_to<char*, char*>);
            static_assert(convertible_to<char*, const char*>);
            static_assert(!convertible_to<str, cstrview>); // Will not bind to a temporary.
            static_assert(!convertible_to<u8*, char*>);
            static_assert(!convertible_to<const char*, char*>);

            return true;
        }

        constexpr bool test_deep_const()
        {
            static_assert(std::is_same_v<deep_const_t<int>, int>);
            static_assert(std::is_same_v<deep_const_t<const int>, int>);

            static_assert(std::is_same_v<deep_const_t<int&>, const int&>);
            static_assert(std::is_same_v<deep_const_t<const int&>, const int&>);
            static_assert(std::is_same_v<deep_const_t<int&&>, const int&&>);
            static_assert(std::is_same_v<deep_const_t<const int&&>, const int&&>);

            static_assert(std::is_same_v<deep_const_t<int*>, const int*>);
            static_assert(std::is_same_v<deep_const_t<int* const>, const int*>);
            static_assert(std::is_same_v<deep_const_t<const int*>, const int*>);
            static_assert(std::is_same_v<deep_const_t<const int* const>, const int*>);

            // Note that deep_const only adds const for the first level (first indirection).
            static_assert(std::is_same_v<deep_const_t<int**>, int* const*>);
            static_assert(std::is_same_v<deep_const_t<int***>, int** const*>);

            return true;
        }

        constexpr bool test_floating_point()
        {
            static_assert(floating_point<float>);
            static_assert(floating_point<double>);
            static_assert(floating_point<const float>);
            static_assert(!floating_point<bool>);
            static_assert(!floating_point<char>);
            static_assert(!floating_point<int>);
            static_assert(!floating_point<usize>);
            static_assert(!floating_point<const int>);
            static_assert(!floating_point<float&>);
            static_assert(!floating_point<float*>);
            static_assert(!floating_point<cstrview>);

            return true;
        }

        constexpr bool test_force_signed()
        {
            static_assert(force_signed(-123) == -123);
            static_assert(force_signed(constant::limit<u32>::max) == -1);

            static_assert(std::is_same_v<decltype(force_signed(char{})), signed char>);
            static_assert(std::is_same_v<decltype(force_signed(byte{})), signed char>);
            static_assert(std::is_same_v<decltype(force_signed(i8{})), i8>);
            static_assert(std::is_same_v<decltype(force_signed(u8{})), i8>);
            static_assert(std::is_same_v<decltype(force_signed(i16{})), i16>);
            static_assert(std::is_same_v<decltype(force_signed(u16{})), i16>);
            static_assert(std::is_same_v<decltype(force_signed(i32{})), i32>);
            static_assert(std::is_same_v<decltype(force_signed(u32{})), i32>);
            static_assert(std::is_same_v<decltype(force_signed(i64{})), i64>);
            static_assert(std::is_same_v<decltype(force_signed(u64{})), i64>);
            static_assert(std::is_same_v<decltype(force_signed(isize{})), isize>);
            static_assert(std::is_same_v<decltype(force_signed(usize{})), isize>);
#if SNN_INT128_ENABLED
            static_assert(std::is_same_v<decltype(force_signed(i128{})), i128>);
            static_assert(std::is_same_v<decltype(force_signed(u128{})), i128>);
#endif
            static_assert(std::is_same_v<decltype(force_signed(char8_t{})), i8>);
            static_assert(std::is_same_v<decltype(force_signed(char16_t{})), i16>);
            static_assert(std::is_same_v<decltype(force_signed(char32_t{})), i32>);

            return true;
        }

        constexpr bool test_force_unsigned()
        {
            static_assert(force_unsigned(123) == 123);
            static_assert(force_unsigned(i32{-1}) == constant::limit<u32>::max);

            static_assert(std::is_same_v<decltype(force_unsigned(char{})), unsigned char>);
            static_assert(std::is_same_v<decltype(force_unsigned(byte{})), unsigned char>);
            static_assert(std::is_same_v<decltype(force_unsigned(i8{})), u8>);
            static_assert(std::is_same_v<decltype(force_unsigned(u8{})), u8>);
            static_assert(std::is_same_v<decltype(force_unsigned(i16{})), u16>);
            static_assert(std::is_same_v<decltype(force_unsigned(u16{})), u16>);
            static_assert(std::is_same_v<decltype(force_unsigned(i32{})), u32>);
            static_assert(std::is_same_v<decltype(force_unsigned(u32{})), u32>);
            static_assert(std::is_same_v<decltype(force_unsigned(i64{})), u64>);
            static_assert(std::is_same_v<decltype(force_unsigned(u64{})), u64>);
            static_assert(std::is_same_v<decltype(force_unsigned(isize{})), usize>);
            static_assert(std::is_same_v<decltype(force_unsigned(usize{})), usize>);
#if SNN_INT128_ENABLED
            static_assert(std::is_same_v<decltype(force_unsigned(i128{})), u128>);
            static_assert(std::is_same_v<decltype(force_unsigned(u128{})), u128>);
#endif
            static_assert(std::is_same_v<decltype(force_unsigned(char8_t{})), u8>);
            static_assert(std::is_same_v<decltype(force_unsigned(char16_t{})), u16>);
            static_assert(std::is_same_v<decltype(force_unsigned(char32_t{})), u32>);

            return true;
        }

        constexpr bool test_ignore_if_unused()
        {
            i32 i = 3;
            ignore_if_unused(i);

            int a = 1;
            int b = 22;
            ignore_if_unused(a, b);

            return true;
        }

        constexpr bool test_integral()
        {
            static_assert(integral<bool>);
            static_assert(integral<char>);
            static_assert(integral<char8_t>);
            static_assert(integral<char16_t>);
            static_assert(integral<char32_t>);
            static_assert(integral<int>);
            static_assert(integral<usize>);
            static_assert(integral<const int>);
            static_assert(integral<const usize>);
            static_assert(!integral<int&>);
            static_assert(!integral<int*>);
            static_assert(!integral<float>);
            static_assert(!integral<cstrview>);

#if SNN_INT128_ENABLED
            static_assert(integral<i128>);
            static_assert(integral<u128>);
#endif

            return true;
        }

        constexpr bool test_callable()
        {
            static_assert(callable<fn::add, int, int>);
            static_assert(!callable<fn::add>);
            static_assert(!callable<fn::add, int>);
            static_assert(!callable<fn::add, int, int, int>);

            static_assert(callable<fn::blackhole>);
            static_assert(callable<fn::blackhole&>);
            static_assert(callable<fn::blackhole&&>);
            static_assert(callable<const fn::blackhole>);
            static_assert(callable<const fn::blackhole&>);
            static_assert(callable<const fn::blackhole&&>);

            static_assert(callable<int()>);
            static_assert(callable<int(int), int>);

            static_assert(!callable<int(), int>);
            static_assert(!callable<cstrview>);
            static_assert(!callable<cstrview, int>);

            struct callable_when_not_const
            {
                constexpr int operator()(const int i)
                {
                    return i;
                }
            };

            static_assert(callable<callable_when_not_const, int>);

            static_assert(!callable<const callable_when_not_const, int>);

            static_assert(callable_when_not_const{}(3) == 3); // To silence compiler warning.

            struct callable_with_lvalue_ref
            {
                constexpr int operator()(int& i) const
                {
                    return i;
                }
            };

            static_assert(callable<callable_with_lvalue_ref, int&>);

            static_assert(!callable<callable_with_lvalue_ref, int>);
            static_assert(!callable<callable_with_lvalue_ref, const int&>);
            static_assert(!callable<callable_with_lvalue_ref, int&&>);
            static_assert(!callable<callable_with_lvalue_ref, const int&&>);

            int i = 3;
            snn_require(callable_with_lvalue_ref{}(i) == 3); // To silence compiler warning.

            return true;
        }

        constexpr bool test_sane()
        {
            static_assert(sane<i32>);
            static_assert(sane<str>);

            // Here is an example of a class that isn't sane:

            struct not_sane
            {
                const str should_not_be_const;

                // This class is move-constructible but the const member makes it not
                // nothrow-move-constructible.
            };

            static_assert(!sane<not_sane>);

            static_assert(std::is_copy_constructible_v<not_sane>);
            static_assert(std::is_move_constructible_v<not_sane>);
            static_assert(!std::is_nothrow_move_constructible_v<not_sane>); // Important

            static_assert(!std::is_copy_assignable_v<not_sane>);
            static_assert(!std::is_move_assignable_v<not_sane>);
            static_assert(!std::is_nothrow_move_assignable_v<not_sane>);

            // If a const member is required, use the `semi_const` wrapper:

            struct sane_alternative
            {
                semi_const<str> this_works;
            };

            static_assert(sane<sane_alternative>);

            static_assert(std::is_copy_constructible_v<sane_alternative>);
            static_assert(std::is_move_constructible_v<sane_alternative>);
            static_assert(std::is_nothrow_move_constructible_v<sane_alternative>); // Important

            static_assert(!std::is_copy_assignable_v<sane_alternative>);
            static_assert(!std::is_move_assignable_v<sane_alternative>);
            static_assert(!std::is_nothrow_move_assignable_v<sane_alternative>);

            return true;
        }

        constexpr bool test_is_strict_integral()
        {
            static_assert(is_strict_integral_v<char>);
            static_assert(is_strict_integral_v<const char>);
            static_assert(is_strict_integral_v<wchar_t>);
            static_assert(is_strict_integral_v<char8_t>);
            static_assert(is_strict_integral_v<char16_t>);
            static_assert(is_strict_integral_v<char32_t>);

            static_assert(is_strict_integral_v<i8>);
            static_assert(is_strict_integral_v<i16>);
            static_assert(is_strict_integral_v<i32>);
            static_assert(is_strict_integral_v<i64>);
            static_assert(is_strict_integral_v<isize>);
            static_assert(is_strict_integral_v<const isize>);

            static_assert(is_strict_integral_v<u8>);
            static_assert(is_strict_integral_v<u16>);
            static_assert(is_strict_integral_v<u32>);
            static_assert(is_strict_integral_v<u64>);
            static_assert(is_strict_integral_v<usize>);
            static_assert(is_strict_integral_v<const usize>);

#if SNN_INT128_ENABLED
            static_assert(is_strict_integral_v<i128>);
            static_assert(is_strict_integral_v<u128>);
#endif

            static_assert(!is_strict_integral_v<bool>);
            static_assert(!is_strict_integral_v<const bool>);

            static_assert(!is_strict_integral_v<float>);
            static_assert(!is_strict_integral_v<str>);

            return true;
        }

        constexpr bool test_remove_cv_rvalue_ref()
        {
            static_assert(std::is_same_v<remove_cv_rvalue_ref_t<int>, int>);
            static_assert(std::is_same_v<remove_cv_rvalue_ref_t<int&>, int&>);
            static_assert(std::is_same_v<remove_cv_rvalue_ref_t<int&&>, int>);
            static_assert(std::is_same_v<remove_cv_rvalue_ref_t<const int>, int>);
            static_assert(std::is_same_v<remove_cv_rvalue_ref_t<const int&>, const int&>);
            static_assert(std::is_same_v<remove_cv_rvalue_ref_t<const int&&>, int>);

            return true;
        }

        constexpr bool test_is_trivially_relocatable()
        {
            static_assert(is_trivially_relocatable_v<char>);
            static_assert(is_trivially_relocatable_v<const char>);
            static_assert(is_trivially_relocatable_v<i32>);
            static_assert(is_trivially_relocatable_v<const i32>);
            static_assert(is_trivially_relocatable_v<double>);
            static_assert(is_trivially_relocatable_v<const double>);
            static_assert(is_trivially_relocatable_v<str>);
            static_assert(is_trivially_relocatable_v<const str>);

            static_assert(!is_trivially_relocatable_v<std::list<i32>>);
            static_assert(!is_trivially_relocatable_v<const std::list<i32>>);

            return true;
        }

        constexpr bool test_front_value()
        {
            static_assert(std::is_same_v<front_value_t<str>, char>);
            static_assert(std::is_same_v<front_value_t<const str>, char>);
            static_assert(std::is_same_v<front_value_t<str&>, char>);
            static_assert(std::is_same_v<front_value_t<const str&>, char>);
            static_assert(std::is_same_v<front_value_t<str&&>, char>);
            static_assert(std::is_same_v<front_value_t<const str&&>, char>);

            static_assert(std::is_same_v<front_value_t<array_view<int>>, int>);
            static_assert(std::is_same_v<front_value_t<const array_view<int>>, int>);
            static_assert(std::is_same_v<front_value_t<array_view<const int>>, int>);
            static_assert(std::is_same_v<front_value_t<const array_view<const int>>, int>);

            static_assert(std::is_same_v<front_value_t<array_view<int*>>, int*>);
            static_assert(std::is_same_v<front_value_t<const array_view<int*>>, int*>);
            static_assert(std::is_same_v<front_value_t<array_view<const int*>>, const int*>);
            static_assert(std::is_same_v<front_value_t<const array_view<const int*>>, const int*>);

            return true;
        }

        constexpr bool test_has_at()
        {
            static_assert(has_at<str>);
            static_assert(has_at<const str>);
            static_assert(!has_at<int>);
            static_assert(!has_at<const int>);
            static_assert(!has_at<fn::is_zero>);
            static_assert(!has_at<const fn::is_zero>);

            return true;
        }

        constexpr bool test_has_contiguous_iterator()
        {
            static_assert(has_contiguous_iterator<str>);
            static_assert(has_contiguous_iterator<const str>);
            static_assert(!has_contiguous_iterator<int>);
            static_assert(!has_contiguous_iterator<const int>);
            static_assert(!has_contiguous_iterator<fn::is_zero>);
            static_assert(!has_contiguous_iterator<const fn::is_zero>);
            static_assert(!has_contiguous_iterator<std::list<i32>>);
            static_assert(!has_contiguous_iterator<const std::list<i32>>);

            return true;
        }

        constexpr bool test_has_count()
        {
            static_assert(has_count<str>);
            static_assert(has_count<const str>);
            static_assert(has_count<vec<i32>>);
            static_assert(has_count<const vec<i32>>);

            struct with_signed_count
            {
                constexpr int count()
                {
                    return -1;
                }
            };
            ignore_if_unused(with_signed_count{}.count());

            static_assert(!has_count<usize>);
            static_assert(!has_count<const usize>);
            static_assert(!has_count<fn::is_zero>);
            static_assert(!has_count<const fn::is_zero>);
            static_assert(!has_count<with_signed_count>);
            static_assert(!has_count<const with_signed_count>);

            return true;
        }

        constexpr bool test_has_drop_back()
        {
            static_assert(has_drop_back<cstrrng>);
            static_assert(!has_drop_back<int>);
            static_assert(!has_drop_back<fn::is_zero>);
            static_assert(!has_drop_back<std::list<i32>>);

            return true;
        }

        constexpr bool test_has_drop_front()
        {
            static_assert(has_drop_front<cstrrng>);
            static_assert(!has_drop_front<int>);
            static_assert(!has_drop_front<str>);
            static_assert(!has_drop_front<std::list<i32>>);

            return true;
        }

        constexpr bool test_has_to()
        {
            static_assert(has_to<cstrview, int>);
            static_assert(has_to<cstrview&, int>);
            static_assert(has_to<const cstrview&, int>);
            static_assert(has_to<cstrview&&, int>);
            static_assert(has_to<const cstrview&&, int>);

            static_assert(!has_to<cstrview, std::list<i32>>);
            static_assert(!has_to<cstrview, std::list<i32>>);
            static_assert(!has_to<cstrview&, std::list<i32>>);
            static_assert(!has_to<const cstrview&, std::list<i32>>);
            static_assert(!has_to<cstrview&&, std::list<i32>>);
            static_assert(!has_to<const cstrview&&, std::list<i32>>);

            static_assert(!has_to<int, int>);

            static_assert(!has_to<vec<i32>, int>);
            static_assert(!has_to<vec<i32>&, int>);
            static_assert(!has_to<const vec<i32>&, int>);
            static_assert(!has_to<vec<i32>&&, int>);
            static_assert(!has_to<const vec<i32>&&, int>);

            return true;
        }

        constexpr bool test_has_append_inplace()
        {
            static_assert(has_append_inplace<vec<i32>>);
            static_assert(has_append_inplace<vec<i32>, i32>);

            static_assert(has_append_inplace<vec<cstrview>>);
            static_assert(has_append_inplace<vec<cstrview>, cstrview>);

            static_assert(!has_append_inplace<const vec<i32>>);
            static_assert(!has_append_inplace<const vec<cstrview>>);
            static_assert(!has_append_inplace<i32>);
            static_assert(!has_append_inplace<cstrview>);
            static_assert(!has_append_inplace<str>);
            static_assert(!has_append_inplace<strbuf>);
            static_assert(!has_append_inplace<std::list<i32>>);

            return true;
        }

        constexpr bool test_legacy_iterable()
        {
            static_assert(legacy_iterable<cstrview>);
            static_assert(legacy_iterable<cstrrng>);
            static_assert(!legacy_iterable<int>);
            static_assert(!legacy_iterable<range::integral<int>>);

            return true;
        }

        constexpr bool test_loop_count()
        {
            usize count = 0;

            for (loop::count lc{3}; lc--;)
            {
                ++count;
            }
            snn_require(count == 3);

            for (loop::count lc{2}; lc--;)
            {
                ++count;
            }
            snn_require(count == 5);

            for (loop::count lc{1}; lc--;)
            {
                snn_require(lc.value == 0);
                ++count;
            }
            snn_require(count == 6);

            for (loop::count lc{0}; lc--;)
            {
                snn_require(false && "Block must not be entered.");
            }

            for (loop::count lc{}; lc--;)
            {
                snn_require(false && "Block must not be entered.");
            }

            for (loop::count lc; lc--;)
            {
                snn_require(false && "Block must not be entered.");
            }

            constexpr loop::count lc;
            static_assert(lc.value == 0);

            return true;
        }

        constexpr bool test_meta_all()
        {
            static_assert(meta::all_v<std::is_unsigned>);
            static_assert(meta::all_v<std::is_unsigned, u16>);
            static_assert(meta::all_v<std::is_unsigned, u16, u32>);

            static_assert(!meta::all_v<std::is_unsigned, i16>);
            static_assert(!meta::all_v<std::is_unsigned, i16, i32>);
            static_assert(!meta::all_v<std::is_unsigned, i16, u32>);
            static_assert(!meta::all_v<std::is_unsigned, u16, i32>);

            return true;
        }

        constexpr bool test_meta_always_false()
        {
            static_assert(!meta::always_false<bool>);
            static_assert(!meta::always_false<i32>);
            static_assert(!meta::always_false<const i32>);
            static_assert(!meta::always_false<str>);
            static_assert(!meta::always_false<decltype("")>);

            return true;
        }

        constexpr bool test_meta_any()
        {
            static_assert(meta::any_v<std::is_unsigned, u16>);
            static_assert(meta::any_v<std::is_unsigned, u16, u32>);
            static_assert(meta::any_v<std::is_unsigned, i16, u32>);
            static_assert(meta::any_v<std::is_unsigned, u16, i32>);

            static_assert(!meta::any_v<std::is_unsigned>);
            static_assert(!meta::any_v<std::is_unsigned, i16>);
            static_assert(!meta::any_v<std::is_unsigned, i16, i32>);

            return true;
        }

        constexpr bool test_meta_none()
        {
            static_assert(meta::none_v<std::is_unsigned>);
            static_assert(meta::none_v<std::is_unsigned, i16>);
            static_assert(meta::none_v<std::is_unsigned, i16, i32>);

            static_assert(!meta::none_v<std::is_unsigned, u16>);
            static_assert(!meta::none_v<std::is_unsigned, u16, u32>);
            static_assert(!meta::none_v<std::is_unsigned, i16, u32>);
            static_assert(!meta::none_v<std::is_unsigned, u16, i32>);

            return true;
        }

        constexpr bool test_meta_index()
        {
            constexpr auto idx = meta::index<123>;
            static_assert(std::is_same_v<decltype(idx), const std::integral_constant<usize, 123>>);
            static_assert(std::is_same_v<decltype(idx)::value_type, usize>);
            static_assert(sizeof(idx) == 1);
            static_assert(idx.value == 123);

            return true;
        }

        constexpr bool test_meta_type()
        {
            constexpr auto t = meta::type<cstrview>;
            static_assert(std::is_same_v<decltype(t), const std::type_identity<cstrview>>);
            static_assert(std::is_same_v<decltype(t)::type, cstrview>);
            static_assert(sizeof(t) == 1);

            return true;
        }

        constexpr bool test_not_deduced()
        {
            static_assert(std::is_same_v<not_deduced_t<int>, int>);
            static_assert(std::is_same_v<not_deduced_t<const int>, const int>);

            return true;
        }

        constexpr bool test_not_null()
        {
            auto src = to_array("Hello");
            static_assert(std::is_same_v<decltype(src), array<char, 5>>);

            const not_null<char*> a{src.begin()};
            snn_require(a.get() == src.begin());

            // Conversion
            const not_null<const char*> b{a};
            snn_require(b.get() == src.begin());

            // Deduction
            not_null c{src.begin()};
            static_assert(std::is_same_v<decltype(c), not_null<char*>>);
            snn_require(c.get() == src.begin());

            return true;
        }

        constexpr bool test_not_zero()
        {
            static_assert(not_zero{3}.get() == 3);
            static_assert(not_zero{42}.get() == 42);

            constexpr not_zero<u16> a{123};
            constexpr not_zero<u16> b{123};
            constexpr not_zero<u16> c{92};

            static_assert(a == b);
            static_assert(!(a == c));

            static_assert(b != c);
            static_assert(!(a != b));

            static_assert(c < a);
            static_assert(!(a < c));

            static_assert(a > c);
            static_assert(!(c > a));

            return true;
        }

        constexpr bool test_numeric()
        {
            decltype(auto) a = as_num(123);
            static_assert(std::is_same_v<decltype(a), numeric<int>>);
            snn_require(a.get() == 123);

            a = as_num(456);
            snn_require(a.get() == 456);

            decltype(auto) b = as_num<i16>(78);
            static_assert(std::is_same_v<decltype(b), numeric<i16>>);
            snn_require(b.get() == 78);

            numeric<i64> c = b; // Implicit conversion from `i16` to `i32` is allowed.
            snn_require(c.get() == 78);

            static_assert(std::is_convertible_v<numeric<i8>, numeric<i8>>);
            static_assert(std::is_convertible_v<numeric<i8>, numeric<i16>>);
            static_assert(std::is_convertible_v<numeric<i8>, numeric<i32>>);
            static_assert(std::is_convertible_v<numeric<i8>, numeric<i64>>);

            static_assert(std::is_convertible_v<numeric<u8>, numeric<u8>>);
            static_assert(std::is_convertible_v<numeric<u8>, numeric<u16>>);
            static_assert(std::is_convertible_v<numeric<u8>, numeric<u32>>);
            static_assert(std::is_convertible_v<numeric<u8>, numeric<u64>>);

            static_assert(!std::is_convertible_v<numeric<i8>, numeric<u8>>);
            static_assert(!std::is_convertible_v<numeric<i8>, numeric<u16>>);
            static_assert(!std::is_convertible_v<numeric<i8>, numeric<u32>>);
            static_assert(!std::is_convertible_v<numeric<i8>, numeric<u64>>);

            static_assert(!std::is_convertible_v<numeric<u8>, numeric<i8>>);
            static_assert(!std::is_convertible_v<numeric<u8>, numeric<i16>>);
            static_assert(!std::is_convertible_v<numeric<u8>, numeric<i32>>);
            static_assert(!std::is_convertible_v<numeric<u8>, numeric<i64>>);

            static_assert(!std::is_convertible_v<numeric<i64>, numeric<i8>>);
            static_assert(!std::is_convertible_v<numeric<i64>, numeric<i16>>);
            static_assert(!std::is_convertible_v<numeric<i64>, numeric<i32>>);

            static_assert(std::is_convertible_v<numeric<float>, numeric<float>>);

            static_assert(!std::is_convertible_v<numeric<float>, numeric<double>>);
            static_assert(!std::is_convertible_v<numeric<double>, numeric<float>>);

            return true;
        }

        constexpr bool test_octet()
        {
            static_assert(octet<char>);
            static_assert(octet<const char>);
            static_assert(octet<signed char>);
            static_assert(octet<const signed char>);
            static_assert(octet<unsigned char>);
            static_assert(octet<const unsigned char>);
            static_assert(octet<u8>);
            static_assert(octet<const u8>);
            static_assert(octet<i8>);
            static_assert(octet<const i8>);
            static_assert(octet<byte>);
            static_assert(octet<const byte>);
            static_assert(octet<char8_t>);
            static_assert(octet<const char8_t>);

            static_assert(!octet<int>);
            static_assert(!octet<bool>);
            static_assert(!octet<const bool>);
            static_assert(!octet<char&>);
            static_assert(!octet<const char&>);
            static_assert(!octet<char*>);
            static_assert(!octet<const char*>);
            static_assert(!octet<cstrview>);

            return true;
        }

        constexpr bool test_pointer()
        {
            static_assert(pointer<int*>);
            static_assert(pointer<const float*>);
            static_assert(!pointer<int&>);
            static_assert(!pointer<const int&>);
            static_assert(!pointer<cstrview>);
            static_assert(!pointer<const cstrview>);

            return true;
        }

        constexpr bool test_power_of_two()
        {
            static_assert(power_of_two<1>);
            static_assert(power_of_two<2>);
            static_assert(power_of_two<4>);
            static_assert(power_of_two<8>);
            static_assert(power_of_two<16>);
            static_assert(power_of_two<32>);
            static_assert(power_of_two<64>);
            static_assert(power_of_two<128>);
            static_assert(power_of_two<256>);
            static_assert(power_of_two<512>);
            static_assert(power_of_two<1024>);

            static_assert(!power_of_two<0>);
            static_assert(!power_of_two<3>);
            static_assert(!power_of_two<5>);
            static_assert(!power_of_two<6>);
            static_assert(!power_of_two<7>);
            static_assert(!power_of_two<9>);
            static_assert(!power_of_two<10>);
            static_assert(!power_of_two<11>);
            static_assert(!power_of_two<12>);
            static_assert(!power_of_two<13>);
            static_assert(!power_of_two<14>);
            static_assert(!power_of_two<15>);
            static_assert(!power_of_two<44>);
            static_assert(!power_of_two<1234>);

            return true;
        }

        constexpr bool test_predicate()
        {
            static_assert(predicate<fn::is_zero, int>);
            static_assert(predicate<fn::less_than, int, int>);
            static_assert(!predicate<int>);
            static_assert(!predicate<int, int>);

            return true;
        }

        constexpr bool test_promote()
        {
            static_assert(promote(123) == 123);
            static_assert(promote(true) == 1);
            static_assert(promote(false) == 0);
            static_assert(promote(constant::limit<i8>::min) == -128);
            static_assert(promote(constant::limit<u8>::max) == 255);

            static_assert(std::is_same_v<decltype(promote(bool{})), u32>);
            static_assert(std::is_same_v<decltype(promote<8>(bool{})), u8>);
            static_assert(std::is_same_v<decltype(promote<16>(bool{})), u16>);

            static_assert(std::is_same_v<decltype(promote(u32{})), u32>);
            static_assert(std::is_same_v<decltype(promote<32>(u32{})), u32>);
            static_assert(std::is_same_v<decltype(promote<64>(u32{})), u64>);

            static_assert(std::is_same_v<decltype(promote(i8{})), i32>);
            static_assert(std::is_same_v<decltype(promote<8>(i8{})), i8>);
            static_assert(std::is_same_v<decltype(promote<16>(i8{})), i16>);

            static_assert(std::is_same_v<decltype(promote(i32{})), i32>);
            static_assert(std::is_same_v<decltype(promote<32>(i32{})), i32>);
            static_assert(std::is_same_v<decltype(promote<64>(i32{})), i64>);

            return true;
        }

        constexpr bool test_promote_integral()
        {
            static_assert(std::is_same_v<promote_integral_t<bool, 8>, u8>);
            static_assert(std::is_same_v<promote_integral_t<bool, 16>, u16>);
            static_assert(std::is_same_v<promote_integral_t<bool, 32>, u32>);
            static_assert(std::is_same_v<promote_integral_t<bool, 64>, u64>);

            static_assert(std::is_same_v<promote_integral_t<u8, 8>, u8>);
            static_assert(std::is_same_v<promote_integral_t<u8, 16>, u16>);
            static_assert(std::is_same_v<promote_integral_t<u8, 32>, u32>);
            static_assert(std::is_same_v<promote_integral_t<u8, 64>, u64>);

            static_assert(std::is_same_v<promote_integral_t<u16, 8>, u16>);
            static_assert(std::is_same_v<promote_integral_t<u16, 16>, u16>);
            static_assert(std::is_same_v<promote_integral_t<u16, 32>, u32>);
            static_assert(std::is_same_v<promote_integral_t<u16, 64>, u64>);

            static_assert(std::is_same_v<promote_integral_t<const u16, 8>, u16>);
            static_assert(std::is_same_v<promote_integral_t<const u16, 16>, u16>);
            static_assert(std::is_same_v<promote_integral_t<const u16, 32>, u32>);
            static_assert(std::is_same_v<promote_integral_t<const u16, 64>, u64>);

            static_assert(std::is_same_v<promote_integral_t<u32, 8>, u32>);
            static_assert(std::is_same_v<promote_integral_t<u32, 16>, u32>);
            static_assert(std::is_same_v<promote_integral_t<u32, 32>, u32>);
            static_assert(std::is_same_v<promote_integral_t<u32, 64>, u64>);

            static_assert(std::is_same_v<promote_integral_t<u64, 8>, u64>);
            static_assert(std::is_same_v<promote_integral_t<u64, 16>, u64>);
            static_assert(std::is_same_v<promote_integral_t<u64, 32>, u64>);
            static_assert(std::is_same_v<promote_integral_t<u64, 64>, u64>);

            static_assert(std::is_same_v<promote_integral_t<char8_t, 8>, u8>);
            static_assert(std::is_same_v<promote_integral_t<char8_t, 16>, u16>);
            static_assert(std::is_same_v<promote_integral_t<char8_t, 32>, u32>);
            static_assert(std::is_same_v<promote_integral_t<char8_t, 64>, u64>);

            static_assert(std::is_same_v<promote_integral_t<char16_t, 8>, u16>);
            static_assert(std::is_same_v<promote_integral_t<char16_t, 16>, u16>);
            static_assert(std::is_same_v<promote_integral_t<char16_t, 32>, u32>);
            static_assert(std::is_same_v<promote_integral_t<char16_t, 64>, u64>);

            static_assert(std::is_same_v<promote_integral_t<char32_t, 8>, u32>);
            static_assert(std::is_same_v<promote_integral_t<char32_t, 16>, u32>);
            static_assert(std::is_same_v<promote_integral_t<char32_t, 32>, u32>);
            static_assert(std::is_same_v<promote_integral_t<char32_t, 64>, u64>);

            static_assert(std::is_same_v<promote_integral_t<i8, 8>, i8>);
            static_assert(std::is_same_v<promote_integral_t<i8, 16>, i16>);
            static_assert(std::is_same_v<promote_integral_t<i8, 32>, i32>);
            static_assert(std::is_same_v<promote_integral_t<i8, 64>, i64>);

            static_assert(std::is_same_v<promote_integral_t<i16, 8>, i16>);
            static_assert(std::is_same_v<promote_integral_t<i16, 16>, i16>);
            static_assert(std::is_same_v<promote_integral_t<i16, 32>, i32>);
            static_assert(std::is_same_v<promote_integral_t<i16, 64>, i64>);

            static_assert(std::is_same_v<promote_integral_t<const i16, 8>, i16>);
            static_assert(std::is_same_v<promote_integral_t<const i16, 16>, i16>);
            static_assert(std::is_same_v<promote_integral_t<const i16, 32>, i32>);
            static_assert(std::is_same_v<promote_integral_t<const i16, 64>, i64>);

            static_assert(std::is_same_v<promote_integral_t<i32, 8>, i32>);
            static_assert(std::is_same_v<promote_integral_t<i32, 16>, i32>);
            static_assert(std::is_same_v<promote_integral_t<i32, 32>, i32>);
            static_assert(std::is_same_v<promote_integral_t<i32, 64>, i64>);

            static_assert(std::is_same_v<promote_integral_t<i64, 8>, i64>);
            static_assert(std::is_same_v<promote_integral_t<i64, 16>, i64>);
            static_assert(std::is_same_v<promote_integral_t<i64, 32>, i64>);
            static_assert(std::is_same_v<promote_integral_t<i64, 64>, i64>);

#if SNN_INT128_ENABLED
            static_assert(std::is_same_v<promote_integral_t<bool, 128>, u128>);
            static_assert(std::is_same_v<promote_integral_t<u8, 128>, u128>);
            static_assert(std::is_same_v<promote_integral_t<u16, 128>, u128>);
            static_assert(std::is_same_v<promote_integral_t<u32, 128>, u128>);
            static_assert(std::is_same_v<promote_integral_t<u64, 128>, u128>);
            static_assert(std::is_same_v<promote_integral_t<u128, 128>, u128>);

            static_assert(std::is_same_v<promote_integral_t<i8, 128>, i128>);
            static_assert(std::is_same_v<promote_integral_t<i16, 128>, i128>);
            static_assert(std::is_same_v<promote_integral_t<i32, 128>, i128>);
            static_assert(std::is_same_v<promote_integral_t<i64, 128>, i128>);
            static_assert(std::is_same_v<promote_integral_t<i128, 128>, i128>);
#endif

            return true;
        }

        constexpr bool test_same_as()
        {
            static_assert(same_as<int, int>);
            static_assert(same_as<const int, const int>);
            static_assert(!same_as<int, const int>);
            static_assert(!same_as<const int, int>);
            static_assert(!same_as<char, i8>);
            static_assert(!same_as<char, u8>);

            return true;
        }

        constexpr bool test_same_signedness_as()
        {
            static_assert(same_signedness_as<u32, u8>);
            static_assert(same_signedness_as<u32, u16>);
            static_assert(same_signedness_as<u32, u32>);
            static_assert(same_signedness_as<u32, u64>);

            static_assert(same_signedness_as<i32, i8>);
            static_assert(same_signedness_as<i32, i16>);
            static_assert(same_signedness_as<i32, i32>);
            static_assert(same_signedness_as<i32, i64>);

            static_assert(!same_signedness_as<u32, i8>);
            static_assert(!same_signedness_as<u32, i16>);
            static_assert(!same_signedness_as<u32, i32>);
            static_assert(!same_signedness_as<u32, i64>);

            static_assert(!same_signedness_as<str, u32>);
            static_assert(!same_signedness_as<u32, str>);
            static_assert(!same_signedness_as<str, str>);

            return true;
        }

        constexpr bool test_semi_const()
        {
            semi_const<int> i = 123;
            snn_require(i.get() == 123);
            static_assert(std::is_same_v<decltype(i.get()), const int&>);

            // i = 999; // Will not compile, but `i` can be moved as if it was a plain `int`.
            static_assert(!std::is_assignable_v<semi_const<int>&, int>);
            static_assert(std::is_assignable_v<int&, int>); // If `i` was a regular `int`.

            // Comparison.
            constexpr semi_const<int> a = 123;
            constexpr semi_const<int> b = 456;
            constexpr semi_const<int> c = 456;
            static_assert(a != b);
            static_assert(a < b);
            static_assert(b > a);
            static_assert(b == c);

            // Copy constructor.
            semi_const<int> d{a};
            snn_require(d.get() == 123);

            // Move constructor.
            semi_const<int> e{std::move(d)};
            snn_require(e.get() == 123);

            // Useful as a const class member that doesn't break `sane` requirements.

            struct good_person
            {
                semi_const<str> name;
            };

            static_assert(sane<good_person>);

            static_assert(std::is_copy_constructible_v<good_person>);
            static_assert(std::is_move_constructible_v<good_person>);
            static_assert(std::is_nothrow_move_constructible_v<good_person>); // Important

            static_assert(!std::is_copy_assignable_v<good_person>);
            static_assert(!std::is_move_assignable_v<good_person>);
            static_assert(!std::is_nothrow_move_assignable_v<good_person>);

            struct bad_person
            {
                const str name; // Don't have const member variables!
            };

            static_assert(!sane<bad_person>);

            static_assert(std::is_copy_constructible_v<bad_person>);
            static_assert(std::is_move_constructible_v<bad_person>);
            static_assert(!std::is_nothrow_move_constructible_v<bad_person>); // Important

            static_assert(!std::is_copy_assignable_v<bad_person>);
            static_assert(!std::is_move_assignable_v<bad_person>);
            static_assert(!std::is_nothrow_move_assignable_v<bad_person>);

            return true;
        }

        constexpr bool test_signed_integral()
        {
            static_assert(signed_integral<i8>);
            static_assert(signed_integral<int>);
            static_assert(signed_integral<const int>);
            static_assert(!signed_integral<usize>);
            static_assert(!signed_integral<const usize>);
            static_assert(!signed_integral<int&>);
            static_assert(!signed_integral<int*>);
            static_assert(!signed_integral<float>);
            static_assert(!signed_integral<cstrview>);

#if SNN_INT128_ENABLED
            static_assert(signed_integral<i128>);
            static_assert(!signed_integral<u128>);
#endif

            return true;
        }

        constexpr bool test_strict_integral()
        {
            static_assert(strict_integral<char>);
            static_assert(strict_integral<const char>);
            static_assert(strict_integral<wchar_t>);
            static_assert(strict_integral<char8_t>);
            static_assert(strict_integral<char16_t>);
            static_assert(strict_integral<char32_t>);
            static_assert(strict_integral<int>);
            static_assert(strict_integral<usize>);
            static_assert(strict_integral<const int>);
            static_assert(strict_integral<const usize>);
            static_assert(!strict_integral<bool>);
            static_assert(!strict_integral<int&>);
            static_assert(!strict_integral<int*>);
            static_assert(!strict_integral<float>);
            static_assert(!strict_integral<cstrview>);

#if SNN_INT128_ENABLED
            static_assert(strict_integral<i128>);
            static_assert(strict_integral<u128>);
#endif

            return true;
        }

        constexpr bool test_strict_integral_min()
        {
            static_assert(strict_integral_min<char, 8>);
            static_assert(strict_integral_min<int, 8>);
            static_assert(strict_integral_min<usize, 8>);
            static_assert(strict_integral_min<int, 16>);
            static_assert(strict_integral_min<const int, 16>);
            static_assert(strict_integral_min<usize, 16>);
            static_assert(strict_integral_min<usize, 64>);

            static_assert(!strict_integral_min<char, 16>);
            static_assert(!strict_integral_min<char, 64>);
            static_assert(!strict_integral_min<int&, 16>);
            static_assert(!strict_integral_min<int, 64>);
            static_assert(!strict_integral_min<bool, 8>);
            static_assert(!strict_integral_min<float, 8>);
            static_assert(!strict_integral_min<cstrview, 8>);

#if SNN_INT128_ENABLED
            static_assert(strict_integral_min<i128, 128>);
            static_assert(strict_integral_min<u128, 128>);
            static_assert(!strict_integral_min<i64, 128>);
            static_assert(!strict_integral_min<u64, 128>);
#endif

            return true;
        }

        constexpr bool test_strict_signed_integral()
        {
            static_assert(strict_signed_integral<i8>);
            static_assert(strict_signed_integral<int>);
            static_assert(strict_signed_integral<const int>);
            static_assert(!strict_signed_integral<usize>);
            static_assert(!strict_signed_integral<const usize>);
            static_assert(!strict_signed_integral<int&>);
            static_assert(!strict_signed_integral<int*>);
            static_assert(!strict_signed_integral<float>);
            static_assert(!strict_signed_integral<cstrview>);

            static_assert(strict_signed_integral<char> != strict_unsigned_integral<char>);

#if SNN_INT128_ENABLED
            static_assert(strict_signed_integral<i128>);
            static_assert(!strict_signed_integral<u128>);
#endif

            return true;
        }

        constexpr bool test_strict_unsigned_integral()
        {
            static_assert(strict_unsigned_integral<u8>);
            static_assert(strict_unsigned_integral<char8_t>);
            static_assert(strict_unsigned_integral<char16_t>);
            static_assert(strict_unsigned_integral<char32_t>);
            static_assert(strict_unsigned_integral<usize>);
            static_assert(strict_unsigned_integral<const usize>);
            static_assert(!strict_unsigned_integral<bool>);
            static_assert(!strict_unsigned_integral<int>);
            static_assert(!strict_unsigned_integral<const int>);
            static_assert(!strict_unsigned_integral<usize&>);
            static_assert(!strict_unsigned_integral<usize*>);
            static_assert(!strict_unsigned_integral<float>);

            static_assert(strict_signed_integral<char> != strict_unsigned_integral<char>);

#if SNN_INT128_ENABLED
            static_assert(strict_unsigned_integral<u128>);
            static_assert(!strict_unsigned_integral<i128>);
#endif

            return true;
        }

        constexpr bool test_string_size()
        {
            static_assert(string_size("") == 0);
            static_assert(string_size("a") == 1);
            static_assert(string_size("ab") == 2);
            static_assert(string_size("abc") == 3);
            static_assert(string_size("Å") == 2);
            static_assert(string_size("ÅÄÖ") == 6);
            static_assert(string_size("abc\0def") == 7);
            static_assert(string_size("\0") == 1);
            static_assert(string_size("\0\0\0") == 3);

            return true;
        }

        constexpr bool test_to_byte()
        {
            constexpr char c = 'A';
            static_assert(to_byte(c) == 65);
            static_assert(to_byte('B') == 66);
            static_assert(std::is_same_v<decltype(to_byte(c)), byte>);

            return true;
        }

        constexpr bool test_to_char()
        {
            constexpr byte b = 65;
            static_assert(to_char(b) == 'A');
            static_assert(to_char(66) == 'B');
            static_assert(std::is_same_v<decltype(to_char(b)), char>);

            return true;
        }

        constexpr bool test_to_i8()
        {
            constexpr u8 i = 255;
            static_assert(to_i8(i) == -1);
            static_assert(to_i8(123u) == 123);
            static_assert(std::is_same_v<decltype(to_i8(i)), i8>);

            return true;
        }

        constexpr bool test_to_u8()
        {
            constexpr i8 i = -1;
            static_assert(to_u8(i) == 255);
            static_assert(to_u8(123) == 123);
            static_assert(std::is_same_v<decltype(to_u8(i)), u8>);

            return true;
        }

        constexpr bool test_to_isize()
        {
            constexpr usize i = 18446744073709551615u;
            static_assert(to_isize(i) == -1);
            static_assert(to_isize(1234567890123456789u) == 1234567890123456789);
            static_assert(std::is_same_v<decltype(to_isize(i)), isize>);

            return true;
        }

        constexpr bool test_to_usize()
        {
            constexpr isize i = -1;
            static_assert(to_usize(i) == 18446744073709551615u);
            static_assert(to_usize(1234567890123456789) == 1234567890123456789u);
            static_assert(std::is_same_v<decltype(to_usize(i)), usize>);

            return true;
        }

        constexpr bool test_to_i16()
        {
            constexpr u16 i = 65535;
            static_assert(to_i16(i) == -1);
            static_assert(to_i16(1234u) == 1234);
            static_assert(std::is_same_v<decltype(to_i16(i)), i16>);

            return true;
        }

        constexpr bool test_to_u16()
        {
            constexpr i16 i = -1;
            static_assert(to_u16(i) == 65535u);
            static_assert(to_u16(1234) == 1234u);
            static_assert(std::is_same_v<decltype(to_u16(i)), u16>);

            return true;
        }

        constexpr bool test_to_i32()
        {
            constexpr u32 i = 4294967295u;
            static_assert(to_i32(i) == -1);
            static_assert(to_i32(1234567890u) == 1234567890);
            static_assert(std::is_same_v<decltype(to_i32(i)), i32>);

            return true;
        }

        constexpr bool test_to_u32()
        {
            constexpr i32 i = -1;
            static_assert(to_u32(i) == 4294967295u);
            static_assert(to_u32(1234567890) == 1234567890u);
            static_assert(std::is_same_v<decltype(to_u32(i)), u32>);

            return true;
        }

        constexpr bool test_to_i64()
        {
            constexpr u64 i = 18446744073709551615u;
            static_assert(to_i64(i) == -1);
            static_assert(to_i64(1234567890123456789u) == 1234567890123456789);
            static_assert(std::is_same_v<decltype(to_i64(i)), i64>);

            return true;
        }

        constexpr bool test_to_u64()
        {
            constexpr i64 i = -1;
            static_assert(to_u64(i) == 18446744073709551615u);
            static_assert(to_u64(1234567890123456789) == 1234567890123456789u);
            static_assert(std::is_same_v<decltype(to_u64(i)), u64>);

            return true;
        }

        constexpr bool test_to_underlying()
        {
            enum class one_two : u8
            {
                one = 1,
                two = 2,
            };

            static_assert(to_underlying(one_two::one) == 1);
            static_assert(to_underlying(one_two::two) == 2);

            static_assert(std::is_same_v<decltype(to_underlying(one_two::one)), u8>);

            enum class foo_bar // int
            {
                foo = 123,
                bar = 456,
            };

            static_assert(to_underlying(foo_bar::foo) == 123);
            static_assert(to_underlying(foo_bar::bar) == 456);

            static_assert(std::is_same_v<decltype(to_underlying(foo_bar::foo)), int>);

            return true;
        }

        constexpr bool test_transient()
        {
            // `cstrview` will not bind to a temporary `str`.
            static_assert(!std::is_constructible_v<cstrview, str>);

            // But a transient `cstrview` will.
            static_assert(std::is_constructible_v<transient<cstrview>, str>);

            static_assert(sizeof(transient<cstrview>) == sizeof(cstrview));

            static_assert(is_trivially_relocatable_v<transient<cstrview>>);
            static_assert(std::is_trivially_copy_constructible_v<transient<cstrview>>);
            static_assert(std::is_nothrow_copy_constructible_v<transient<cstrview>>);
            static_assert(std::is_trivially_move_constructible_v<transient<cstrview>>);
            static_assert(std::is_nothrow_move_constructible_v<transient<cstrview>>);
            static_assert(std::is_trivially_copy_assignable_v<transient<cstrview>>);
            static_assert(std::is_nothrow_copy_assignable_v<transient<cstrview>>);
            static_assert(std::is_trivially_move_assignable_v<transient<cstrview>>);
            static_assert(std::is_nothrow_move_assignable_v<transient<cstrview>>);
            static_assert(std::is_trivially_destructible_v<transient<cstrview>>);

            return true;
        }

        constexpr bool test_trivially_relocatable_if()
        {
            static_assert(std::is_same_v<trivially_relocatable_if_t<i32, str>, i32>);
            static_assert(std::is_same_v<trivially_relocatable_if_t<i32, str, vec<i8>>, i32>);

            static_assert(std::is_same_v<trivially_relocatable_if_t<i32, vec<i8, 1>>, void>);
            static_assert(std::is_same_v<trivially_relocatable_if_t<i32, str, vec<i8, 1>>, void>);
            static_assert(std::is_same_v<trivially_relocatable_if_t<i32, vec<i8, 1>, str>, void>);

            return true;
        }

        constexpr bool test_unsigned_integral()
        {
            static_assert(unsigned_integral<u8>);
            static_assert(unsigned_integral<usize>);
            static_assert(unsigned_integral<const usize>);
            static_assert(unsigned_integral<char8_t>);
            static_assert(unsigned_integral<char16_t>);
            static_assert(unsigned_integral<char32_t>);
            static_assert(!unsigned_integral<int>);
            static_assert(!unsigned_integral<const int>);
            static_assert(!unsigned_integral<usize&>);
            static_assert(!unsigned_integral<usize*>);
            static_assert(!unsigned_integral<float>);

#if SNN_INT128_ENABLED
            static_assert(unsigned_integral<u128>);
            static_assert(!unsigned_integral<i128>);
#endif

            return true;
        }

        constexpr bool test_value_type_or()
        {
            static_assert(value_type_or<int&, int&>);
            static_assert(value_type_or<const int&, const int&>);
            static_assert(value_type_or<int, int&>);
            static_assert(value_type_or<int, const int&>);

            static_assert(!value_type_or<int&, const int&>);
            static_assert(!value_type_or<const int&, int&>);

            static_assert(!value_type_or<const int, int&>);
            static_assert(!value_type_or<const int, const int&>);

            static_assert(!value_type_or<char&, int&>);
            static_assert(!value_type_or<const char&, const int&>);
            static_assert(!value_type_or<char, int&>);
            static_assert(!value_type_or<char, const int&>);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_arithmetic());
        snn_static_require(app::test_array_count());
        snn_static_require(app::test_brace_constructible_from());
        snn_static_require(app::test_byte_size());
        snn_static_require(app::test_character());
        snn_static_require(app::test_constant());
        snn_static_require(app::test_constant_exit());
        snn_require(app::test_constant_fp());
        snn_static_require(app::test_constant_limit());
        snn_static_require(app::test_constant_size());
        snn_static_require(app::test_constructible_from());
        snn_static_require(app::test_constructible_from_iterators());
        snn_static_require(app::test_convertible_to());
        snn_static_require(app::test_deep_const());
        snn_static_require(app::test_explicitly_default_constructible());
        snn_static_require(app::test_floating_point());
        snn_static_require(app::test_force_signed());
        snn_static_require(app::test_force_unsigned());
        snn_static_require(app::test_front_value());
        snn_static_require(app::test_has_at());
        snn_static_require(app::test_has_contiguous_iterator());
        snn_static_require(app::test_has_count());
        snn_static_require(app::test_has_drop_back());
        snn_static_require(app::test_has_drop_front());
        snn_static_require(app::test_ignore_if_unused());
        snn_static_require(app::test_implicitly_default_constructible());
        snn_static_require(app::test_has_to());
        snn_static_require(app::test_has_append_inplace());
        snn_static_require(app::test_integral());
        snn_static_require(app::test_callable());
        snn_static_require(app::test_sane());
        snn_static_require(app::test_is_strict_integral());
        snn_static_require(app::test_remove_cv_rvalue_ref());
        snn_static_require(app::test_is_trivially_relocatable());
        snn_static_require(app::test_legacy_iterable());
        snn_static_require(app::test_loop_count());
        snn_static_require(app::test_meta_all());
        snn_static_require(app::test_meta_always_false());
        snn_static_require(app::test_meta_any());
        snn_static_require(app::test_meta_index());
        snn_static_require(app::test_meta_type());
        snn_static_require(app::test_meta_none());
        snn_static_require(app::test_not_deduced());
        snn_static_require(app::test_not_null());
        snn_static_require(app::test_not_zero());
        snn_static_require(app::test_numeric());
        snn_static_require(app::test_octet());
        snn_static_require(app::test_pointer());
        snn_static_require(app::test_power_of_two());
        snn_static_require(app::test_predicate());
        snn_static_require(app::test_promote());
        snn_static_require(app::test_promote_integral());
        snn_static_require(app::test_same_as());
        snn_static_require(app::test_same_signedness_as());
        snn_static_require(app::test_semi_const());
        snn_static_require(app::test_signed_integral());
        snn_static_require(app::test_strict_integral());
        snn_static_require(app::test_strict_integral_min());
        snn_static_require(app::test_strict_signed_integral());
        snn_static_require(app::test_strict_unsigned_integral());
        snn_static_require(app::test_string_size());
        snn_static_require(app::test_to_byte());
        snn_static_require(app::test_to_char());
        snn_static_require(app::test_to_i16());
        snn_static_require(app::test_to_i32());
        snn_static_require(app::test_to_i64());
        snn_static_require(app::test_to_i8());
        snn_static_require(app::test_to_isize());
        snn_static_require(app::test_to_u16());
        snn_static_require(app::test_to_u32());
        snn_static_require(app::test_to_u64());
        snn_static_require(app::test_to_u8());
        snn_static_require(app::test_to_underlying());
        snn_static_require(app::test_to_usize());
        snn_static_require(app::test_transient());
        snn_static_require(app::test_trivially_relocatable_if());
        snn_static_require(app::test_unsigned_integral());
        snn_static_require(app::test_value_type_or());

        // Macro constants.
        static_assert(std::is_same_v<decltype(SNN_ADDRESS_SANITIZER_ENABLED), int>);
        static_assert(std::is_same_v<decltype(SNN_ASSERT_ENABLED), int>);
        static_assert(std::is_same_v<decltype(SNN_SHOULD_ENABLED), int>);
        static_assert(std::is_same_v<decltype(SNN_INT128_ENABLED), int>);

        // Types aliases.
        static_assert(sizeof(usize) == sizeof(isize));

        // Posix types.
        static_assert(std::is_same_v<isize, ssize_t>);
        static_assert(std::is_same_v<isize, off_t>);

        // ASCII character literals.
        static_assert('\0' == 0);
        static_assert('\a' == 7);
        static_assert('\b' == 8);
        static_assert('\t' == 9);
        static_assert('\n' == 10);
        static_assert('\v' == 11);
        static_assert('\f' == 12);
        static_assert('\r' == 13);
        static_assert(' ' == 32);
        static_assert('!' == 33);
        static_assert('"' == 34);
        static_assert('#' == 35);
        static_assert('$' == 36);
        static_assert('%' == 37);
        static_assert('&' == 38);
        static_assert('\'' == 39);
        static_assert('(' == 40);
        static_assert(')' == 41);
        static_assert('*' == 42);
        static_assert('+' == 43);
        static_assert(',' == 44);
        static_assert('-' == 45);
        static_assert('.' == 46);
        static_assert('/' == 47);
        static_assert('0' == 48);
        static_assert('1' == 49);
        static_assert('2' == 50);
        static_assert('3' == 51);
        static_assert('4' == 52);
        static_assert('5' == 53);
        static_assert('6' == 54);
        static_assert('7' == 55);
        static_assert('8' == 56);
        static_assert('9' == 57);
        static_assert(':' == 58);
        static_assert(';' == 59);
        static_assert('<' == 60);
        static_assert('=' == 61);
        static_assert('>' == 62);
        static_assert('?' == 63);
        static_assert('@' == 64);
        static_assert('A' == 65);
        static_assert('B' == 66);
        static_assert('C' == 67);
        static_assert('D' == 68);
        static_assert('E' == 69);
        static_assert('F' == 70);
        static_assert('G' == 71);
        static_assert('H' == 72);
        static_assert('I' == 73);
        static_assert('J' == 74);
        static_assert('K' == 75);
        static_assert('L' == 76);
        static_assert('M' == 77);
        static_assert('N' == 78);
        static_assert('O' == 79);
        static_assert('P' == 80);
        static_assert('Q' == 81);
        static_assert('R' == 82);
        static_assert('S' == 83);
        static_assert('T' == 84);
        static_assert('U' == 85);
        static_assert('V' == 86);
        static_assert('W' == 87);
        static_assert('X' == 88);
        static_assert('Y' == 89);
        static_assert('Z' == 90);
        static_assert('[' == 91);
        static_assert('\\' == 92);
        static_assert(']' == 93);
        static_assert('^' == 94);
        static_assert('_' == 95);
        static_assert('`' == 96);
        static_assert('a' == 97);
        static_assert('b' == 98);
        static_assert('c' == 99);
        static_assert('d' == 100);
        static_assert('e' == 101);
        static_assert('f' == 102);
        static_assert('g' == 103);
        static_assert('h' == 104);
        static_assert('i' == 105);
        static_assert('j' == 106);
        static_assert('k' == 107);
        static_assert('l' == 108);
        static_assert('m' == 109);
        static_assert('n' == 110);
        static_assert('o' == 111);
        static_assert('p' == 112);
        static_assert('q' == 113);
        static_assert('r' == 114);
        static_assert('s' == 115);
        static_assert('t' == 116);
        static_assert('u' == 117);
        static_assert('v' == 118);
        static_assert('w' == 119);
        static_assert('x' == 120);
        static_assert('y' == 121);
        static_assert('z' == 122);
        static_assert('{' == 123);
        static_assert('|' == 124);
        static_assert('}' == 125);
        static_assert('~' == 126);

        // In C++ 'x' is a char literal and sizeof(char) is always 1.
        static_assert(sizeof('x') == 1);
    }
}
