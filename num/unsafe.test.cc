// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/num/unsafe.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            num::unsafe<i16> i{30'000};
            snn_require(i);
            snn_require(i.has_value());
            snn_require(i.value() == 30'000);
            snn_require(i.value_or(99) == 30'000);

            num::unsafe<i16> j = i + 2'000;
            snn_require(j);
            snn_require(j.has_value());
            snn_require(j.value() == 32'000);
            snn_require(j.value_or(99) == 32'000);

            return true;
        }

        constexpr bool test_unsafe()
        {
            // explicit operator bool(), has_value(), value(), value_or()
            {
                num::unsafe<int> a;
                num::unsafe<int> b{99};
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

            // Copy assignment, value_or()
            {
                num::unsafe<int> i;
                snn_require(i.value() == 0);
                i = num::unsafe{123};
                snn_require(i.value() == 123);
                snn_require(i.value_or(111) == 123);
            }

            // Addition
            // unsafe& add(I)
            {
                num::unsafe<u8> i;
                snn_require(i.value() == 0);
                i.add(120).add(80);
                snn_require(i.value() == 200);
            }
            // unsafe& add(unsafe)
            {
                num::unsafe<u8> i;
                snn_require(i.value() == 0);
                i.add(num::unsafe{30}).add(num::unsafe{70});
                snn_require(i.value() == 100);
            }
            // unsafe& operator+=(I)
            {
                num::unsafe<u8> i;
                snn_require(i.value() == 0);
                i += 200;
                snn_require(i.value() == 200);
            }
            // unsafe& operator+=(unsafe)
            {
                num::unsafe<u8> i;
                snn_require(i.value() == 0);
                i += num::unsafe{100};
                snn_require(i.value() == 100);
                i += num::unsafe{100};
                snn_require(i.value() == 200);
            }
            // unsafe operator+(I)
            {
                num::unsafe<i16> i{30'000};
                num::unsafe<i16> j = i + 2'000;
                snn_require(j.value() == 32'000);
            }
            // unsafe operator+(unsafe)
            {
                num::unsafe<u8> i{5};
                num::unsafe<u8> j = i + num::unsafe{100};
                snn_require(j.value() == 105);
            }
            // unsafe& operator++()
            {
                num::unsafe<i16> i{32'765};
                ++i;
                snn_require(i.value() == 32'766);
                snn_require((++i).value() == 32'767);
                snn_require(i.value() == 32'767);
            }
            // unsafe operator++(int)
            {
                num::unsafe<i16> i{32'766};
                const num::unsafe<i16> j = i++;
                snn_require(i.value() == 32'767);
                snn_require(j.value() == 32'766);
            }

            // Subtraction
            // unsafe& subtract(I)
            {
                num::unsafe<i8> i{-18};
                i.subtract(20).subtract(80);
                snn_require(i.value() == -118);
            }
            // unsafe& subtract(unsafe)
            {
                num::unsafe<i8> i{};
                i.subtract(num::unsafe{-100});
                snn_require(i.value() == 100);
            }
            // unsafe& operator-=(I)
            {
                num::unsafe<i8> i{-18};
                i -= 100;
                snn_require(i.value() == -118);
            }
            // unsafe& operator-=(unsafe)
            {
                num::unsafe<i8> i;
                i -= num::unsafe{-100};
                snn_require(i.value() == 100);
            }
            // unsafe operator-(I)
            {
                num::unsafe<i16> i{-30'000};
                num::unsafe<i16> j = i - 2'000;
                snn_require(j.value() == -32'000);
            }
            // unsafe operator-(unsafe)
            {
                num::unsafe<u8> i{255};
                num::unsafe<u8> j = i - num::unsafe<u8>{100};
                snn_require(j.value() == 155);
            }
            // unsafe& operator--()
            {
                num::unsafe<i16> i{-32'766};
                --i;
                snn_require(i.value() == -32'767);
                snn_require((--i).value() == -32'768);
                snn_require(i.value() == -32'768);
            }
            // unsafe operator--(int)
            {
                num::unsafe<i16> i{-32'767};
                const num::unsafe<i16> j = i--;
                snn_require(i.value() == -32'768);
                snn_require(j.value() == -32'767);
            }

            // Multiplication
            // unsafe& multiply(I)
            {
                num::unsafe<u8> i{2};
                i.multiply(2).multiply(50);
                snn_require(i.value() == 200);
            }
            // unsafe& multiply(unsafe)
            {
                num::unsafe<u8> i{2};
                i.multiply(num::unsafe{2}).multiply(num::unsafe{50});
                snn_require(i.value() == 200);
            }
            // unsafe& operator*=(I)
            {
                num::unsafe<u8> i{2};
                i *= 100;
                snn_require(i.value() == 200);
            }
            // unsafe& operator*=(unsafe)
            {
                num::unsafe<u8> i{2};
                i *= num::unsafe{100};
                snn_require(i.value() == 200);
            }
            // unsafe operator*(I)
            {
                num::unsafe<i16> i = num::unsafe<i16>{14} * 2'000;
                snn_require(i.value() == 28'000);
            }
            // unsafe operator*(unsafe)
            {
                num::unsafe<u8> i = num::unsafe<u8>{2} * num::unsafe{100};
                snn_require(i.value() == 200);
            }

            // Division
            // unsafe& divide(I)
            {
                num::unsafe<i8> i{120};
                i.divide(2).divide(1);
                snn_require(i.value() == 60);
            }
            // unsafe& divide(unsafe)
            {
                num::unsafe<i8> i{120};
                i.divide(num::unsafe{50});
                snn_require(i.value() == 2);
                i.divide(num::unsafe{50}).divide(num::unsafe{50});
                snn_require(i.value() == 0);
            }
            // unsafe& operator/=(I)
            {
                num::unsafe<i8> i{120};
                i /= 2;
                snn_require(i.value() == 60);
                i /= 1;
                snn_require(i.value() == 60);
            }
            // unsafe& operator/=(unsafe)
            {
                num::unsafe<i8> i{120};
                i /= num::unsafe{50};
                snn_require(i.value() == 2);
                i /= num::unsafe{50};
                snn_require(i.value() == 0);
                i /= num::unsafe{50};
                snn_require(i.value() == 0);
            }
            // unsafe operator/(I)
            {
                num::unsafe<i16> i{28'000};
                num::unsafe<i16> j = i / 2'000;
                snn_require(j.value() == 14);
            }
            {
                num::unsafe<i16> i{32'767};
                num::unsafe<i16> j = i / -1;
                snn_require(i.value() == 32'767);
                snn_require(j.value() == -32'767);
            }
            // unsafe operator/(unsafe)
            {
                num::unsafe<i8> i = num::unsafe<i8>{100} / num::unsafe{2};
                snn_require(i.value() == 50);
            }

            // Modulo operation / Remainder
            // unsafe& modulo(I)
            {
                num::unsafe<u8> i{79};
                i.modulo(10).modulo(10);
                snn_require(i.value() == 9);
                i.modulo(1);
                snn_require(i.value() == 0);
            }
            {
                num::unsafe<i16> i{-32'768};
                i.modulo(-1);
                snn_require(i.value() == 0);
            }
            // unsafe& modulo(unsafe)
            {
                num::unsafe<i32> i{102};
                i.modulo(num::unsafe{100}).modulo(num::unsafe{100});
                snn_require(i.value() == 2);
            }
            // unsafe& operator%=(I)
            {
                num::unsafe<u8> i{79};
                i %= 10;
                snn_require(i.value() == 9);
                i %= 1;
                snn_require(i.value() == 0);
                i %= 1;
                snn_require(i.value() == 0);
            }
            {
                num::unsafe<i16> i{-32'768};
                i %= -1;
                snn_require(i.value() == 0);
            }
            // unsafe& operator%=(unsafe)
            {
                num::unsafe<i32> i{102};
                i %= num::unsafe{100};
                snn_require(i.value() == 2);
                i %= num::unsafe{100};
                snn_require(i.value() == 2);
            }
            // unsafe operator%(I)
            {
                num::unsafe<i16> i = num::unsafe<i16>{28'000} % 2'300;
                snn_require(i.value() == 400);
            }
            {
                num::unsafe<i16> i{32'767};
                num::unsafe<i16> j = i % -1;
                snn_require(j.value() == 0);
            }
            {
                num::unsafe<i16> i{-32'768};
                num::unsafe<i16> j = i % -1;
                snn_require(j.value() == 0);
            }
            // unsafe operator%(unsafe)
            {
                num::unsafe<u8> i = num::unsafe<u8>{100} % num::unsafe{3};
                snn_require(i.value() == 1);
            }

            // Conversion
            {
                num::unsafe<i32> i{-999};
                snn_require(i.has_value());
                snn_require(i.value() == -999);

                snn_require(i.to<u8>().value() == 25);
                snn_require(i.to<u16>().value() == 64537);
                snn_require(i.to<u32>().value() == 4294966297);
                snn_require(i.to<u64>().value() == 18446744073709550617u);

                snn_require(i.to<i8>().value() == 25);
                snn_require(i.to<i16>().value() == -999);
                snn_require(i.to<i32>().value() == -999);
                snn_require(i.to<i64>().value() == -999);
            }

            // Comparison
            {
                num::unsafe<int> a;
                snn_require(a.value() == 0);
                num::unsafe<int> b;
                num::unsafe<int> c{1};

                // bool operator==(unsafe)
                snn_require(a == b);
                snn_require(!(b == c));

                // bool operator!=(unsafe)
                snn_require(!(a != b));
                snn_require(b != c);

                // bool operator>(unsafe)
                snn_require(!(b > a));
                snn_require(c > b);

                // bool operator>=(unsafe)
                snn_require(b >= a);
                snn_require(c >= b);
                snn_require(!(b >= c));

                // bool operator<(unsafe)
                snn_require(!(a < b));
                snn_require(b < c);

                // bool operator<=(unsafe)
                snn_require(a <= b);
                snn_require(b <= c);
                snn_require(!(c <= b));
            }

            // Comparison with an Int
            {
                snn_require(num::unsafe<int>{5} == 5);
                snn_require(!(num::unsafe<int>{5} == 6));

                snn_require(5 == num::unsafe<int>{5});
                snn_require(!(6 == num::unsafe<int>{5}));

                snn_require(num::unsafe<int>{5} != 6);
                snn_require(!(num::unsafe<int>{5} != 5));

                snn_require(6 != num::unsafe<int>{5});
                snn_require(!(5 != num::unsafe<int>{5}));

                snn_require(num::unsafe<int>{5} < 6);
                snn_require(!(num::unsafe<int>{5} < 5));
                snn_require(!(num::unsafe<int>{5} < 4));

                snn_require(4 < num::unsafe<int>{5});
                snn_require(!(5 < num::unsafe<int>{5}));
                snn_require(!(6 < num::unsafe<int>{5}));

                snn_require(num::unsafe<int>{5} > 4);
                snn_require(!(num::unsafe<int>{5} > 5));
                snn_require(!(num::unsafe<int>{5} > 6));

                snn_require(6 > num::unsafe<int>{5});
                snn_require(!(5 > num::unsafe<int>{5}));
                snn_require(!(4 > num::unsafe<int>{5}));

                snn_require(num::unsafe<int>{5} <= 6);
                snn_require(num::unsafe<int>{5} <= 5);
                snn_require(!(num::unsafe<int>{5} <= 4));

                snn_require(4 <= num::unsafe<int>{5});
                snn_require(5 <= num::unsafe<int>{5});
                snn_require(!(6 <= num::unsafe<int>{5}));

                snn_require(num::unsafe<int>{5} >= 4);
                snn_require(num::unsafe<int>{5} >= 5);
                snn_require(!(num::unsafe<int>{5} >= 6));

                snn_require(6 >= num::unsafe<int>{5});
                snn_require(5 >= num::unsafe<int>{5});
                snn_require(!(4 >= num::unsafe<int>{5}));
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
        snn_static_require(app::test_unsafe());
    }
}
