// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/ascii/to_integral_prefix.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/range/forward.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(ascii::to_integral_prefix<u32>(cstrrng{"38293"}) ==
                        pair::value_count<u32, usize>{38293, 5});
            snn_require(ascii::to_integral_prefix<i32>(cstrrng{"-38293"}) ==
                        pair::value_count<i32, usize>{-38293, 6});
            snn_require(ascii::to_integral_prefix<u32>(cstrrng{"38293abc"}) ==
                        pair::value_count<u32, usize>{38293, 5});
            snn_require(ascii::to_integral_prefix<i32>(cstrrng{"-38293abc"}) ==
                        pair::value_count<i32, usize>{-38293, 6});

            // Min/Max
            snn_require(ascii::to_integral_prefix<u32>(cstrrng{"0"}) ==
                        pair::value_count<u32, usize>{0, 1});
            snn_require(ascii::to_integral_prefix<u32>(cstrrng{"4294967295"}) ==
                        pair::value_count<u32, usize>{constant::limit<u32>::max, 10});
            snn_require(ascii::to_integral_prefix<i32>(cstrrng{"-2147483648"}) ==
                        pair::value_count<i32, usize>{constant::limit<i32>::min, 11});
            snn_require(ascii::to_integral_prefix<i32>(cstrrng{"2147483647"}) ==
                        pair::value_count<i32, usize>{constant::limit<i32>::max, 10});

            // Overflow/Invalid
            snn_require(ascii::to_integral_prefix<u32>(cstrrng{"4294967296"}) ==
                        pair::value_count<u32, usize>{0, 0});
            snn_require(ascii::to_integral_prefix<i32>(cstrrng{""}) ==
                        pair::value_count<i32, usize>{0, 0});
            snn_require(ascii::to_integral_prefix<i32>(cstrrng{"-"}) ==
                        pair::value_count<i32, usize>{0, 0});
            snn_require(ascii::to_integral_prefix<i32>(cstrrng{"-2147483649"}) ==
                        pair::value_count<i32, usize>{0, 0});

            return true;
        }
    }

    template <integral Int, math::base Base, usize MaxDigits>
    struct integral_prefix_tester
    {
        using pair_type = pair::value_count<Int, usize>;

        static constexpr pair_type parse(const cstrview s) noexcept
        {
            return ascii::to_integral_prefix<Int, Base, MaxDigits>(s.range());
        }

        static constexpr pair_type pair(const Int value, const usize count) noexcept
        {
            return pair_type{value, count};
        }

        static constexpr Int min() noexcept
        {
            return constant::limit<Int>::min;
        }

        static constexpr Int max() noexcept
        {
            return constant::limit<Int>::max;
        }
    };
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        {
            constexpr app::integral_prefix_tester<u64, math::base::decimal, 0> t;

            // Max
            static_assert(t.parse("18446744073709551615") == t.pair(t.max(), 20));
            // Overflow
            static_assert(t.parse("18446744073709551616") == t.pair(0, 0));
            static_assert(t.parse("99999999999999999999") == t.pair(0, 0));
            // Zero prefix.
            static_assert(t.parse("0184467440") == t.pair(0, 1));
            // Partial
            static_assert(t.parse("922337203x238") == t.pair(922337203, 9));
            static_assert(t.parse("3foo") == t.pair(3, 1));
            static_assert(t.parse("3\0") == t.pair(3, 1));
            // Valid
            static_assert(t.parse("7") == t.pair(7, 1));
            static_assert(t.parse("0") == t.pair(0, 1));
            static_assert(t.parse("18446744") == t.pair(18446744, 8));
            // Invalid
            static_assert(t.parse("-184467440") == t.pair(0, 0));
            static_assert(t.parse("-") == t.pair(0, 0));
            static_assert(t.parse("abc") == t.pair(0, 0));
            static_assert(t.parse("a1") == t.pair(0, 0));
            static_assert(t.parse("") == t.pair(0, 0));
            static_assert(t.parse("\0") == t.pair(0, 0));
            static_assert(t.parse("-\0") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<i64, math::base::decimal, 0> t;

            // Max
            static_assert(t.parse("9223372036854775807") == t.pair(t.max(), 19));
            // Overflow
            static_assert(t.parse("9223372036854775808") == t.pair(0, 0));
            // Min
            static_assert(t.parse("-9223372036854775808") == t.pair(t.min(), 20));
            // Overflow
            static_assert(t.parse("-9223372036854775809") == t.pair(0, 0));
            // Partial
            static_assert(t.parse("922337203x238") == t.pair(922337203, 9));
            static_assert(t.parse("-922337203x238") == t.pair(-922337203, 10));
            static_assert(t.parse("-7a") == t.pair(-7, 2));
            static_assert(t.parse("3foo") == t.pair(3, 1));
            static_assert(t.parse("3\0") == t.pair(3, 1));
            // Zero
            static_assert(t.parse("0") == t.pair(0, 1));
            static_assert(t.parse("-0") == t.pair(0, 2));
            // Zero prefix.
            static_assert(t.parse("-0922337203") == t.pair(0, 2));
            static_assert(t.parse("0922337203") == t.pair(0, 1));
            // Valid
            static_assert(t.parse("7") == t.pair(7, 1));
            static_assert(t.parse("-3") == t.pair(-3, 2));
            static_assert(t.parse("123") == t.pair(123, 3));
            static_assert(t.parse("-38272") == t.pair(-38272, 6));
            // Invalid
            static_assert(t.parse("-") == t.pair(0, 0));
            static_assert(t.parse("abc") == t.pair(0, 0));
            static_assert(t.parse("a1") == t.pair(0, 0));
            static_assert(t.parse("") == t.pair(0, 0));
            static_assert(t.parse("\0") == t.pair(0, 0));
            static_assert(t.parse("-\0") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<u32, math::base::decimal, 0> t;

            // Max
            static_assert(t.parse("4294967295") == t.pair(t.max(), 10));
            // Overflow
            static_assert(t.parse("4294967296") == t.pair(0, 0));
            static_assert(t.parse("18446744073709551616") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<i32, math::base::decimal, 0> t;

            // Max
            static_assert(t.parse("2147483647") == t.pair(t.max(), 10));
            // Overflow
            static_assert(t.parse("2147483648") == t.pair(0, 0));
            static_assert(t.parse("18446744073709551616") == t.pair(0, 0));
            // Min
            static_assert(t.parse("-2147483648") == t.pair(t.min(), 11));
            // Overflow
            static_assert(t.parse("-2147483649") == t.pair(0, 0));
            static_assert(t.parse("-9223372036854775809") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<u16, math::base::decimal, 0> t;

            // Max
            static_assert(t.parse("65535") == t.pair(t.max(), 5));
            // Overflow
            static_assert(t.parse("65536") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<u16, math::base::hex, 0> t;

            // Max
            static_assert(t.parse("ffFF") == t.pair(t.max(), 4));
            // Overflow
            static_assert(t.parse("1ffff") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<i16, math::base::decimal, 0> t;

            // Max
            static_assert(t.parse("32767") == t.pair(t.max(), 5));
            // Overflow
            static_assert(t.parse("32768") == t.pair(0, 0));
            // Min
            static_assert(t.parse("-32768") == t.pair(t.min(), 6));
            // Overflow
            static_assert(t.parse("-32769") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<i16, math::base::hex, 0> t;

            // Max
            static_assert(t.parse("7FfF") == t.pair(t.max(), 4));
            // Overflow
            static_assert(t.parse("8000") == t.pair(0, 0));
            // Min
            static_assert(t.parse("-8000") == t.pair(t.min(), 5));
            // Overflow
            static_assert(t.parse("-8001") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<u8, math::base::decimal, 0> t;

            // Max
            static_assert(t.parse("255") == t.pair(t.max(), 3));
            // Overflow
            static_assert(t.parse("256") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<u8, math::base::binary, 0> t;

            static_assert(t.parse("11011000") == t.pair(216, 8));

            // Max
            static_assert(t.parse("11111111") == t.pair(t.max(), 8));
            // Overflow
            static_assert(t.parse("100000000") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<u8, math::base::octal, 0> t;

            static_assert(t.parse("100") == t.pair(64, 3));
            static_assert(t.parse("1008") == t.pair(64, 3));

            // Max
            static_assert(t.parse("377") == t.pair(t.max(), 3));
            // Overflow
            static_assert(t.parse("400") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<u8, math::base::hex, 0> t;

            // Max
            static_assert(t.parse("ff") == t.pair(t.max(), 2));
            // Overflow
            static_assert(t.parse("100") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<i8, math::base::decimal, 0> t;

            // Max
            static_assert(t.parse("127") == t.pair(t.max(), 3));
            // Overflow
            static_assert(t.parse("128") == t.pair(0, 0));
            // Min
            static_assert(t.parse("-128") == t.pair(t.min(), 4));
            // Overflow
            static_assert(t.parse("-129") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<i8, math::base::binary, 0> t;

            static_assert(t.parse("-101") == t.pair(-5, 4));

            // Max
            static_assert(t.parse("1111111") == t.pair(t.max(), 7));
            // Overflow
            static_assert(t.parse("10000000") == t.pair(0, 0));
            // Min
            static_assert(t.parse("-10000000") == t.pair(t.min(), 9));
            // Overflow
            static_assert(t.parse("-10000001") == t.pair(0, 0));
        }

        {
            constexpr app::integral_prefix_tester<i8, math::base::octal, 0> t;

            static_assert(t.parse("-101") == t.pair(-65, 4));
            static_assert(t.parse("-1018") == t.pair(-65, 4));

            // Max
            static_assert(t.parse("177") == t.pair(t.max(), 3));
            // Overflow
            static_assert(t.parse("200") == t.pair(0, 0));
            // Min
            static_assert(t.parse("-200") == t.pair(t.min(), 4));
            // Overflow
            static_assert(t.parse("-201") == t.pair(0, 0));
        }

        // Max digits.
        {
            constexpr app::integral_prefix_tester<u16, math::base::decimal, 1> t;
            static_assert(t.parse("") == t.pair(0, 0));
            static_assert(t.parse("0") == t.pair(0, 1));
            static_assert(t.parse("01") == t.pair(0, 1));
            static_assert(t.parse("-") == t.pair(0, 0));
            static_assert(t.parse("-0") == t.pair(0, 0));
            static_assert(t.parse("abc") == t.pair(0, 0));
            static_assert(t.parse("6abc") == t.pair(6, 1));
            static_assert(t.parse("65abc") == t.pair(6, 1));
            static_assert(t.parse("65536") == t.pair(6, 1));
            static_assert(t.parse("999999999999999999999") == t.pair(9, 1));
        }
        {
            constexpr app::integral_prefix_tester<u16, math::base::decimal, 2> t;
            static_assert(t.parse("") == t.pair(0, 0));
            static_assert(t.parse("0") == t.pair(0, 1));
            static_assert(t.parse("01") == t.pair(0, 1));
            static_assert(t.parse("-") == t.pair(0, 0));
            static_assert(t.parse("-0") == t.pair(0, 0));
            static_assert(t.parse("abc") == t.pair(0, 0));
            static_assert(t.parse("6abc") == t.pair(6, 1));
            static_assert(t.parse("65abc") == t.pair(65, 2));
            static_assert(t.parse("65536") == t.pair(65, 2));
            static_assert(t.parse("999999999999999999999") == t.pair(99, 2));
        }
        {
            constexpr app::integral_prefix_tester<u16, math::base::hex, 2> t;
            static_assert(t.parse("") == t.pair(0, 0));
            static_assert(t.parse("0") == t.pair(0, 1));
            static_assert(t.parse("01") == t.pair(0, 1));
            static_assert(t.parse("-") == t.pair(0, 0));
            static_assert(t.parse("-0") == t.pair(0, 0));
            static_assert(t.parse("abc") == t.pair(171, 2));
            static_assert(t.parse("6abc") == t.pair(106, 2));
            static_assert(t.parse("65abc") == t.pair(101, 2));
            static_assert(t.parse("65536") == t.pair(101, 2));
            static_assert(t.parse("999999999999999999999") == t.pair(153, 2));
        }
        {
            constexpr app::integral_prefix_tester<u16, math::base::decimal, 3> t;
            static_assert(t.parse("") == t.pair(0, 0));
            static_assert(t.parse("0") == t.pair(0, 1));
            static_assert(t.parse("01") == t.pair(0, 1));
            static_assert(t.parse("-") == t.pair(0, 0));
            static_assert(t.parse("-0") == t.pair(0, 0));
            static_assert(t.parse("abc") == t.pair(0, 0));
            static_assert(t.parse("6abc") == t.pair(6, 1));
            static_assert(t.parse("65abc") == t.pair(65, 2));
            static_assert(t.parse("65536") == t.pair(655, 3));
            static_assert(t.parse("999999999999999999999") == t.pair(999, 3));
        }
        {
            constexpr app::integral_prefix_tester<u16, math::base::decimal, 4> t;
            static_assert(t.parse("") == t.pair(0, 0));
            static_assert(t.parse("0") == t.pair(0, 1));
            static_assert(t.parse("01") == t.pair(0, 1));
            static_assert(t.parse("-") == t.pair(0, 0));
            static_assert(t.parse("-0") == t.pair(0, 0));
            static_assert(t.parse("abc") == t.pair(0, 0));
            static_assert(t.parse("6abc") == t.pair(6, 1));
            static_assert(t.parse("65abc") == t.pair(65, 2));
            static_assert(t.parse("65536") == t.pair(6553, 4));
            static_assert(t.parse("999999999999999999999") == t.pair(9999, 4));
        }
        // Will not compile (5 digits can overflow u16).
        // {
        //     constexpr app::integral_prefix_tester<u16, math::base::decimal, 5> t;
        //     static_assert(t.parse("65535") == t.pair(65535, 5));
        // }

        {
            constexpr app::integral_prefix_tester<i16, math::base::decimal, 1> t;
            static_assert(t.parse("") == t.pair(0, 0));
            static_assert(t.parse("0") == t.pair(0, 1));
            static_assert(t.parse("01") == t.pair(0, 1));
            static_assert(t.parse("-") == t.pair(0, 0));
            static_assert(t.parse("-0") == t.pair(0, 2));
            static_assert(t.parse("abc") == t.pair(0, 0));
            static_assert(t.parse("6abc") == t.pair(6, 1));
            static_assert(t.parse("65abc") == t.pair(6, 1));
            static_assert(t.parse("65536") == t.pair(6, 1));
            static_assert(t.parse("999999999999999999999") == t.pair(9, 1));
            static_assert(t.parse("-6abc") == t.pair(-6, 2));
            static_assert(t.parse("-65abc") == t.pair(-6, 2));
            static_assert(t.parse("-65536") == t.pair(-6, 2));
            static_assert(t.parse("-999999999999999999999") == t.pair(-9, 2));
        }
        {
            constexpr app::integral_prefix_tester<i16, math::base::decimal, 2> t;
            static_assert(t.parse("") == t.pair(0, 0));
            static_assert(t.parse("0") == t.pair(0, 1));
            static_assert(t.parse("01") == t.pair(0, 1));
            static_assert(t.parse("-") == t.pair(0, 0));
            static_assert(t.parse("-0") == t.pair(0, 2));
            static_assert(t.parse("abc") == t.pair(0, 0));
            static_assert(t.parse("6abc") == t.pair(6, 1));
            static_assert(t.parse("65abc") == t.pair(65, 2));
            static_assert(t.parse("65536") == t.pair(65, 2));
            static_assert(t.parse("999999999999999999999") == t.pair(99, 2));
            static_assert(t.parse("-6abc") == t.pair(-6, 2));
            static_assert(t.parse("-65abc") == t.pair(-65, 3));
            static_assert(t.parse("-65536") == t.pair(-65, 3));
            static_assert(t.parse("-999999999999999999999") == t.pair(-99, 3));
        }
        {
            constexpr app::integral_prefix_tester<i16, math::base::hex, 2> t;
            static_assert(t.parse("") == t.pair(0, 0));
            static_assert(t.parse("0") == t.pair(0, 1));
            static_assert(t.parse("01") == t.pair(0, 1));
            static_assert(t.parse("-") == t.pair(0, 0));
            static_assert(t.parse("-0") == t.pair(0, 2));
            static_assert(t.parse("abc") == t.pair(171, 2));
            static_assert(t.parse("6abc") == t.pair(106, 2));
            static_assert(t.parse("65abc") == t.pair(101, 2));
            static_assert(t.parse("65536") == t.pair(101, 2));
            static_assert(t.parse("999999999999999999999") == t.pair(153, 2));
            static_assert(t.parse("-6abc") == t.pair(-106, 3));
            static_assert(t.parse("-65abc") == t.pair(-101, 3));
            static_assert(t.parse("-65536") == t.pair(-101, 3));
            static_assert(t.parse("-999999999999999999999") == t.pair(-153, 3));
        }
        {
            constexpr app::integral_prefix_tester<i16, math::base::decimal, 3> t;
            static_assert(t.parse("") == t.pair(0, 0));
            static_assert(t.parse("0") == t.pair(0, 1));
            static_assert(t.parse("01") == t.pair(0, 1));
            static_assert(t.parse("-") == t.pair(0, 0));
            static_assert(t.parse("-0") == t.pair(0, 2));
            static_assert(t.parse("abc") == t.pair(0, 0));
            static_assert(t.parse("6abc") == t.pair(6, 1));
            static_assert(t.parse("65abc") == t.pair(65, 2));
            static_assert(t.parse("65536") == t.pair(655, 3));
            static_assert(t.parse("999999999999999999999") == t.pair(999, 3));
            static_assert(t.parse("-6abc") == t.pair(-6, 2));
            static_assert(t.parse("-65abc") == t.pair(-65, 3));
            static_assert(t.parse("-65536") == t.pair(-655, 4));
            static_assert(t.parse("-999999999999999999999") == t.pair(-999, 4));
        }
        {
            constexpr app::integral_prefix_tester<i16, math::base::decimal, 4> t;
            static_assert(t.parse("") == t.pair(0, 0));
            static_assert(t.parse("0") == t.pair(0, 1));
            static_assert(t.parse("01") == t.pair(0, 1));
            static_assert(t.parse("-") == t.pair(0, 0));
            static_assert(t.parse("-0") == t.pair(0, 2));
            static_assert(t.parse("abc") == t.pair(0, 0));
            static_assert(t.parse("6abc") == t.pair(6, 1));
            static_assert(t.parse("65abc") == t.pair(65, 2));
            static_assert(t.parse("65536") == t.pair(6553, 4));
            static_assert(t.parse("999999999999999999999") == t.pair(9999, 4));
            static_assert(t.parse("-6abc") == t.pair(-6, 2));
            static_assert(t.parse("-65abc") == t.pair(-65, 3));
            static_assert(t.parse("-65536") == t.pair(-6553, 5));
            static_assert(t.parse("-999999999999999999999") == t.pair(-9999, 5));
        }
        // Will not compile (5 digits can overflow i16).
        // {
        //     constexpr app::integral_prefix_tester<i16, math::base::decimal, 5> t;
        //     static_assert(t.parse("-32768") == t.pair(-32768, 6));
        // }

        // Allow leading zeros.
        {
            constexpr cstrview s{"0922337203"};
            constexpr auto p =
                ascii::to_integral_prefix<u64>(s.range(), ascii::leading_zeros::allow);
            static_assert(p.value == 922337203);
            static_assert(p.count == 10);
        }
        {
            constexpr cstrview s{"00099"};
            constexpr auto p =
                ascii::to_integral_prefix<i64>(s.range(), ascii::leading_zeros::allow);
            static_assert(p.value == 99);
            static_assert(p.count == 5);
        }
        {
            constexpr cstrview s{"-00099"};
            constexpr auto p =
                ascii::to_integral_prefix<i64>(s.range(), ascii::leading_zeros::allow);
            static_assert(p.value == -99);
            static_assert(p.count == 6);
        }
        {
            constexpr cstrview s{"00000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000000000000000000083292365102"};
            constexpr auto p =
                ascii::to_integral_prefix<i64>(s.range(), ascii::leading_zeros::allow);
            static_assert(p.value == 83292365102);
            static_assert(p.count == 500);
        }

        // Forward range (not necessarily contiguous).
        {
            static constexpr array a{'-', '1', '2', '9'};
            constexpr range::forward rng{meta::iterators, a.begin(), a.end()};
            static_assert(ascii::to_integral_prefix<i16>(rng) ==
                          pair::value_count<i16, usize>{-129, 4});
        }
    }
}
