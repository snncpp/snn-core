// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/num/safe.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            num::safe<i16> i{30'000};
            snn_require(i);
            snn_require(i.has_value());
            snn_require(i.value() == 30'000);
            snn_require(i.value_or(99) == 30'000);

            num::safe<i16> j = i + 2'000;
            snn_require(j);
            snn_require(j.has_value());
            snn_require(j.value() == 32'000);
            snn_require(j.value_or(99) == 32'000);

            num::safe<i16> k = i + 3'000; // Overflows
            snn_require(!k);
            snn_require(!k.has_value());
            snn_require_throws_code(k.value(), num::error::overflow_or_divide_by_zero);
            snn_require(k.value_or(99) == 99);

            return true;
        }

        template <typename Int>
        constexpr num::safe<Int> make_safe_overflow() noexcept
        {
            constexpr auto i = num::safe<Int>{1} + constant::limit<Int>::max;
            static_assert(!i.has_value());
            return i;
        }

        constexpr bool test_safe()
        {
            // explicit operator bool(), has_value(), value(), value_or()
            {
                num::safe<int> a;
                num::safe<int> b = 99;
                snn_require(!a);
                snn_require(b);
                snn_require(a.has_value());
                snn_require(b.has_value());
                snn_require(a.value() == 0);
                snn_require(b.value() == 99);
                snn_require(a.value_or(123) == 0);
                snn_require(b.value_or(123) == 99);
                snn_require(a.value(promise::has_value) == 0);
                snn_require(b.value(promise::has_value) == 99);
            }

            // value()
            {
                auto i = app::make_safe_overflow<int>();
                snn_require(!i);
                snn_require(!i.has_value());
                snn_require_throws_code(i.value(), num::error::overflow_or_divide_by_zero);
            }

            // Copy assignment, value_or()
            {
                num::safe<int> i;
                snn_require(i.value() == 0);
                // i = 32; // This does not compile.
                i = num::safe{32};
                snn_require(i.value() == 32);
                i = {123};
                snn_require(i.value() == 123);
                snn_require(i.value_or(111) == 123);
                i = app::make_safe_overflow<int>();
                snn_require(!i.has_value());
                snn_require(i.value_or(111) == 111);
                i = num::safe<int>{99};
                snn_require(i.has_value());
                snn_require(i.value() == 99);
            }

            // Addition
            // safe& add(I)
            {
                num::safe<u8> i;
                snn_require(i.value() == 0);
                i.add(120).add(80);
                snn_require(i.has_value());
                snn_require(i.value() == 200);
                i.add(56);
                snn_require(!i.has_value());
                i.add(0);
                snn_require(!i.has_value());
            }
            // safe& add(safe)
            {
                num::safe<u8> i;
                snn_require(i.value() == 0);
                i.add(num::safe{30}).add(num::safe{70});
                snn_require(i.value() == 100);
                i.add(num::safe{100});
                snn_require(i.value() == 200);
                i.add(num::safe{100});
                snn_require(!i.has_value());
                i.add(num::safe{0});
                snn_require(!i.has_value());
                i.add(app::make_safe_overflow<int>());
                snn_require(!i.has_value());
            }
            {
                num::safe<i32> i;
                i.add(app::make_safe_overflow<int>());
                snn_require(!i.has_value());
            }
            // safe& operator+=(I)
            {
                num::safe<u8> i;
                snn_require(i.value() == 0);
                i += 200;
                snn_require(i.has_value());
                snn_require(i.value() == 200);
                i += 56;
                snn_require(!i.has_value());
                i += 1;
                snn_require(!i.has_value());
            }
            // safe& operator+=(safe)
            {
                num::safe<u8> i;
                snn_require(i.value() == 0);
                i += num::safe{100};
                snn_require(i.value() == 100);
                i += num::safe{100};
                snn_require(i.value() == 200);
                i += num::safe{100};
                snn_require(!i.has_value());
                i += num::safe{0};
                snn_require(!i.has_value());
                i += app::make_safe_overflow<int>();
                snn_require(!i.has_value());
            }
            {
                num::safe<i32> i;
                i += app::make_safe_overflow<int>();
                snn_require(!i.has_value());
            }
            // safe operator+(I)
            {
                num::safe<i16> i{30'000};

                num::safe<i16> j = i + 2'000;
                snn_require(j.has_value());
                snn_require(j.value() == 32'000);

                num::safe<i16> k = i + 3'000;
                snn_require(!k.has_value());

                num::safe<i16> m = k + 0;
                snn_require(!m.has_value());
            }
            // safe operator+(safe)
            {
                num::safe<u8> i{5};
                num::safe<u8> j = i + num::safe{100};
                snn_require(j.value() == 105);

                num::safe<u8> k = i + num::safe{251};
                snn_require(!k.has_value());

                num::safe<u8> l = k + num::safe{0};
                snn_require(!l.has_value());

                num::safe<u8> m = l + app::make_safe_overflow<int>();
                snn_require(!m.has_value());
            }
            {
                num::safe<i32> i;
                num::safe<i32> j = i + app::make_safe_overflow<int>();
                snn_require(!j.has_value());
            }
            // safe& operator++()
            {
                num::safe<i16> i{32'765};
                ++i;
                snn_require(i.value() == 32'766);
                snn_require((++i).value() == 32'767);
                ++i;
                snn_require(!i.has_value());
                ++i;
                snn_require(!i.has_value());
            }
            // safe operator++(int)
            {
                num::safe<i16> i{32'766};
                const num::safe<i16> j = i++;
                snn_require(i.value() == 32'767);
                snn_require(j.value() == 32'766);
                const num::safe<i16> k = i++;
                snn_require(k.value() == 32'767);
                snn_require(!i.has_value());
                const num::safe<i16> l = i++;
                snn_require(!l.has_value());
                snn_require(!i.has_value());
            }

            // Subtraction
            // safe& subtract(I)
            {
                num::safe<i8> i{-18};
                i.subtract(20).subtract(80);
                snn_require(i.value() == -118);
                i.subtract(20).subtract(80);
                snn_require(!i.has_value());
                i.subtract(0);
                snn_require(!i.has_value());
            }
            // safe& subtract(safe)
            {
                num::safe<i8> i{};
                i.subtract(num::safe{-100});
                snn_require(i.value() == 100);
                i.subtract(num::safe{-50}).subtract(num::safe{-50});
                snn_require(!i.has_value());
                i.subtract(app::make_safe_overflow<int>());
                snn_require(!i.has_value());
            }
            {
                num::safe<i8> i;
                i.subtract(app::make_safe_overflow<int>());
                snn_require(!i.has_value());
            }
            // safe& operator-=(I)
            {
                num::safe<i8> i{-18};
                i -= 100;
                snn_require(i.value() == -118);
                i -= 20;
                snn_require(!i.has_value());
                i -= 0;
                snn_require(!i.has_value());
            }
            // safe& operator-=(safe)
            {
                num::safe<i8> i;
                i -= num::safe{-100};
                snn_require(i.value() == 100);
                i -= num::safe{-100};
                snn_require(!i.has_value());
                i -= num::safe{0};
                snn_require(!i.has_value());
                i -= app::make_safe_overflow<int>();
                snn_require(!i.has_value());
            }
            {
                num::safe<i8> i;
                i -= app::make_safe_overflow<int>();
                snn_require(!i.has_value());
            }
            // safe operator-(I)
            {
                num::safe<i16> i{-30'000};

                num::safe<i16> j = i - 2'000;
                snn_require(j.has_value());
                snn_require(j.value() == -32'000);

                num::safe<i16> k = i - 3'000;
                snn_require(!k.has_value());

                num::safe<i16> l = k - 0;
                snn_require(!l.has_value());
            }
            // safe operator-(safe)
            {
                num::safe<u8> i{255};
                num::safe<u8> j = i - num::safe<u8>{100};
                snn_require(j.value() == 155);

                num::safe<u8> k = j - i;
                snn_require(!k.has_value());

                num::safe<u8> l = k - num::safe<u8>{0};
                snn_require(!l.has_value());

                num::safe<u8> m = k - app::make_safe_overflow<int>();
                snn_require(!m.has_value());
            }
            {
                num::safe<u8> i;
                num::safe<u8> j = i - app::make_safe_overflow<int>();
                snn_require(!j.has_value());
            }
            // safe& operator--()
            {
                num::safe<i16> i{-32'766};
                --i;
                snn_require(i.value() == -32'767);
                snn_require((--i).value() == -32'768);
                --i;
                snn_require(!i.has_value());
                --i;
                snn_require(!i.has_value());
            }
            // safe operator--(int)
            {
                num::safe<i16> i{-32'767};
                const num::safe<i16> j = i--;
                snn_require(i.value() == -32'768);
                snn_require(j.value() == -32'767);
                const num::safe<i16> k = i--;
                snn_require(!i.has_value());
                snn_require(k.value() == -32'768);
                (void)i--;
                snn_require(!i.has_value());
            }

            // Multiplication
            // safe& multiply(I)
            {
                num::safe<u8> i{2};
                i.multiply(2).multiply(50);
                snn_require(i.value() == 200);
                i.multiply(2);
                snn_require(!i.has_value());
                i.multiply(1);
                snn_require(!i.has_value());
            }
            // safe& multiply(safe)
            {
                num::safe<u8> i{2};
                i.multiply(num::safe{2}).multiply(num::safe{50});
                snn_require(i.value() == 200);
                i.multiply(num::safe{100});
                snn_require(!i.has_value());
                i.multiply(num::safe{1});
                snn_require(!i.has_value());
                i.multiply(app::make_safe_overflow<int>());
                snn_require(!i.has_value());
            }
            {
                num::safe<i32> i;
                i.multiply(app::make_safe_overflow<int>());
                snn_require(!i.has_value());
            }
            // safe& operator*=(I)
            {
                num::safe<u8> i{2};
                i *= 100;
                snn_require(i.value() == 200);
                i *= 2;
                snn_require(!i.has_value());
                i *= 1;
                snn_require(!i.has_value());
            }
            // safe& operator*=(safe)
            {
                num::safe<u8> i{2};
                i *= num::safe{100};
                snn_require(i.value() == 200);
                i *= num::safe{100};
                snn_require(!i.has_value());
                i *= num::safe{1};
                snn_require(!i.has_value());
                i *= app::make_safe_overflow<int>();
                snn_require(!i.has_value());
            }
            {
                num::safe<i32> i;
                i *= app::make_safe_overflow<int>();
                snn_require(!i.has_value());
            }
            // safe operator*(I)
            {
                num::safe<i16> i = num::safe<i16>{14} * 2'000;
                snn_require(i.value() == 28'000);
                num::safe<i16> j = num::safe<i16>{14} * 3'000;
                snn_require(!j.has_value());
                num::safe<i16> k = j * 1;
                snn_require(!k.has_value());
            }
            // safe operator*(safe)
            {
                num::safe<u8> i = num::safe<u8>{2} * num::safe{100};
                snn_require(i.value() == 200);
                num::safe<u8> j = i * num::safe{2};
                snn_require(!j.has_value());
                num::safe<u8> k = j * num::safe{1};
                snn_require(!k.has_value());
                num::safe<u8> m = k * app::make_safe_overflow<int>();
                snn_require(!m.has_value());
            }
            {
                num::safe<i32> i = num::safe{0} * app::make_safe_overflow<int>();
                snn_require(!i.has_value());
            }

            // Division
            // safe& divide(I)
            {
                num::safe<i8> i{120};
                i.divide(2).divide(1);
                snn_require(i.value() == 60);
                i.divide(0);
                snn_require(!i.has_value());
                i.divide(1);
                snn_require(!i.has_value());
            }
            // safe& divide(safe)
            {
                num::safe<i8> i{120};

                i.divide(num::safe{50});
                snn_require(i.value() == 2);
                i.divide(num::safe{50}).divide(num::safe{50});
                snn_require(i.value() == 0);

                num::safe j{12};
                j.divide(i);
                snn_require(!j.has_value());
                snn_require(i.value() == 0);

                j.divide(num::safe{1});
                snn_require(!j.has_value());

                j.divide(app::make_safe_overflow<int>());
                snn_require(!j.has_value());
            }
            {
                num::safe<i32> i;
                i.divide(app::make_safe_overflow<int>());
                snn_require(!i.has_value());
            }
            // safe& operator/=(I)
            {
                num::safe<i8> i{120};
                i /= 2;
                snn_require(i.value() == 60);
                i /= 1;
                snn_require(i.value() == 60);
                i /= 0;
                snn_require(!i.has_value());
                i /= 1;
                snn_require(!i.has_value());
            }
            // safe& operator/=(safe)
            {
                num::safe<i8> i{120};

                i /= num::safe{50};
                snn_require(i.value() == 2);
                i /= num::safe{50};
                snn_require(i.value() == 0);
                i /= num::safe{50};
                snn_require(i.value() == 0);

                num::safe j{12};
                j /= i;
                snn_require(!j.has_value());
                snn_require(i.value() == 0);

                j /= num::safe{1};
                snn_require(!j.has_value());

                j /= app::make_safe_overflow<int>();
                snn_require(!j.has_value());
            }
            {
                num::safe<i32> i;
                i /= app::make_safe_overflow<int>();
                snn_require(!i.has_value());
            }
            // safe operator/(I)
            {
                num::safe<i16> i{28'000};
                num::safe<i16> j = i / 2'000;
                snn_require(j.value() == 14);
                num::safe<i16> k = i / 0;
                snn_require(!k.has_value());
                snn_require(i.value() == 28'000);
                num::safe<i16> m = k / 1;
                snn_require(!m.has_value());
            }
            {
                num::safe<i16> i{32'767};
                num::safe<i16> j = i / -1;
                snn_require(i.value() == 32'767);
                snn_require(j.value() == -32'767);
            }
            {
                num::safe<i16> i{-32'768};
                num::safe<i16> j = i / -1;
                snn_require(!j.has_value());
            }
            // safe operator/(safe)
            {
                num::safe<i8> i = num::safe<i8>{100} / num::safe{2};
                snn_require(i.value() == 50);
                num::safe<i8> j = i / num::safe{0};
                snn_require(!j.has_value());
                num::safe<i8> k = j / num::safe{1};
                snn_require(!k.has_value());
                num::safe<i8> m = k / app::make_safe_overflow<int>();
                snn_require(!m.has_value());
            }
            {
                num::safe<i32> i = num::safe{0} / app::make_safe_overflow<int>();
                snn_require(!i.has_value());
            }

            // Modulo operation / Remainder
            // safe& modulo(I)
            {
                num::safe<u8> i{79};
                i.modulo(10).modulo(10);
                snn_require(i.value() == 9);
                i.modulo(1);
                snn_require(i.value() == 0);
                i.modulo(0);
                snn_require(!i.has_value());
                i.modulo(1);
                snn_require(!i.has_value());
            }
            {
                num::safe<i16> i{-32'768};
                i.modulo(-1);
                snn_require(i.value() == 0);
            }
            // safe& modulo(safe)
            {
                num::safe<i32> i{102};
                i.modulo(num::safe{100}).modulo(num::safe{100});
                snn_require(i.value() == 2);
                i.modulo(num::safe{0});
                snn_require(!i.has_value());
                i.modulo(num::safe{1});
                snn_require(!i.has_value());
                i.modulo(app::make_safe_overflow<int>());
                snn_require(!i.has_value());
            }
            {
                num::safe<i32> i;
                i.modulo(app::make_safe_overflow<int>());
                snn_require(!i.has_value());
            }
            // safe& operator%=(I)
            {
                num::safe<u8> i{79};
                i %= 10;
                snn_require(i.value() == 9);
                i %= 1;
                snn_require(i.value() == 0);
                i %= 1;
                snn_require(i.value() == 0);
                i %= 0;
                snn_require(!i.has_value());
                i %= 1;
                snn_require(!i.has_value());
            }
            {
                num::safe<i16> i{-32'768};
                i %= -1;
                snn_require(i.value() == 0);
            }
            // safe& operator%=(safe)
            {
                num::safe<i32> i{102};
                i %= num::safe{100};
                snn_require(i.value() == 2);
                i %= num::safe{100};
                snn_require(i.value() == 2);
                i %= num::safe{0};
                snn_require(!i.has_value());
                i %= num::safe{1};
                snn_require(!i.has_value());
                i %= app::make_safe_overflow<int>();
                snn_require(!i.has_value());
            }
            {
                num::safe<i32> i;
                i %= app::make_safe_overflow<int>();
                snn_require(!i.has_value());
            }
            // safe operator%(I)
            {
                num::safe<i16> i = num::safe<i16>{28'000} % 2'300;
                snn_require(i.value() == 400);
                num::safe<i16> j = i % 0;
                snn_require(!j.has_value());
                num::safe<i16> k = j % 1;
                snn_require(!k.has_value());
            }
            {
                num::safe<i16> i{32'767};
                num::safe<i16> j = i % -1;
                snn_require(j.value() == 0);
            }
            {
                num::safe<i16> i{-32'768};
                num::safe<i16> j = i % -1;
                snn_require(j.value() == 0);
            }
            // safe operator%(safe)
            {
                num::safe<u8> i = num::safe<u8>{100} % num::safe{3};
                snn_require(i.value() == 1);
                num::safe<u8> j = i % num::safe{0};
                snn_require(!j.has_value());
                num::safe<u8> k = j % num::safe{1};
                snn_require(!k.has_value());
                num::safe<u8> l = k % app::make_safe_overflow<int>();
                snn_require(!l.has_value());
            }
            {
                num::safe<i32> i = num::safe{0} % app::make_safe_overflow<int>();
                snn_require(!i.has_value());
            }

            // Conversion
            {
                num::safe<u32> i{999};
                snn_require(i.has_value());
                snn_require(i.value() == 999);

                snn_require(!i.to<u8>().has_value());
                snn_require(i.to<u16>().has_value());
                snn_require(i.to<u16>().value() == 999);
                snn_require(i.to<u32>().has_value());
                snn_require(i.to<u32>().value() == 999);
                snn_require(i.to<u64>().has_value());
                snn_require(i.to<u64>().value() == 999);

                snn_require(!i.to<i8>().has_value());
                snn_require(i.to<i16>().has_value());
                snn_require(i.to<i16>().value() == 999);
                snn_require(i.to<i32>().has_value());
                snn_require(i.to<i32>().value() == 999);
                snn_require(i.to<i64>().has_value());
                snn_require(i.to<i64>().value() == 999);
            }
            {
                num::safe<i32> i{999};
                snn_require(i.has_value());
                snn_require(i.value() == 999);

                snn_require(!i.to<u8>().has_value());
                snn_require(i.to<u16>().has_value());
                snn_require(i.to<u16>().value() == 999);
                snn_require(i.to<u32>().has_value());
                snn_require(i.to<u32>().value() == 999);
                snn_require(i.to<u64>().has_value());
                snn_require(i.to<u64>().value() == 999);

                snn_require(!i.to<i8>().has_value());
                snn_require(i.to<i16>().has_value());
                snn_require(i.to<i16>().value() == 999);
                snn_require(i.to<i32>().has_value());
                snn_require(i.to<i32>().value() == 999);
                snn_require(i.to<i64>().has_value());
                snn_require(i.to<i64>().value() == 999);
            }
            {
                num::safe<i32> i{-999};
                snn_require(i.has_value());
                snn_require(i.value() == -999);

                snn_require(!i.to<u8>().has_value());
                snn_require(!i.to<u16>().has_value());
                snn_require(!i.to<u32>().has_value());
                snn_require(!i.to<u64>().has_value());

                snn_require(!i.to<i8>().has_value());
                snn_require(i.to<i16>().has_value());
                snn_require(i.to<i16>().value() == -999);
                snn_require(i.to<i32>().has_value());
                snn_require(i.to<i32>().value() == -999);
                snn_require(i.to<i64>().has_value());
                snn_require(i.to<i64>().value() == -999);
            }
            {
                snn_require(num::safe<i32>{-128}.to<i8>().has_value());
                snn_require(num::safe<i32>{-128}.to<i8>().value() == -128);
                snn_require(!num::safe<i32>{-129}.to<i8>().has_value());

                snn_require(num::safe<i32>{127}.to<i8>().has_value());
                snn_require(num::safe<i32>{127}.to<i8>().value() == 127);
                snn_require(!num::safe<i32>{128}.to<i8>().has_value());

                snn_require(num::safe<i32>{0}.to<u8>().has_value());
                snn_require(num::safe<i32>{0}.to<u8>().value() == 0);
                snn_require(!num::safe<i32>{-1}.to<u8>().has_value());

                snn_require(num::safe<i32>{255}.to<u8>().has_value());
                snn_require(num::safe<i32>{255}.to<u8>().value() == 255);
                snn_require(!num::safe<i32>{256}.to<u8>().has_value());

                snn_require(num::safe<u16>{127}.to<i8>().has_value());
                snn_require(num::safe<u16>{127}.to<i8>().value() == 127);
                snn_require(!num::safe<u16>{128}.to<i8>().has_value());

                snn_require(num::safe<u16>{255}.to<u8>().has_value());
                snn_require(num::safe<u16>{255}.to<u8>().value() == 255);
                snn_require(!num::safe<u16>{256}.to<u8>().has_value());
            }
            {
                auto i = app::make_safe_overflow<u32>();
                snn_require(!i.has_value());

                snn_require(!i.to<u8>().has_value());
                snn_require(!i.to<u16>().has_value());
                snn_require(!i.to<u32>().has_value());
                snn_require(!i.to<u64>().has_value());

                snn_require(!i.to<i8>().has_value());
                snn_require(!i.to<i16>().has_value());
                snn_require(!i.to<i32>().has_value());
                snn_require(!i.to<i64>().has_value());
            }
            {
                auto i = app::make_safe_overflow<i32>();
                snn_require(!i.has_value());

                snn_require(!i.to<u8>().has_value());
                snn_require(!i.to<u16>().has_value());
                snn_require(!i.to<u32>().has_value());
                snn_require(!i.to<u64>().has_value());

                snn_require(!i.to<i8>().has_value());
                snn_require(!i.to<i16>().has_value());
                snn_require(!i.to<i32>().has_value());
                snn_require(!i.to<i64>().has_value());
            }

            // Comparison
            {
                num::safe<int> a;
                snn_require(a.value() == 0);
                num::safe<int> b;
                num::safe<int> c{1};

                // bool operator==(safe)
                snn_require(a == b);
                snn_require(!(b == c));

                // bool operator!=(safe)
                snn_require(!(a != b));
                snn_require(b != c);

                // bool operator>(safe)
                snn_require(!(b > a));
                snn_require(c > b);

                // bool operator>=(safe)
                snn_require(b >= a);
                snn_require(c >= b);
                snn_require(!(b >= c));

                // bool operator<(safe)
                snn_require(!(a < b));
                snn_require(b < c);

                // bool operator<=(safe)
                snn_require(a <= b);
                snn_require(b <= c);
                snn_require(!(c <= b));
            }

            // Comparison (without a value).
            {
                num::safe<int> a;
                snn_require(a.has_value());
                auto b = app::make_safe_overflow<int>();
                snn_require(!b.has_value());
                auto c = app::make_safe_overflow<int>();
                snn_require(!c.has_value());

                // bool operator==(safe)
                snn_require(!(a == b));
                snn_require(!(b == a));
                snn_require(c == b);

                // bool operator!=(safe)
                snn_require(a != b);
                snn_require(b != a);
                snn_require(!(c != b));

                // bool operator>(safe)
                snn_require(a > b);
                snn_require(!(b > a));
                snn_require(!(c > b));

                // bool operator>=(safe)
                snn_require(a >= b);
                snn_require(!(b >= a));
                snn_require(c >= b);

                // bool operator<(safe)
                snn_require(!(a < b));
                snn_require(b < a);
                snn_require(!(c < b));

                // bool operator<=(safe)
                snn_require(!(a <= b));
                snn_require(b <= a);
                snn_require(c <= b);
            }

            // Comparison with an Int

            {
                snn_require(num::safe<int>{5} == 5);
                snn_require(!(num::safe<int>{5} == 6));
                snn_require(!(app::make_safe_overflow<int>() == 0));

                snn_require(5 == num::safe<int>{5});
                snn_require(!(6 == num::safe<int>{5}));
                snn_require(!(0 == app::make_safe_overflow<int>()));

                snn_require(num::safe<int>{5} != 6);
                snn_require(!(num::safe<int>{5} != 5));
                snn_require(app::make_safe_overflow<int>() != 0);

                snn_require(6 != num::safe<int>{5});
                snn_require(!(5 != num::safe<int>{5}));
                snn_require(0 != app::make_safe_overflow<int>());

                snn_require(num::safe<int>{5} < 6);
                snn_require(!(num::safe<int>{5} < 5));
                snn_require(!(num::safe<int>{5} < 4));
                snn_require(app::make_safe_overflow<int>() < constant::limit<int>::min);

                snn_require(4 < num::safe<int>{5});
                snn_require(!(5 < num::safe<int>{5}));
                snn_require(!(6 < num::safe<int>{5}));
                snn_require(!(constant::limit<int>::min < app::make_safe_overflow<int>()));

                snn_require(num::safe<int>{5} > 4);
                snn_require(!(num::safe<int>{5} > 5));
                snn_require(!(num::safe<int>{5} > 6));
                snn_require(!(app::make_safe_overflow<int>() > constant::limit<int>::min));

                snn_require(6 > num::safe<int>{5});
                snn_require(!(5 > num::safe<int>{5}));
                snn_require(!(4 > num::safe<int>{5}));
                snn_require(constant::limit<int>::min > app::make_safe_overflow<int>());

                snn_require(num::safe<int>{5} <= 6);
                snn_require(num::safe<int>{5} <= 5);
                snn_require(!(num::safe<int>{5} <= 4));
                snn_require(app::make_safe_overflow<int>() <= constant::limit<int>::min);

                snn_require(4 <= num::safe<int>{5});
                snn_require(5 <= num::safe<int>{5});
                snn_require(!(6 <= num::safe<int>{5}));
                snn_require(!(constant::limit<int>::min <= app::make_safe_overflow<int>()));

                snn_require(num::safe<int>{5} >= 4);
                snn_require(num::safe<int>{5} >= 5);
                snn_require(!(num::safe<int>{5} >= 6));
                snn_require(!(app::make_safe_overflow<int>() >= constant::limit<int>::min));

                snn_require(6 >= num::safe<int>{5});
                snn_require(5 >= num::safe<int>{5});
                snn_require(!(4 >= num::safe<int>{5}));
                snn_require(constant::limit<int>::min >= app::make_safe_overflow<int>());
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
        snn_static_require(app::test_safe());

        static_assert(std::is_copy_constructible_v<num::safe<int>>);
        static_assert(std::is_copy_assignable_v<num::safe<int>>);
        static_assert(sane<num::safe<int>>);
    }
}
