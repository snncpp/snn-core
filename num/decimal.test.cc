// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/num/decimal.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                num::decimal<2> val{"42.55"};
                snn_require(val.value() == 4255);
                num::decimal<2> mul{".19"};
                snn_require(mul.value() == 19);
                val *= mul;
                snn_require(val.value() == 808); // Rounded from 80845.
                snn_require(mul.value() == 19);
            }
            {
                num::decimal<4> val{"42.55"};
                snn_require(val.value() == 425500);
                num::decimal<2> mul{".19"};
                snn_require(mul.value() == 19);
                val *= mul;
                snn_require(val.value() == 80845); // No precision loss.
                snn_require(mul.value() == 19);
            }

            return true;
        }

        constexpr bool test_decimal()
        {
            // Static scale.
            static_assert(sizeof(num::decimal<4>) == 16);
            static_assert(sizeof(num::decimal<4, i16>) == 4);

            // Dynamic scale.
            static_assert(sizeof(num::decimal{0, 4}) == 24);
            static_assert(sizeof(num::decimal<num::dynamic_scale, i16>{0, 2}) == 6);

            // explicit operator bool(), has_value(), value()
            {
                constexpr num::decimal<4> a;     // Static scale.
                constexpr num::decimal b{99, 4}; // Dynamic scale.
                static_assert(!a);
                static_assert(b);
                static_assert(a.has_value());
                static_assert(b.has_value());
                static_assert(a.value() == 0);
                static_assert(b.value() == 99);
                static_assert(a.value(promise::has_value) == 0);
                static_assert(b.value(promise::has_value) == 99);
                static_assert(a.denominator() == 10000);
                static_assert(b.denominator() == 10000);
                static_assert(a.scale() == 4);
                static_assert(b.scale() == 4);
            }

            // Constructor, value()
            {
                static_assert(num::decimal<4>{0}.value() == 0);
                static_assert(num::decimal<4>{100}.value() == 100);
                static_assert(num::decimal<4>{-3892332}.value() == -3892332);
                static_assert(num::decimal<4>{-28}.value() == -28);
                static_assert(num::decimal<4>{38236561293282}.value() == 38236561293282);
            }
            {
                static_assert(num::decimal<4>{"-"}.value() == 0);  // Not worth protecting against.
                static_assert(num::decimal<4>{"-."}.value() == 0); // Not worth protecting against.
                static_assert(num::decimal<4>{"0"}.value() == 0);
                static_assert(num::decimal<4>{"-0"}.value() == 0);
                static_assert(num::decimal<4>{"1"}.value() == 10000);
                static_assert(num::decimal<4>{"-1"}.value() == -10000);
                static_assert(num::decimal<4>{"5."}.value() == 50000);
                static_assert(num::decimal<4>{"-5"}.value() == -50000);
                static_assert(num::decimal<4>{"-42."}.value() == -420000);
                static_assert(num::decimal<4>{"-42.0"}.value() == -420000);
                static_assert(num::decimal<4>{"42.50"}.value() == 425000);
                static_assert(num::decimal<4>{"42.5"}.value() == 425000);
                static_assert(num::decimal<4>{"-0.05"}.value() == -500);
                static_assert(num::decimal<4>{"-.05"}.value() == -500);
                static_assert(num::decimal<4>{"0.0583"}.value() == 583);
                static_assert(num::decimal<4>{"389238299.9876"}.value() == 3892382999876);

                static_assert(num::decimal<2>{"0"}.value() == 0);
                static_assert(num::decimal<2>{"-0"}.value() == 0);
                static_assert(num::decimal<2>{"1"}.value() == 100);
                static_assert(num::decimal<2>{"-1"}.value() == -100);
                static_assert(num::decimal<2>{"5."}.value() == 500);
                static_assert(num::decimal<2>{"-5"}.value() == -500);
                static_assert(num::decimal<2>{"-42."}.value() == -4200);
                static_assert(num::decimal<2>{"-42.0"}.value() == -4200);
                static_assert(num::decimal<2>{"42.50"}.value() == 4250);
                static_assert(num::decimal<2>{"42.5"}.value() == 4250);
                static_assert(num::decimal<2>{"-0.05"}.value() == -5);
                static_assert(num::decimal<2>{"0.99"}.value() == 99);
                static_assert(num::decimal<2>{"389238299.89"}.value() == 38923829989);
            }

            // denominator, scale
            {
                static_assert(num::decimal<0>{}.denominator() == 1);
                static_assert(num::decimal<1>{}.denominator() == 10);
                static_assert(num::decimal<2>{}.denominator() == 100);
                static_assert(num::decimal<3>{}.denominator() == 1000);
                static_assert(num::decimal<4>{}.denominator() == 10000);
                static_assert(num::decimal<5>{}.denominator() == 100000);
                static_assert(num::decimal<6>{}.denominator() == 1000000);

                static_assert(num::decimal<0>{}.scale() == 0);
                static_assert(num::decimal<1>{}.scale() == 1);
                static_assert(num::decimal<2>{}.scale() == 2);
                static_assert(num::decimal<3>{}.scale() == 3);
                static_assert(num::decimal<4>{}.scale() == 4);
                static_assert(num::decimal<5>{}.scale() == 5);
                static_assert(num::decimal<6>{}.scale() == 6);
            }

            // format()
            {
                constexpr num::decimal<0> m{123456789};
                static_assert(m.denominator() == 1);
                snn_require(m.format(0) == "123456789");
                snn_require(m.format(1) == "123456789.0");
                snn_require(m.format(2) == "123456789.00");
                snn_require(m.format(3) == "123456789.000");
                snn_require(m.format(4) == "123456789.0000");
                snn_require(m.format(5) == "123456789.00000");
                snn_require(m.format(6) == "123456789.000000");
                snn_require(m.format(18) == "123456789.000000000000000000");
            }
            {
                constexpr num::decimal<0> m{-123456789};
                static_assert(m.denominator() == 1);
                snn_require(m.format(0) == "-123456789");
                snn_require(m.format(1) == "-123456789.0");
                snn_require(m.format(2) == "-123456789.00");
                snn_require(m.format(3) == "-123456789.000");
                snn_require(m.format(4) == "-123456789.0000");
                snn_require(m.format(5) == "-123456789.00000");
                snn_require(m.format(6) == "-123456789.000000");
                snn_require(m.format(18) == "-123456789.000000000000000000");
            }
            {
                constexpr num::decimal<1> m{123456789};
                static_assert(m.denominator() == 10);
                snn_require(m.format(0) == "12345679");
                snn_require(m.format(1) == "12345678.9");
                snn_require(m.format(2) == "12345678.90");
                snn_require(m.format(3) == "12345678.900");
                snn_require(m.format(4) == "12345678.9000");
                snn_require(m.format(5) == "12345678.90000");
                snn_require(m.format(6) == "12345678.900000");
                snn_require(m.format(18) == "12345678.900000000000000000");
            }
            {
                constexpr num::decimal<1> m{-123456789};
                static_assert(m.denominator() == 10);
                snn_require(m.format(0) == "-12345679");
                snn_require(m.format(1) == "-12345678.9");
                snn_require(m.format(2) == "-12345678.90");
                snn_require(m.format(3) == "-12345678.900");
                snn_require(m.format(4) == "-12345678.9000");
                snn_require(m.format(5) == "-12345678.90000");
                snn_require(m.format(6) == "-12345678.900000");
                snn_require(m.format(18) == "-12345678.900000000000000000");
            }
            {
                constexpr num::decimal<2> m{123456789};
                static_assert(m.denominator() == 100);
                snn_require(m.format(0) == "1234568");
                snn_require(m.format(1) == "1234567.9");
                snn_require(m.format(2) == "1234567.89");
                snn_require(m.format(3) == "1234567.890");
                snn_require(m.format(4) == "1234567.8900");
                snn_require(m.format(5) == "1234567.89000");
                snn_require(m.format(6) == "1234567.890000");
                snn_require(m.format(18) == "1234567.890000000000000000");
            }
            {
                constexpr num::decimal<2> m{-123456789};
                static_assert(m.denominator() == 100);
                snn_require(m.format(0) == "-1234568");
                snn_require(m.format(1) == "-1234567.9");
                snn_require(m.format(2) == "-1234567.89");
                snn_require(m.format(3) == "-1234567.890");
                snn_require(m.format(4) == "-1234567.8900");
                snn_require(m.format(5) == "-1234567.89000");
                snn_require(m.format(6) == "-1234567.890000");
                snn_require(m.format(18) == "-1234567.890000000000000000");
            }
            {
                constexpr num::decimal<3> m{123456789};
                static_assert(m.denominator() == 1000);
                snn_require(m.format(0) == "123457");
                snn_require(m.format(1) == "123456.8");
                snn_require(m.format(2) == "123456.79");
                snn_require(m.format(3) == "123456.789");
                snn_require(m.format(4) == "123456.7890");
                snn_require(m.format(5) == "123456.78900");
                snn_require(m.format(6) == "123456.789000");
                snn_require(m.format(18) == "123456.789000000000000000");
            }
            {
                constexpr num::decimal<3> m{-123456789};
                static_assert(m.denominator() == 1000);
                snn_require(m.format(0) == "-123457");
                snn_require(m.format(1) == "-123456.8");
                snn_require(m.format(2) == "-123456.79");
                snn_require(m.format(3) == "-123456.789");
                snn_require(m.format(4) == "-123456.7890");
                snn_require(m.format(5) == "-123456.78900");
                snn_require(m.format(6) == "-123456.789000");
                snn_require(m.format(18) == "-123456.789000000000000000");
            }
            {
                constexpr num::decimal<4> m{123456789};
                static_assert(m.denominator() == 10'000);
                snn_require(m.format(0) == "12346");
                snn_require(m.format(1) == "12345.7");
                snn_require(m.format(2) == "12345.68");
                snn_require(m.format(3) == "12345.679");
                snn_require(m.format(4) == "12345.6789");
                snn_require(m.format(5) == "12345.67890");
                snn_require(m.format(6) == "12345.678900");
                snn_require(m.format(18) == "12345.678900000000000000");
            }
            {
                constexpr num::decimal<4> m{-123456789};
                static_assert(m.denominator() == 10'000);
                snn_require(m.format(0) == "-12346");
                snn_require(m.format(1) == "-12345.7");
                snn_require(m.format(2) == "-12345.68");
                snn_require(m.format(3) == "-12345.679");
                snn_require(m.format(4) == "-12345.6789");
                snn_require(m.format(5) == "-12345.67890");
                snn_require(m.format(6) == "-12345.678900");
                snn_require(m.format(18) == "-12345.678900000000000000");
            }
            {
                constexpr num::decimal<5> m{123456789};
                static_assert(m.denominator() == 100000);
                snn_require(m.format(0) == "1235");
                snn_require(m.format(1) == "1234.6");
                snn_require(m.format(2) == "1234.57");
                snn_require(m.format(3) == "1234.568");
                snn_require(m.format(4) == "1234.5679");
                snn_require(m.format(5) == "1234.56789");
                snn_require(m.format(6) == "1234.567890");
                snn_require(m.format(18) == "1234.567890000000000000");
            }
            {
                constexpr num::decimal<5> m{-123456789};
                static_assert(m.denominator() == 100000);
                snn_require(m.format(0) == "-1235");
                snn_require(m.format(1) == "-1234.6");
                snn_require(m.format(2) == "-1234.57");
                snn_require(m.format(3) == "-1234.568");
                snn_require(m.format(4) == "-1234.5679");
                snn_require(m.format(5) == "-1234.56789");
                snn_require(m.format(6) == "-1234.567890");
                snn_require(m.format(18) == "-1234.567890000000000000");
            }
            {
                constexpr num::decimal<6> m{123456789};
                static_assert(m.denominator() == 1000000);
                snn_require(m.format(0) == "123");
                snn_require(m.format(1) == "123.5");
                snn_require(m.format(2) == "123.46");
                snn_require(m.format(3) == "123.457");
                snn_require(m.format(4) == "123.4568");
                snn_require(m.format(5) == "123.45679");
                snn_require(m.format(6) == "123.456789");
                snn_require(m.format(18) == "123.456789000000000000");
            }
            {
                constexpr num::decimal<6> m{-123456789};
                static_assert(m.denominator() == 1000000);
                snn_require(m.format(0) == "-123");
                snn_require(m.format(1) == "-123.5");
                snn_require(m.format(2) == "-123.46");
                snn_require(m.format(3) == "-123.457");
                snn_require(m.format(4) == "-123.4568");
                snn_require(m.format(5) == "-123.45679");
                snn_require(m.format(6) == "-123.456789");
                snn_require(m.format(18) == "-123.456789000000000000");
            }
            {
                constexpr num::decimal<0> m{5};
                static_assert(m.denominator() == 1);
                snn_require(m.format(0) == "5");
                snn_require(m.format(1) == "5.0");
                snn_require(m.format(2) == "5.00");
                snn_require(m.format(3) == "5.000");
                snn_require(m.format(4) == "5.0000");
                snn_require(m.format(5) == "5.00000");
                snn_require(m.format(6) == "5.000000");
                snn_require(m.format(18) == "5.000000000000000000");
            }
            {
                constexpr num::decimal<0> m{-5};
                static_assert(m.denominator() == 1);
                snn_require(m.format(0) == "-5");
                snn_require(m.format(1) == "-5.0");
                snn_require(m.format(2) == "-5.00");
                snn_require(m.format(3) == "-5.000");
                snn_require(m.format(4) == "-5.0000");
                snn_require(m.format(5) == "-5.00000");
                snn_require(m.format(6) == "-5.000000");
                snn_require(m.format(18) == "-5.000000000000000000");
            }
            {
                constexpr num::decimal<1> m{5};
                static_assert(m.denominator() == 10);
                snn_require(m.format(0) == "1");
                snn_require(m.format(1) == "0.5");
                snn_require(m.format(2) == "0.50");
                snn_require(m.format(3) == "0.500");
                snn_require(m.format(4) == "0.5000");
                snn_require(m.format(5) == "0.50000");
                snn_require(m.format(6) == "0.500000");
                snn_require(m.format(18) == "0.500000000000000000");
            }
            {
                constexpr num::decimal<1> m{-5};
                static_assert(m.denominator() == 10);
                snn_require(m.format(0) == "-1");
                snn_require(m.format(1) == "-0.5");
                snn_require(m.format(2) == "-0.50");
                snn_require(m.format(3) == "-0.500");
                snn_require(m.format(4) == "-0.5000");
                snn_require(m.format(5) == "-0.50000");
                snn_require(m.format(6) == "-0.500000");
                snn_require(m.format(18) == "-0.500000000000000000");
            }
            {
                constexpr num::decimal<2> m{5};
                static_assert(m.denominator() == 100);
                snn_require(m.format(0) == "0");
                snn_require(m.format(1) == "0.1");
                snn_require(m.format(2) == "0.05");
                snn_require(m.format(3) == "0.050");
                snn_require(m.format(4) == "0.0500");
                snn_require(m.format(5) == "0.05000");
                snn_require(m.format(6) == "0.050000");
                snn_require(m.format(18) == "0.050000000000000000");
            }
            {
                constexpr num::decimal<2> m{-5};
                static_assert(m.denominator() == 100);
                snn_require(m.format(0) == "-0");
                snn_require(m.format(1) == "-0.1");
                snn_require(m.format(2) == "-0.05");
                snn_require(m.format(3) == "-0.050");
                snn_require(m.format(4) == "-0.0500");
                snn_require(m.format(5) == "-0.05000");
                snn_require(m.format(6) == "-0.050000");
                snn_require(m.format(18) == "-0.050000000000000000");
            }
            {
                constexpr num::decimal<3> m{5};
                static_assert(m.denominator() == 1000);
                snn_require(m.format(0) == "0");
                snn_require(m.format(1) == "0.0");
                snn_require(m.format(2) == "0.01");
                snn_require(m.format(3) == "0.005");
                snn_require(m.format(4) == "0.0050");
                snn_require(m.format(5) == "0.00500");
                snn_require(m.format(6) == "0.005000");
                snn_require(m.format(18) == "0.005000000000000000");
            }
            {
                constexpr num::decimal<3> m{-5};
                static_assert(m.denominator() == 1000);
                snn_require(m.format(0) == "-0");
                snn_require(m.format(1) == "-0.0");
                snn_require(m.format(2) == "-0.01");
                snn_require(m.format(3) == "-0.005");
                snn_require(m.format(4) == "-0.0050");
                snn_require(m.format(5) == "-0.00500");
                snn_require(m.format(6) == "-0.005000");
                snn_require(m.format(18) == "-0.005000000000000000");
            }
            {
                constexpr num::decimal<4> m{5};
                static_assert(m.denominator() == 10'000);
                snn_require(m.format(0) == "0");
                snn_require(m.format(1) == "0.0");
                snn_require(m.format(2) == "0.00");
                snn_require(m.format(3) == "0.001");
                snn_require(m.format(4) == "0.0005");
                snn_require(m.format(5) == "0.00050");
                snn_require(m.format(6) == "0.000500");
                snn_require(m.format(18) == "0.000500000000000000");
            }
            {
                constexpr num::decimal<4> m{-5};
                static_assert(m.denominator() == 10'000);
                snn_require(m.format(0) == "-0");
                snn_require(m.format(1) == "-0.0");
                snn_require(m.format(2) == "-0.00");
                snn_require(m.format(3) == "-0.001");
                snn_require(m.format(4) == "-0.0005");
                snn_require(m.format(5) == "-0.00050");
                snn_require(m.format(6) == "-0.000500");
                snn_require(m.format(18) == "-0.000500000000000000");
            }
            {
                constexpr num::decimal<5> m{5};
                static_assert(m.denominator() == 100000);
                snn_require(m.format(0) == "0");
                snn_require(m.format(1) == "0.0");
                snn_require(m.format(2) == "0.00");
                snn_require(m.format(3) == "0.000");
                snn_require(m.format(4) == "0.0001");
                snn_require(m.format(5) == "0.00005");
                snn_require(m.format(6) == "0.000050");
                snn_require(m.format(18) == "0.000050000000000000");
            }
            {
                constexpr num::decimal<5> m{-5};
                static_assert(m.denominator() == 100000);
                snn_require(m.format(0) == "-0");
                snn_require(m.format(1) == "-0.0");
                snn_require(m.format(2) == "-0.00");
                snn_require(m.format(3) == "-0.000");
                snn_require(m.format(4) == "-0.0001");
                snn_require(m.format(5) == "-0.00005");
                snn_require(m.format(6) == "-0.000050");
                snn_require(m.format(18) == "-0.000050000000000000");
            }
            {
                constexpr num::decimal<6> m{5};
                static_assert(m.denominator() == 1000000);
                snn_require(m.format(0) == "0");
                snn_require(m.format(1) == "0.0");
                snn_require(m.format(2) == "0.00");
                snn_require(m.format(3) == "0.000");
                snn_require(m.format(4) == "0.0000");
                snn_require(m.format(5) == "0.00001");
                snn_require(m.format(6) == "0.000005");
                snn_require(m.format(18) == "0.000005000000000000");
            }
            {
                constexpr num::decimal<6> m{-5};
                static_assert(m.denominator() == 1000000);
                snn_require(m.format(0) == "-0");
                snn_require(m.format(1) == "-0.0");
                snn_require(m.format(2) == "-0.00");
                snn_require(m.format(3) == "-0.000");
                snn_require(m.format(4) == "-0.0000");
                snn_require(m.format(5) == "-0.00001");
                snn_require(m.format(6) == "-0.000005");
                snn_require(m.format(18) == "-0.000005000000000000");
            }
            {
                // 9223372036854775807
                constexpr num::decimal<4> m{constant::limit<i64>::max};
                static_assert(m.value() == constant::limit<i64>::max);
                snn_require(m.format(0) == "922337203685478");
                snn_require(m.format(1) == "922337203685477.6");
                snn_require(m.format(2) == "922337203685477.58");
                snn_require(m.format(3) == "922337203685477.581");
                snn_require(m.format(4) == "922337203685477.5807");
                snn_require(m.format(5) == "922337203685477.58070");
                snn_require(m.format(6) == "922337203685477.580700");
                snn_require(m.format(18) == "922337203685477.580700000000000000");
            }
            {
                // -9223372036854775808
                constexpr num::decimal<4> m{constant::limit<i64>::min};
                static_assert(m.value() == constant::limit<i64>::min);
                snn_require(m.format(0) == "-922337203685478");
                snn_require(m.format(1) == "-922337203685477.6");
                snn_require(m.format(2) == "-922337203685477.58");
                snn_require(m.format(3) == "-922337203685477.581");
                snn_require(m.format(4) == "-922337203685477.5808");
                snn_require(m.format(5) == "-922337203685477.58080");
                snn_require(m.format(6) == "-922337203685477.580800");
                snn_require(m.format(18) == "-922337203685477.580800000000000000");
            }
            {
                // 9223372036854775807
                constexpr num::decimal<4> m{constant::limit<i64>::max};
                static_assert(m.value() == constant::limit<i64>::max);
                snn_require(m.format(0, ',', '\'') == "922'337'203'685'478");
                snn_require(m.format(1, ',', '\'') == "922'337'203'685'477,6");
                snn_require(m.format(2, ',', '\'') == "922'337'203'685'477,58");
                snn_require(m.format(3, ',', '\'') == "922'337'203'685'477,581");
                snn_require(m.format(4, ',', '\'') == "922'337'203'685'477,5807");
                snn_require(m.format(5, ',', '\'') == "922'337'203'685'477,58070");
                snn_require(m.format(6, ',', '\'') == "922'337'203'685'477,580700");
                snn_require(m.format(18, ',', '\'') == "922'337'203'685'477,580700000000000000");
            }
            {
                // -9223372036854775808
                constexpr num::decimal<4> m{constant::limit<i64>::min};
                static_assert(m.value() == constant::limit<i64>::min);
                snn_require(m.format(0, ',', '\'') == "-922'337'203'685'478");
                snn_require(m.format(1, ',', '\'') == "-922'337'203'685'477,6");
                snn_require(m.format(2, ',', '\'') == "-922'337'203'685'477,58");
                snn_require(m.format(3, ',', '\'') == "-922'337'203'685'477,581");
                snn_require(m.format(4, ',', '\'') == "-922'337'203'685'477,5808");
                snn_require(m.format(5, ',', '\'') == "-922'337'203'685'477,58080");
                snn_require(m.format(6, ',', '\'') == "-922'337'203'685'477,580800");
                snn_require(m.format(18, ',', '\'') == "-922'337'203'685'477,580800000000000000");
            }

            // round()
            {
                constexpr i64 max = constant::limit<i64>::max;
                snn_require_throws_code(num::decimal<4>{max}.round().value(),
                                        num::error::overflow_or_divide_by_zero);
                snn_require_throws_code(num::decimal<4>{max}.round(0).value(),
                                        num::error::overflow_or_divide_by_zero);
                snn_require_throws_code(num::decimal<4>{max}.round(1).value(),
                                        num::error::overflow_or_divide_by_zero);
                snn_require_throws_code(num::decimal<4>{max}.round(2).value(),
                                        num::error::overflow_or_divide_by_zero);
                snn_require_throws_code(num::decimal<4>{max}.round(3).value(),
                                        num::error::overflow_or_divide_by_zero);
                static_assert(num::decimal<4>{max}.round(4).value() == max);
                static_assert(num::decimal<4>{max}.round(5).value() == max);
            }
            {
                constexpr i64 min = constant::limit<i64>::min;
                snn_require_throws_code(num::decimal<4>{min}.round().value(),
                                        num::error::overflow_or_divide_by_zero);
                snn_require_throws_code(num::decimal<4>{min}.round(0).value(),
                                        num::error::overflow_or_divide_by_zero);
                snn_require_throws_code(num::decimal<4>{min}.round(1).value(),
                                        num::error::overflow_or_divide_by_zero);
                snn_require_throws_code(num::decimal<4>{min}.round(2).value(),
                                        num::error::overflow_or_divide_by_zero);
                snn_require_throws_code(num::decimal<4>{min}.round(3).value(),
                                        num::error::overflow_or_divide_by_zero);
                static_assert(num::decimal<4>{min}.round(4).value() == min);
                static_assert(num::decimal<4>{min}.round(5).value() == min);
            }
            {
                static_assert(num::decimal<0>{425555}.round().value() == 425555);
                static_assert(num::decimal<0>{425555}.round(0).value() == 425555);
                static_assert(num::decimal<0>{425555}.round(1).value() == 425555);
                static_assert(num::decimal<0>{425555}.round(2).value() == 425555);
                static_assert(num::decimal<0>{425555}.round(3).value() == 425555);
                static_assert(num::decimal<0>{425555}.round(4).value() == 425555);
                static_assert(num::decimal<0>{425555}.round(5).value() == 425555);
            }
            {
                static_assert(num::decimal<1>{425555}.round().value() == 425560);
                static_assert(num::decimal<1>{425555}.round(0).value() == 425560);
                static_assert(num::decimal<1>{425555}.round(1).value() == 425555);
                static_assert(num::decimal<1>{425555}.round(2).value() == 425555);
                static_assert(num::decimal<1>{425555}.round(3).value() == 425555);
                static_assert(num::decimal<1>{425555}.round(4).value() == 425555);
                static_assert(num::decimal<1>{425555}.round(5).value() == 425555);
            }
            {
                static_assert(num::decimal<2>{425555}.round().value() == 425600);
                static_assert(num::decimal<2>{425555}.round(0).value() == 425600);
                static_assert(num::decimal<2>{425555}.round(1).value() == 425560);
                static_assert(num::decimal<2>{425555}.round(2).value() == 425555);
                static_assert(num::decimal<2>{425555}.round(3).value() == 425555);
                static_assert(num::decimal<2>{425555}.round(4).value() == 425555);
                static_assert(num::decimal<2>{425555}.round(5).value() == 425555);
            }
            {
                static_assert(num::decimal<3>{425555}.round().value() == 426000);
                static_assert(num::decimal<3>{425555}.round(0).value() == 426000);
                static_assert(num::decimal<3>{425555}.round(1).value() == 425600);
                static_assert(num::decimal<3>{425555}.round(2).value() == 425560);
                static_assert(num::decimal<3>{425555}.round(3).value() == 425555);
                static_assert(num::decimal<3>{425555}.round(4).value() == 425555);
                static_assert(num::decimal<3>{425555}.round(5).value() == 425555);
            }
            {
                static_assert(num::decimal<4>{425555}.round().value() == 430000);
                static_assert(num::decimal<4>{425555}.round(0).value() == 430000);
                static_assert(num::decimal<4>{425555}.round(1).value() == 426000);
                static_assert(num::decimal<4>{425555}.round(2).value() == 425600);
                static_assert(num::decimal<4>{425555}.round(3).value() == 425560);
                static_assert(num::decimal<4>{425555}.round(4).value() == 425555);
                static_assert(num::decimal<4>{425555}.round(5).value() == 425555);
            }
            {
                static_assert(num::decimal<5>{425555}.round().value() == 400000);
                static_assert(num::decimal<5>{425555}.round(0).value() == 400000);
                static_assert(num::decimal<5>{425555}.round(1).value() == 430000);
                static_assert(num::decimal<5>{425555}.round(2).value() == 426000);
                static_assert(num::decimal<5>{425555}.round(3).value() == 425600);
                static_assert(num::decimal<5>{425555}.round(4).value() == 425560);
                static_assert(num::decimal<5>{425555}.round(5).value() == 425555);
            }
            {
                static_assert(num::decimal<6>{425555}.round().value() == 0);
                static_assert(num::decimal<6>{425555}.round(0).value() == 0);
                static_assert(num::decimal<6>{425555}.round(1).value() == 400000);
                static_assert(num::decimal<6>{425555}.round(2).value() == 430000);
                static_assert(num::decimal<6>{425555}.round(3).value() == 426000);
                static_assert(num::decimal<6>{425555}.round(4).value() == 425600);
                static_assert(num::decimal<6>{425555}.round(5).value() == 425560);
                static_assert(num::decimal<6>{425555}.round(6).value() == 425555);
            }
            {
                static_assert(num::decimal<0>{-425555}.round().value() == -425555);
                static_assert(num::decimal<0>{-425555}.round(0).value() == -425555);
                static_assert(num::decimal<0>{-425555}.round(1).value() == -425555);
                static_assert(num::decimal<0>{-425555}.round(2).value() == -425555);
                static_assert(num::decimal<0>{-425555}.round(3).value() == -425555);
                static_assert(num::decimal<0>{-425555}.round(4).value() == -425555);
                static_assert(num::decimal<0>{-425555}.round(5).value() == -425555);
            }
            {
                static_assert(num::decimal<1>{-425555}.round().value() == -425560);
                static_assert(num::decimal<1>{-425555}.round(0).value() == -425560);
                static_assert(num::decimal<1>{-425555}.round(1).value() == -425555);
                static_assert(num::decimal<1>{-425555}.round(2).value() == -425555);
                static_assert(num::decimal<1>{-425555}.round(3).value() == -425555);
                static_assert(num::decimal<1>{-425555}.round(4).value() == -425555);
                static_assert(num::decimal<1>{-425555}.round(5).value() == -425555);
            }
            {
                static_assert(num::decimal<2>{-425555}.round().value() == -425600);
                static_assert(num::decimal<2>{-425555}.round(0).value() == -425600);
                static_assert(num::decimal<2>{-425555}.round(1).value() == -425560);
                static_assert(num::decimal<2>{-425555}.round(2).value() == -425555);
                static_assert(num::decimal<2>{-425555}.round(3).value() == -425555);
                static_assert(num::decimal<2>{-425555}.round(4).value() == -425555);
                static_assert(num::decimal<2>{-425555}.round(5).value() == -425555);
            }
            {
                static_assert(num::decimal<3>{-425555}.round().value() == -426000);
                static_assert(num::decimal<3>{-425555}.round(0).value() == -426000);
                static_assert(num::decimal<3>{-425555}.round(1).value() == -425600);
                static_assert(num::decimal<3>{-425555}.round(2).value() == -425560);
                static_assert(num::decimal<3>{-425555}.round(3).value() == -425555);
                static_assert(num::decimal<3>{-425555}.round(4).value() == -425555);
                static_assert(num::decimal<3>{-425555}.round(5).value() == -425555);
            }
            {
                static_assert(num::decimal<4>{-425555}.round().value() == -430000);
                static_assert(num::decimal<4>{-425555}.round(0).value() == -430000);
                static_assert(num::decimal<4>{-425555}.round(1).value() == -426000);
                static_assert(num::decimal<4>{-425555}.round(2).value() == -425600);
                static_assert(num::decimal<4>{-425555}.round(3).value() == -425560);
                static_assert(num::decimal<4>{-425555}.round(4).value() == -425555);
                static_assert(num::decimal<4>{-425555}.round(5).value() == -425555);
            }
            {
                static_assert(num::decimal<5>{-425555}.round().value() == -400000);
                static_assert(num::decimal<5>{-425555}.round(0).value() == -400000);
                static_assert(num::decimal<5>{-425555}.round(1).value() == -430000);
                static_assert(num::decimal<5>{-425555}.round(2).value() == -426000);
                static_assert(num::decimal<5>{-425555}.round(3).value() == -425600);
                static_assert(num::decimal<5>{-425555}.round(4).value() == -425560);
                static_assert(num::decimal<5>{-425555}.round(5).value() == -425555);
            }
            {
                static_assert(num::decimal<6>{-425555}.round().value() == 0);
                static_assert(num::decimal<6>{-425555}.round(0).value() == 0);
                static_assert(num::decimal<6>{-425555}.round(1).value() == -400000);
                static_assert(num::decimal<6>{-425555}.round(2).value() == -430000);
                static_assert(num::decimal<6>{-425555}.round(3).value() == -426000);
                static_assert(num::decimal<6>{-425555}.round(4).value() == -425600);
                static_assert(num::decimal<6>{-425555}.round(5).value() == -425560);
                static_assert(num::decimal<6>{-425555}.round(6).value() == -425555);
            }
            {
                static_assert(num::decimal<0>{5}.round().value() == 5);
                static_assert(num::decimal<0>{5}.round(0).value() == 5);
                static_assert(num::decimal<0>{5}.round(1).value() == 5);
                static_assert(num::decimal<0>{5}.round(2).value() == 5);
                static_assert(num::decimal<0>{5}.round(3).value() == 5);
                static_assert(num::decimal<0>{5}.round(4).value() == 5);
                static_assert(num::decimal<0>{5}.round(5).value() == 5);
            }
            {
                static_assert(num::decimal<1>{5}.round().value() == 10);
                static_assert(num::decimal<1>{5}.round(0).value() == 10);
                static_assert(num::decimal<1>{5}.round(1).value() == 5);
                static_assert(num::decimal<1>{5}.round(2).value() == 5);
                static_assert(num::decimal<1>{5}.round(3).value() == 5);
                static_assert(num::decimal<1>{5}.round(4).value() == 5);
                static_assert(num::decimal<1>{5}.round(5).value() == 5);
            }
            {
                static_assert(num::decimal<2>{5}.round().value() == 0);
                static_assert(num::decimal<2>{5}.round(0).value() == 0);
                static_assert(num::decimal<2>{5}.round(1).value() == 10);
                static_assert(num::decimal<2>{5}.round(2).value() == 5);
                static_assert(num::decimal<2>{5}.round(3).value() == 5);
                static_assert(num::decimal<2>{5}.round(4).value() == 5);
                static_assert(num::decimal<2>{5}.round(5).value() == 5);
            }
            {
                static_assert(num::decimal<3>{5}.round().value() == 0);
                static_assert(num::decimal<3>{5}.round(0).value() == 0);
                static_assert(num::decimal<3>{5}.round(1).value() == 0);
                static_assert(num::decimal<3>{5}.round(2).value() == 10);
                static_assert(num::decimal<3>{5}.round(3).value() == 5);
                static_assert(num::decimal<3>{5}.round(4).value() == 5);
                static_assert(num::decimal<3>{5}.round(5).value() == 5);
            }
            {
                static_assert(num::decimal<4>{5}.round().value() == 0);
                static_assert(num::decimal<4>{5}.round(0).value() == 0);
                static_assert(num::decimal<4>{5}.round(1).value() == 0);
                static_assert(num::decimal<4>{5}.round(2).value() == 0);
                static_assert(num::decimal<4>{5}.round(3).value() == 10);
                static_assert(num::decimal<4>{5}.round(4).value() == 5);
                static_assert(num::decimal<4>{5}.round(5).value() == 5);
            }
            {
                static_assert(num::decimal<5>{5}.round().value() == 0);
                static_assert(num::decimal<5>{5}.round(0).value() == 0);
                static_assert(num::decimal<5>{5}.round(1).value() == 0);
                static_assert(num::decimal<5>{5}.round(2).value() == 0);
                static_assert(num::decimal<5>{5}.round(3).value() == 0);
                static_assert(num::decimal<5>{5}.round(4).value() == 10);
                static_assert(num::decimal<5>{5}.round(5).value() == 5);
            }
            {
                static_assert(num::decimal<6>{5}.round().value() == 0);
                static_assert(num::decimal<6>{5}.round(0).value() == 0);
                static_assert(num::decimal<6>{5}.round(1).value() == 0);
                static_assert(num::decimal<6>{5}.round(2).value() == 0);
                static_assert(num::decimal<6>{5}.round(3).value() == 0);
                static_assert(num::decimal<6>{5}.round(4).value() == 0);
                static_assert(num::decimal<6>{5}.round(5).value() == 10);
                static_assert(num::decimal<6>{5}.round(6).value() == 5);
            }
            {
                static_assert(num::decimal<0>{-5}.round().value() == -5);
                static_assert(num::decimal<0>{-5}.round(0).value() == -5);
                static_assert(num::decimal<0>{-5}.round(1).value() == -5);
                static_assert(num::decimal<0>{-5}.round(2).value() == -5);
                static_assert(num::decimal<0>{-5}.round(3).value() == -5);
                static_assert(num::decimal<0>{-5}.round(4).value() == -5);
                static_assert(num::decimal<0>{-5}.round(5).value() == -5);
            }
            {
                static_assert(num::decimal<1>{-5}.round().value() == -10);
                static_assert(num::decimal<1>{-5}.round(0).value() == -10);
                static_assert(num::decimal<1>{-5}.round(1).value() == -5);
                static_assert(num::decimal<1>{-5}.round(2).value() == -5);
                static_assert(num::decimal<1>{-5}.round(3).value() == -5);
                static_assert(num::decimal<1>{-5}.round(4).value() == -5);
                static_assert(num::decimal<1>{-5}.round(5).value() == -5);
            }
            {
                static_assert(num::decimal<2>{-5}.round().value() == 0);
                static_assert(num::decimal<2>{-5}.round(0).value() == 0);
                static_assert(num::decimal<2>{-5}.round(1).value() == -10);
                static_assert(num::decimal<2>{-5}.round(2).value() == -5);
                static_assert(num::decimal<2>{-5}.round(3).value() == -5);
                static_assert(num::decimal<2>{-5}.round(4).value() == -5);
                static_assert(num::decimal<2>{-5}.round(5).value() == -5);
            }
            {
                static_assert(num::decimal<3>{-5}.round().value() == 0);
                static_assert(num::decimal<3>{-5}.round(0).value() == 0);
                static_assert(num::decimal<3>{-5}.round(1).value() == 0);
                static_assert(num::decimal<3>{-5}.round(2).value() == -10);
                static_assert(num::decimal<3>{-5}.round(3).value() == -5);
                static_assert(num::decimal<3>{-5}.round(4).value() == -5);
                static_assert(num::decimal<3>{-5}.round(5).value() == -5);
            }
            {
                static_assert(num::decimal<4>{-5}.round().value() == 0);
                static_assert(num::decimal<4>{-5}.round(0).value() == 0);
                static_assert(num::decimal<4>{-5}.round(1).value() == 0);
                static_assert(num::decimal<4>{-5}.round(2).value() == 0);
                static_assert(num::decimal<4>{-5}.round(3).value() == -10);
                static_assert(num::decimal<4>{-5}.round(4).value() == -5);
                static_assert(num::decimal<4>{-5}.round(5).value() == -5);
            }
            {
                static_assert(num::decimal<5>{-5}.round().value() == 0);
                static_assert(num::decimal<5>{-5}.round(0).value() == 0);
                static_assert(num::decimal<5>{-5}.round(1).value() == 0);
                static_assert(num::decimal<5>{-5}.round(2).value() == 0);
                static_assert(num::decimal<5>{-5}.round(3).value() == 0);
                static_assert(num::decimal<5>{-5}.round(4).value() == -10);
                static_assert(num::decimal<5>{-5}.round(5).value() == -5);
            }
            {
                static_assert(num::decimal<6>{-5}.round().value() == 0);
                static_assert(num::decimal<6>{-5}.round(0).value() == 0);
                static_assert(num::decimal<6>{-5}.round(1).value() == 0);
                static_assert(num::decimal<6>{-5}.round(2).value() == 0);
                static_assert(num::decimal<6>{-5}.round(3).value() == 0);
                static_assert(num::decimal<6>{-5}.round(4).value() == 0);
                static_assert(num::decimal<6>{-5}.round(5).value() == -10);
                static_assert(num::decimal<6>{-5}.round(6).value() == -5);
            }

#if SNN_INT128_BOOL
            {
                constexpr num::decimal<18, i128> m{"982638329232.796222182832823234"};
                snn_require(m.format(2, '.', ' ') == "982 638 329 232.80");
                snn_require(m.format(4, '.', ' ') == "982 638 329 232.7962");
                snn_require(m.format(18, '.', ' ') == "982 638 329 232.796222182832823234");

                constexpr num::decimal<4> multiplier{"5.5729"};
                auto prod = m * multiplier;
                static_assert(std::is_same_v<decltype(prod), num::decimal<18, i128>>);
                snn_require(prod.format(18, '.', ' ') == "5 476 145 144 981.450066602709040601");

                prod *= num::decimal<0>{-1};
                snn_require(prod.format(18, '.', ' ') == "-5 476 145 144 981.450066602709040601");
            }
#endif

            // Addition
            // decimal& operator+=(const decimal other)
            {
                num::decimal<4> i{78};
                snn_require(i.value() == 78);
                num::decimal<4> j{10'000};
                snn_require(j.value() == 10'000);
                i += j;
                snn_require(i.value() == 10'078);
                snn_require(j.value() == 10'000);
                snn_require((i += j).value() == 20'078);
                snn_require(j.value() == 10'000);
                num::decimal<4> k{constant::limit<i64>::max};
                i += k;
                snn_require(!i.has_value());
                snn_require_throws_code(i.value(), num::error::overflow_or_divide_by_zero);
            }
            {
                num::decimal<2> i{"900.25"};
                snn_require(i.value() == 90025);
                i += "1.50";
                snn_require(i.value() == 90175);
            }
            // decimal operator+(const decimal other) const
            {
                num::decimal<4> i{5};
                num::decimal<4> j{100};
                const num::decimal<4> k = i + j;
                snn_require(i.value() == 5);
                snn_require(j.value() == 100);
                snn_require(k.value() == 105);
                num::decimal<4> l{constant::limit<i64>::max};
                num::decimal<4> m = i + l;
                snn_require(!m.has_value());
                snn_require_throws_code(m.value(), num::error::overflow_or_divide_by_zero);
            }
            {
                num::decimal<2> i{"900.25"};
                auto j = i + "1.50";
                static_assert(std::is_same_v<decltype(j), num::decimal<2>>);
                snn_require(i.value() == 90025);
                snn_require(j.value() == 90175);
            }

            // Subtraction
            // decimal& operator-=(const decimal other)
            {
                num::decimal<4> i{-78};
                snn_require(i.value() == -78);
                num::decimal<4> j{10'000};
                snn_require(j.value() == 10'000);
                i -= j;
                snn_require(i.value() == -10'078);
                snn_require(j.value() == 10'000);
                snn_require((i -= j).value() == -20'078);
                snn_require(j.value() == 10'000);
                num::decimal<4> k{constant::limit<i64>::max};
                i -= k;
                snn_require(!i.has_value());
                snn_require_throws_code(i.value(), num::error::overflow_or_divide_by_zero);
            }
            {
                num::decimal<2> i{"900.25"};
                snn_require(i.value() == 90025);
                i -= "1.50";
                snn_require(i.value() == 89875);
            }
            // decimal operator-(const decimal other) const
            {
                num::decimal<4> i{-5};
                num::decimal<4> j{100};
                const num::decimal<4> k = i - j;
                snn_require(i.value() == -5);
                snn_require(j.value() == 100);
                snn_require(k.value() == -105);
                num::decimal<4> l{constant::limit<i64>::max};
                num::decimal<4> m = i - l;
                snn_require(!m.has_value());
                snn_require_throws_code(m.value(), num::error::overflow_or_divide_by_zero);
            }
            {
                num::decimal<2> i{"900.25"};
                auto j = i - "1.50";
                static_assert(std::is_same_v<decltype(j), num::decimal<2>>);
                snn_require(i.value() == 90025);
                snn_require(j.value() == 89875);
            }

            // Multiplication
            // decimal& operator*=(const decimal<S> other)
            {
                num::decimal<4> m{"42.56"};
                snn_require(m.value() == 425600);
                num::decimal<2> vat{".19"};
                snn_require(vat.value() == 19);
                m *= vat;
                snn_require(m.value() == 80864);
                snn_require(vat.value() == 19);
            }
            {
                num::decimal<2> m{"42.56"};
                snn_require(m.value() == 4256);
                num::decimal<2> vat{".19"};
                snn_require(vat.value() == 19);
                m *= vat;
                snn_require(m.value() == 809); // Rounded from 80864.
                snn_require(vat.value() == 19);
            }
            {
                num::decimal<2> m{"42.56"};
                snn_require(m.value() == 4256);
                num::decimal<2> vat{"-.19"};
                snn_require(vat.value() == -19);
                m *= vat;
                snn_require(m.value() == -809); // Rounded from -80864.
                snn_require(vat.value() == -19);
            }
            {
                num::decimal<2> m{"42.55"};
                snn_require(m.value() == 4255);
                num::decimal<2> vat{".19"};
                snn_require(vat.value() == 19);
                m *= vat;
                snn_require(m.value() == 808); // Rounded from 80845.
                snn_require(vat.value() == 19);
            }
            {
                num::decimal<2> m{"42.55"};
                snn_require(m.value() == 4255);
                num::decimal<2> vat{"-.19"};
                snn_require(vat.value() == -19);
                m *= vat;
                snn_require(m.value() == -808); // Rounded from -80845.
                snn_require(vat.value() == -19);
            }
            {
                num::decimal<2> i{"900.25"};
                snn_require(i.value() == 90025);
                i *= "1.50";
                snn_require(i.value() == 135038); // Rounded from 13503750.
            }
            // decimal operator*(const decimal other) const
            {
                num::decimal<2> m{"42.56"};
                snn_require(m.value() == 4256);
                num::decimal<2> vat{"-.19"};
                snn_require(vat.value() == -19);
                const auto r = m * vat;
                snn_require(r.value() == -809); // Rounded from -80864.
                snn_require(m.value() == 4256);
                snn_require(vat.value() == -19);
            }
            {
                num::decimal<2> m{"42.55"};
                snn_require(m.value() == 4255);
                num::decimal<2> vat{".19"};
                snn_require(vat.value() == 19);
                const auto r = m * vat;
                snn_require(r.value() == 808); // Rounded from 80845.
                snn_require(m.value() == 4255);
                snn_require(vat.value() == 19);
            }
            {
                num::decimal<2> i{"900.25"};
                auto j = i * "1.50";
                static_assert(std::is_same_v<decltype(j), num::decimal<2>>);
                snn_require(i.value() == 90025);
                snn_require(j.value() == 135038); // Rounded from 13503750.
            }

            // Comparison
            // constexpr bool operator==(const decimal other) const noexcept
            {
                constexpr num::decimal<4> a;
                static_assert(a.value() == 0);
                constexpr num::decimal<4> b;
                constexpr num::decimal<4> c{1};
                static_assert(a == b);
                static_assert(!(b == c));
            }
            // constexpr bool operator!=(const decimal other) const noexcept
            {
                constexpr num::decimal<4> a;
                constexpr num::decimal<4> b;
                constexpr num::decimal<4> c{1};
                static_assert(!(a != b));
                static_assert(b != c);
            }
            // constexpr bool operator>(const decimal other) const noexcept
            {
                constexpr num::decimal<4> a;
                constexpr num::decimal<4> b;
                constexpr num::decimal<4> c{1};
                static_assert(!(b > a));
                static_assert(c > b);
            }
            // constexpr bool operator>=(const decimal other) const noexcept
            {
                constexpr num::decimal<4> a;
                constexpr num::decimal<4> b;
                constexpr num::decimal<4> c{1};
                static_assert(b >= a);
                static_assert(c >= b);
                static_assert(!(b >= c));
            }
            // constexpr bool operator<(const decimal other) const noexcept
            {
                constexpr num::decimal<4> a;
                constexpr num::decimal<4> b;
                constexpr num::decimal<4> c{1};
                static_assert(!(a < b));
                static_assert(b < c);
            }
            // constexpr bool operator<=(const decimal other) const noexcept
            {
                constexpr num::decimal<4> a;
                constexpr num::decimal<4> b;
                constexpr num::decimal<4> c{1};
                static_assert(a <= b);
                static_assert(b <= c);
                static_assert(!(c <= b));
            }

            return true;
        }

        inline bool test_decimal_imprecise()
        {
            {
                snn_require(num::decimal<4>{num::imprecise{0.05}}.value() == 500);
                snn_require(num::decimal<4>{num::imprecise{-0.05}}.value() == -500);
                snn_require(num::decimal<4>{num::imprecise{0.49999999999999994}}.value() == 5000);
                snn_require(num::decimal<0>{num::imprecise{0.49999999999999994}}.value() == 0);
                snn_require(num::decimal<4>{num::imprecise{0.0583}}.value() == 583);
                snn_require(num::decimal<4>{num::imprecise{389238299.98764}}.value() ==
                            3892382999876);
                snn_require(num::decimal<4>{num::imprecise{-389238299.98764}}.value() ==
                            -3892382999876);
                snn_require(num::decimal<4>{num::imprecise{389238299.98765}}.value() ==
                            3892382999877);
                snn_require(num::decimal<4>{num::imprecise{-389238299.98765}}.value() ==
                            -3892382999877);
                snn_require(num::decimal<2>{num::imprecise{0.0}}.value() == 0);
                snn_require(num::decimal<2>{num::imprecise{-0.0}}.value() == 0);
                snn_require(num::decimal<2>{num::imprecise{1.0}}.value() == 100);
                snn_require(num::decimal<2>{num::imprecise{-1.0}}.value() == -100);
                snn_require(num::decimal<2>{num::imprecise{389238299.98498}}.value() ==
                            38923829998);
                snn_require(num::decimal<2>{num::imprecise{-389238299.98498}}.value() ==
                            -38923829998);
                snn_require(num::decimal<2>{num::imprecise{389238299.98598}}.value() ==
                            38923829999);
                snn_require(num::decimal<2>{num::imprecise{-389238299.98598}}.value() ==
                            -38923829999);

                constexpr double max = static_cast<double>(constant::limit<i64>::max);
                snn_require_throws_code(num::decimal<4>{num::imprecise{max}},
                                        num::error::precision_loss);
                snn_require_throws_code(num::decimal<0>{num::imprecise{max}},
                                        num::error::precision_loss);
                constexpr double min = static_cast<double>(constant::limit<i64>::min);
                snn_require_throws_code(num::decimal<4>{num::imprecise{min}},
                                        num::error::precision_loss);
                snn_require_throws_code(num::decimal<0>{num::imprecise{min}},
                                        num::error::precision_loss);
            }

            // to<double>()
            {
                constexpr num::decimal<4> m{999'999'999'999'999};
                constexpr double d = m.to<double>().value();
                static_assert(d > 99'999'999'999.9998 && d < 100'000'000'000.0);
                num::decimal<4> n{num::imprecise{d}};
                snn_require(n.value() == 999'999'999'999'999);
            }
            {
                constexpr num::decimal<4> m{-999'999'999'999'999};
                constexpr double d = m.to<double>().value();
                static_assert(d < -99'999'999'999.9998 && d > -100'000'000'000.0);
                num::decimal<4> n{num::imprecise{d}};
                snn_require(n.value() == -999'999'999'999'999);
            }
            {
                constexpr i64 max = constant::limit<i64>::max;
                static_assert(!num::decimal<4>{max}.to<double>().has_value());
                constexpr i64 min = constant::limit<i64>::min;
                static_assert(!num::decimal<4>{min}.to<double>().has_value());
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
        snn_static_require(app::test_decimal());
        snn_require(app::test_decimal_imprecise());
    }
}
