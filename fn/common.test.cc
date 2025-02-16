// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fn/common.hh"

#include "snn-core/strcore.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/ascii/fn/less_than_icase.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/env/argument.hh"
#include "snn-core/pair/common.hh"
#include "snn-core/range/generate.hh"
#include "snn-core/range/view/take.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(fn::all{fn::is_zero{}}(0));
            snn_require(fn::all{fn::is_zero{}}(0, 0));
            snn_require(!fn::all{fn::is_zero{}}(0, 0, 1));

            snn_require(fn::is{fn::equal_to{}, 123}(123));
            snn_require(fn::is{fn::equal_to{}, 123}(123, 123));
            snn_require(!fn::is{fn::equal_to{}, 123}(456));
            snn_require(!fn::is{fn::equal_to{}, 123}(123, 456));

            snn_require(fn::is{fn::less_than{}, 10}(9));
            snn_require(fn::is{fn::less_than{}, 10}(9, 3, 7));
            snn_require(!fn::is{fn::less_than{}, 10}(10));
            snn_require(!fn::is{fn::less_than{}, 10}(9, 3, 7, 10));

            snn_require(fn::_not{chr::is_alpha_lower}('A'));
            snn_require(!fn::_not{chr::is_alpha_lower}('a'));

            snn_require(fn::contains{"abc"}(cstrview{"abc"}, cstrview{"123abc"}));
            snn_require(!fn::contains{"abc"}(cstrview{"abc"}, cstrview{"123"}));

            snn_require(fn::is_empty{}(cstrview{}));
            snn_require(!fn::is_empty{}(cstrview{"abc"}));

            snn_require(fn::in_array{'a', 'b', 'c'}('a'));
            snn_require(fn::in_array{'a', 'b', 'c'}('c', 'b'));
            snn_require(!fn::in_array{'a', 'b', 'c'}('c', 'b', 'd'));

            snn_require(fn::in_interval{0, 3}(2));
            snn_require(!fn::in_interval{0, 3}(3));

            const auto is_digit_but_not_four = fn::is_all_of{chr::is_digit, fn::is{fn::ne{}, '4'}};
            snn_require(is_digit_but_not_four('3'));
            snn_require(is_digit_but_not_four('7'));
            snn_require(!is_digit_but_not_four('4'));
            snn_require(!is_digit_but_not_four('a'));

            const auto multiply_by_two = fn::bind{fn::multiply{}, 2};
            snn_require(multiply_by_two(3) == 6);
            snn_require(multiply_by_two(6) == 12);

            return true;
        }

        constexpr bool test_arithmetic()
        {
            snn_require(fn::add{}(1, 1) == 2);
            snn_require(fn::add{}(3, 5) == 8);
            snn_require(fn::add{}(5, 3) == 8);
            snn_require(fn::add{}(15, 3) == 18);

            snn_require(fn::divide{}(1, 1) == 1);
            snn_require(fn::divide{}(3, 5) == 0);
            snn_require(fn::divide{}(5, 3) == 1);
            snn_require(fn::divide{}(15, 3) == 5);

            snn_require(fn::modulo{}(1, 1) == 0);
            snn_require(fn::modulo{}(3, 5) == 3);
            snn_require(fn::modulo{}(5, 3) == 2);
            snn_require(fn::modulo{}(15, 3) == 0);

            snn_require(fn::multiply{}(1, 1) == 1);
            snn_require(fn::multiply{}(3, 5) == 15);
            snn_require(fn::multiply{}(5, 3) == 15);
            snn_require(fn::multiply{}(15, 3) == 45);

            snn_require(fn::negate{}(-3) == 3);
            snn_require(fn::negate{}(-1) == 1);
            snn_require(fn::negate{}(0) == 0);
            snn_require(fn::negate{}(1) == -1);
            snn_require(fn::negate{}(3) == -3);

            snn_require(fn::subtract{}(1, 1) == 0);
            snn_require(fn::subtract{}(3, 5) == -2);
            snn_require(fn::subtract{}(5, 3) == 2);
            snn_require(fn::subtract{}(15, 3) == 12);

            return true;
        }

        constexpr bool test_comparison()
        {
            snn_require(fn::equal_to{}(1, 1));
            snn_require(!fn::equal_to{}(1, 2));
            snn_require(!fn::equal_to{}(2, 1));
            static_assert(std::is_same_v<fn::equal_to, fn::eq>);

            snn_require(fn::greater_than{}(2, 1));
            snn_require(!fn::greater_than{}(1, 1));
            snn_require(!fn::greater_than{}(1, 2));
            static_assert(std::is_same_v<fn::greater_than, fn::gt>);

            snn_require(fn::greater_than_or_equal_to{}(1, 1));
            snn_require(fn::greater_than_or_equal_to{}(2, 1));
            snn_require(!fn::greater_than_or_equal_to{}(1, 2));
            static_assert(std::is_same_v<fn::greater_than_or_equal_to, fn::gte>);

            snn_require(fn::less_than{}(1, 2));
            snn_require(!fn::less_than{}(1, 1));
            snn_require(!fn::less_than{}(2, 1));
            static_assert(std::is_same_v<fn::less_than, fn::lt>);

            // `fn::less_than` with pointers (uses `std::less<void>` internally).
            array a{'a', 'b', 'c'};
            snn_require(fn::less_than{}(a.begin(), a.end()));
            snn_require(!fn::less_than{}(a.begin(), a.begin()));
            snn_require(!fn::less_than{}(a.end(), a.begin()));

            snn_require(fn::less_than_or_equal_to{}(1, 1));
            snn_require(fn::less_than_or_equal_to{}(1, 2));
            snn_require(!fn::less_than_or_equal_to{}(2, 1));
            static_assert(std::is_same_v<fn::less_than_or_equal_to, fn::lte>);

            snn_require(fn::not_equal_to{}(1, 2));
            snn_require(fn::not_equal_to{}(2, 1));
            snn_require(!fn::not_equal_to{}(1, 1));
            static_assert(std::is_same_v<fn::not_equal_to, fn::ne>);

            return true;
        }

        constexpr bool test_incrementing()
        {
            fn::incrementing gen{0};
            static_assert(std::is_same_v<decltype(gen), fn::incrementing<int>>);
            static_assert(std::is_same_v<decltype(gen()), int>);
            snn_require(gen() == 0);
            snn_require(gen() == 1);
            snn_require(gen() == 2);

            snn_require(algo::is_equal(range::generate{fn::incrementing{3}} | range::v::take{4},
                                       {3, 4, 5, 6}));
            snn_require(algo::is_equal(range::generate{fn::incrementing{'a'}} | range::v::take{3},
                                       {'a', 'b', 'c'}));

            return true;
        }

        constexpr bool test_decrementing()
        {
            fn::decrementing gen{0};
            static_assert(std::is_same_v<decltype(gen), fn::decrementing<int>>);
            static_assert(std::is_same_v<decltype(gen()), int>);
            snn_require(gen() == 0);
            snn_require(gen() == -1);
            snn_require(gen() == -2);

            snn_require(algo::is_equal(range::generate{fn::decrementing{3}} | range::v::take{4},
                                       {3, 2, 1, 0}));
            snn_require(algo::is_equal(range::generate{fn::decrementing{'c'}} | range::v::take{3},
                                       {'c', 'b', 'a'}));

            return true;
        }

        constexpr bool test_min()
        {
            {
                constexpr int a = 123;
                constexpr int b = 456;
                static_assert(fn::min{}(a, b) == 123);
            }
            {
                int a              = 123;
                int b              = 456;
                decltype(auto) min = fn::min{}(a, b);
                static_assert(std::is_same_v<decltype(min), int&>);
                snn_require(min == 123);
                snn_require(std::addressof(min) == std::addressof(a));
            }
            {
                const int a        = 123;
                int b              = 456;
                decltype(auto) min = fn::min{}(b, a);
                static_assert(std::is_same_v<decltype(min), const int&>);
                snn_require(min == 123);
                snn_require(std::addressof(min) == std::addressof(a));
            }

            return true;
        }

        constexpr bool test_max()
        {
            {
                constexpr int a = 123;
                constexpr int b = 456;
                static_assert(fn::max{}(a, b) == 456);
            }
            {
                int a              = 123;
                int b              = 456;
                decltype(auto) max = fn::max{}(a, b);
                static_assert(std::is_same_v<decltype(max), int&>);
                snn_require(max == 456);
                snn_require(std::addressof(max) == std::addressof(b));
            }
            {
                const int a        = 123;
                int b              = 456;
                decltype(auto) max = fn::max{}(b, a);
                static_assert(std::is_same_v<decltype(max), const int&>);
                snn_require(max == 456);
                snn_require(std::addressof(max) == std::addressof(b));
            }

            return true;
        }

        constexpr bool test_all()
        {
            auto all_is_zero = fn::all{fn::is_zero{}};
            snn_require(all_is_zero(0));
            snn_require(all_is_zero(0, 0));
            snn_require(!all_is_zero(1));
            snn_require(!all_is_zero(1, 1));
            snn_require(!all_is_zero(1, 0));
            snn_require(!all_is_zero(0, 1));

            return true;
        }

        constexpr bool test_any()
        {
            auto any_is_zero = fn::any{fn::is_zero{}};
            snn_require(any_is_zero(0));
            snn_require(any_is_zero(0, 0));
            snn_require(!any_is_zero(1));
            snn_require(!any_is_zero(1, 1));
            snn_require(any_is_zero(1, 0));
            snn_require(any_is_zero(0, 1));

            return true;
        }

        constexpr bool test_none()
        {
            auto none_is_zero = fn::none{fn::is_zero{}};
            snn_require(!none_is_zero(0));
            snn_require(!none_is_zero(0, 0));
            snn_require(none_is_zero(1));
            snn_require(none_is_zero(1, 1));
            snn_require(!none_is_zero(1, 0));
            snn_require(!none_is_zero(0, 1));

            return true;
        }

        constexpr bool test_contains()
        {
            {
                snn_require(fn::contains{"abc"}(cstrview{"abc"}));
                snn_require(fn::contains{"abc"}(cstrview{"_abc_"}));
                snn_require(!fn::contains{"abc"}(cstrview{"ABC"}));

                snn_require(fn::contains{"abc"}(cstrview{"abc"}, cstrview{"123abc"}));
                snn_require(!fn::contains{"abc"}(cstrview{"123"}, cstrview{"123abc"}));
                snn_require(!fn::contains{"abc"}(cstrview{"abc"}, cstrview{"123"}));
            }
            {
                auto f = fn::contains{123};
                static_assert(std::is_same_v<decltype(f), fn::contains<int>>);

                auto cpy = fn::contains{f}; // Copying is preferred over wrapping.
                static_assert(std::is_same_v<decltype(cpy), fn::contains<int>>);
            }
            {
                auto f = fn::contains{"abc"};
                static_assert(std::is_same_v<decltype(f), fn::contains<const char (&)[4]>>);
            }
            {
                int i{};
                auto f = fn::contains{i};
                static_assert(std::is_same_v<decltype(f), fn::contains<int&>>);
            }
            {
                str s;
                auto f = fn::contains{s};
                static_assert(std::is_same_v<decltype(f), fn::contains<str&>>);
            }
            {
                const str s;
                auto f = fn::contains{s};
                static_assert(std::is_same_v<decltype(f), fn::contains<const str&>>);
            }
            {
                str s;
                auto f = fn::contains{std::move(s)};
                static_assert(std::is_same_v<decltype(f), fn::contains<str>>);
            }

            return true;
        }

        constexpr bool test_has_back()
        {
            {
                snn_require(fn::has_back{"abc"}(cstrview{"abc"}));
                snn_require(fn::has_back{"abc"}(cstrview{"123abc"}));
                snn_require(!fn::has_back{"abc"}(cstrview{"abcdef"}));
                snn_require(!fn::has_back{"abc"}(cstrview{"ABC"}));

                snn_require(fn::has_back{"abc"}(cstrview{"abc"}, cstrview{"123abc"}));
                snn_require(!fn::has_back{"abc"}(cstrview{"123"}, cstrview{"abcdef"}));
                snn_require(!fn::has_back{"abc"}(cstrview{"abc"}, cstrview{"123"}));
            }
            {
                auto f = fn::has_back{123};
                static_assert(std::is_same_v<decltype(f), fn::has_back<int>>);

                auto cpy = fn::has_back{f}; // Copying is preferred over wrapping.
                static_assert(std::is_same_v<decltype(cpy), fn::has_back<int>>);
            }
            {
                auto f = fn::has_back{"abc"};
                static_assert(std::is_same_v<decltype(f), fn::has_back<const char (&)[4]>>);
            }
            {
                int i{};
                auto f = fn::has_back{i};
                static_assert(std::is_same_v<decltype(f), fn::has_back<int&>>);
            }
            {
                str s;
                auto f = fn::has_back{s};
                static_assert(std::is_same_v<decltype(f), fn::has_back<str&>>);
            }
            {
                const str s;
                auto f = fn::has_back{s};
                static_assert(std::is_same_v<decltype(f), fn::has_back<const str&>>);
            }
            {
                str s;
                auto f = fn::has_back{std::move(s)};
                static_assert(std::is_same_v<decltype(f), fn::has_back<str>>);
            }

            return true;
        }

        constexpr bool test_has_front()
        {
            {
                snn_require(fn::has_front{"abc"}(cstrview{"abc"}));
                snn_require(fn::has_front{"abc"}(cstrview{"abcdef"}));
                snn_require(!fn::has_front{"abc"}(cstrview{"123abc"}));
                snn_require(!fn::has_front{"abc"}(cstrview{"ABC"}));

                snn_require(fn::has_front{"abc"}(cstrview{"abc"}, cstrview{"abcdef"}));
                snn_require(!fn::has_front{"abc"}(cstrview{"123"}, cstrview{"123abc"}));
                snn_require(!fn::has_front{"abc"}(cstrview{"abc"}, cstrview{"123"}));
            }
            {
                auto f = fn::has_front{123};
                static_assert(std::is_same_v<decltype(f), fn::has_front<int>>);

                auto cpy = fn::has_front{f}; // Copying is preferred over wrapping.
                static_assert(std::is_same_v<decltype(cpy), fn::has_front<int>>);
            }
            {
                auto f = fn::has_front{"abc"};
                static_assert(std::is_same_v<decltype(f), fn::has_front<const char (&)[4]>>);
            }
            {
                int i{};
                auto f = fn::has_front{i};
                static_assert(std::is_same_v<decltype(f), fn::has_front<int&>>);
            }
            {
                str s;
                auto f = fn::has_front{s};
                static_assert(std::is_same_v<decltype(f), fn::has_front<str&>>);
            }
            {
                const str s;
                auto f = fn::has_front{s};
                static_assert(std::is_same_v<decltype(f), fn::has_front<const str&>>);
            }
            {
                str s;
                auto f = fn::has_front{std::move(s)};
                static_assert(std::is_same_v<decltype(f), fn::has_front<str>>);
            }

            return true;
        }

        constexpr bool test_is()
        {
            {
                auto is_less_than_ten = fn::is{fn::less_than{}, 10};

                snn_require(is_less_than_ten(9));
                snn_require(!is_less_than_ten(10));

                int a = 3;
                int b = 7;
                int c = 10;
                snn_require(is_less_than_ten(a));
                snn_require(is_less_than_ten(b));
                snn_require(is_less_than_ten(a, b));
                snn_require(is_less_than_ten(b, a));
                snn_require(is_less_than_ten(a, a));
                snn_require(!is_less_than_ten(c));
                snn_require(!is_less_than_ten(a, c));
                snn_require(!is_less_than_ten(c, a));
                snn_require(!is_less_than_ten(c, c));
                snn_require(!is_less_than_ten(a, b, c));
            }
            // Deduction
            {
                auto f = fn::is{fn::equal_to{}, 123};
                static_assert(std::is_same_v<decltype(f), fn::is<fn::equal_to, int>>);

                auto cpy = fn::is{f}; // Copying is preferred over wrapping.
                static_assert(std::is_same_v<decltype(cpy), fn::is<fn::equal_to, int>>);
            }
            {
                auto f = fn::is{fn::equal_to{}, "abc"};
                static_assert(std::is_same_v<decltype(f), fn::is<fn::equal_to, const char (&)[4]>>);
            }
            {
                fn::equal_to eq{};
                int i{};
                auto f = fn::is{eq, i};
                static_assert(std::is_same_v<decltype(f), fn::is<fn::equal_to, int&>>);
            }
            {
                const fn::equal_to eq{};
                const str s;
                auto f = fn::is{eq, s};
                static_assert(std::is_same_v<decltype(f), fn::is<fn::equal_to, const str&>>);
            }
            {
                fn::equal_to eq{};
                str s;
                auto f = fn::is{std::move(eq), std::move(s)};
                static_assert(std::is_same_v<decltype(f), fn::is<fn::equal_to, str>>);
            }

            return true;
        }

        constexpr bool test_is_empty()
        {
            fn::is_empty is_empty;

            cstrview a{};
            cstrview b{};
            cstrview c{"123"};

            snn_require(is_empty(a));
            snn_require(is_empty(a, b));
            snn_require(is_empty(b, a));
            snn_require(is_empty(a, a));
            snn_require(!is_empty(c));
            snn_require(!is_empty(a, c));
            snn_require(!is_empty(c, a));
            snn_require(!is_empty(c, c));
            snn_require(!is_empty(a, b, c));

            return true;
        }

        constexpr bool test_not()
        {
            snn_require(fn::_not{chr::is_alpha_lower}('A'));
            snn_require(!fn::_not{chr::is_alpha_lower}('a'));

            return true;
        }

        constexpr bool test_is_zero()
        {
            int a = 0;
            int b = 0;
            int c = 123;
            snn_require(fn::is_zero{}(a));
            snn_require(fn::is_zero{}(b));
            snn_require(fn::is_zero{}(a, b));
            snn_require(fn::is_zero{}(b, a));
            snn_require(fn::is_zero{}(a, a));
            snn_require(!fn::is_zero{}(c));
            snn_require(!fn::is_zero{}(a, c));
            snn_require(!fn::is_zero{}(c, a));
            snn_require(!fn::is_zero{}(c, c));
            snn_require(!fn::is_zero{}(a, b, c));

            return true;
        }

        constexpr bool test_in_array()
        {
            {
                static_assert(fn::in_array{'a', 'b'}('a'));
                static_assert(fn::in_array{'a', 'b'}('b'));
                static_assert(!fn::in_array{'a', 'b'}('c'));
                static_assert(!fn::in_array{'a', 'b'}('A'));

                static_assert(fn::in_array{'a'}('a'));
                static_assert(!fn::in_array{'a'}('b'));

                static_assert(fn::in_array{'\r', '\n'}('\r'));
                static_assert(fn::in_array{'\r', '\n'}('\n'));
                static_assert(!fn::in_array{'\r', '\n'}('\t'));

                // Multiple values.
                static_assert(fn::in_array{'a', 'b'}('a', 'b'));
                static_assert(fn::in_array{'a', 'b'}('b', 'a'));
                static_assert(fn::in_array{'a', 'b'}('a', 'a'));
                static_assert(fn::in_array{'a', 'b'}('b', 'b'));
                static_assert(!fn::in_array{'a', 'b'}('a', 'c'));
                static_assert(!fn::in_array{'a', 'b'}('c', 'a'));
                static_assert(!fn::in_array{'a', 'b'}('c', 'c'));
                static_assert(!fn::in_array{'a', 'b'}('a', 'b', 'c'));

                snn_require(fn::in_array{'a', 'b'}('b', 'b'));
                snn_require(!fn::in_array{'a', 'b'}('a', 'c'));

                // static_assert(!fn::in_array{}('a'));          // Will not compile.
                // static_assert(!fn::in_array<char, 0>{}('a')); // Will not compile.
            }
            {
                // String literals are deduced to cstrview.
                auto is_abc_or_def = fn::in_array{"abc", "def"};
                static_assert(std::is_same_v<decltype(is_abc_or_def), fn::in_array<cstrview, 2>>);
                snn_require(is_abc_or_def("abc"));
                snn_require(is_abc_or_def("def"));
                snn_require(!is_abc_or_def(""));
                snn_require(!is_abc_or_def("ab"));
                snn_require(!is_abc_or_def("abcd"));
                snn_require(!is_abc_or_def("ABC"));
                snn_require(!is_abc_or_def("xyz"));
            }
            {
                auto f = fn::in_array{'a'};
                static_assert(std::is_same_v<decltype(f), fn::in_array<char, 1>>);

                auto cpy = fn::in_array{f}; // Copying is preferred over wrapping.
                static_assert(std::is_same_v<decltype(cpy), fn::in_array<char, 1>>);
            }
            {
                auto f = fn::in_array{'a', 'b'};
                static_assert(std::is_same_v<decltype(f), fn::in_array<char, 2>>);
            }
            {
                const char c{};
                auto f = fn::in_array{c};
                static_assert(std::is_same_v<decltype(f), fn::in_array<char, 1>>);
            }
            {
                str s;
                auto f = fn::in_array{std::move(s)};
                static_assert(std::is_same_v<decltype(f), fn::in_array<str, 1>>);
            }

            return true;
        }

        constexpr bool test_in_interval()
        {
            {
                static_assert(fn::in_interval{0, 3}(0));
                static_assert(fn::in_interval{0, 3}(1));
                static_assert(fn::in_interval{0, 3}(2));
                static_assert(!fn::in_interval{0, 3}(3));
                static_assert(!fn::in_interval{0, 3}(4));
                static_assert(!fn::in_interval{0, 3}(-1));

                // Multiple values.
                static_assert(fn::in_interval{0, 3}(0, 2));
                static_assert(fn::in_interval{0, 3}(1, 0, 2));
                static_assert(!fn::in_interval{0, 3}(1, 3));
                static_assert(!fn::in_interval{0, 3}(3, 1));
                static_assert(!fn::in_interval{0, 3}(3, 3));
                static_assert(!fn::in_interval{0, 3}(1, 2, 3));

                snn_require(fn::in_interval{0, 3}(1, 0, 2));
                snn_require(!fn::in_interval{0, 3}(1, 3));

                // Custom predicate (less-than comparison).
                static_assert(fn::in_interval{'a', 'z'}('a'));
                static_assert(!fn::in_interval{'a', 'z'}('A'));
                static_assert(fn::in_interval{'a', 'z', ascii::fn::less_than_icase{}}('A'));
            }
            {
                auto f = fn::in_interval{0, 3};
                static_assert(std::is_same_v<decltype(f), fn::in_interval<int, int>>);
            }
            {
                auto f = fn::in_interval<u8>{0, 3};
                static_assert(std::is_same_v<decltype(f), fn::in_interval<u8, u8>>);
            }
            {
                const int i{};
                auto f = fn::in_interval{i, 123};
                static_assert(std::is_same_v<decltype(f), fn::in_interval<const int&, int>>);
            }
            {
                str s1;
                str s2;
                auto f = fn::in_interval{std::move(s1), s2};
                static_assert(std::is_same_v<decltype(f), fn::in_interval<str, str&>>);
            }
            {
                auto f = fn::in_interval{'a', 'z', ascii::fn::less_than_icase{}};
                static_assert(
                    std::is_same_v<decltype(f),
                                   fn::in_interval<char, char, ascii::fn::less_than_icase>>);
            }
            {
                char lower   = 'a';
                char upper   = 'z';
                auto is_less = ascii::fn::less_than_icase{};
                auto f       = fn::in_interval{lower, upper, is_less};
                // Predicate is always passed by value.
                static_assert(
                    std::is_same_v<decltype(f),
                                   fn::in_interval<char&, char&, ascii::fn::less_than_icase>>);
            }

            return true;
        }

        constexpr bool test_in_interval_incl()
        {
            {
                static_assert(fn::in_interval_incl{0, 3}(0));
                static_assert(fn::in_interval_incl{0, 3}(1));
                static_assert(fn::in_interval_incl{0, 3}(2));
                static_assert(fn::in_interval_incl{0, 3}(3));
                static_assert(!fn::in_interval_incl{0, 3}(4));
                static_assert(!fn::in_interval_incl{0, 3}(-1));

                // Multiple values.
                static_assert(fn::in_interval_incl{0, 3}(0, 3));
                static_assert(fn::in_interval_incl{0, 3}(1, 0, 3));
                static_assert(!fn::in_interval_incl{0, 3}(1, 4));
                static_assert(!fn::in_interval_incl{0, 3}(4, 1));
                static_assert(!fn::in_interval_incl{0, 3}(4, 4));
                static_assert(!fn::in_interval_incl{0, 3}(1, 2, 4));

                snn_require(fn::in_interval_incl{0, 3}(1, 0, 3));
                snn_require(!fn::in_interval_incl{0, 3}(1, 4));

                // Custom predicate (less-than comparison).
                static_assert(fn::in_interval_incl{'a', 'z'}('z'));
                static_assert(!fn::in_interval_incl{'a', 'z'}('Z'));
                static_assert(fn::in_interval_incl{'a', 'z', ascii::fn::less_than_icase{}}('Z'));
            }
            {
                auto f = fn::in_interval_incl{0, 3};
                static_assert(std::is_same_v<decltype(f), fn::in_interval_incl<int, int>>);
            }
            {
                auto f = fn::in_interval_incl<u8>{0, 3};
                static_assert(std::is_same_v<decltype(f), fn::in_interval_incl<u8, u8>>);
            }
            {
                const int i{};
                auto f = fn::in_interval_incl{i, 123};
                static_assert(std::is_same_v<decltype(f), fn::in_interval_incl<const int&, int>>);
            }
            {
                str s1;
                str s2;
                auto f = fn::in_interval_incl{std::move(s1), s2};
                static_assert(std::is_same_v<decltype(f), fn::in_interval_incl<str, str&>>);
            }
            {
                auto f = fn::in_interval_incl{'a', 'z', ascii::fn::less_than_icase{}};
                static_assert(
                    std::is_same_v<decltype(f),
                                   fn::in_interval_incl<char, char, ascii::fn::less_than_icase>>);
            }
            {
                char lower   = 'a';
                char upper   = 'z';
                auto is_less = ascii::fn::less_than_icase{};
                auto f       = fn::in_interval_incl{lower, upper, is_less};
                // Predicate is always passed by value.
                static_assert(
                    std::is_same_v<decltype(f),
                                   fn::in_interval_incl<char&, char&, ascii::fn::less_than_icase>>);
            }

            return true;
        }

        constexpr bool test_in_tuple()
        {
            {
                static_assert(fn::in_tuple{"abcd", "1234"}(cstrview{"abcd"}));
                static_assert(fn::in_tuple{"abcd", "1234"}(cstrview{"1234"}));
                static_assert(!fn::in_tuple{"abcd", "1234"}(cstrview{"aBCd"}));

                snn_require(fn::in_tuple{"abcd"}(cstrview{"abcd"}));
                snn_require(!fn::in_tuple{"abcd"}(cstrview{"aBCd"}));

                snn_require(!fn::in_tuple{}(cstrview{"abcd"}));

                // Multiple values.

                const i64 i  = 123;
                const auto f = fn::in_tuple{i, 456};
                static_assert(std::is_same_v<decltype(f), const fn::in_tuple<const i64&, int>>);

                snn_require(f(123));
                snn_require(f(123, 456));
                snn_require(f(456, 123));
                snn_require(f(123, 123));
                snn_require(!f(789));
                snn_require(!f(123, 789));
                snn_require(!f(789, 123));
                snn_require(!f(789, 789));
                snn_require(!f(123, 456, 789));
            }
            {
                const char a{};
                str s1;
                str s2;
                auto f = fn::in_tuple{a, 'x', "abcd", std::move(s1), 123, s2};
                static_assert(
                    std::is_same_v<decltype(f), fn::in_tuple<const char&, char, const char (&)[5],
                                                             str, int, str&>>);

                auto cpy = fn::in_tuple{f}; // Copying is preferred over wrapping.
                static_assert(
                    std::is_same_v<decltype(cpy), fn::in_tuple<const char&, char, const char (&)[5],
                                                               str, int, str&>>);
            }

            return true;
        }

        constexpr bool test_is_all_of()
        {
            {
                auto is_digit_but_not_four = fn::is_all_of{chr::is_digit, fn::is{fn::ne{}, '4'}};

                snn_require(is_digit_but_not_four('0'));
                snn_require(is_digit_but_not_four('2'));
                snn_require(is_digit_but_not_four('9'));

                snn_require(!is_digit_but_not_four('a'));
                snn_require(!is_digit_but_not_four('4'));
                snn_require(!is_digit_but_not_four('.'));

                // Multiple values.

                snn_require(is_digit_but_not_four('1', '3', '7'));

                snn_require(!is_digit_but_not_four('1', '4'));
                snn_require(!is_digit_but_not_four('4', '1'));
                snn_require(!is_digit_but_not_four('4', 'a'));
                snn_require(!is_digit_but_not_four('1', 'a'));
                snn_require(!is_digit_but_not_four('1', '3', '4'));
            }
            {
                auto f = fn::is_all_of{chr::is_digit, fn::is{fn::ne{}, '4'}};
                static_assert(
                    std::is_same_v<decltype(f),
                                   fn::is_all_of<bool (*)(char) noexcept, fn::is<fn::ne, char>>>);

                auto cpy = fn::is_all_of{f}; // Copying is preferred over wrapping.
                static_assert(
                    std::is_same_v<decltype(cpy),
                                   fn::is_all_of<bool (*)(char) noexcept, fn::is<fn::ne, char>>>);
            }

            return true;
        }

        constexpr bool test_is_any_of()
        {
            {
                auto is_digit_or_plus_minus = fn::is_any_of{chr::is_digit, fn::in_array{'-', '+'}};

                snn_require(is_digit_or_plus_minus('0'));
                snn_require(is_digit_or_plus_minus('3'));
                snn_require(is_digit_or_plus_minus('9'));
                snn_require(is_digit_or_plus_minus('+'));
                snn_require(is_digit_or_plus_minus('-'));

                snn_require(!is_digit_or_plus_minus('x'));
                snn_require(!is_digit_or_plus_minus('/'));

                // Multiple values.

                snn_require(is_digit_or_plus_minus('-', '3'));

                snn_require(!is_digit_or_plus_minus('-', 'x'));
                snn_require(!is_digit_or_plus_minus('x', '-'));
                snn_require(!is_digit_or_plus_minus('x', 'x'));
                snn_require(!is_digit_or_plus_minus('-', '3', 'x'));
            }
            {
                auto f = fn::is_any_of{chr::is_digit, fn::in_array{'-', '+'}};
                static_assert(
                    std::is_same_v<decltype(f),
                                   fn::is_any_of<bool (*)(char) noexcept, fn::in_array<char, 2>>>);

                auto cpy = fn::is_any_of{f}; // Copying is preferred over wrapping.
                static_assert(
                    std::is_same_v<decltype(cpy),
                                   fn::is_any_of<bool (*)(char) noexcept, fn::in_array<char, 2>>>);
            }

            return true;
        }

        constexpr bool test_is_none_of()
        {
            {
                auto is_not_a_digit_or_plus_minus =
                    fn::is_none_of{chr::is_digit, fn::in_array{'-', '+'}};

                snn_require(is_not_a_digit_or_plus_minus('a'));
                snn_require(is_not_a_digit_or_plus_minus('B'));
                snn_require(is_not_a_digit_or_plus_minus('/'));

                snn_require(!is_not_a_digit_or_plus_minus('3'));
                snn_require(!is_not_a_digit_or_plus_minus('+'));

                // Multiple values.

                snn_require(is_not_a_digit_or_plus_minus('a', '/'));

                snn_require(!is_not_a_digit_or_plus_minus('-', 'a'));
                snn_require(!is_not_a_digit_or_plus_minus('a', '-'));
                snn_require(!is_not_a_digit_or_plus_minus('3', '7'));
                snn_require(!is_not_a_digit_or_plus_minus('a', 'b', '+'));
            }
            {
                auto f = fn::is_none_of{chr::is_digit, fn::in_array{'-', '+'}};
                static_assert(
                    std::is_same_v<decltype(f),
                                   fn::is_none_of<bool (*)(char) noexcept, fn::in_array<char, 2>>>);

                auto cpy = fn::is_none_of{f}; // Copying is preferred over wrapping.
                static_assert(
                    std::is_same_v<decltype(cpy),
                                   fn::is_none_of<bool (*)(char) noexcept, fn::in_array<char, 2>>>);
            }

            return true;
        }

        constexpr bool test_ret()
        {
            auto return_true = fn::ret{true};
            static_assert(std::is_same_v<decltype(return_true), fn::ret<bool>>);
            static_assert(std::is_same_v<decltype(return_true()), const bool&>);
            snn_require(return_true() == true);
            snn_require(return_true(123) == true);
            snn_require(return_true(123, "foo", "bar") == true);

            auto return_hello = fn::ret{"hello"};
            static_assert(std::is_same_v<decltype(return_hello), fn::ret<const char (&)[6]>>);
            static_assert(std::is_same_v<decltype(return_hello()), const char (&)[6]>);
            snn_require(return_hello() == cstrview{"hello"});
            snn_require(return_hello(123) == cstrview{"hello"});
            snn_require(return_hello(123, "foo", "bar") == cstrview{"hello"});

            // Deduction

            str s = "OneTwo";

            auto return_s = fn::ret{s};
            static_assert(std::is_same_v<decltype(return_s), fn::ret<str&>>);
            static_assert(std::is_same_v<decltype(return_s()), str&>);
            snn_require(return_s() == "OneTwo");

            auto return_const_s = fn::ret{std::as_const(s)};
            static_assert(std::is_same_v<decltype(return_const_s), fn::ret<const str&>>);
            static_assert(std::is_same_v<decltype(return_const_s()), const str&>);
            snn_require(return_const_s() == "OneTwo");

            auto return_str = fn::ret{std::move(s)};
            static_assert(std::is_same_v<decltype(return_str), fn::ret<str>>);
            static_assert(std::is_same_v<decltype(return_str()), const str&>);
            snn_require(return_str() == "OneTwo");

            return true;
        }

        constexpr bool test_stream_to()
        {
            str s;
            auto stream_append = fn::stream_to{s};

            stream_append('A');
            snn_require(s == "A");

            stream_append(as_num(123));
            snn_require(s == "A123");

            stream_append("def");
            snn_require(s == "A123def");

            decltype(auto) ref = stream_append("XYZ", as_num(456), '.');
            snn_require(s == "A123defXYZ456.");
            static_assert(std::is_same_v<decltype(ref), str&>);
            snn_require(std::addressof(ref) == std::addressof(s));

            return true;
        }

        constexpr bool test_as_const()
        {
            auto f = [](cstrview s) { return s.size(); };

            // This will not compile (cstrview doesn't bind to a temporary str):
            // snn_require(f(str{"abc"}) == 3);

            // This is perfectly safe, the temporary is valid until the end of the function call:
            snn_require(fn::as_const{f}(str{"abc"}) == 3);

            return true;
        }

        constexpr bool test_bind()
        {
            auto multiply_by_two = fn::bind{fn::multiply{}, 2};
            snn_require(multiply_by_two(3) == 6);
            snn_require(multiply_by_two(6) == 12);
            static_assert(std::is_same_v<decltype(multiply_by_two), fn::bind<fn::multiply, int>>);

            const int x = 5;
            auto add_x  = fn::bind{fn::add{}, x};
            snn_require(add_x(0) == 5);
            snn_require(add_x(2) == 7);
            static_assert(std::is_same_v<decltype(add_x), fn::bind<fn::add, const int&>>);

            const i8 y  = 9;
            auto add_xy = fn::bind{fn::add{}, x, y};
            snn_require(add_xy() == 14);
            static_assert(
                std::is_same_v<decltype(add_xy), fn::bind<fn::add, const int&, const i8&>>);

            return true;
        }

        constexpr bool test_element()
        {
            {
                auto is_less_first  = fn::element{meta::index<0>, fn::less_than{}};
                auto is_less_second = fn::element{meta::index<1>, fn::less_than{}};

                pair::first_second p1{99, 17};
                std::pair p2{98, 18};

                snn_require(!is_less_first(p1, p2));
                snn_require(is_less_second(p1, p2));
            }
            {
                str s;
                const auto append_first  = fn::element{meta::index<0>, fn::stream_to{s}};
                const auto append_second = fn::element{meta::index<1>, fn::stream_to{s}};

                pair::first_second<cstrview> p1{"abc", "ABC"};
                pair::first_second<cstrview> p2{"def", "DEF"};

                append_first(p1, p2);

                snn_require(s == "abcdef");

                append_second(p1);
                append_second(p2);

                snn_require(s == "abcdefABCDEF");
            }

            return true;
        }

        constexpr bool test_to()
        {
            str buf;

            fn::to{meta::type<cstrview>, fn::stream_to{buf}}(cstrview{"One"}, env::argument{"Two"});
            snn_require(buf == "OneTwo");

            fn::to{meta::type<cstrview>, fn::stream_to{buf}}(env::argument{"Three"});
            snn_require(buf == "OneTwoThree");

            fn::to{meta::type<numeric<int>>, fn::stream_to{buf}}(123);
            snn_require(buf == "OneTwoThree123");

            fn::to{meta::type<numeric<int>>, fn::stream_to{buf}}(456, 789);
            snn_require(buf == "OneTwoThree123456789");

            return true;
        }

        constexpr bool test_transparent()
        {
            fn::transparent transparent{};

            static_assert(std::is_same_v<decltype(transparent(int{})), int&&>);

            int i{};

            static_assert(std::is_same_v<decltype(transparent(i)), int&>);
            static_assert(std::is_same_v<decltype(transparent(std::move(i))), int&&>);

            const int j{};

            static_assert(std::is_same_v<decltype(transparent(j)), const int&>);
            static_assert(std::is_same_v<decltype(transparent(std::move(j))), const int&&>);

            return true;
        }

        constexpr bool test_invoke()
        {
            {
                fn::invoke clear_str{&str::clear};

                str s = "abc";
                snn_require(s == "abc");
                clear_str(s);
                snn_require(s == "");
            }
            {
                struct person
                {
                    int id;
                    cstrview name;
                    cstrview title;
                };

                constexpr person a{123, "foo", "abc"};
                constexpr person b{456, "bar", "def"};

                static_assert(fn::invoke{fn::less_than{}, &person::id}(a, b));
                static_assert(!fn::invoke{fn::less_than{}, &person::id}(b, a));

                static_assert(!fn::invoke{fn::less_than{}, &person::name}(a, b));
                static_assert(fn::invoke{fn::less_than{}, &person::name}(b, a));

                static_assert(fn::invoke{fn::less_than{}, &person::title, &person::name}(a, b));
                static_assert(!fn::invoke{fn::less_than{}, &person::name, &person::title}(a, b));
            }
            return true;
        }

        constexpr bool test_unpack()
        {
            pair::first_second p1{3, 5};
            pair::first_second p2{7, 7};

            snn_require(fn::unpack{fn::less_than{}}(p1));
            snn_require(!fn::unpack{fn::less_than{}}(p2));

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
        snn_static_require(app::test_comparison());
        snn_static_require(app::test_incrementing());
        snn_static_require(app::test_decrementing());
        snn_static_require(app::test_min());
        snn_static_require(app::test_max());
        snn_static_require(app::test_all());
        snn_static_require(app::test_any());
        snn_static_require(app::test_none());
        snn_static_require(app::test_contains());
        snn_static_require(app::test_has_back());
        snn_static_require(app::test_has_front());
        snn_static_require(app::test_is());
        snn_static_require(app::test_is_empty());
        snn_static_require(app::test_not());
        snn_static_require(app::test_is_zero());
        snn_static_require(app::test_in_array());
        snn_static_require(app::test_in_interval());
        snn_static_require(app::test_in_interval_incl());
        snn_static_require(app::test_in_tuple());
        snn_static_require(app::test_is_all_of());
        snn_static_require(app::test_is_any_of());
        snn_static_require(app::test_is_none_of());
        snn_static_require(app::test_ret());
        snn_static_require(app::test_stream_to());
        snn_static_require(app::test_as_const());
        snn_static_require(app::test_bind());
        snn_static_require(app::test_element());
        snn_static_require(app::test_to());
        snn_static_require(app::test_transparent());
        snn_static_require(app::test_invoke());
        snn_static_require(app::test_unpack());
    }
}
