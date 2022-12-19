// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/array.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/algo/replace.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/fn/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array a{'a', '1', 'x'};

            static_assert(std::is_same_v<decltype(a), array<char, 3>>);
            static_assert(std::is_aggregate_v<decltype(a)>);

            snn_require(a); // Not empty.
            snn_require(!a.is_empty());
            snn_require(a.capacity() == 3);
            snn_require(a.count() == 3);
            snn_require(a.count<i64>() == 3);
            snn_require(a.size() == 3);
            snn_require(a.byte_size().get() == 3);

            snn_require(a.at(0).value() == 'a');
            snn_require(a.at(1).value() == '1');
            snn_require(a.at<char>(2).value() == 'x');

            snn_require(a.at(0, promise::within_bounds) == 'a');
            snn_require(a.at(1, promise::within_bounds) == '1');
            snn_require(a.at(2, promise::within_bounds) == 'x');

            decltype(auto) opt = a.at(3);
            static_assert(std::is_same_v<decltype(opt), optional<char&>>);
            snn_require(!opt.has_value());
            snn_require(opt.error_code() == nullopt);

            decltype(auto) opt_val = a.at<char>(3);
            static_assert(std::is_same_v<decltype(opt_val), optional<char>>);
            snn_require(!opt_val.has_value());

            snn_require(a.front().value() == 'a');
            snn_require(a.back().value() == 'x');
            snn_require(a.front<char>().value() == 'a');
            snn_require(a.back<char>().value() == 'x');
            snn_require(a.front(promise::not_empty) == 'a');
            snn_require(a.back(promise::not_empty) == 'x');

            snn_require(a.get<0>() == 'a');
            snn_require(a.get<1>() == '1');
            snn_require(a.get<2>() == 'x');

            snn_require(get<0>(a) == 'a');
            snn_require(get<1>(a) == '1');
            snn_require(get<2>(a) == 'x');

            snn_require(!a.is_sorted());

            snn_require(a.all(chr::is_alphanumeric));

            snn_require(a.count('x') == 1);
            snn_require(a.count('y') == 0);

            snn_require(a.count_if(chr::is_alpha_lower) == 2);
            snn_require(a.count_if(chr::is_digit) == 1);
            snn_require(a.count_if(chr::is_alpha_upper) == 0);

            decltype(auto) v = a.view();
            static_assert(std::is_same_v<decltype(v), strview>);
            snn_require(v == "a1x");

            snn_require(a.view(1) == "1x");
            snn_require(a.view_offset(-2) == "1x");

            snn_require(algo::is_equal(a.range(), {'a', '1', 'x'}));

            return true;
        }

        constexpr bool test_array()
        {
            {
                constexpr array<int, 3> a;
                static_assert(std::is_aggregate_v<decltype(a)>);
                static_assert(sizeof(a) == (3 * sizeof(int)));
                static_assert(a);
                static_assert(!a.is_empty());
                static_assert(a.capacity() == 3);
                static_assert(a.count() == 3);
                static_assert(a.byte_size().get() == 12);
                static_assert(a.at(0).value() == 0); // Value-initialized.
                static_assert(a.at(1).value() == 0); // Value-initialized.
                static_assert(a.at(2).value() == 0); // Value-initialized.
                static_assert(a.data().get() == &a.at(0).value());
            }

            {
                constexpr array<char, 2> a;
                static_assert(std::is_aggregate_v<decltype(a)>);
                static_assert(sizeof(a) == 2);
                static_assert(a);
                static_assert(!a.is_empty());
                static_assert(a.capacity() == 2);
                static_assert(a.count() == 2);
                static_assert(a.byte_size().get() == 2);
                static_assert(a.at(0).value() == '\0');                 // Value-initialized.
                static_assert(a.at(0, promise::within_bounds) == '\0'); // Value-initialized.
                static_assert(a.at(1).value() == '\0');                 // Value-initialized.
                static_assert(a.at(1, promise::within_bounds) == '\0'); // Value-initialized.
            }

            {
                constexpr array<char, 127> a;
                static_assert(a.count() == 127);
                static_assert(a.count<u64>() == 127);
                static_assert(a.count<u32>() == 127);
                static_assert(a.count<u16>() == 127);
                static_assert(a.count<u8>() == 127);
                static_assert(a.count<i64>() == 127);
                static_assert(a.count<i32>() == 127);
                static_assert(a.count<i16>() == 127);
                static_assert(a.count<i8>() == 127);
            }
            {
                constexpr array<char, 128> a;
                static_assert(a.count() == 128);
                static_assert(a.count<u64>() == 128);
                static_assert(a.count<u32>() == 128);
                static_assert(a.count<u16>() == 128);
                static_assert(a.count<u8>() == 128);
                static_assert(a.count<i64>() == 128);
                static_assert(a.count<i32>() == 128);
                static_assert(a.count<i16>() == 128);
                // static_assert(a.count<i8>() == 128); // Will not compile (type max is too small).
            }

            {
                array<int, 3> a;
                static_assert(std::is_aggregate_v<decltype(a)>);
                snn_require(a);
                snn_require(!a.is_empty());
                snn_require(a.capacity() == 3);
                snn_require(a.count() == 3);
                snn_require(a.byte_size().get() == 12);
                snn_require(a.at(0).value() == 0); // Value-initialized.
                snn_require(a.at(1).value() == 0); // Value-initialized.
                snn_require(a.at(2).value() == 0); // Value-initialized.

                a.fill(123);

                snn_require(a.at(0).value() == 123);
                snn_require(a.at(1).value() == 123);
                snn_require(a.at(2).value() == 123);
            }

            {
                constexpr array a{'a', 'b', 'b', '1', '3', 'x'};
                static_assert(std::is_same_v<decltype(a), const array<char, 6>>);

                static_assert(a.find('a').value_or_npos() == 0);
                static_assert(a.find('b').value_or_npos() == 1);
                static_assert(a.find('b', 1).value_or_npos() == 1);
                static_assert(a.find('b', 2).value_or_npos() == 2);
                static_assert(a.find('b', 3).value_or_npos() == constant::npos);
                static_assert(a.find('x').value_or_npos() == 5);
                static_assert(a.find('\0').value_or_npos() == constant::npos);

                static_assert(a.find_if(chr::is_digit).value_or_npos() == 3);
                static_assert(a.find_if(chr::is_digit, 3).value_or_npos() == 3);
                static_assert(a.find_if(chr::is_digit, 4).value_or_npos() == 4);
                static_assert(a.find_if(chr::is_digit, 5).value_or_npos() == constant::npos);

                static_assert(a.find_in_reverse('a').value_or_npos() == 0);
                static_assert(a.find_in_reverse('b').value_or_npos() == 2);
                static_assert(a.find_in_reverse('b', 2).value_or_npos() == 2);
                static_assert(a.find_in_reverse('b', 1).value_or_npos() == 1);
                static_assert(a.find_in_reverse('b', 0).value_or_npos() == constant::npos);
                static_assert(a.find_in_reverse('x').value_or_npos() == 5);
                static_assert(a.find_in_reverse('\0').value_or_npos() == constant::npos);

                static_assert(a.find_in_reverse_if(chr::is_digit).value_or_npos() == 4);
                static_assert(a.find_in_reverse_if(chr::is_digit, 4).value_or_npos() == 4);
                static_assert(a.find_in_reverse_if(chr::is_digit, 3).value_or_npos() == 3);
                static_assert(a.find_in_reverse_if(chr::is_digit, 2).value_or_npos() ==
                              constant::npos);
            }

            // "Empty"
            {
                constexpr array<int, 0> a;
                static_assert(std::is_aggregate_v<decltype(a)>);
                static_assert(sizeof(a) == sizeof(int)); // The internal array can not be empty.
                static_assert(!a);
                static_assert(a.is_empty());
                static_assert(a.capacity() == 0);
                static_assert(a.count() == 0);
                static_assert(a.count<i64>() == 0);
                static_assert(a.byte_size().get() == 0);
                static_assert(!a.at(0).has_value());
                static_assert(!a.front().has_value());
                static_assert(!a.back().has_value());
                static_assert(a.begin() == a.end());
                static_assert(a.is_sorted());
                static_assert(!a.has_front(3));
                static_assert(!a.has_back(3));
            }

            // Deduction
            {
                constexpr array a{'a'};
                static_assert(std::is_same_v<decltype(a), const array<char, 1>>);
                static_assert(std::is_aggregate_v<decltype(a)>);
                static_assert(a);
                static_assert(!a.is_empty());
                static_assert(a.capacity() == 1);
                static_assert(a.count() == 1);
                static_assert(a.size() == 1);
                static_assert(a.byte_size().get() == 1);
                static_assert(a.at(0).value() == 'a');
                static_assert(a.at(0, promise::within_bounds) == 'a');
                static_assert(a.at(0, bounds::mask) == 'a');
                static_assert(a.at(192, bounds::mask) == 'a');
                static_assert(!a.at(1).has_value());
                static_assert(a.front().value() == 'a');
                static_assert(a.back().value() == 'a');
                static_assert(a.has_front('a'));
                static_assert(!a.has_front('b'));
                static_assert(a.has_back('a'));
                static_assert(!a.has_back('b'));
            }
            {
                constexpr array a{'a', '1'};
                static_assert(std::is_same_v<decltype(a), const array<char, 2>>);
                static_assert(std::is_aggregate_v<decltype(a)>);
                static_assert(a);
                static_assert(!a.is_empty());
                static_assert(a.capacity() == 2);
                static_assert(a.count() == 2);
                static_assert(a.size() == 2);
                static_assert(a.byte_size().get() == 2);
                static_assert(a.at(0).value() == 'a');
                static_assert(a.at(1).value() == '1');
                static_assert(a.at(0, promise::within_bounds) == 'a');
                static_assert(a.at(1, promise::within_bounds) == '1');
                static_assert(a.at(0, bounds::mask) == 'a');
                static_assert(a.at(1, bounds::mask) == '1');
                static_assert(a.at(192, bounds::mask) == 'a');
                static_assert(a.at(193, bounds::mask) == '1');
                static_assert(!a.at(2).has_value());
                static_assert(a.front().value() == 'a');
                static_assert(a.back().value() == '1');
                static_assert(a.has_front('a'));
                static_assert(!a.has_front('1'));
                static_assert(a.has_back('1'));
                static_assert(!a.has_back('a'));
                static_assert(a.contains('a'));
                static_assert(!a.contains('b'));
            }
            {
                constexpr array a{"abc"};
                static_assert(std::is_same_v<decltype(a), const array<cstrview, 1>>);
                static_assert(std::is_aggregate_v<decltype(a)>);
                static_assert(a);
                static_assert(!a.is_empty());
                static_assert(a.capacity() == 1);
                static_assert(a.count() == 1);
                static_assert(a.byte_size().get() == 16);
                static_assert(a.at(0).value() == "abc");
                static_assert(a.at(0, promise::within_bounds) == "abc");
            }
            {
                constexpr array a{"ab", "cdef", "ghijkl"};
                static_assert(std::is_same_v<decltype(a), const array<cstrview, 3>>);
                static_assert(std::is_aggregate_v<decltype(a)>);
                static_assert(a);
                static_assert(!a.is_empty());
                static_assert(a.capacity() == 3);
                static_assert(a.count() == 3);
                static_assert(a.byte_size().get() == 48);
                static_assert(a.at(0).value() == "ab");
                static_assert(a.at(1, promise::within_bounds) == "cdef");
                static_assert(a.at(2).value() == "ghijkl");
                static_assert(!a.at(3).has_value());
                static_assert(a.is_sorted());
            }
            {
                constexpr array a{cstrview{"ab"}, "cdef", "ghijkl"};
                static_assert(std::is_same_v<decltype(a), const array<cstrview, 3>>);
                static_assert(a.is_sorted());
            }

            // Structured binding.
            // get<Index>(...)
            // tuple_element
            // tuple_size
            {
                array<char, 3> a{'1', '2', '3'};

                auto [x, y, z] = a;
                snn_require(x == '1');
                snn_require(y == '2');
                snn_require(z == '3');

                static_assert(std::is_same_v<decltype(get<0>(a)), char&>);
                static_assert(std::is_same_v<decltype(get<0>(std::as_const(a))), const char&>);
                static_assert(std::is_same_v<decltype(get<0>(std::move(a))), char&&>);
                static_assert(
                    std::is_same_v<decltype(get<0>(std::move(std::as_const(a)))), const char&&>);

                static_assert(std::tuple_size_v<decltype(a)> == 3);
                static_assert(std::is_same_v<std::tuple_element_t<0, decltype(a)>, char>);
            }

            // view
            {
                array<str, 4> a{"abc", "ABC", "abc", "123"};
                static_assert(std::is_aggregate_v<decltype(a)>);

                algo::replace(a.range(), "abc", "abcdefghijklmnopqrstuvwxyz");

                snn_require(a.count() == 4);
                snn_require(a.at(0).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(a.at(1).value() == "ABC");
                snn_require(a.at(2).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(a.at(3).value() == "123");

                snn_require(a.byte_size().get() == 96);
                snn_require(a.data().get() == &a.at(0).value());
                snn_require(a.writable().get() == &a.at(0).value());

                {
                    auto s = a.view(1, 2);
                    static_assert(std::is_same_v<decltype(s), array_view<str>>);
                    snn_require(s.count() == 2);
                    snn_require(s.at(0).value() == "ABC");
                    snn_require(s.at(1).value() == "abcdefghijklmnopqrstuvwxyz");
                }
                {
                    auto s = a.view(3);
                    snn_require(s.count() == 1);
                    snn_require(s.at(0).value() == "123");
                }
            }
            {
                const array<int, 4> a{9, 3, 7, 2};
                snn_require(a.count() == 4);
                snn_require(a.byte_size().get() == 16);

                {
                    auto s = a.view(1, 2);
                    static_assert(std::is_same_v<decltype(s), array_view<const int>>);
                    snn_require(s.count() == 2);
                    snn_require(s.at(0).value() == 3);
                    snn_require(s.at(1).value() == 7);
                }
                {
                    auto s = a.view(3);
                    snn_require(s.count() == 1);
                    snn_require(s.at(0).value() == 2);
                }
            }

            // view<>
            {
                const array a{'a', 'b', 'c', 'd', 'e', 'f'};
                {
                    auto v = a.view<>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 6>>);
                    snn_require(v == "abcdef");
                }
                {
                    auto v = a.view<2>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 4>>);
                    snn_require(v == "cdef");
                }
                {
                    auto v = a.view<2, constant::npos>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 4>>);
                    snn_require(v == "cdef");
                }
                {
                    auto v = a.view<2, 2>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 2>>);
                    snn_require(v == "cd");
                }
                {
                    auto v = a.view<2, 0>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 0>>);
                    snn_require(v == "");
                }
                {
                    auto v = a.view<5, 99>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 1>>);
                    snn_require(v == "f");
                }
                {
                    auto v = a.view<6>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 0>>);
                    snn_require(v == "");
                }
                {
                    auto v = a.view<99>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 0>>);
                    snn_require(v == "");
                }
            }
            {
                array a{'a', 'b', 'c', 'd', 'e', 'f'};
                {
                    auto v = a.view<>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 6>>);
                    snn_require(v == "abcdef");
                }
                {
                    auto v = a.view<2>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 4>>);
                    snn_require(v == "cdef");
                }
                {
                    auto v = a.view<2, constant::npos>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 4>>);
                    snn_require(v == "cdef");
                }
                {
                    auto v = a.view<2, 2>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 2>>);
                    snn_require(v == "cd");
                }
                {
                    auto v = a.view<2, 0>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 0>>);
                    snn_require(v == "");
                }
                {
                    auto v = a.view<5, 99>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 1>>);
                    snn_require(v == "f");
                }
                {
                    auto v = a.view<6>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 0>>);
                    snn_require(v == "");
                }
                {
                    auto v = a.view<99>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 0>>);
                    snn_require(v == "");
                }
            }

            // view_offset
            {
                const array<int, 4> a{9, 3, 7, 2};
                snn_require(a.count() == 4);

                {
                    const auto s = a.view_offset(1, -1);
                    snn_require(s.count() == 2);
                    snn_require(s.at(0).value() == 3);
                    snn_require(s.at(1).value() == 7);
                }
                {
                    const auto s = a.view_offset(-2);
                    snn_require(s.count() == 2);
                    snn_require(s.at(0).value() == 7);
                    snn_require(s.at(1).value() == 2);
                }
            }

            // view_exactly<>
            {
                const array a{'a', 'b', 'c', 'd', 'e', 'f'};
                {
                    auto v = a.view_exactly<0, 6>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 6>>);
                    snn_require(v == "abcdef");
                }
                {
                    auto v = a.view_exactly<0, 3>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 3>>);
                    snn_require(v == "abc");
                }
                {
                    auto v = a.view_exactly<2, 2>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 2>>);
                    snn_require(v == "cd");
                }
                {
                    auto v = a.view_exactly<2, 4>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 4>>);
                    snn_require(v == "cdef");
                }
                {
                    auto v = a.view_exactly<0, 6>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 6>>);
                    snn_require(v == "abcdef");
                }
                {
                    auto v = a.view_exactly<2, 4>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 4>>);
                    snn_require(v == "cdef");
                }
            }
            {
                array a{'a', 'b', 'c', 'd', 'e', 'f'};
                {
                    auto v = a.view_exactly<0, 6>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 6>>);
                    snn_require(v == "abcdef");
                }
                {
                    auto v = a.view_exactly<0, 3>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 3>>);
                    snn_require(v == "abc");
                }
                {
                    auto v = a.view_exactly<2, 2>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 2>>);
                    snn_require(v == "cd");
                }
                {
                    auto v = a.view_exactly<2, 4>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 4>>);
                    snn_require(v == "cdef");
                }
                {
                    auto v = a.view_exactly<0, 6>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<char, 6>>);
                    snn_require(v == "abcdef");
                }
                {
                    auto v = a.view_exactly<2, 4>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<char, 4>>);
                    snn_require(v == "cdef");
                }
            }

            // view_offset<>
            {
                const array a{'a', 'b', 'c', 'd', 'e', 'f'};
                {
                    auto v = a.view_offset<-9>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 6>>);
                    snn_require(v == "abcdef");
                }
                {
                    auto v = a.view_offset<-9, 6>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 3>>);
                    snn_require(v == "abc");
                }
                {
                    auto v = a.view_offset<-4, 2>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 2>>);
                    snn_require(v == "cd");
                }
                {
                    auto v = a.view_offset<-4, -2>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 2>>);
                    snn_require(v == "cd");
                }
                {
                    auto v = a.view_offset<2>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 4>>);
                    snn_require(v == "cdef");
                }
            }
            {
                array a{'a', 'b', 'c', 'd', 'e', 'f'};
                {
                    auto v = a.view_offset<-9>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 6>>);
                    snn_require(v == "abcdef");
                }
                {
                    auto v = a.view_offset<-9, 6>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 3>>);
                    snn_require(v == "abc");
                }
                {
                    auto v = a.view_offset<-4, 2>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 2>>);
                    snn_require(v == "cd");
                }
                {
                    auto v = a.view_offset<-4, -2>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 2>>);
                    snn_require(v == "cd");
                }
                {
                    auto v = a.view_offset<2>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 4>>);
                    snn_require(v == "cdef");
                }
            }

            // swap
            {
                array<str, 2> a{"abc", "ABC"};
                array<str, 2> b{"abcdefghijklmnopqrstuvwxyz", "123"};

                snn_require(a.at(0).value() == "abc");
                snn_require(a.at(1).value() == "ABC");
                snn_require(b.at(0).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(b.at(1).value() == "123");

                b.swap(b); // Swap with self.

                snn_require(a.at(0).value() == "abc");
                snn_require(a.at(1).value() == "ABC");
                snn_require(b.at(0).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(b.at(1).value() == "123");

                a.swap(b);

                snn_require(a.at(0).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(a.at(1).value() == "123");
                snn_require(b.at(0).value() == "abc");
                snn_require(b.at(1).value() == "ABC");

                swap(a, b); // Non-member `swap(...)`.

                snn_require(a.at(0).value() == "abc");
                snn_require(a.at(1).value() == "ABC");
                snn_require(b.at(0).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(b.at(1).value() == "123");
            }

            // fill_front
            {
                array<char, 6> a;
                snn_require(a);
                snn_require(!a.is_empty());
                snn_require(a.capacity() == 6);
                snn_require(a.count() == 6);
                snn_require(a.byte_size().get() == 6);

                snn_require(a.all(fn::is_zero{})); // Value-initialized.

                snn_require(a.fill_front(cstrview{}) == 0);
                snn_require(a.all(fn::is_zero{}));

                {
                    constexpr cstrview s{"abc"};
                    static_assert(s.count() == 3);
                    snn_require(a.fill_front(s) == 3);
                    snn_require(a.view() == "abc\0\0\0");
                }

                {
                    constexpr array arr{'d', 'd', 'e', 'e'};
                    static_assert(arr.count() == 4);
                    snn_require(a.fill_front(arr) == 4);
                    snn_require(a.view() == "ddee\0\0");
                }

                {
                    constexpr cstrview s{"0123456789"};
                    static_assert(s.count() == 10);
                    snn_require(a.fill_front(s) == 6);
                    snn_require(a.view() == "012345");
                }

                {
                    constexpr array arr{'a', 'b', 'c', '1', '2', '3', '4'};
                    static_assert(arr.count() == 7);
                    snn_require(a.fill_front(arr) == 6);
                    snn_require(a.view() == "abc123");
                }
            }
            {
                array<char, 9> a;

                snn_require(a.all(fn::is_zero{})); // Value-initialized.

                snn_require(a.fill_front(cstrview{}) == 0);
                snn_require(a.all(fn::is_zero{}));

                snn_require(a.fill_front(cstrview{"abc"}) == 3);
                snn_require(a.view() == "abc\0\0\0\0\0\0");

                snn_require(a.fill_front(cstrview{"abcdefghijklmnopqrstuvwxyz"}) == 9);
                snn_require(a.view() == "abcdefghi");
            }

            // each_in_reverse
            {
                array a{'a', 'b', 'c'};
                static_assert(a.count() == 3);
                str dest;
                a.each_in_reverse([&dest](char& c) { dest.append(c); });
                snn_require(dest.size() == 3);
                snn_require(dest == "cba");
            }

            // each_in_reverse (const)
            {
                constexpr array a{'a', 'b', 'c'};
                static_assert(a.count() == 3);
                str dest;
                a.each_in_reverse([&dest](const char& c) { dest.append(c); });
                snn_require(dest.size() == 3);
                snn_require(dest == "cba");
            }

            // replace/replace_if
            {
                array a{'a', 'b', 'c', 'c', '1', '2', '3'};
                static_assert(a.count() == 7);
                snn_require(a.view() == "abcc123");
                snn_require(a.replace('c', 'C') == 2);
                snn_require(a.view() == "abCC123");
                snn_require(a.replace_if(chr::is_digit, '.') == 3);
                snn_require(a.view() == "abCC...");
            }

            // all/any/none
            {
                constexpr array<char, 2> a{'a', 'b'};
                static_assert(a.all(chr::is_alpha_lower));
                static_assert(a.any(chr::is_alpha_lower));
                static_assert(!a.none(chr::is_alpha_lower));
            }
            {
                constexpr array<char, 2> a{'A', 'b'};
                static_assert(!a.all(chr::is_alpha_lower));
                static_assert(a.any(chr::is_alpha_lower));
                static_assert(!a.none(chr::is_alpha_lower));
            }
            {
                constexpr array<char, 2> a{'a', 'B'};
                static_assert(!a.all(chr::is_alpha_lower));
                static_assert(a.any(chr::is_alpha_lower));
                static_assert(!a.none(chr::is_alpha_lower));
            }
            {
                constexpr array<char, 2> a{'A', 'B'};
                static_assert(!a.all(chr::is_alpha_lower));
                static_assert(!a.any(chr::is_alpha_lower));
                static_assert(a.none(chr::is_alpha_lower));
            }

            // reverse/sort
            {
                array a{'a', 'b', 'c'};
                snn_require(a.view() == "abc");
                a.reverse();
                snn_require(a.view() == "cba");
                a.sort();
                snn_require(a.view() == "abc");
                a.sort(fn::greater_than{});
                snn_require(a.view() == "cba");
            }

            // transform
            {
                array a{1, 3, 7};
                a.transform(fn::bind{fn::multiply{}, 2});
                snn_require(a.at(0).value() == 2);
                snn_require(a.at(1).value() == 6);
                snn_require(a.at(2).value() == 14);
            }
            {
                array a{1, 3, 7};
                array b{2, 1, 3};
                a.transform(b, fn::multiply{});
                snn_require(a.at(0).value() == 2);
                snn_require(a.at(1).value() == 3);
                snn_require(a.at(2).value() == 21);

                snn_require(b.at(0).value() == 2);
                snn_require(b.at(1).value() == 1);
                snn_require(b.at(2).value() == 3);
            }

            // operator=
            {
                array<str, 2> a{"abc", "ABC"};
                array<str, 2> b{"abcdefghijklmnopqrstuvwxyz", "123"};

                snn_require(a.at(0).value() == "abc");
                snn_require(a.at(1).value() == "ABC");
                snn_require(b.at(0).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(b.at(1).value() == "123");

                a = b;

                snn_require(a.at(0).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(a.at(1).value() == "123");
                snn_require(b.at(0).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(b.at(1).value() == "123");
            }

            // operator==, operator<=>
            {
                constexpr array<int, 2> a{9, 8};
                constexpr array<int, 2> b{9, 8};

                // Constexpr context.
                static_assert(a == b);
                static_assert(a <= b);
                static_assert(a >= b);
                static_assert(!(a != b));
                static_assert(!(a > b));
                static_assert(!(a < b));

                // Non-constexpr context.
                snn_require(a == b);
                snn_require(a <= b);
                snn_require(a >= b);
                snn_require(!(a != b));
                snn_require(!(a > b));
                snn_require(!(a < b));
            }
            {
                constexpr array<char, 2> a{'a', 'b'};
                constexpr array<char, 2> b{'a', 'b'};

                // Constexpr context.
                static_assert(a == b);
                static_assert(a <= b);
                static_assert(a >= b);
                static_assert(!(a != b));
                static_assert(!(a > b));
                static_assert(!(a < b));

                // Non-constexpr context.
                snn_require(a == b);
                snn_require(a <= b);
                snn_require(a >= b);
                snn_require(!(a != b));
                snn_require(!(a > b));
                snn_require(!(a < b));
            }
            {
                constexpr array<int, 2> a{9, 8};
                constexpr array<int, 2> b{9, 7};

                // Constexpr context.
                static_assert(a != b);
                static_assert(!(a == b));
                static_assert(a > b);
                static_assert(b < a);
                static_assert(a >= b);
                static_assert(!(a < b));
                static_assert(!(a <= b));

                // Non-constexpr context.
                snn_require(a != b);
                snn_require(!(a == b));
                snn_require(a > b);
                snn_require(b < a);
                snn_require(a >= b);
                snn_require(!(a < b));
                snn_require(!(a <= b));
            }
            {
                constexpr array<char, 2> a{'a', 'c'};
                constexpr array<char, 2> b{'a', 'b'};

                // Constexpr context.
                static_assert(a != b);
                static_assert(!(a == b));
                static_assert(a > b);
                static_assert(b < a);
                static_assert(a >= b);
                static_assert(!(a < b));
                static_assert(!(a <= b));

                // Non-constexpr context.
                snn_require(a != b);
                snn_require(!(a == b));
                snn_require(a > b);
                snn_require(b < a);
                snn_require(a >= b);
                snn_require(!(a < b));
                snn_require(!(a <= b));
            }
            {
                array<str, 4> a{"abc", "ABC", "abc", "123"};
                const array<str, 4> b{"abcdefghijklmnopqrstuvwxyz", "ABC",
                                      "abcdefghijklmnopqrstuvwxyz", "123"};
                snn_require(a != b);
                snn_require(!(a == b));
                algo::replace(a.range(), "abc", "abcdefghijklmnopqrstuvwxyz");
                snn_require(a == b);
                snn_require(!(a != b));
            }

            // to_array
            {
                constexpr int ints[] = {11, 22, 33};
                static_assert(std::is_same_v<decltype(ints), const int[3]>);

                constexpr auto a = to_array(ints);
                static_assert(std::is_same_v<decltype(a), const array<int, 3>>);

                static_assert(a.get<0>() == 11);
                static_assert(a.get<1>() == 22);
                static_assert(a.get<2>() == 33);
            }
            {
                constexpr auto a = to_array("");
                static_assert(std::is_same_v<decltype(a), const array<char, 0>>);
            }
            {
                constexpr auto a = to_array("a");
                static_assert(std::is_same_v<decltype(a), const array<char, 1>>);
                static_assert(a.get<0>() == 'a');
            }
            {
                constexpr auto a = to_array("abc");
                static_assert(std::is_same_v<decltype(a), const array<char, 3>>);
                static_assert(a.get<0>() == 'a');
                static_assert(a.get<1>() == 'b');
                static_assert(a.get<2>() == 'c');
            }
            {
                constexpr auto a = to_array("a\0c");
                static_assert(std::is_same_v<decltype(a), const array<char, 3>>);
                static_assert(a.get<0>() == 'a');
                static_assert(a.get<1>() == '\0');
                static_assert(a.get<2>() == 'c');
            }
            {
                constexpr auto a = to_array({11});
                static_assert(std::is_same_v<decltype(a), const array<int, 1>>);
                static_assert(a.get<0>() == 11);
            }
            {
                constexpr auto a = to_array({11, 22});
                static_assert(std::is_same_v<decltype(a), const array<int, 2>>);
                static_assert(a.get<0>() == 11);
                static_assert(a.get<1>() == 22);
            }
            {
                constexpr auto a = to_array({'a'});
                static_assert(std::is_same_v<decltype(a), const array<char, 1>>);
                static_assert(a.get<0>() == 'a');
            }
            {
                constexpr auto a = to_array({'a', 'b'});
                static_assert(std::is_same_v<decltype(a), const array<char, 2>>);
                static_assert(a.get<0>() == 'a');
                static_assert(a.get<1>() == 'b');
            }
            {
                constexpr auto a = to_array({"abc"});
                static_assert(std::is_same_v<decltype(a), const array<const char*, 1>>);
            }
            {
                constexpr auto a = to_array({"abc", "foobar"});
                static_assert(std::is_same_v<decltype(a), const array<const char*, 2>>);
            }
            {
                str strings[] = {"ABC", "abcdefghijklmnopqrstuvwxyz", "123"};
                static_assert(std::is_same_v<decltype(strings), str[3]>);

                auto a = to_array(strings); // Copy
                static_assert(std::is_same_v<decltype(a), array<str, 3>>);

                snn_require(a.get<0>() == "ABC");
                snn_require(a.get<1>() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(a.get<2>() == "123");

                // The original array has not been modified.
                snn_require(strings[0] == "ABC");
                snn_require(strings[1] == "abcdefghijklmnopqrstuvwxyz");
                snn_require(strings[2] == "123");
            }
            {
                str strings[] = {"ABC", "abcdefghijklmnopqrstuvwxyz", "123"};
                static_assert(std::is_same_v<decltype(strings), str[3]>);

                auto a = to_array(std::move(strings));
                static_assert(std::is_same_v<decltype(a), array<str, 3>>);

                snn_require(a.get<0>() == "ABC");
                snn_require(a.get<1>() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(a.get<2>() == "123");

                snn_require(strings[0] == ""); // Moved
                snn_require(strings[1] == ""); // Moved
                snn_require(strings[2] == ""); // Moved
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
        snn_static_require(app::test_array());

        static_assert(detail::can_overflow_count<i8, 256>());
        static_assert(detail::can_overflow_count<i16, 256>());
        static_assert(detail::can_overflow_count<i32, 256>());
        static_assert(detail::can_overflow_count<i64, 256>());
        static_assert(detail::can_overflow_count<u8, 255>());
        static_assert(!detail::can_overflow_count<u8, 256>());
        static_assert(detail::can_overflow_count<u16, 256>());
        static_assert(detail::can_overflow_count<u32, 256>());
        static_assert(detail::can_overflow_count<u64, 256>());
    }
}
