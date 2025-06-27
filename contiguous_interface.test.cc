// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/contiguous_interface.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/algo/remove_consecutive_duplicates.hh"
#include "snn-core/algo/remove_if.hh"
#include "snn-core/algo/sort.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/ascii/fn/equal_to_icase.hh"
#include "snn-core/ascii/fn/less_than_icase.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            str s = "One Two 3";

            snn_require(s.has_front('O'));
            snn_require(s.has_front("One"));
            snn_require(s.has_back('3'));

            snn_require(s.view_offset(4, -2) == "Two");

            snn_require(s.contains("Two"));

            snn_require(s.count_if(chr::is_alpha_upper) == 2);

            snn_require(s.find('T').value_or_npos() == 4);
            snn_require(s.find_if(chr::is_alpha_upper).value_or_npos() == 0);

            snn_require(s.find_in_reverse(' ').value_or_npos() == 7);
            snn_require(s.find_in_reverse_if(chr::is_alpha_upper).value_or_npos() == 4);

            snn_require(s.all(chr::is_ascii));

            auto v = s.view_exactly<0, 3>(assume::within_bounds);
            static_assert(std::is_same_v<decltype(v), array_view<char, 3>>);
            snn_require(v == "One");

            snn_require(s.replace_if(chr::is_alpha, '0'));
            snn_require(s == "000 000 3");

            s.remove_consecutive_duplicates();
            snn_require(s == "0 0 3");

            s.sort();
            snn_require(s == "  003");

            return true;
        }

        constexpr bool test_count_if()
        {
            {
                cstrview s;
                snn_require(s.count_if(chr::is_digit) == 0);
            }
            {
                cstrview s = "aaB123";
                snn_require(s.count_if(chr::is_alpha) == 3);
                snn_require(s.count_if(chr::is_alpha_lower) == 2);
                snn_require(s.count_if(chr::is_alpha_upper) == 1);
                snn_require(s.count_if(chr::is_digit) == 3);
                snn_require(s.count_if(chr::is_ascii_control) == 0);
                snn_require(s.count_if(chr::is_ascii_printable) == 6);
            }

            return true;
        }

        constexpr bool test_find_if()
        {
            {
                cstrview s;
                snn_require(!s.find_if(chr::is_digit));
                snn_require(s.find_if(chr::is_digit).value_or_npos() == constant::npos);
            }
            {
                cstrview s = "aaaBBB123";

                snn_require(s.find_if(chr::is_alpha_lower).value_or_npos() == 0);
                snn_require(s.find_if(chr::is_alpha_upper).value_or_npos() == 3);
                snn_require(s.find_if(chr::is_digit).value_or_npos() == 6);
                snn_require(s.find_if(chr::is_ascii_control).value_or_npos() == constant::npos);

                snn_require(s.find_if(chr::is_alpha_lower, 2).value_or_npos() == 2);
                snn_require(s.find_if(chr::is_alpha_lower, 3).value_or_npos() == constant::npos);

                snn_require(s.find_if(chr::is_digit, 8).value_or_npos() == 8);
                snn_require(s.find_if(chr::is_digit, 9).value_or_npos() == constant::npos);
            }

            return true;
        }

        constexpr bool test_find_in_reverse_if()
        {
            {
                cstrview s;
                snn_require(!s.find_in_reverse_if(chr::is_digit));
                snn_require(s.find_in_reverse_if(chr::is_digit).value_or_npos() == constant::npos);
            }
            {
                cstrview s = "aaaBBB123";

                snn_require(s.find_in_reverse_if(chr::is_alpha_lower));
                snn_require(s.find_in_reverse_if(chr::is_alpha_lower).value_or_npos() == 2);
                snn_require(s.find_in_reverse_if(chr::is_alpha_lower, 99).value_or_npos() == 2);
                snn_require(s.find_in_reverse_if(chr::is_alpha_lower, 3).value_or_npos() == 2);
                snn_require(s.find_in_reverse_if(chr::is_alpha_lower, 2).value_or_npos() == 2);
                snn_require(s.find_in_reverse_if(chr::is_alpha_lower, 1).value_or_npos() == 1);
                snn_require(s.find_in_reverse_if(chr::is_alpha_lower, 0));
                snn_require(s.find_in_reverse_if(chr::is_alpha_lower, 0).value_or_npos() == 0);

                snn_require(s.find_in_reverse_if(chr::is_digit).value_or_npos() == 8);
                snn_require(s.find_in_reverse_if(chr::is_digit, 99).value_or_npos() == 8);
                snn_require(s.find_in_reverse_if(chr::is_digit, 9).value_or_npos() == 8);
                snn_require(s.find_in_reverse_if(chr::is_digit, 8).value_or_npos() == 8);
                snn_require(s.find_in_reverse_if(chr::is_digit, 7).value_or_npos() == 7);
                snn_require(s.find_in_reverse_if(chr::is_digit, 6).value_or_npos() == 6);
                snn_require(s.find_in_reverse_if(chr::is_digit, 5).value_or_npos() ==
                            constant::npos);
            }

            return true;
        }

        constexpr bool test_is_sorted()
        {
            snn_require(cstrview{}.is_sorted());
            snn_require(cstrview{"abc"}.is_sorted());
            snn_require(!cstrview{"aBc"}.is_sorted());
            snn_require(cstrview{"aBc"}.is_sorted(ascii::fn::less_than_icase{}));

            return true;
        }

        constexpr bool test_remove()
        {
            array<int, 4> a{123, 456, 456, 789};

            auto v = a.view();

            snn_require(v.remove(99) == 0);
            snn_require(v.count() == 4);
            snn_require(algo::is_equal(v.range(), {123, 456, 456, 789}));

            snn_require(v.remove(456) == 2);
            snn_require(v.count() == 2);
            snn_require(algo::is_equal(v.range(), {123, 789}));

            snn_require(v.remove(123) == 1);
            snn_require(v.count() == 1);
            snn_require(algo::is_equal(v.range(), {789}));

            snn_require(v.remove(789) == 1);
            snn_require(v.is_empty());

            return true;
        }

        constexpr bool test_remove_consecutive_duplicates()
        {
            str s;

            snn_require(s.remove_consecutive_duplicates() == 0);
            snn_require(s == "");
            snn_require(s.size() == 0);

            s = "aaaAA1123";
            snn_require(s == "aaaAA1123");
            snn_require(s.size() == 9);

            snn_require(s.remove_consecutive_duplicates() == 4);
            snn_require(s == "aA123");
            snn_require(s.size() == 5);

            snn_require(s.remove_consecutive_duplicates() == 0);
            snn_require(s == "aA123");
            snn_require(s.size() == 5);

            snn_require(s.remove_consecutive_duplicates(ascii::fn::equal_to_icase{}) == 1);
            snn_require(s == "a123");
            snn_require(s.size() == 4);

            return true;
        }

        constexpr bool test_remove_if()
        {
            str s;

            snn_require(s.remove_if(chr::is_alpha_upper) == 0);
            snn_require(s.size() == 0);

            s = "aaabbbcccdddeeefff";
            snn_require(s == "aaabbbcccdddeeefff");
            snn_require(s.size() == 18);

            snn_require(s.remove_if(chr::is_alpha_upper) == 0);
            snn_require(s == "aaabbbcccdddeeefff");
            snn_require(s.size() == 18);

            s = "aaaBBBCCCDDDeeeFFF";
            snn_require(s == "aaaBBBCCCDDDeeeFFF");
            snn_require(s.size() == 18);

            snn_require(s.remove_if(chr::is_alpha_upper) == 12);
            snn_require(s == "aaaeee");
            snn_require(s.size() == 6);

            snn_require(s.remove_if(chr::is_alpha_lower) == 6);
            snn_require(s.size() == 0);

            return true;
        }

        constexpr bool test_replace()
        {
            array<int, 4> a{123, 456, 456, 789};
            snn_require(a.view().replace(456, -1) == 2);
            snn_require(algo::is_equal(a.range(), {123, -1, -1, 789}));
            snn_require(a.view().replace(123, 456) == 1);
            snn_require(algo::is_equal(a.range(), {456, -1, -1, 789}));
            snn_require(a.view().replace(123, 456) == 0);
            snn_require(algo::is_equal(a.range(), {456, -1, -1, 789}));

            return true;
        }

        constexpr bool test_replace_if()
        {
            array<int, 4> a{123, 0, 0, 789};
            snn_require(a.view().replace_if(fn::is_zero{}, -1) == 2);
            snn_require(algo::is_equal(a.range(), {123, -1, -1, 789}));
            snn_require(a.view().replace_if(fn::is{fn::equal_to{}, 123}, 456) == 1);
            snn_require(algo::is_equal(a.range(), {456, -1, -1, 789}));
            snn_require(a.view().replace_if(fn::is_zero{}, 999) == 0);
            snn_require(algo::is_equal(a.range(), {456, -1, -1, 789}));

            return true;
        }

        constexpr bool test_reverse()
        {
            str s;

            s.reverse();
            snn_require(s == "");
            snn_require(s.size() == 0);

            s = "a";
            s.reverse();
            snn_require(s == "a");
            snn_require(s.size() == 1);

            s = "ab";
            s.reverse();
            snn_require(s == "ba");
            snn_require(s.size() == 2);

            s = "abc";
            s.reverse();
            snn_require(s == "cba");
            snn_require(s.size() == 3);

            return true;
        }

        constexpr bool test_sort()
        {
            str s;

            s.sort();
            snn_require(s == "");
            snn_require(s.size() == 0);

            s = "a";
            s.sort();
            snn_require(s == "a");
            snn_require(s.size() == 1);

            s = "aB";
            s.sort();
            snn_require(s == "Ba");
            snn_require(s.size() == 2);

            s = "abC";
            s.sort();
            snn_require(s == "Cab");
            snn_require(s.size() == 3);

            s = "a832B98-";
            s.sort();
            snn_require(s == "-23889Ba");
            snn_require(s.size() == 8);
            s.sort(ascii::fn::less_than_icase{});
            snn_require(s == "-23889aB");
            snn_require(s.size() == 8);

            return true;
        }

        constexpr bool test_view_exactly()
        {
            str s;

            {
                auto v = s.view_exactly<0, 0>(assume::within_bounds);
                static_assert(std::is_same_v<decltype(v), array_view<char, 0>>);
                snn_require(v.begin() == s.begin());
            }
            {
                auto v = std::as_const(s).view_exactly<0, 0>(assume::within_bounds);
                static_assert(std::is_same_v<decltype(v), array_view<const char, 0>>);
                snn_require(v.begin() == s.begin());
            }

            s = "abcdef";

            {
                auto v = s.view_exactly<0, 3>(assume::within_bounds);
                static_assert(std::is_same_v<decltype(v), array_view<char, 3>>);
                snn_require(v == "abc");
            }
            {
                auto v = s.view_exactly<0, 6>(assume::within_bounds);
                static_assert(std::is_same_v<decltype(v), array_view<char, 6>>);
                snn_require(v == "abcdef");
            }
            {
                auto v = std::as_const(s).view_exactly<1, 4>(assume::within_bounds);
                static_assert(std::is_same_v<decltype(v), array_view<const char, 4>>);
                snn_require(v == "bcde");
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
        snn_static_require(app::test_count_if());
        snn_static_require(app::test_find_if());
        snn_static_require(app::test_find_in_reverse_if());
        snn_static_require(app::test_is_sorted());
        snn_static_require(app::test_remove());
        snn_static_require(app::test_remove_consecutive_duplicates());
        snn_static_require(app::test_remove_if());
        snn_static_require(app::test_replace());
        snn_static_require(app::test_replace_if());
        snn_static_require(app::test_reverse());
        snn_static_require(app::test_sort());
        snn_static_require(app::test_view_exactly());

        static_assert(std::is_base_of_v<contiguous_interface<str>, str>);
    }
}
