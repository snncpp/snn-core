// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/strcore.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/sort.hh"
#include "snn-core/algo/remove_if.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/file/read.hh"
#include "snn-core/mem/raw/copy.hh"
#include "snn-core/mem/raw/optimal_size.hh"
#include "snn-core/random/number.hh"
#include "snn-core/random/string.hh"
#include <string_view> // string_view

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                str s;

                snn_require(!s); // Empty.
                snn_require(s.is_empty());
                snn_require(s.capacity() > 0);

                s.append("One");
                snn_require(s == "One");

                snn_require(s); // Not empty.
                snn_require(!s.is_empty());
                snn_require(s.count() == 3);
                snn_require(s.size() == 3);
                snn_require(s.byte_size().get() == 3);

                s << "Two" << as_num(333);
                snn_require(s == "OneTwo333");

                snn_require(s.at(0).value() == 'O');
                snn_require(s.at(1).value() == 'n');

                snn_require(s.front().value() == 'O');
                snn_require(s.back().value() == '3');

                snn_require(s.has_front('O'));
                snn_require(s.has_front("One"));
                snn_require(s.has_back("33"));

                snn_require(s.all(chr::is_alphanumeric));

                snn_require(s.view(3, 3) == "Two");
                snn_require(s.view_offset(-6) == "Two333");

                s.append_for_overwrite(10).fill("Four");
                snn_require(s == "OneTwo333FourFourFo");

                s.append_integral<math::base::binary>(5);
                snn_require(s == "OneTwo333FourFourFo101");

                snn_require(s.view_offset(-3) == "101");
                snn_require(s.view_offset(-3).to<int>().value() == 101);

                s.replace("Four", "FOUR");
                snn_require(s == "OneTwo333FOURFOURFo101");

                snn_require(s.count("FOUR") == 2);

                snn_require(s.find("333").value_or_npos() == 6);
                snn_require(s.find_if(chr::is_digit).value_or_npos() == 6);

                s.transform(chr::to_alpha_lower);
                snn_require(s == "onetwo333fourfourfo101");

                s.view(3, 3).transform(chr::to_alpha_upper);
                snn_require(s == "oneTWO333fourfourfo101");
            }
            {
                str s = concat("One", as_num(2), "Three", '4');
                snn_require(s == "One2Three4");
            }
            {
                str s{init::fill, 10, 'a'};
                snn_require(s == "aaaaaaaaaa");
            }

            return true;
        }

        template <typename T>
        class string_like final
        {
          public:
            using size_type = usize;

            constexpr explicit string_like(T* const data, usize size) noexcept
                : data_{data},
                  size_{size}
            {
            }

            [[nodiscard]] constexpr T* data() noexcept
            {
                return data_;
            }

            [[nodiscard]] constexpr const T* data() const noexcept
            {
                return data_;
            }

            [[nodiscard]] constexpr usize size() const noexcept
            {
                return size_;
            }

          private:
            T* data_;
            usize size_;
        };

        constexpr bool test_append_array()
        {
            str s;

            array<char, 0> zero;
            s.append(zero);
            snn_require(s.is_empty());

            array one{'a'};
            s.append(one);
            snn_require(s == "a");
            snn_require(s.size() == 1);

            array two{'A', 'Z'};
            s.append(two);
            snn_require(s == "aAZ");
            snn_require(s.size() == 3);

            array three{'1', '7', '2'};
            s.append(three);
            snn_require(s == "aAZ172");
            snn_require(s.size() == 6);

            return true;
        }

        constexpr bool test_stream_append_array()
        {
            str s;

            array<char, 0> zero;
            s << zero;
            snn_require(s.is_empty());

            array one{'a'};
            s << one;
            snn_require(s == "a");
            snn_require(s.size() == 1);

            array two{'A', 'Z'};
            array three{'1', '7', '2'};
            s << two << three;
            snn_require(s == "aAZ172");
            snn_require(s.size() == 6);

            return true;
        }

        constexpr bool test_concat()
        {
            snn_require(concat() == "");
            snn_require(concat("One") == "One");
            snn_require(concat("One", "Two") == "OneTwo");
            snn_require(concat("One", "Two", "Three") == "OneTwoThree");
            snn_require(concat("One", as_num(2), "Three") == "One2Three");

            str ref{"Reference"};
            snn_require(concat(ref, '&', str{"Temporary"}) == "Reference&Temporary");

            snn_require(concat<strbuf>("abcdefghijklmnopqrstuvwxyz", '&', as_num(123)) ==
                        "abcdefghijklmnopqrstuvwxyz&123");

            return true;
        }

        constexpr bool size_eq(const str& s, const usize size)
        {
            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            snn_require(s.data().get() != nullptr);
            if (s.size() == size && s.count() == size && s.byte_size().get() == size &&
                s.capacity() >= size && s.data().get()[size] == 0)
            {
                if ((size == 0 && s.is_empty() && s == "" && !s) ||
                    (size > 0 && !s.is_empty() && s != "" && s))
                {
                    return true;
                }
            }
            return false;

            SNN_DIAGNOSTIC_POP
        }

        constexpr bool size_eq(const strbuf& s, const usize size)
        {
            snn_require(s.data().get() != nullptr);
            if (s.size() == size && s.count() == size && s.byte_size().get() == size &&
                s.capacity() >= size)
            {
                if ((size == 0 && s.is_empty() && s == "" && !s) ||
                    (size > 0 && !s.is_empty() && s != "" && s))
                {
                    return true;
                }
            }
            return false;
        }

        template <character Char>
        constexpr bool size_eq(const array_view<Char> s, const usize size)
        {
            snn_require(s.data().get() != nullptr);
            if (s.size() == size && s.count() == size && s.byte_size().get() == size)
            {
                if ((size == 0 && s.is_empty() && s == "" && !s) ||
                    (size > 0 && !s.is_empty() && s != "" && s))
                {
                    return true;
                }
            }
            return false;
        }

        template <typename T>
        constexpr void indirect_self_assignment(T& a, T& b)
        {
            a = b;
        }

        template <typename T>
        constexpr bool test_strview_interface()
        {
            static_assert(is_trivially_relocatable_v<T>);
            static_assert(is_trivially_relocatable_v<const T>);

            using value_type = typename T::value_type;

            // T(init::from_t, ContiguousIt first, ContiguousIt last)
            {
                const T src{""};
                const T s{init::from, src.cbegin(), src.cend()};
                snn_require(size_eq(s, 0));
                snn_require(!size_eq(s, 1));
            }
            {
                const T src{"One Two Three"};
                const T s{init::from, src.cend(), src.cend()};
                snn_require(size_eq(s, 0));
            }
            {
                // Const iter.
                const T src{"One Two Three"};
                const T s{init::from, src.view(4).cbegin(), src.cend()};
                snn_require(size_eq(s, 9));
                snn_require(!size_eq(s, 0));
                snn_require(!size_eq(s, 8));
                snn_require(s == "Two Three");
            }
            {
                // Non-const iter.
                T src{"One Two Three"};
                const auto first = src.view(4).begin();
                const auto last  = src.view(7).begin();
                const T s{init::from, first, last};
                snn_require(size_eq(s, 3));
                snn_require(!size_eq(s, 2));
                snn_require(s == "Two");
            }
            {
                const value_type* const first = nullptr;
                const value_type* const last  = nullptr;
                T s{init::from, first, last};
                snn_require(size_eq(s, 0));
            }

            // T(ContiguousRange rng)
            {
                const T src{"One Two Three"};
                const T s{src.range()};
                snn_require(size_eq(s, 13));
                snn_require(s == "One Two Three");
            }
            {
                const value_type* const first = nullptr;
                const value_type* const last  = nullptr;
                range::contiguous rng{init::from, first, last};
                T s{rng};
                snn_require(size_eq(s, 0));
            }
            {
                const char arr[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f};
                range::contiguous rng{init::from, std::begin(arr), std::end(arr)};
                T s{rng};
                snn_require(size_eq(s, 5));
                snn_require(s == "Hello");
            }
            {
                const char arr[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f};
                range::contiguous rng{init::from, std::end(arr), std::end(arr)};
                T s{rng};
                snn_require(size_eq(s, 0));
            }

            // const iterator
            {
                const T s{"Å1"};
                usize count = 0;
                for (const char c : s)
                {
                    snn_require(count <= 2);

                    const auto b = to_byte(c);
                    if (count == 0)
                    {
                        snn_require(b == 0xc3);
                    }
                    else if (count == 1)
                    {
                        snn_require(b == 0x85);
                    }
                    else if (count == 2)
                    {
                        snn_require(b == 0x31);
                    }

                    ++count;
                }
                snn_require(count == 3);
            }

            // at(pos, assume::within_bounds_t)
            {
                const T s{"One"};
                snn_require(s.at(0, assume::within_bounds) == 'O');
                snn_require(s.at(1, assume::within_bounds) == 'n');
                snn_require(s.at(2, assume::within_bounds) == 'e');
            }

            // at
            {
                const T s{"One"};
                snn_require(s.at(0).value() == 'O');
                snn_require(s.at(1).value() == 'n');
                snn_require(s.at(2).value() == 'e');
                snn_require(!s.at(3).has_value());
            }

            // compare
            {
                T s{"16 byte count..."};
                snn_require(size_eq(s, 16));
                snn_require(s == "16 byte count...");
                snn_require(s == cstrview{"16 byte count..."});
                snn_require(!(s != "16 byte count..."));
                snn_require(s != "16 byte count..X");
                snn_require(s != "X6 byte count...");
                snn_require(s != "15 byte count..");
                snn_require(s != "");
            }

            // compare
            {
                T s{"ÅÄÖ\0åäö"};
                snn_require(size_eq(s, 13));
                snn_require(s == "ÅÄÖ\0åäö");
                snn_require(s == cstrview{"ÅÄÖ\0åäö"});
                snn_require(s != "ÅÄO\0åäö");
                snn_require(s != "ÅÄ\0åäö");
                snn_require(s != "ÅÄÖ\0åaö");
                snn_require(s != "ÅÄÖ\0\0åäö");
                snn_require(s != "");
            }

            // front/back
            {
                const T s{"123"};
                snn_require(s.front(promise::not_empty) == '1');
                snn_require(s.back(promise::not_empty) == '3');
            }
            {
                const T s{"4"};
                snn_require(s.front(promise::not_empty) == '4');
                snn_require(s.back(promise::not_empty) == '4');
            }
            {
                const T s{"abcdefghijklmnopqrstuvwxyz"};
                snn_require(s.front(promise::not_empty) == 'a');
                snn_require(s.back(promise::not_empty) == 'z');
            }

            // drop_back_n
            {
                T s{"aaabbbcccdddeeefff"};
                snn_require(s == "aaabbbcccdddeeefff");
                snn_require(size_eq(s, 18));

                s.drop_back_n(0);
                snn_require(s == "aaabbbcccdddeeefff");
                snn_require(size_eq(s, 18));

                s.drop_back_n(3);
                snn_require(s == "aaabbbcccdddeee");
                snn_require(size_eq(s, 15));

                s.drop_back_n(1);
                snn_require(s == "aaabbbcccdddee");
                snn_require(size_eq(s, 14));

                s.drop_back_n(2);
                snn_require(s == "aaabbbcccddd");
                snn_require(size_eq(s, 12));

                s.drop_back_n(99);
                snn_require(size_eq(s, 0));

                s = T{"1234"};
                snn_require(s == "1234");
                snn_require(size_eq(s, 4));

                s.drop_back_n(4);
                snn_require(size_eq(s, 0));

                s = T{"1"};
                snn_require(s == "1");
                snn_require(size_eq(s, 1));

                s.drop_back_n(1);
                snn_require(size_eq(s, 0));
            }

            // truncate
            {
                T s{"abcd"};
                snn_require(size_eq(s, 4));
                s.truncate(constant::npos);
                snn_require(size_eq(s, 4));
                s.truncate(5);
                snn_require(size_eq(s, 4));
                s.truncate(4);
                snn_require(size_eq(s, 4));
                snn_require(s == "abcd");
                s.truncate(3);
                snn_require(size_eq(s, 3));
                snn_require(s == "abc");
                s.truncate(1);
                snn_require(size_eq(s, 1));
                snn_require(s == "a");
                s.truncate(0);
                snn_require(size_eq(s, 0));
                s.truncate(10);
                snn_require(size_eq(s, 0));
            }
            {
                T s{"abcdefghijklmnopqrstuvwxyz"};
                snn_require(size_eq(s, 26));
                s.truncate(constant::npos);
                snn_require(size_eq(s, 26));
                s.truncate(100);
                snn_require(size_eq(s, 26));
                s.truncate(27);
                snn_require(size_eq(s, 26));
                s.truncate(26);
                snn_require(size_eq(s, 26));
                snn_require(s == "abcdefghijklmnopqrstuvwxyz");
                s.truncate(25);
                snn_require(size_eq(s, 25));
                snn_require(s == "abcdefghijklmnopqrstuvwxy");
                s.truncate(1);
                snn_require(size_eq(s, 1));
                snn_require(s == "a");
                s.truncate(0);
                snn_require(size_eq(s, 0));
                s.truncate(10);
                snn_require(size_eq(s, 0));
            }

            // count(needle)
            {
                T s{""};
                snn_require(s.count('a') == 0);
                snn_require(s.count("") == 1);
                snn_require(s.count("a") == 0);
                snn_require(s.count("abc") == 0);
            }
            {
                T s{"abcaabc"};

                snn_require(s.count('a') == 3);
                snn_require(s.count("") == 8);
                snn_require(s.count("a") == 3);
                snn_require(s.count("abc") == 2);

                snn_require(s.count('x') == 0);
                snn_require(s.count("x") == 0);
                snn_require(s.count("aBc") == 0);
                snn_require(s.count("abcd") == 0);
                snn_require(s.count("abc\0") == 0);
            }

            // find
            {
                T s{""};
                snn_require(s.find("").value_or_npos() == 0);
                snn_require(s.find("", 1).value_or_npos() == constant::npos);
                snn_require(s.find("One").value_or_npos() == constant::npos);
                snn_require(s.find("One").value_or_npos() == constant::npos);
                snn_require(s.find("O").value_or_npos() == constant::npos);
                snn_require(s.find('O').value_or_npos() == constant::npos);
            }
            {
                T s{"O"};
                snn_require(s.find("").value_or_npos() == 0);
                snn_require(s.find("", 1).value_or_npos() == 1);
                snn_require(s.find("", 2).value_or_npos() == constant::npos);
                snn_require(s.find("One").value_or_npos() == constant::npos);
                snn_require(s.find("O").value_or_npos() == 0);
                snn_require(s.find('O').value_or_npos() == 0);
            }
            {
                T s{"On"};
                snn_require(s.find("").value_or_npos() == 0);
                snn_require(s.find("", 1).value_or_npos() == 1);
                snn_require(s.find("", 2).value_or_npos() == 2);
                snn_require(s.find("", 3).value_or_npos() == constant::npos);
                snn_require(s.find("One").value_or_npos() == constant::npos);
                snn_require(s.find("O").value_or_npos() == 0);
                snn_require(s.find('O').value_or_npos() == 0);
            }
            {
                T s{"aaaaaa"};
                snn_require(s.find("aa").value_or_npos() == 0);
                snn_require(s.find("aa", 1).value_or_npos() == 1);
                snn_require(s.find("aa", 2).value_or_npos() == 2);
                snn_require(s.find("aa", 3).value_or_npos() == 3);
                snn_require(s.find("aa", 4).value_or_npos() == 4);
                snn_require(s.find("aa", 5).value_or_npos() == constant::npos);
                snn_require(s.find("aaa").value_or_npos() == 0);
                snn_require(s.find("aaa", 1).value_or_npos() == 1);
                snn_require(s.find("aaa", 2).value_or_npos() == 2);
                snn_require(s.find("aaa", 3).value_or_npos() == 3);
                snn_require(s.find("aaa", 4).value_or_npos() == constant::npos);
                snn_require(s.find("aaaa").value_or_npos() == 0);
                snn_require(s.find("aaaa", 1).value_or_npos() == 1);
                snn_require(s.find("aaaa", 2).value_or_npos() == 2);
                snn_require(s.find("aaaa", 3).value_or_npos() == constant::npos);
                snn_require(s.find("aaaaa").value_or_npos() == 0);
                snn_require(s.find("aaaaa", 1).value_or_npos() == 1);
                snn_require(s.find("aaaaa", 2).value_or_npos() == constant::npos);
                snn_require(s.find("aaaaaa").value_or_npos() == 0);
                snn_require(s.find("aaaaaa", 1).value_or_npos() == constant::npos);
            }
            {
                T s{"OnOnOn"};
                snn_require(s.find("On").value_or_npos() == 0);
                snn_require(s.find("On", 1).value_or_npos() == 2);
                snn_require(s.find("On", 2).value_or_npos() == 2);
                snn_require(s.find("On", 3).value_or_npos() == 4);
                snn_require(s.find("On", 4).value_or_npos() == 4);
                snn_require(s.find("On", 5).value_or_npos() == constant::npos);
            }
            {
                T s{"One"};
                snn_require(s.find("One").value_or_npos() == 0);
                snn_require(s.find("O").value_or_npos() == 0);
                snn_require(s.find('O').value_or_npos() == 0);
            }
            {
                T s{"OneOneOne"};
                snn_require(s.find("On").value_or_npos() == 0);
                snn_require(s.find("On", 1).value_or_npos() == 3);
                snn_require(s.find("On", 2).value_or_npos() == 3);
                snn_require(s.find("On", 3).value_or_npos() == 3);
                snn_require(s.find("On", 4).value_or_npos() == 6);
                snn_require(s.find("On", 5).value_or_npos() == 6);
                snn_require(s.find("On", 6).value_or_npos() == 6);
                snn_require(s.find("On", 7).value_or_npos() == constant::npos);
                snn_require(s.find("One").value_or_npos() == 0);
                snn_require(s.find("One", 1).value_or_npos() == 3);
                snn_require(s.find("One", 2).value_or_npos() == 3);
                snn_require(s.find("One", 3).value_or_npos() == 3);
                snn_require(s.find("One", 4).value_or_npos() == 6);
                snn_require(s.find("One", 5).value_or_npos() == 6);
                snn_require(s.find("One", 6).value_or_npos() == 6);
                snn_require(s.find("One", 7).value_or_npos() == constant::npos);
                snn_require(s.find("ne").value_or_npos() == 1);
                snn_require(s.find("ne", 1).value_or_npos() == 1);
                snn_require(s.find("ne", 2).value_or_npos() == 4);
                snn_require(s.find("ne", 3).value_or_npos() == 4);
                snn_require(s.find("ne", 4).value_or_npos() == 4);
                snn_require(s.find("ne", 5).value_or_npos() == 7);
                snn_require(s.find("ne", 6).value_or_npos() == 7);
                snn_require(s.find("ne", 7).value_or_npos() == 7);
                snn_require(s.find("ne", 8).value_or_npos() == constant::npos);
            }
            {
                T s{"One Two Three"};
                snn_require(s.find("T").value_or_npos() == 4);
                snn_require(s.find("One").value_or_npos() == 0);
                snn_require(s.find("Two").value_or_npos() == 4);
                snn_require(s.find("Two", 3).value_or_npos() == 4);
                snn_require(s.find("Two", 4).value_or_npos() == 4);
                snn_require(s.find("Three").value_or_npos() == 8);
                snn_require(s.find("Three", 8).value_or_npos() == 8);

                snn_require(s.find("One", 1).value_or_npos() == constant::npos);
                snn_require(s.find("Onee", 0).value_or_npos() == constant::npos);
                snn_require(s.find("Three", 9).value_or_npos() == constant::npos);
                snn_require(s.find("Three", 99).value_or_npos() == constant::npos);
                snn_require(s.find("Four").value_or_npos() == constant::npos);
            }
            {
                T s{"TThree Three"};
                snn_require(s.find("Three").value_or_npos() == 1);
                snn_require(s.find("Three", 1).value_or_npos() == 1);
                snn_require(s.find("Three", 2).value_or_npos() == 7);
                snn_require(s.find("Three", 7).value_or_npos() == 7);
                snn_require(s.find("Three", 8).value_or_npos() == constant::npos);
            }
            {
                T s{"One Two Three"};
                snn_require(s.find('O').value_or_npos() == 0);
                snn_require(s.find('o').value_or_npos() == 6);
                snn_require(s.find('n').value_or_npos() == 1);
                snn_require(s.find('T').value_or_npos() == 4);
                snn_require(s.find('e').value_or_npos() == 2);
                snn_require(s.find('e', 2).value_or_npos() == 2);
                snn_require(s.find('e', 3).value_or_npos() == 11);
                snn_require(s.find('e', 12).value_or_npos() == 12);

                snn_require(s.find('Q').value_or_npos() == constant::npos);
                snn_require(s.find('W').value_or_npos() == constant::npos);
                snn_require(s.find('O', 1).value_or_npos() == constant::npos);
                snn_require(s.find('e', 13).value_or_npos() == constant::npos);
            }

            // find_in_reverse
            {
                T s{""};
                snn_require(s.find_in_reverse("").value_or_npos() == 0);
                snn_require(s.find_in_reverse("", 0).value_or_npos() == 0);
                snn_require(s.find_in_reverse("", 1).value_or_npos() == 0);
                snn_require(s.find_in_reverse("", 99).value_or_npos() == 0);
                snn_require(s.find_in_reverse("One").value_or_npos() == constant::npos);
                snn_require(s.find_in_reverse("O").value_or_npos() == constant::npos);
            }
            {
                T s{"O"};
                snn_require(s.find_in_reverse("").value_or_npos() == 1);
                snn_require(s.find_in_reverse("", 0).value_or_npos() == 0);
                snn_require(s.find_in_reverse("", 1).value_or_npos() == 1);
                snn_require(s.find_in_reverse("", 99).value_or_npos() == 1);
                snn_require(s.find_in_reverse("One").value_or_npos() == constant::npos);
                snn_require(s.find_in_reverse("O").value_or_npos() == 0);
            }
            {
                T s{"On"};
                snn_require(s.find_in_reverse("").value_or_npos() == 2);
                snn_require(s.find_in_reverse("", 0).value_or_npos() == 0);
                snn_require(s.find_in_reverse("", 1).value_or_npos() == 1);
                snn_require(s.find_in_reverse("", 2).value_or_npos() == 2);
                snn_require(s.find_in_reverse("", 3).value_or_npos() == 2);
                snn_require(s.find_in_reverse("", 99).value_or_npos() == 2);
                snn_require(s.find_in_reverse("One").value_or_npos() == constant::npos);
                snn_require(s.find_in_reverse("on").value_or_npos() == constant::npos);
                snn_require(s.find_in_reverse("oN").value_or_npos() == constant::npos);
                snn_require(s.find_in_reverse("N").value_or_npos() == constant::npos);
                snn_require(s.find_in_reverse("O").value_or_npos() == 0);
                snn_require(s.find_in_reverse("O", 0).value_or_npos() == 0);
                snn_require(s.find_in_reverse("On").value_or_npos() == 0);
                snn_require(s.find_in_reverse("On", 1).value_or_npos() == 0);
                snn_require(s.find_in_reverse("On", 0).value_or_npos() == 0);
            }
            {
                T s{"aaaa"};
                snn_require(s.find_in_reverse("a").value_or_npos() == 3);
                snn_require(s.find_in_reverse("a", 4).value_or_npos() == 3);
                snn_require(s.find_in_reverse("a", 3).value_or_npos() == 3);
                snn_require(s.find_in_reverse("a", 2).value_or_npos() == 2);
                snn_require(s.find_in_reverse("a", 1).value_or_npos() == 1);
                snn_require(s.find_in_reverse("a", 0).value_or_npos() == 0);
                snn_require(s.find_in_reverse("aa").value_or_npos() == 2);
                snn_require(s.find_in_reverse("aa", 4).value_or_npos() == 2);
                snn_require(s.find_in_reverse("aa", 3).value_or_npos() == 2);
                snn_require(s.find_in_reverse("aa", 2).value_or_npos() == 2);
                snn_require(s.find_in_reverse("aa", 1).value_or_npos() == 1);
                snn_require(s.find_in_reverse("aa", 0).value_or_npos() == 0);
                snn_require(s.find_in_reverse("aaa").value_or_npos() == 1);
                snn_require(s.find_in_reverse("aaa", 4).value_or_npos() == 1);
                snn_require(s.find_in_reverse("aaa", 3).value_or_npos() == 1);
                snn_require(s.find_in_reverse("aaa", 2).value_or_npos() == 1);
                snn_require(s.find_in_reverse("aaa", 1).value_or_npos() == 1);
                snn_require(s.find_in_reverse("aaa", 0).value_or_npos() == 0);
                snn_require(s.find_in_reverse("aaaa").value_or_npos() == 0);
                snn_require(s.find_in_reverse("aaaa", 4).value_or_npos() == 0);
                snn_require(s.find_in_reverse("aaaa", 3).value_or_npos() == 0);
                snn_require(s.find_in_reverse("aaaa", 2).value_or_npos() == 0);
                snn_require(s.find_in_reverse("aaaa", 1).value_or_npos() == 0);
                snn_require(s.find_in_reverse("aaaa", 0).value_or_npos() == 0);
                snn_require(s.find_in_reverse("aaaaa").value_or_npos() == constant::npos);
            }
            {
                T s{"OnOnOn"};
                snn_require(s.find_in_reverse("On").value_or_npos() == 4);
                snn_require(s.find_in_reverse("On", 5).value_or_npos() == 4);
                snn_require(s.find_in_reverse("On", 4).value_or_npos() == 4);
                snn_require(s.find_in_reverse("On", 3).value_or_npos() == 2);
                snn_require(s.find_in_reverse("On", 2).value_or_npos() == 2);
                snn_require(s.find_in_reverse("On", 1).value_or_npos() == 0);
                snn_require(s.find_in_reverse("On", 0).value_or_npos() == 0);
            }
            {
                T s{"TThreee Three"};
                snn_require(s.find_in_reverse("Three").value_or_npos() == 8);
                snn_require(s.find_in_reverse("Three", 13).value_or_npos() == 8);
                snn_require(s.find_in_reverse("Three", 12).value_or_npos() == 8);
                snn_require(s.find_in_reverse("Three", 11).value_or_npos() == 8);
                snn_require(s.find_in_reverse("Three", 10).value_or_npos() == 8);
                snn_require(s.find_in_reverse("Three", 9).value_or_npos() == 8);
                snn_require(s.find_in_reverse("Three", 8).value_or_npos() == 8);
                snn_require(s.find_in_reverse("Three", 7).value_or_npos() == 1);
                snn_require(s.find_in_reverse("Three", 6).value_or_npos() == 1);
                snn_require(s.find_in_reverse("Three", 5).value_or_npos() == 1);
                snn_require(s.find_in_reverse("Three", 4).value_or_npos() == 1);
                snn_require(s.find_in_reverse("Three", 3).value_or_npos() == 1);
                snn_require(s.find_in_reverse("Three", 2).value_or_npos() == 1);
                snn_require(s.find_in_reverse("Three", 1).value_or_npos() == 1);
                snn_require(s.find_in_reverse("Three", 0).value_or_npos() == constant::npos);
            }
            {
                T s{""};
                snn_require(!s.find_in_reverse('T'));
            }
            {
                T s{"T"};
                snn_require(s.find_in_reverse('T').value_or_npos() == 0);
                snn_require(s.find_in_reverse('T', 0).value_or_npos() == 0);
                snn_require(s.find_in_reverse('A').value_or_npos() == constant::npos);
            }
            {
                T s{"One Two Three"};
                snn_require(s.find_in_reverse('T').value_or_npos() == 8);
                snn_require(s.find_in_reverse('T', 99).value_or_npos() == 8);
                snn_require(s.find_in_reverse('T', 8).value_or_npos() == 8);
                snn_require(s.find_in_reverse('T', 7).value_or_npos() == 4);
                snn_require(s.find_in_reverse('T', 4).value_or_npos() == 4);
                snn_require(s.find_in_reverse('T', 3).value_or_npos() == constant::npos);
                snn_require(s.find_in_reverse('T', 0).value_or_npos() == constant::npos);
                snn_require(s.find_in_reverse('Q').value_or_npos() == constant::npos);
            }

            // contains
            {
                T s{""};
                snn_require(s.contains(""));

                snn_require(!s.contains("a"));
                snn_require(!s.contains('a'));
                snn_require(!s.contains('\0'));
            }
            {
                T s{"a\0c"};
                snn_require(size_eq(s, 3));
                snn_require(s.contains('a'));
                snn_require(s.contains('\0'));
                snn_require(s.contains('c'));

                snn_require(!s.contains('d'));
                snn_require(!s.contains('\n'));
            }

            // contains
            {
                T s{"FooBar"};

                snn_require(s.contains(""));
                snn_require(s.contains("FooBar"));
                snn_require(s.contains("F"));
                snn_require(s.contains("Foo"));
                snn_require(s.contains("B"));
                snn_require(s.contains("Bar"));
                snn_require(s.contains("r"));

                snn_require(!s.contains("FooBar!"));
                snn_require(!s.contains("FOOBAR"));
                snn_require(!s.contains("FOO"));
                snn_require(!s.contains("BAR"));
                snn_require(!s.contains("b"));
                snn_require(!s.contains("f"));
                snn_require(!s.contains("R"));
                snn_require(!s.contains("O"));
            }

            // has_front
            {
                T s{"FooBar"};

                snn_require(s.has_front(""));
                snn_require(s.has_front("F"));
                snn_require(s.has_front("Fo"));
                snn_require(s.has_front("Foo"));
                snn_require(s.has_front("FooBar"));
                snn_require(s.has_front('F'));

                snn_require(!s.has_front("B"));
                snn_require(!s.has_front("Ba"));
                snn_require(!s.has_front("Bar"));
                snn_require(!s.has_front("FooBar!"));
                snn_require(!s.has_front("FooF"));
                snn_require(!s.has_front("FOO"));
                snn_require(!s.has_front("BAR"));
                snn_require(!s.has_front('B'));

                s = T{""};

                snn_require(s.has_front(""));

                snn_require(!s.has_front("F"));
                snn_require(!s.has_front("Fo"));
                snn_require(!s.has_front("Foo"));
                snn_require(!s.has_front("FooBar"));
                snn_require(!s.has_front('F'));

                snn_require(!s.has_front("B"));
                snn_require(!s.has_front("Ba"));
                snn_require(!s.has_front("Bar"));
                snn_require(!s.has_front("FooBar!"));
                snn_require(!s.has_front("FooF"));
                snn_require(!s.has_front("FOO"));
                snn_require(!s.has_front("BAR"));
                snn_require(!s.has_front('B'));
            }

            // has_back
            {
                T s{"FooBar"};

                snn_require(s.has_back(""));
                snn_require(s.has_back("r"));
                snn_require(s.has_back("Bar"));
                snn_require(s.has_back("FooBar"));
                snn_require(s.has_back('r'));

                snn_require(!s.has_back("R"));
                snn_require(!s.has_back("Fo"));
                snn_require(!s.has_back("Foo"));
                snn_require(!s.has_back("FooBar!"));
                snn_require(!s.has_back("aFooBar"));
                snn_require(!s.has_back("OBar"));
                snn_require(!s.has_back("BAR"));
                snn_require(!s.has_back('o'));

                s = T{""};

                snn_require(s.has_back(""));

                snn_require(!s.has_back("r"));
                snn_require(!s.has_back("Bar"));
                snn_require(!s.has_back("FooBar"));
                snn_require(!s.has_back('r'));

                snn_require(!s.has_back("R"));
                snn_require(!s.has_back("Fo"));
                snn_require(!s.has_back("Foo"));
                snn_require(!s.has_back("FooBar!"));
                snn_require(!s.has_back("aFooBar"));
                snn_require(!s.has_back("OBar"));
                snn_require(!s.has_back("BAR"));
                snn_require(!s.has_back('o'));
            }

            // range
            {
                T s{"One Two Three"};
                auto rng = s.range();
                snn_require(rng.count() == 13);
                snn_require(rng.begin() == s.begin());
                snn_require(rng.end() == s.end());
            }
            {
                const T s{"One Two Three"};
                const auto rng = s.range();
                snn_require(rng.count() == 13);
                snn_require(rng.begin() == s.begin());
                snn_require(rng.end() == s.end());
            }

            // view
            {
                T s{"One Two Three"};
                auto p = s.view();
                snn_require(p.data().get() == s.data().get());
                snn_require(p.size() == s.size());
                snn_require(p == "One Two Three");
            }
            {
                T s{""};
                auto p = s.view();
                snn_require(p.data().get() == s.data().get());
                snn_require(p.size() == s.size());
                snn_require(p == "");
            }
            {
                T s{"One Two Three"};
                auto p = s.view(0, 3);
                snn_require(p.data().get() == s.data().get());
                snn_require(p.size() == 3);
                snn_require(p == "One");
            }

            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            {
                T s{"One Two Three"};
                auto p = s.view(4, 3);
                snn_require(p.data().get() == (s.data().get() + 4));
                snn_require(p.size() == 3);
                snn_require(p == "Two");
            }
            {
                T s{"One Two Three"};
                auto p = s.view(4, 0);
                snn_require(p.data().get() == (s.data().get() + 4));
                snn_require(p.size() == 0);
                snn_require(p == "");
            }
            {
                T s{"One Two Three"};
                auto p = s.view(8, 5);
                snn_require(p.data().get() == (s.data().get() + 8));
                snn_require(p.size() == 5);
                snn_require(p == "Three");
            }
            {
                T s{"One Two Three"};
                auto p = s.view(8, 6);
                snn_require(p.data().get() == (s.data().get() + 8));
                snn_require(p.size() == 5);
                snn_require(p == "Three");
            }
            {
                T s{"One Two Three"};
                auto p = s.view(12, 1);
                snn_require(p.data().get() == (s.data().get() + 12));
                snn_require(p.size() == 1);
                snn_require(p == "e");
            }
            {
                T s{"One Two Three"};
                auto p = s.view(13, 1);
                snn_require(p.data().get() == (s.data().get() + s.size()));
                snn_require(p.size() == 0);
                snn_require(p == "");
            }
            {
                T s{"One Two Three"};
                auto p = s.view(99, 10);
                snn_require(p.data().get() == (s.data().get() + s.size()));
                snn_require(p.size() == 0);
                snn_require(p == "");
            }
            {
                T s{"One Two Three"};
                auto p = s.view(0);
                snn_require(p.data().get() == s.data().get());
                snn_require(p.size() == 13);
                snn_require(p == "One Two Three");
            }
            {
                T s{"One Two Three"};
                auto p = s.view(8);
                snn_require(p.data().get() == (s.data().get() + 8));
                snn_require(p.size() == 5);
                snn_require(p == "Three");
            }
            {
                T s{"One Two Three"};
                auto p = s.view(12);
                snn_require(p.data().get() == (s.data().get() + 12));
                snn_require(p.size() == 1);
                snn_require(p == "e");
            }
            {
                T s{"One Two Three"};
                auto p = s.view(13);
                snn_require(p.data().get() == (s.data().get() + s.size()));
                snn_require(p.size() == 0);
                snn_require(p == "");
            }
            {
                T s{"One Two Three"};
                auto p = s.view(12, 99);
                snn_require(p.data().get() == (s.data().get() + 12));
                snn_require(p.size() == 1);
                snn_require(p == "e");
            }
            {
                T s{"One Two Three"};
                auto p = s.view(13, 99);
                snn_require(p.data().get() == (s.data().get() + s.size()));
                snn_require(p.size() == 0);
                snn_require(p == "");
            }

            SNN_DIAGNOSTIC_POP

            {
                const T s{"abcdefghijklmnopqrstuvwxyz"};
                const auto p = s.view();
                snn_require(p.data().get() == s.data().get());
                snn_require(p.size() == s.size());
                snn_require(p == "abcdefghijklmnopqrstuvwxyz");
            }

            // to<Int>
            {
                snn_require(T{"12"}.template to<usize>().value() == 12);
                snn_require(T{"-12"}.template to<i8>().value() == -12);
                snn_require(T{"82"}.template to<u8>().value() == 82);

                snn_require(!T{"OneTwo"}.template to<usize>());
                snn_require(!T{"12Trailing"}.template to<usize>());
                snn_require(!T{"a12"}.template to<usize>());
                snn_require(!T{"-12"}.template to<usize>());
                snn_require(!T{"082"}.template to<u8>());
                snn_require(!T{"382"}.template to<u8>());
            }
            {
                T s{""};
                snn_require(!s.template to<u8>());
            }
            {
                T hex{"eFa030ffbD"};
                const usize n = hex.template to<usize, math::base::hex>().value();
                snn_require(n == 1029184749501);
            }

            // to_prefix
            {
                const auto p = T{""}.template to_prefix<int>();
                snn_require(p.value == 0 && p.count == 0);
            }
            {
                const auto p = T{"OneTwo"}.template to_prefix<int>();
                snn_require(p.value == 0 && p.count == 0);
            }
            {
                const auto p = T{"-1Two"}.template to_prefix<int>();
                snn_require(p.value == -1 && p.count == 2);
            }
            {
                const auto p = T{"082"}.template to_prefix<int>();
                snn_require(p.value == 0 && p.count == 1);
            }
            {
                const auto p = T{"082"}.template to_prefix<int>(ascii::leading_zeros::allow);
                snn_require(p.value == 82 && p.count == 3);
            }
            {
                const auto p = T{"00eFa030ffbD_"}.template to_prefix<usize, math::base::hex>(
                    ascii::leading_zeros::allow);
                snn_require(p.value == 1029184749501 && p.count == 12);
            }
            {
                const T s{"8928239832983293892393829"};
                {
                    // No limit (overflow).
                    const auto p = s.template to_prefix<u32, math::base::decimal>();
                    snn_require(p.value == 0 && p.count == 0);
                }
                {
                    // Max 4 digits.
                    const auto p = s.template to_prefix<u32, math::base::decimal, 4>();
                    snn_require(p.value == 8928 && p.count == 4);
                }
            }

            // operator==
            {
                T s{"abc"};
                snn_require(s == cstrview{"abc"});
                snn_require(s == "abc");
                snn_require(s == str{"abc"});
                snn_require(s == strbuf{"abc"});
                snn_require(!(s == cstrview{}));
                snn_require(!(s == ""));
                snn_require(!(s == str{}));
                snn_require(!(s == strbuf{}));
            }

            // operator!=
            {
                T s{"abc"};
                snn_require(s != cstrview{});
                snn_require(s != "");
                snn_require(s != str{});
                snn_require(s != strbuf{});
                snn_require(!(s != cstrview{"abc"}));
                snn_require(!(s != "abc"));
                snn_require(!(s != str{"abc"}));
                snn_require(!(s != strbuf{"abc"}));
            }

            // operator<
            {
                T a{""};
                T b{""};
                snn_require(!(a < b));
            }
            {
                T a{"a"};
                T b{"a"};
                snn_require(!(a < b));
            }
            {
                T a{""};
                T b{"a"};
                snn_require(a < b);
            }
            {
                T a{"A"};
                T b{"a"};
                snn_require(a < b);
            }
            {
                T a{"a"};
                T b{"A"};
                snn_require(!(a < b));
            }
            {
                T a{"a"};
                T b{"aa"};
                snn_require(a < b);
            }
            {
                array<T, 3> arr{"Foo", "Bar", "Bar+"};

                arr.sort();

                snn_require(arr.at(0, assume::within_bounds) == "Bar");
                snn_require(arr.at(1, assume::within_bounds) == "Bar+");
                snn_require(arr.at(2, assume::within_bounds) == "Foo");
            }

            // operator>
            {
                T a{""};
                T b{""};
                snn_require(!(a > b));
            }
            {
                T a{"a"};
                T b{"a"};
                snn_require(!(a > b));
            }
            {
                T a{"a"};
                T b{""};
                snn_require(a > b);
            }
            {
                T a{"a"};
                T b{"A"};
                snn_require(a > b);
            }
            {
                T a{"A"};
                T b{"a"};
                snn_require(!(a > b));
            }
            {
                T a{"aa"};
                T b{"a"};
                snn_require(a > b);
            }
            {
                array<T, 3> arr{"Bar", "Foo", "Bar+"};

                arr.sort(fn::greater_than{});

                snn_require(arr.at(0, assume::within_bounds) == "Foo");
                snn_require(arr.at(1, assume::within_bounds) == "Bar+");
                snn_require(arr.at(2, assume::within_bounds) == "Bar");
            }

            return true;
        }

        template <typename T>
        constexpr bool test_str_interface()
        {
            static_assert(str::default_capacity() == 23);
            static_assert(strbuf::default_capacity() == 0);

            static_assert(str::is_null_terminated());
            static_assert(!strbuf::is_null_terminated());

            constexpr usize default_capacity = T::default_capacity();

            // strbuf should never allocate less than 16 bytes.
            static_assert(mem::raw::optimal_size(not_zero<usize>{1}).get() == 16);
            constexpr usize min_capacity = math::max(default_capacity, 16);

            // T()
            {
                T s;
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);
            }

            // T(init::reserve)
            {
                T s{init::reserve, 0};
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);
            }
            {
                T s{init::reserve, min_capacity};
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == min_capacity);
            }
            {
                T s{init::reserve, min_capacity + 1};
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() > min_capacity);
            }
            {
                snn_require_throws_code((T{init::reserve, constant::npos}),
                                        generic::error::size_would_exceed_max_size);
            }

            // T{init::size_for_overwrite, size}
            {
                T s{init::size_for_overwrite, 0};
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);
            }
            {
                T s{init::size_for_overwrite, min_capacity};
                snn_require(size_eq(s, min_capacity));
                snn_require(s.capacity() == min_capacity);
            }
            {
                T s{init::size_for_overwrite, min_capacity + 1};
                snn_require(size_eq(s, min_capacity + 1));
                snn_require(s.capacity() > min_capacity);
            }
            {
                snn_require_throws_code((T{init::size_for_overwrite, constant::npos}),
                                        generic::error::size_would_exceed_max_size);
            }

            // T(char(&s)[N])
            {
                const T s{""};
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);
            }
            {
                const T s{"One Two Three"};
                snn_require(size_eq(s, 13));
                snn_require(s.capacity() == min_capacity);
                snn_require(s == "One Two Three");
            }

            // T(init_list<char>)
            {
                T s{'a'};
                snn_require(size_eq(s, 1));
                snn_require(s == "a");
            }
            {
                T s{'a', 'B'};
                snn_require(size_eq(s, 2));
                snn_require(s == "aB");
            }
            {
                T s{'a', 'B', '4'};
                snn_require(size_eq(s, 3));
                snn_require(s == "aB4");
            }
            {
                constexpr char x = 'x';
                char y           = 'Y';
                T s{'a', 'B', x, y};
                snn_require(size_eq(s, 4));
                snn_require(s == "aBxY");
            }
            {
                T s{init_list<char>{}}; // Empty init-list.
                snn_require(size_eq(s, 0));
            }

            // T(init_list<int>)
            {
                // The following should not compile:
                // T s{65};
                // T s{65u};
                // T s{65, 97};
                // T s{{}};
                // T s{{}, {}};
            }

            // T(const strcore<U>&)
            {
                const T s{str{"One Two Three"}};
                snn_require(size_eq(s, 13));
                snn_require(s.capacity() == min_capacity);
                snn_require(s == "One Two Three");
            }
            {
                const T s{strbuf{"One Two Three"}};
                snn_require(size_eq(s, 13));
                snn_require(s.capacity() == min_capacity);
                snn_require(s == "One Two Three");
            }

            // T(cstrview)
            {
                const cstrview tmp{"One Two Three"};
                snn_require(size_eq(tmp, 13));

                T s{tmp};
                snn_require(size_eq(s, 13));
                snn_require(s.capacity() == min_capacity);
                snn_require(s == "One Two Three");
            }
            {
                const cstrview tmp{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "9876543210_-<>()"};
                snn_require(tmp.size() == 68);

                T s{tmp};
                snn_require(size_eq(s, 68));
                snn_require(s.capacity() >= 68);
                snn_require(s == "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "9876543210_-<>()");
            }

            // T(init::fill_t, usize, char)
            {
                T s{init::fill, 10, 'a'}; // int to usize implicit conversion.
                snn_require(size_eq(s, 10));
                snn_require(s == "aaaaaaaaaa");
            }

            // T(const char*, usize)
            // T(not_null<const char*>, usize)
            // T(init::fill_t, usize, char)
            {
                for (const auto i : init_list<usize>{0, 1, 2, 15, 16, 17, 22, 23, 24, 58, 283})
                {
                    T s1{init::fill, i, 'a'};
                    snn_require(size_eq(s1, i));

                    static_assert(std::is_same_v<decltype(s1.data()), not_null<const char*>>);
                    T s2{s1.data(), s1.size()};
                    snn_require(size_eq(s2, i));

                    static_assert(std::is_same_v<decltype(s1.data().get()), const char*>);
                    T s3{s1.data().get(), s1.size()};
                    snn_require(size_eq(s3, i));

                    if (i == 0)
                    {
                        snn_require(s1.capacity() == default_capacity);
                        snn_require(s2.capacity() == default_capacity);
                        snn_require(s3.capacity() == default_capacity);
                    }
                    else if (i <= min_capacity)
                    {
                        snn_require(s1.capacity() == min_capacity);
                        snn_require(s2.capacity() == min_capacity);
                        snn_require(s3.capacity() == min_capacity);
                    }
                    else
                    {
                        snn_require(s1.capacity() >= i);
                        snn_require(s2.capacity() >= i);
                        snn_require(s3.capacity() >= i);
                    }

                    for (usize j = 0; j < i; ++j)
                    {
                        snn_require(s1.at(j).value() == 'a');
                        snn_require(s2.at(j).value() == 'a');
                        snn_require(s3.at(j).value() == 'a');
                    }
                }
            }
            {
                T s{nullptr, 0};
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);
            }
            {
                T s{"abcdef", 3};
                snn_require(size_eq(s, 3));
                snn_require(s.capacity() == min_capacity);
                snn_require(s == "abc");
            }
            {
                snn_require_throws_code((T{"", constant::npos}),
                                        generic::error::size_would_exceed_max_size);
                snn_require_throws_code((T{not_null{""}, constant::npos}),
                                        generic::error::size_would_exceed_max_size);
                snn_require_throws_code((T{init::fill, constant::npos, 'a'}),
                                        generic::error::size_would_exceed_max_size);
            }

            // T(init::from_t, const StdContainer&)
            {
                // The source container can be something like a `std::string[_view]`.
                const string_like container{"abcdef", 3};

                T s{init::from, container};
                snn_require(size_eq(s, 3));
                snn_require(s.capacity() == min_capacity);
                snn_require(s == "abc");
            }
            {
                string_like<char> container{nullptr, 0};
                snn_require(container.data() == nullptr);
                snn_require(container.size() == 0);

                T s{init::from, container};
                snn_require(size_eq(s, 0));
            }
            {
                using sl  = string_like<char>;
                using csl = string_like<const char>;

                static_assert(constructible_from<T, init::from_t, sl&>);
                static_assert(constructible_from<T, init::from_t, csl&>);
                static_assert(constructible_from<T, init::from_t, const sl&>);
                static_assert(constructible_from<T, init::from_t, const csl&>);
                static_assert(constructible_from<T, init::from_t, sl&&>);
                static_assert(constructible_from<T, init::from_t, csl&&>);
                static_assert(constructible_from<T, init::from_t, const sl&&>);
                static_assert(constructible_from<T, init::from_t, const csl&&>);

                static_assert(!constructible_from<T, init::from_t, int&>);
                static_assert(!constructible_from<T, init::from_t, not_null<char*>&>);
            }
            {
                using csl = string_like<const char>;
                snn_require_throws_code((T{init::from, csl{"", constant::npos}}),
                                        generic::error::size_would_exceed_max_size);
            }

            // T(const char* const s, assume::null_terminated_t)
            {
                T s{nullptr, assume::null_terminated};
                snn_require(size_eq(s, 0));
                s.append("abc");
                snn_require(size_eq(s, 3));
                snn_require(s == "abc");
            }
            {
                const char* const c_string = "";
                T s{c_string, assume::null_terminated};
                snn_require(size_eq(s, 0));
                s.append("abc");
                snn_require(size_eq(s, 3));
                snn_require(s == "abc");
            }
            {
                const char* const c_string = "Hello";
                T s{c_string, assume::null_terminated};
                snn_require(size_eq(s, 5));
                snn_require(s == "Hello");
            }
            {
                const char* const c_string = "abc\0def";
                T s{c_string, assume::null_terminated};
                snn_require(size_eq(s, 3));
                snn_require(s == "abc");
            }

            // T(const not_null<const_pointer> s, assume::null_terminated_t)
            {
                const char* const c_string = "";
                T s{not_null{c_string}, assume::null_terminated};
                snn_require(size_eq(s, 0));
            }
            {
                const char* const c_string = "Hello";
                T s{not_null{c_string}, assume::null_terminated};
                snn_require(size_eq(s, 5));
                snn_require(s == "Hello");
            }
            {
                const char* const c_string = "abc\0def";
                T s{not_null{c_string}, assume::null_terminated};
                snn_require(size_eq(s, 3));
                snn_require(s == "abc");
            }

            // T(init::from_t, ContiguousIt first, ContiguousIt last)
            {
                const cstrview p{""};
                const T s{init::from, p.cbegin(), p.cend()};
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);
            }
            {
                // Const iter.
                const cstrview p{"One Two Three"};
                const T s{init::from, p.view(4).cbegin(), p.cend()};
                snn_require(size_eq(s, 9));
                snn_require(s == "Two Three");
                snn_require(s.capacity() == min_capacity);
            }

            // iterator
            {
                T s{"Å1"};
                snn_require(size_eq(s, 3));

                usize count = 0;
                for (char& c : s)
                {
                    snn_require(count <= 2);

                    if (count == 0)
                    {
                        snn_require(c == to_char(0xc3));
                        c = to_char(0xc2);
                    }
                    else if (count == 1)
                    {
                        snn_require(c == to_char(0x85));
                        c = to_char(0xae);
                    }
                    else if (count == 2)
                    {
                        snn_require(c == to_char(0x31));
                        c = to_char(0x32);
                    }

                    ++count;
                }
                snn_require(count == 3);
                snn_require(s == "®2");
                snn_require(size_eq(s, 3));
            }
            {
                T s{"\x40\x41"};
                snn_require(size_eq(s, 2));

                usize i = 0;
                for (auto& c : s)
                {
                    ++i;
                    ++c;
                }
                snn_require(i == 2);
                snn_require(s == "\x41\x42");
                snn_require(size_eq(s, 2));
            }

            // append
            {
                cstrview src{"abcdefghijklmnopqrstuvw"};

                T s;

                s.append(src);
                snn_require(size_eq(s, 23));
                snn_require(s == "abcdefghijklmnopqrstuvw");

                s.append(src.view());
                snn_require(size_eq(s, 46));
                snn_require(s == "abcdefghijklmnopqrstuvwabcdefghijklmnopqrstuvw");

                s.append(src.view(src.count())); // Empty view.
                snn_require(size_eq(s, 46));
                snn_require(s == "abcdefghijklmnopqrstuvwabcdefghijklmnopqrstuvw");

                s.append(src.view(0, 0));
                snn_require(size_eq(s, 46));
                snn_require(s == "abcdefghijklmnopqrstuvwabcdefghijklmnopqrstuvw");

                s.append("");
                snn_require(size_eq(s, 46));
                snn_require(s == "abcdefghijklmnopqrstuvwabcdefghijklmnopqrstuvw");

                s.append("1234");
                snn_require(size_eq(s, 50));
                snn_require(s == "abcdefghijklmnopqrstuvwabcdefghijklmnopqrstuvw1234");

                s.append(src);
                snn_require(size_eq(s, 73));
                snn_require(s == "abcdefghijklmnopqrstuvwabcdefghijklmnopqrstuvw1234"
                                 "abcdefghijklmnopqrstuvw");

                const usize capacity = s.capacity();
                while (s.size() < capacity)
                {
                    s.append('a');
                }
                snn_require(s.capacity() == capacity);
                snn_require(s.size() == capacity);

                // Empty append must never trigger a growth.
                s.append(src.view(4, 0));
                snn_require(s.capacity() == capacity);
                snn_require(s.size() == capacity);
                s.append("");
                snn_require(s.capacity() == capacity);
                snn_require(s.size() == capacity);
            }
            {
                const cstrview this_is_invalid{not_null<const char*>{"something"}, constant::npos};

                T s;
                s.append("one two");

                snn_require_throws_code(s.append(this_is_invalid),
                                        generic::error::size_would_exceed_max_size);
                snn_require(s == "one two");

                s.append("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

                snn_require_throws_code(s.append(this_is_invalid),
                                        generic::error::size_would_exceed_max_size);
                snn_require(s == "one twoABCDEFGHIJKLMNOPQRSTUVWXYZ");
            }

            {
                // Append init_list<char>.

                T s;

                s.append({'a'});
                snn_require(size_eq(s, 1));

                s.append({'b', 'c'});
                snn_require(size_eq(s, 3));

                s.append({'d', 'e', 'f'});
                snn_require(size_eq(s, 6));

                snn_require(s == "abcdef");

                s.append({'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                          'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'});
                snn_require(size_eq(s, 26));

                snn_require(s == "abcdefghijklmnopqrstuvwxyz");
            }
            {
                // Append self.
                T s{"a"};
                while (s.size() < 128)
                {
                    s.append(s.view());
                }
                snn_require(s.all(fn::is{fn::equal_to{}, 'a'}));
            }
            {
                T s;

                s.append(str{"abcdefghijklmnopqrstuvwabcdefghijklmnopqrstuvw"});
                snn_require(size_eq(s, 46));
                snn_require(s == "abcdefghijklmnopqrstuvwabcdefghijklmnopqrstuvw");

                s.append(strbuf{"1234"});
                snn_require(size_eq(s, 50));
                snn_require(s == "abcdefghijklmnopqrstuvwabcdefghijklmnopqrstuvw1234");
            }

            // append char
            {
                T s;

                s.append('a');
                snn_require(size_eq(s, 1));
                snn_require(s == "a");
                s.append('d');
                snn_require(size_eq(s, 2));
                snn_require(s == "ad");
                s.append('f');
                snn_require(size_eq(s, 3));
                snn_require(s == "adf");
            }
            {
                T s{"foo"};

                s.append('a');
                snn_require(size_eq(s, 4));
                snn_require(s == "fooa");
                snn_require(s.capacity() == min_capacity);
            }

            // append_integral
            {
                T s;

                s.append_integral(123);
                snn_require(s == "123");

                s.clear();
                s.append_integral(-123);
                snn_require(s == "-123");

                s.clear();
                s.append_integral(123, 6);
                snn_require(s == "000123");

                s.clear();
                s.append_integral(-123, 6);
                snn_require(s == "-000123");

                s.clear();
                s.append_integral(constant::limit<i64>::min);
                snn_require(s == "-9223372036854775808");

                s.clear();
                s.append_integral(constant::limit<i64>::min, 7);
                snn_require(s == "-9223372036854775808");

                s.clear();
                s.append_integral(constant::limit<i64>::min, 19);
                snn_require(s == "-9223372036854775808");

                s.clear();
                s.append_integral(constant::limit<i64>::min, 20);
                snn_require(s == "-09223372036854775808");

                s.clear();
                s.append_integral(456);
                snn_require(s == "456");

                s.clear();
                s.append_integral(456, 6);
                snn_require(s == "000456");

                s.clear();
                s.append_integral(constant::limit<u64>::max);
                snn_require(s == "18446744073709551615");

                s.clear();
                s.append_integral(constant::limit<u64>::max, 7);
                snn_require(s == "18446744073709551615");

                s.clear();
                s.append_integral(constant::limit<u64>::max, 20);
                snn_require(s == "18446744073709551615");

                s.clear();
                s.append_integral(constant::limit<u64>::max, 25);
                snn_require(s == "0000018446744073709551615");
            }
            {
                T s;

                s.append_integral(123);
                snn_require(size_eq(s, 3));
                snn_require(s.capacity() == min_capacity);
                snn_require(s == "123");

                s.append_integral(456, 6);
                snn_require(size_eq(s, 9));
                snn_require(s.capacity() == min_capacity);
                snn_require(s == "123000456");

                s.append_integral(18446744073709551615u, 15);
                snn_require(size_eq(s, 29));
                snn_require(s == "12300045618446744073709551615");

                s.append_integral(constant::limit<i64>::min);
                snn_require(size_eq(s, 49));
                snn_require(s == "12300045618446744073709551615-9223372036854775808");

                s.append_integral(-1234567);
                snn_require(size_eq(s, 57));
                snn_require(s == "12300045618446744073709551615-9223372036854775808-1234567");

                s.append_integral(8737516154, 12);
                snn_require(size_eq(s, 69));
                snn_require(s == "12300045618446744073709551615-9223372036854775808-1234567"
                                 "008737516154");
            }
            {
                T s;

                using math::base;

                s.template append_integral<base::binary>(174);
                snn_require(size_eq(s, 8));
                snn_require(s == "10101110");

                s.template append_integral<base::binary>(-9);
                snn_require(size_eq(s, 13));
                snn_require(s == "10101110-1001");

                s.template append_integral<base::hex>(255);
                snn_require(size_eq(s, 15));
                snn_require(s == "10101110-1001ff");

                s.template append_integral<base::hex>(-16417712, 8);
                snn_require(size_eq(s, 24));
                snn_require(s == "10101110-1001ff-00fa83b0");

                s.template append_integral<base::octal>(8429);
                snn_require(size_eq(s, 29));
                snn_require(s == "10101110-1001ff-00fa83b020355");

                s.template append_integral<base::octal>(-1392856429);
                snn_require(size_eq(s, 41));
                snn_require(s == "10101110-1001ff-00fa83b020355-12301246555");

                s.template append_integral<base::octal>(constant::npos);
                snn_require(size_eq(s, 63));
                snn_require(s == "10101110-1001ff-00fa83b020355-12301246555"
                                 "1777777777777777777777");
            }
#if SNN_INT128_ENABLED
            {
                T s;
                s.append_integral(constant::limit<u128>::max);
                s.append('\n');
                s.template append_integral<math::base::hex>(constant::limit<u128>::max);
                s.append('\n');
                s.append_integral(constant::limit<i128>::min);
                s.append('\n');
                s.append_integral(constant::limit<i128>::max);
                s.append('\n');
                snn_require(s == "340282366920938463463374607431768211455\n"
                                 "ffffffffffffffffffffffffffffffff\n"
                                 "-170141183460469231731687303715884105728\n"
                                 "170141183460469231731687303715884105727\n");
            }
#endif

            // clear
            {
                T s{""};
                snn_require(size_eq(s, 0));
                s.clear();
                snn_require(size_eq(s, 0));
            }
            {
                T s{"abc"};
                snn_require(s == "abc");
                snn_require(size_eq(s, 3));
                s.clear();
                snn_require(size_eq(s, 0));
            }

            // grow
            {
                T s{"Short"};
                snn_require(s.capacity() == min_capacity);
                snn_require(size_eq(s, 5));

                s.reserve(200);

                snn_require(s.capacity() >= 200);
                snn_require(size_eq(s, 5));
                snn_require(s == "Short");
            }
            {
                T s;
                snn_require(s.capacity() == default_capacity);
                snn_require(size_eq(s, 0));

                s.reserve(default_capacity);
                snn_require(s.capacity() == default_capacity);
                snn_require(size_eq(s, 0));

                s.reserve(min_capacity);
                snn_require(s.capacity() == min_capacity);
                snn_require(size_eq(s, 0));
            }
            {
                T s;
                snn_require(s.capacity() == default_capacity);
                snn_require(size_eq(s, 0));

                s.reserve(200);
                snn_require(s.capacity() >= 200);
                snn_require(size_eq(s, 0));
            }
            {
                T s{init::reserve, 127};
                snn_require(s.capacity() >= 127);
                snn_require(size_eq(s, 0));

                const usize cap = s.capacity();
                s.reserve(cap + 1);
                snn_require(s.capacity() >= (cap + 1));
                snn_require(size_eq(s, 0));
            }
            {
                T s{init::fill, min_capacity, 'a'};
                snn_require(size_eq(s, min_capacity));
                snn_require(s.capacity() == min_capacity);

                s << 'b';
                snn_require(size_eq(s, min_capacity + 1));
                snn_require(s.capacity() >= (min_capacity + (min_capacity / 2)));
            }
            {
                T s;

                s.reserve_append(1);
                snn_require(s.capacity() == min_capacity);

                s.reserve_append(min_capacity);
                snn_require(s.capacity() == min_capacity);

                s << cstrview{"a"};
                snn_require(size_eq(s, 1));
                snn_require(s.capacity() == min_capacity);

                s.reserve_append(min_capacity);
                snn_require(size_eq(s, 1));
                snn_require(s.capacity() > (min_capacity + 1));
            }
            {
                T s{init::fill, min_capacity, 'a'};
                snn_require(size_eq(s, min_capacity));
                snn_require(s.capacity() == min_capacity);

                s.reserve_append(0);
                snn_require(size_eq(s, min_capacity));
                snn_require(s.capacity() == min_capacity);

                s.reserve_append(1);
                snn_require(size_eq(s, min_capacity));
                snn_require(s.capacity() > (min_capacity + 1));
            }

            // front/back
            {
                T s{"123"};
                snn_require(s.front(promise::not_empty) == '1');
                snn_require(s.back(promise::not_empty) == '3');
                snn_require(s == "123");

                s.front(promise::not_empty) = 'a';
                s.back(promise::not_empty)  = 'c';
                snn_require(s == "a2c");
            }
            {
                T s{"abcdefghijklmnopqrstuvwxyz"};
                snn_require(s.front(promise::not_empty) == 'a');
                snn_require(s.back(promise::not_empty) == 'z');

                s.front(promise::not_empty) = 'A';
                s.back(promise::not_empty)  = 'Z';
                snn_require(s == "AbcdefghijklmnopqrstuvwxyZ");
            }

            // drop_at
            {
                T s;

                s.drop_at(0, 0);
                snn_require(size_eq(s, 0));

                s.drop_at(99, 0);
                snn_require(size_eq(s, 0));

                s.drop_at(99, constant::npos);
                snn_require(size_eq(s, 0));

                s = "aaabbbcccdddeeefff";
                snn_require(s == "aaabbbcccdddeeefff");
                snn_require(size_eq(s, 18));

                s.drop_at(99, 0);
                snn_require(s == "aaabbbcccdddeeefff");
                snn_require(size_eq(s, 18));

                s.drop_at(99, 99);
                snn_require(s == "aaabbbcccdddeeefff");
                snn_require(size_eq(s, 18));

                s.drop_at(18, 1);
                snn_require(s == "aaabbbcccdddeeefff");
                snn_require(size_eq(s, 18));

                s.drop_at(17, 1);
                snn_require(s == "aaabbbcccdddeeeff");
                snn_require(size_eq(s, 17));

                s.drop_at(16, constant::npos);
                snn_require(s == "aaabbbcccdddeeef");
                snn_require(size_eq(s, 16));

                s.drop_at(12, constant::npos);
                snn_require(s == "aaabbbcccddd");
                snn_require(size_eq(s, 12));

                s.drop_at(0, 1);
                snn_require(s == "aabbbcccddd");
                snn_require(size_eq(s, 11));

                s.drop_at(0, 2);
                snn_require(s == "bbbcccddd");
                snn_require(size_eq(s, 9));

                s.drop_at(3, 3);
                snn_require(s == "bbbddd");
                snn_require(size_eq(s, 6));

                s = "abcdef";
                snn_require(s == "abcdef");
                snn_require(size_eq(s, 6));

                s.drop_at(1, 1);
                snn_require(s == "acdef");
                snn_require(size_eq(s, 5));

                s.drop_at(3, 1);
                snn_require(s == "acdf");
                snn_require(size_eq(s, 4));

                s.drop_at(0, constant::npos);
                snn_require(size_eq(s, 0));
            }

            // drop_at (at capacity)
            {
                T s{init::fill, min_capacity, 'a'};
                snn_require(size_eq(s, min_capacity));
                snn_require(s.capacity() == min_capacity);
                snn_require(s.all(fn::is{fn::equal_to{}, 'a'}));

                s.drop_at(7, 8);
                snn_require(size_eq(s, min_capacity - 8));
                snn_require(s.capacity() == min_capacity);
                snn_require(s.all(fn::is{fn::equal_to{}, 'a'}));
            }

            // insert_at
            {
                T s;

                s.insert_at(0, cstrview{""}, assume::no_overlap);
                snn_require(size_eq(s, 0));

                s.insert_at(0, "aaa");
                snn_require(s == "aaa");
                snn_require(size_eq(s, 3));

                s.clear();
                snn_require(size_eq(s, 0));

                s.insert_at(0, "aaa");
                snn_require(s == "aaa");
                snn_require(size_eq(s, 3));

                s.insert_at(3, "bbb");
                snn_require(s == "aaabbb");
                snn_require(size_eq(s, 6));

                s.insert_at(0, "c");
                snn_require(s == "caaabbb");
                snn_require(size_eq(s, 7));

                s.insert_at(0, "dd");
                snn_require(s == "ddcaaabbb");
                snn_require(size_eq(s, 9));

                s.insert_at(3, "e");
                snn_require(s == "ddceaaabbb");
                snn_require(size_eq(s, 10));

                s.insert_at(5, "fff");
                snn_require(s == "ddceafffaabbb");
                snn_require(size_eq(s, 13));

                T s2{"123456789012345678901234567890"};
                s.insert_at(6, s2, assume::no_overlap);
                snn_require(s == "ddceaf123456789012345678901234567890ffaabbb");
                snn_require(size_eq(s, 43));

                snn_require_throws_code(s.insert_at(44, s2, assume::no_overlap),
                                        generic::error::invalid_position_for_replace_or_insert);
                snn_require_throws_code(s.insert_at(44, "abc"),
                                        generic::error::invalid_position_for_replace_or_insert);
            }

            // insert_at (at capacity)
            {
                T s{init::fill, min_capacity - 8, 'a'};
                snn_require(size_eq(s, min_capacity - 8));
                snn_require(s.capacity() == min_capacity);
                snn_require(s.all(fn::is{fn::equal_to{}, 'a'}));

                s.insert_at(7, "abcdefgh");
                snn_require(size_eq(s, min_capacity));
                snn_require(s.capacity() == min_capacity);
                snn_require(s.has_front("aaaaaaa"));
                snn_require(s.view(7, 8) == "abcdefgh");
                const auto t = s.view(15);
                snn_require(size_eq(t, min_capacity - 15));
                snn_require(t.all(fn::is{fn::equal_to{}, 'a'}));
            }

            // replace_at
            {
                T s{"One Two Three"};
                snn_require(size_eq(s, 13));

                s.replace_at(0, 3, "ABC");
                snn_require(s == "ABC Two Three");
                snn_require(size_eq(s, 13));

                s.replace_at(0, 3, "FOOBAR");
                snn_require(s == "FOOBAR Two Three");
                snn_require(size_eq(s, 16));

                s.replace_at(0, 7, "");
                snn_require(s == "Two Three");
                snn_require(size_eq(s, 9));

                s.replace_at(3, 1, "|");
                snn_require(s == "Two|Three");
                snn_require(size_eq(s, 9));

                s.replace_at(3, 1, " | ");
                snn_require(s == "Two | Three");
                snn_require(size_eq(s, 11));

                s.replace_at(6, 5, "Four");
                snn_require(s == "Two | Four");
                snn_require(size_eq(s, 10));

                s.replace_at(6, 99, "One");
                snn_require(s == "Two | One");
                snn_require(size_eq(s, 9));

                s.replace_at(0, 9, "");
                snn_require(size_eq(s, 0));

                s.replace_at(0, 0, "One Two");
                snn_require(s == "One Two");
                snn_require(size_eq(s, 7));
            }
            {
                T s{"One Two Three"};
                snn_require(size_eq(s, 13));
                snn_require(s.capacity() == min_capacity);

                T tmp{init::fill, min_capacity, '1'};
                s.replace_at(4, 3, tmp, assume::no_overlap);
                snn_require(size_eq(s, min_capacity + 10));
                snn_require(s.capacity() >= (min_capacity + 10));
                snn_require(s.has_front("One "));
                snn_require(s.has_back(" Three"));
                snn_require(s.view(4, min_capacity) == tmp);
            }

            // replace
            {
                T s{"One Two Three"};
                snn_require(size_eq(s, 13));

                snn_require(s.replace("", "") == 0);
                snn_require(s == "One Two Three");
                snn_require(size_eq(s, 13));

                snn_require(s.replace('e', '_', 8) == 2);
                snn_require(s == "One Two Thr__");
                snn_require(size_eq(s, 13));

                snn_require(s.replace('_', 'e') == 2);
                snn_require(s == "One Two Three");
                snn_require(size_eq(s, 13));

                snn_require(s.replace('T', 'Q') == 2);
                snn_require(s == "One Qwo Qhree");
                snn_require(size_eq(s, 13));

                snn_require(s.replace(s.view(4, 1), "TTT") == 2);
                snn_require(s == "One TTTwo TTThree");
                snn_require(size_eq(s, 17));

                s = "OneOneOne";
                snn_require(s == "OneOneOne");
                snn_require(size_eq(s, 9));

                snn_require(s.replace("One", "Three") == 3);
                snn_require(s == "ThreeThreeThree");
                snn_require(size_eq(s, 15));

                snn_require(s.replace("Three", "Three") == 3);
                snn_require(s == "ThreeThreeThree");
                snn_require(size_eq(s, 15));

                snn_require(s.replace("Three", "") == 3);
                snn_require(size_eq(s, 0));

                s = "aaaaaaa";
                snn_require(s == "aaaaaaa");
                snn_require(size_eq(s, 7));
                snn_require(s.replace('a', 'b') == 7);
                snn_require(s == "bbbbbbb");
                snn_require(size_eq(s, 7));

                s.clear();
                snn_require(s.replace("ab", "cd") == 0);
                snn_require(size_eq(s, 0));
                snn_require(s.replace('a', 'b') == 0);
                snn_require(size_eq(s, 0));

                s = "One Two Three";
                snn_require(s.replace("One Two Three", "123") == 1);
                snn_require(s == "123");
                snn_require(size_eq(s, 3));

                s = "1234, 1234, 1234, 1234";
                snn_require(s.replace("1234", "One Two Three Four") == 4);
                snn_require(s == "One Two Three Four, One Two Three Four, "
                                 "One Two Three Four, One Two Three Four");
                snn_require(size_eq(s, 78));

                snn_require(s.replace("One Two Three", "Three") == 4);
                snn_require(s == "Three Four, Three Four, Three Four, Three Four");
                snn_require(size_eq(s, 46));

                snn_require(s.replace(", ", "") == 3);
                snn_require(s == "Three FourThree FourThree FourThree Four");
                snn_require(size_eq(s, 40));

                snn_require(s.replace("Four", "") == 4);
                snn_require(s == "Three Three Three Three ");
                snn_require(size_eq(s, 24));

                snn_require(s.replace(" ", "") == 4);
                snn_require(s == "ThreeThreeThreeThree");
                snn_require(size_eq(s, 20));

                snn_require(s.replace(s.view_offset(-5), "Four", 1) == 3);
                snn_require(s == "ThreeFourFourFour");
                snn_require(size_eq(s, 17));
            }

            // remove
            {
                T s{"One OneOneTwo Three"};
                snn_require(size_eq(s, 19));

                snn_require(s.remove("One") == 3);
                snn_require(s == " Two Three");
                snn_require(size_eq(s, 10));

                snn_require(s.remove(" ") == 2);
                snn_require(s == "TwoThree");
                snn_require(size_eq(s, 8));

                snn_require(s.remove("") == 0);
                snn_require(s == "TwoThree");
                snn_require(size_eq(s, 8));

                snn_require(s.remove('T') == 2);
                snn_require(s == "wohree");
                snn_require(size_eq(s, 6));

                snn_require(s.remove('e') == 2);
                snn_require(s == "wohr");
                snn_require(size_eq(s, 4));

                snn_require(s.remove('a') == 0);
                snn_require(s == "wohr");
                snn_require(size_eq(s, 4));
            }

            // reset
            {
                T s;
                s.reset();
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);
            }
            {
                T s{"One Two Three"};
                snn_require(size_eq(s, 13));
                snn_require(s.capacity() == min_capacity);

                T s2{std::move(s)};
                snn_require(size_eq(s2, 13));
                snn_require(s2.capacity() == min_capacity);

                // A move resets.
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);

                s.reset();
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);

                s2.reset();
                snn_require(size_eq(s2, 0));
                snn_require(s2.capacity() == default_capacity);
            }
            {
                T s{init::fill, min_capacity + 1, 'a'};
                snn_require(size_eq(s, min_capacity + 1));
                snn_require(s.capacity() > min_capacity);
                snn_require(s.all(fn::is{fn::equal_to{}, 'a'}));

                s.reset();
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);
            }

            // resize
            {
                T s;

                s = "abcdef";

                s.resize(3, '_');
                snn_require(s == "abc");

                s.resize(6, '_');
                snn_require(s == "abc___");

                s.resize(20, 'A');
                snn_require(s == "abc___AAAAAAAAAAAAAA");

                s.resize(30, 'b');
                snn_require(s == "abc___AAAAAAAAAAAAAAbbbbbbbbbb");

                s.resize(10, 'x');
                snn_require(s == "abc___AAAA");

                s.resize(0, 'x');
                snn_require(s == "");
            }
            {
                T s;

                s.resize(0, 'a');
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);

                for (usize i = 1; i <= min_capacity; ++i)
                {
                    s.resize(i, 'a');
                    snn_require(size_eq(s, i));
                    snn_require(s.capacity() == min_capacity);
                }

                s.resize(min_capacity + 1, 'a');
                snn_require(size_eq(s, min_capacity + 1));
                const usize cap = s.capacity();
                snn_require(cap > min_capacity);

                for (usize i = 0; i <= cap; ++i)
                {
                    s.resize(i, 'a');
                    snn_require(size_eq(s, i));
                    snn_require(s.capacity() == cap);
                }

                s.resize(100, 'a');
                snn_require(size_eq(s, 100));
                snn_require(s.capacity() >= 100);

                s.resize(200, 'a');
                snn_require(size_eq(s, 200));
                snn_require(s.capacity() >= 200);
            }

            // swap
            {
                T s1{"abcdefghijklmnopqrstuvwxyz"};
                T s2{"One Two Three"};

                snn_require(s1 == "abcdefghijklmnopqrstuvwxyz");
                snn_require(s2 == "One Two Three");

                swap(s1, s2);

                snn_require(s1 == "One Two Three");
                snn_require(s2 == "abcdefghijklmnopqrstuvwxyz");

                s1.swap(s2);

                snn_require(s1 == "abcdefghijklmnopqrstuvwxyz");
                snn_require(s2 == "One Two Three");

                // Swap with self.
                s1.swap(s1);
                snn_require(s1 == "abcdefghijklmnopqrstuvwxyz");
                std::swap(s1, s1);
                snn_require(s1 == "abcdefghijklmnopqrstuvwxyz");
            }

            // each
            {
                T s{"One Two Three"};

                s.each(chr::to_alpha_upper_inplace);
                snn_require(size_eq(s, 13));
                snn_require(s == "ONE TWO THREE");

                s.view(0, 7).each(chr::to_alpha_lower_inplace);
                snn_require(size_eq(s, 13));
                snn_require(s == "one two THREE");
            }

            // transform
            {
                T s{"One Two Three"};

                s.transform(chr::to_alpha_upper);
                snn_require(size_eq(s, 13));
                snn_require(s == "ONE TWO THREE");

                s.view(0, 7).transform(chr::to_alpha_lower);
                snn_require(size_eq(s, 13));
                snn_require(s == "one two THREE");
            }

            // all
            {
                snn_require(T{""}.all(chr::is_alpha_lower));
                snn_require(T{"a"}.all(chr::is_alpha_lower));
                snn_require(T{"ab"}.all(chr::is_alpha_lower));
                snn_require(!T{"Ab"}.all(chr::is_alpha_lower));
                snn_require(!T{"aB"}.all(chr::is_alpha_lower));
                snn_require(!T{"AB"}.all(chr::is_alpha_lower));
            }

            // any
            {
                snn_require(!T{""}.any(chr::is_alpha_lower));
                snn_require(T{"a"}.any(chr::is_alpha_lower));
                snn_require(T{"ab"}.any(chr::is_alpha_lower));
                snn_require(T{"Ab"}.any(chr::is_alpha_lower));
                snn_require(T{"aB"}.any(chr::is_alpha_lower));
                snn_require(!T{"AB"}.any(chr::is_alpha_lower));
            }

            // none
            {
                snn_require(T{""}.none(chr::is_alpha_lower));
                snn_require(!T{"a"}.none(chr::is_alpha_lower));
                snn_require(!T{"ab"}.none(chr::is_alpha_lower));
                snn_require(!T{"Ab"}.none(chr::is_alpha_lower));
                snn_require(!T{"aB"}.none(chr::is_alpha_lower));
                snn_require(T{"AB"}.none(chr::is_alpha_lower));
            }

            // operator=
            {
                T s;

                s = "";
                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);

                s = "One";
                snn_require(s == "One");
                snn_require(size_eq(s, 3));
                snn_require(s.capacity() == min_capacity);

                s = 'a';
                snn_require(s == "a");
                snn_require(size_eq(s, 1));

                s = str{"Two"};
                snn_require(s == "Two");
                snn_require(size_eq(s, 3));

                s = strbuf{"Two"};
                snn_require(s == "Two");
                snn_require(size_eq(s, 3));

                T s2 = s = "Three";
                snn_require(s == "Three");
                snn_require(s2 == "Three");
                snn_require(s == s2);
                snn_require(size_eq(s, 5));
                snn_require(size_eq(s2, 5));

                s = "";
                snn_require(s == "");
                snn_require(size_eq(s, 0));

                s = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                    "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
                snn_require(size_eq(s, 104));
                snn_require(s.capacity() >= 104);
                snn_require(s == "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                                 "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");

                // Self assignment.
                indirect_self_assignment(s, s);
                snn_require(size_eq(s, 104));
                snn_require(s.capacity() >= 104);
                snn_require(s == "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                                 "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");

                // Self assignment (indirect).
                s = s.view();
                snn_require(size_eq(s, 104));
                snn_require(s.capacity() >= 104);
                snn_require(s == "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                                 "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");

                s = s.view(1);
                snn_require(size_eq(s, 103));
                snn_require(s.capacity() >= 104);
                snn_require(s == "bcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                                 "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");

                s = s2;
                snn_require(size_eq(s, 5));
                snn_require(s == "Three");

                s = "";
                snn_require(s == "");
                snn_require(size_eq(s, 0));
            }
            {
                T s;

                s = {'a'};
                snn_require(size_eq(s, 1));
                snn_require(s == "a");

                s = {'b', '4', '2'};
                snn_require(size_eq(s, 3));
                snn_require(s == "b42");

                s = init_list<char>{}; // Empty init-list.
                snn_require(size_eq(s, 0));
            }

            // operator=(init_list<int>)
            {
                // The following should not compile:
                // T s;
                // s = {65};
                // s = {65u};
                // s = {65, 97};
                // s = {{}};
                // s = {{}, {}};
            }

            // at(pos, assume::within_bounds_t)
            {
                T s{"One Two Three"};

                s.at(0, assume::within_bounds) = 'o';
                snn_require(s == "one Two Three");

                s.at(4, assume::within_bounds) = 't';
                s.at(8, assume::within_bounds) = 't';
                snn_require(s == "one two three");

                snn_require(s.at(1, assume::within_bounds) == 'n');
                snn_require(s.at(8, assume::within_bounds) == 't');
            }

            // at
            {
                T s{"One"};
                s.at(0).value() = 'e';
                s.at(2).value() = 'O';
                snn_require(s == "enO");
                snn_require_throws_code(s.at(3).value() = 's', generic::error::no_value);
                snn_require(s == "enO");
            }

            // << as_num(...)
            {
                T s;

                s << as_num(923829732);
                snn_require(size_eq(s, 9));
                snn_require(s == "923829732");

                s << as_num<i8>(-42);
                snn_require(size_eq(s, 12));
                snn_require(s == "923829732-42");

                s << as_num<u8>(1);
                snn_require(size_eq(s, 13));
                snn_require(s == "923829732-421");

                s << as_num<usize>(9223372036854775807u) << as_num<i64>(-2382);
                snn_require(size_eq(s, 37));
                snn_require(s == "923829732-4219223372036854775807-2382");

                s << 'Q';
                snn_require(size_eq(s, 38));
                snn_require(s == "923829732-4219223372036854775807-2382Q");

                s << ' ';
                snn_require(size_eq(s, 39));
                snn_require(s == "923829732-4219223372036854775807-2382Q ");

                s << as_num(promote(true));
                snn_require(size_eq(s, 40));
                snn_require(s == "923829732-4219223372036854775807-2382Q 1");

                s << as_num(promote(false));
                snn_require(size_eq(s, 41));
                snn_require(s == "923829732-4219223372036854775807-2382Q 10");

                s << ' ';
                snn_require(size_eq(s, 42));
                snn_require(s == "923829732-4219223372036854775807-2382Q 10 ");

                s << as_num<usize>(18446744073709551615u);
                snn_require(size_eq(s, 62));
                snn_require(s == "923829732-4219223372036854775807-2382Q 10 18446744073709551615");

                s << as_num<i64>(constant::limit<i64>::min);
                snn_require(size_eq(s, 82));
                snn_require(s == "923829732-4219223372036854775807-2382Q 10"
                                 " 18446744073709551615-9223372036854775808");

                s << as_num(0);
                snn_require(size_eq(s, 83));
                snn_require(s == "923829732-4219223372036854775807-2382Q 10"
                                 " 18446744073709551615-92233720368547758080");
            }

            // shrink_to_fit
            {
                T s{"Short"};
                snn_require(s.capacity() == min_capacity);
                snn_require(size_eq(s, 5));
                snn_require(s == "Short");

                s.shrink_to_fit();
                snn_require(s.capacity() == min_capacity);

                s.reserve(2000);
                snn_require(s.capacity() >= 2000);
                snn_require(size_eq(s, 5));
                snn_require(s == "Short");

                s = "0123456789abcdefghijklmnopqrstuvwxyz!@ABCDEFGHIJKLMNOPQRSTUVWXYZ#$";
                snn_require(s.capacity() >= 2000);
                snn_require(size_eq(s, 66));
                snn_require(s == "0123456789abcdefghijklmnopqrstuvwxyz!@"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ#$");

                s.shrink_to_fit();
                snn_require(s.capacity() >= 66);
                snn_require(s.capacity() < 2000);
                snn_require(size_eq(s, 66));
                snn_require(s == "0123456789abcdefghijklmnopqrstuvwxyz!@"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ#$");

                s = "OneTwo";
                snn_require(s.capacity() >= 66);
                snn_require(s.capacity() < 2000);
                snn_require(size_eq(s, 6));
                snn_require(s == "OneTwo");

                s.shrink_to_fit();
                snn_require(s.capacity() == min_capacity);
                snn_require(size_eq(s, 6));
                snn_require(s == "OneTwo");
            }
            {
                T s;
                snn_require(s.capacity() == default_capacity);
                snn_require(size_eq(s, 0));

                s.shrink_to_fit();
                snn_require(s.capacity() == default_capacity);

                s.reserve(2000);
                snn_require(s.capacity() >= 2000);
                snn_require(size_eq(s, 0));

                s.shrink_to_fit();
                snn_require(s.capacity() == default_capacity);
                snn_require(size_eq(s, 0));
            }

            // writable
            {
                T s;

                const char* a = s.data().get();
                char* b       = s.writable().get();
                snn_require(a == b);

                s = "abc";

                a = s.data().get();
                b = s.writable().get();
                snn_require(a == b);

                s = "abcdefghijklmnopqrstuvwxyz";

                a = s.data().get();
                b = s.writable().get();
                snn_require(a == b);
            }

            // resize_for_overwrite
            {
                T s;

                const char* first = s.data().get();
                char* dest        = s.resize_for_overwrite(0).writable().get(); // No reallocation.
                snn_require(dest == first);

                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);

                mem::raw::copy<10>(not_null{"0123456789"}, s.resize_for_overwrite(10).writable(),
                                   assume::no_overlap);
                snn_require(size_eq(s, 10));
                snn_require(s.capacity() == min_capacity);
                snn_require(s == "0123456789");

                cstrview tmp{
                    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ9876543210_-<>()"};
                mem::raw::copy(tmp.data(), s.resize_for_overwrite(tmp.size()).writable(),
                               tmp.byte_size(), assume::no_overlap);
                snn_require(size_eq(s, 68));
                snn_require(s.capacity() >= 68);
                snn_require(s == "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "9876543210_-<>()");

                first = s.data().get();
                dest  = s.resize_for_overwrite(s.capacity()).writable().get(); // No reallocation.
                snn_require(dest == first);
            }

            // append_for_overwrite(size_increase)
            {
                T s;

                {
                    const char* const last        = s.end();
                    const char* const append_dest = s.append_for_overwrite(0).begin();
                    snn_require(append_dest == last);
                }

                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);

                strview buffer = s.append_for_overwrite(10);
                snn_require(buffer.size() == 10);
                mem::raw::copy<10>(not_null{"0123456789"}, buffer.writable(), assume::no_overlap);
                snn_require(size_eq(s, 10));
                snn_require(s.capacity() == min_capacity);
                snn_require(s == "0123456789");

                const cstrview tmp{"abcdefghijklmnopqrstuvwxyz!@ABCDEFGHIJKLMNOPQRSTUVWXYZ#$"};
                buffer = s.append_for_overwrite(tmp.size());
                snn_require(buffer.size() == tmp.size());
                mem::raw::copy(tmp.data(), buffer.writable(), tmp.byte_size(), assume::no_overlap);
                snn_require(size_eq(s, 66));
                snn_require(s.capacity() >= 66);
                snn_require(s == "0123456789abcdefghijklmnopqrstuvwxyz!@"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ#$");

                {
                    const char* const last        = s.end();
                    const char* const append_dest = s.append_for_overwrite(0).begin();
                    snn_require(append_dest == last);
                }
            }
            {
                T s{init::fill, min_capacity, 'a'};
                snn_require(size_eq(s, min_capacity));
                snn_require(s.capacity() == min_capacity);

                const char* const last        = s.end();
                const char* const append_dest = s.append_for_overwrite(0).begin();
                snn_require(append_dest == last);

                snn_require(size_eq(s, min_capacity));
                snn_require(s.capacity() == min_capacity);
            }

            // append_for_overwrite<SizeIncrease>()
            {
                T s = "abc";

                {
                    auto v = s.template append_for_overwrite<3>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 3>>);
                    v.fill('D');
                }

                snn_require(s == "abcDDD");

                s.template append_for_overwrite<2>().fill('e');
                s.template append_for_overwrite<1>().fill('F');

                snn_require(s == "abcDDDeeF");

                {
                    const char* const last_before = s.end();

                    auto v = s.template append_for_overwrite<0>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 0>>);

                    snn_require(v.begin() == last_before);
                }

                snn_require(s == "abcDDDeeF");
            }

            // insert_for_overwrite
            {
                T s;

                {
                    const char* const first = s.begin();
                    const char* const dest  = s.insert_for_overwrite(0, 0).begin();
                    snn_require(dest == first);
                }

                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);

                strview buf = s.insert_for_overwrite(0, 10);
                snn_require(buf.size() == 10);
                mem::raw::copy<10>(not_null{"0123456789"}, buf.writable(), assume::no_overlap);
                snn_require(size_eq(s, 10));
                snn_require(s.capacity() == min_capacity);
                snn_require(s == "0123456789");

                const cstrview tmp{"abcdefghijklmnopqrstuvwxyz!@ABCDEFGHIJKLMNOPQRSTUVWXYZ#$"};
                buf = s.insert_for_overwrite(5, tmp.size());
                snn_require(buf.size() == tmp.size());
                mem::raw::copy(tmp.data(), buf.writable(), tmp.byte_size(), assume::no_overlap);
                snn_require(size_eq(s, 66));
                snn_require(s.capacity() >= 66);
                snn_require(s == "01234abcdefghijklmnopqrstuvwxyz!@"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ#$56789");

                {
                    const char* const first = s.begin();
                    const char* const dest  = s.insert_for_overwrite(0, 0).begin();
                    snn_require(dest == first);
                }
            }

            // replace_for_overwrite
            {
                T s;

                {
                    const char* const first = s.begin();
                    const char* const dest  = s.replace_for_overwrite(0, 0, 0).begin();
                    snn_require(dest == first);
                }

                snn_require(size_eq(s, 0));
                snn_require(s.capacity() == default_capacity);

                strview buf = s.replace_for_overwrite(0, 0, 10);
                snn_require(buf.size() == 10);
                mem::raw::copy<10>(not_null{"0123456789"}, buf.writable(), assume::no_overlap);
                snn_require(size_eq(s, 10));
                snn_require(s.capacity() == min_capacity);
                snn_require(s == "0123456789");

                const cstrview tmp{"abcdefghijklmnopqrstuvwxyz!@ABCDEFGHIJKLMNOPQRSTUVWXYZ#$"};
                buf = s.replace_for_overwrite(3, 4, tmp.size());
                snn_require(buf.size() == tmp.size());
                mem::raw::copy(tmp.data(), buf.writable(), tmp.byte_size(), assume::no_overlap);
                snn_require(size_eq(s, 62));
                snn_require(s.capacity() >= 62);
                snn_require(s == "012abcdefghijklmnopqrstuvwxyz!@"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ#$789");

                {
                    const char* const first = s.begin();
                    const char* const dest  = s.replace_for_overwrite(0, 0, 0).begin();
                    snn_require(dest == first);
                }
            }
            {
                T s = "abcdef";
                s.replace_for_overwrite(2, 2, 1).fill('X');
                snn_require(s == "abXef");
            }
            {
                T s = "abcdefghijklmnopqrstuvwxyz";
                s.replace_for_overwrite(3, 10, 4).fill('X');
                snn_require(s == "abcXXXXnopqrstuvwxyz");
            }
            {
                T s{init::fill, min_capacity, 'a'};
                snn_require(size_eq(s, min_capacity));
                snn_require(s.capacity() == min_capacity);

                const char* const first = s.begin();
                const char* const dest  = s.replace_for_overwrite(0, 0, 0).begin();
                snn_require(dest == first);

                snn_require(size_eq(s, min_capacity));
                snn_require(s.capacity() == min_capacity);
            }

            // 0-byte
            {
                T s;

                s << '\0';

                snn_require(size_eq(s, 1));
                snn_require(s.at(0).value() == 0);
                snn_require(s == "\0");

                s << '\0';
                s << '\0';
                s << '\0';
                s << '\0';
                s << '\0';

                snn_require(size_eq(s, 6));
                snn_require(s.at(0).value() == 0);
                snn_require(s.at(1).value() == 0);
                snn_require(s.at(2).value() == 0);
                snn_require(s.at(3).value() == 0);
                snn_require(s.at(4).value() == 0);
                snn_require(s.at(5).value() == 0);
                snn_require(s == "\0\0\0\0\0\0");

                const cstrview tmp{"\0\0\0\0\0\0"};
                snn_require(tmp.size() == 6);
                snn_require(s == tmp);
            }

            return true;
        }

        constexpr bool test_str()
        {
            // null_terminated
            {
                str s{"abc"};
                snn_require(size_eq(s, 3));
                snn_require(s.at(0).value() == 'a');
                snn_require(s.at(1).value() == 'b');
                snn_require(s.at(2).value() == 'c');

                auto nt = s.null_terminated();
                static_assert(std::is_same_v<decltype(nt), null_term<const char*>>);
                snn_require(nt.to<cstrview>().size() == 3);

                s.at(1).value() = '\0';
                snn_require_throws_code(s.null_terminated(),
                                        generic::error::unexpected_null_character);

                s.view().fill('a');
                nt = s.null_terminated(promise::is_valid);
                snn_require(nt.to<cstrview>().size() == 3);
            }

            return true;
        }

        bool test_find_rnd()
        {
            constexpr usize size = 64'000;

            str contents = file::read("strcore.test.cc").value();
            snn_require(contents.size() >= size);
            contents.truncate(size);

            snn_require(contents.find(contents).value_or_npos() == 0);

            // Longer haystack & needle.
            for (loop::count lc{1000}; lc--;)
            {
                const auto haystack_pos      = random::number<usize>(0, 100);
                const auto haystack_max_size = random::number<usize>(0, size);
                const cstrview haystack      = contents.view(haystack_pos, haystack_max_size);
                const std::string_view alt_haystack{haystack.data().get(), haystack.size()};

                const auto needle_pos      = random::number<usize>(0, size - 300);
                const auto needle_max_size = random::number<usize>(0, 300);
                const cstrview needle      = contents.view(needle_pos, needle_max_size);
                const std::string_view alt_needle{needle.data().get(), needle.size()};

                auto start_pos = random::number<usize>(0, 100);
                usize pos1     = haystack.find(needle, start_pos).value_or_npos();
                usize pos2     = alt_haystack.find(alt_needle, start_pos);
                snn_require(pos1 == pos2);

                start_pos = random::number<usize>(0, size);
                pos1      = haystack.find_in_reverse(needle, start_pos).value_or_npos();
                pos2      = alt_haystack.rfind(alt_needle, start_pos);
                snn_require(pos1 == pos2);
            }

            // Shorter haystack & needle.
            for (loop::count lc{1000}; lc--;)
            {
                const auto haystack_pos      = random::number<usize>(0, size - 1'500);
                const auto haystack_max_size = random::number<usize>(0, 1'500);
                const cstrview haystack      = contents.view(haystack_pos, haystack_max_size);
                const std::string_view alt_haystack{haystack.data().get(), haystack.size()};

                const auto needle_pos      = random::number<usize>(0, size - 50);
                const auto needle_max_size = random::number<usize>(0, 50);
                const cstrview needle      = contents.view(needle_pos, needle_max_size);
                const std::string_view alt_needle{needle.data().get(), needle.size()};

                auto start_pos = random::number<usize>(0, 10);
                usize pos1     = haystack.find(needle, start_pos).value_or_npos();
                usize pos2     = alt_haystack.find(alt_needle, start_pos);
                snn_require(pos1 == pos2);

                start_pos = random::number<usize>(0, size);
                pos1      = haystack.find_in_reverse(needle, start_pos).value_or_npos();
                pos2      = alt_haystack.rfind(alt_needle, start_pos);
                snn_require(pos1 == pos2);
            }

            return true;
        }

        bool test_find_byte_rnd()
        {
            constexpr usize size = 8'000;

            auto buf = random::string<strbuf>(size);

            for (loop::count lc{1000}; lc--;)
            {
                const auto haystack_pos      = random::number<usize>(0, 100);
                const auto haystack_max_size = random::number<usize>(0, size);
                const cstrview haystack      = buf.view(haystack_pos, haystack_max_size);
                const std::string_view alt_haystack{haystack.data().get(), haystack.size()};

                const char needle = random::number<char>();

                auto start_pos = random::number<usize>(0, 100);
                usize pos1     = haystack.find(needle, start_pos).value_or_npos();
                usize pos2     = alt_haystack.find(needle, start_pos);
                snn_require(pos1 == pos2);

                start_pos = random::number<usize>(0, size);
                pos1      = haystack.find_in_reverse(needle, start_pos).value_or_npos();
                pos2      = alt_haystack.rfind(needle, start_pos);
                snn_require(pos1 == pos2);
            }

            return true;
        }

        template <typename T>
        constexpr bool test_comparison()
        {
            {
                T a{"abcd"};
                T b{"abcd"};
                T c{"abcD"};

                snn_require(a == b);
                snn_require(!(b == c));
                snn_require(a == "abcd");
                snn_require("abcd" == a);
                snn_require(!(a == "abcD"));
                snn_require(!("abcD" == a));
                snn_require(a == cstrview{"abcd"});
                snn_require(cstrview{"abcd"} == a);
                snn_require(!(a == cstrview{"abcD"}));
                snn_require(!(cstrview{"abcD"} == a));
            }
            {
                T a{"abcd"};
                T b{"abcd"};
                T c{"abcD"};

                snn_require(b != c);
                snn_require(!(a != b));
                snn_require(a != "abcD");
                snn_require("abcD" != a);
                snn_require(!(a != "abcd"));
                snn_require(!("abcd" != a));
                snn_require(a != cstrview{"abcD"});
                snn_require(cstrview{"abcD"} != a);
                snn_require(!(a != cstrview{"abcd"}));
                snn_require(!(cstrview{"abcd"} != a));
            }
            {
                T a{"abcd"};
                T b{"abc"};
                T c{"ef"};

                snn_require(a < c);
                snn_require(!(a < a));
                snn_require(!(a < b));
                snn_require(a < "bb");
                snn_require("aa" < a);
                snn_require(!(a < "abcd"));
                snn_require(!(a < "123"));
                snn_require(!("abcd" < a));
                snn_require(!("xyz" < a));
            }
            {
                T a{"abcd"};
                T b{"abc"};

                snn_require(a > b);
                snn_require(!(a > a));
                snn_require(!(b > a));
                snn_require(a > "aa");
                snn_require("bb" > a);
                snn_require(!(a > "abcd"));
                snn_require(!(a > "xyz"));
                snn_require(!("abcd" > a));
                snn_require(!("123" > a));
            }
            {
                T a{"abcd"};
                T b{"abc"};
                T c{"ef"};

                snn_require(a <= c);
                snn_require(a <= a);
                snn_require(!(a <= b));
                snn_require(a <= "bb");
                snn_require(a <= "abcd");
                snn_require("aa" <= a);
                snn_require("abcd" <= a);
                snn_require(!(a <= "aa"));
                snn_require(!("bb" <= a));
            }
            {
                T a{"abcd"};
                T b{"abc"};
                T c{"ef"};

                snn_require(c >= a);
                snn_require(a >= a);
                snn_require(!(b >= a));
                snn_require(a >= "aa");
                snn_require(a >= "abcd");
                snn_require("bb" >= a);
                snn_require("abcd" >= a);
                snn_require(!(a >= "bb"));
                snn_require(!("aa" >= a));
            }

            return true;
        }

        constexpr bool test_hash()
        {
            constexpr array<usize, 256> hashes{
                894299094737143437u,   6288834997689259448u,  12075468384263384979u,
                17986011877694212690u, 13393073444536470905u, 17179247011643032329u,
                17418626563588101342u, 11703514117353984575u, 15411987223479858182u,
                13468731722614080161u, 5378572830995800063u,  2402494289382923228u,
                12463911394174227577u, 1011781009512126791u,  14877034289787104010u,
                18414916305938547154u, 7034401305567905177u,  7643742019486170884u,
                15621475306399270651u, 12163651776831689998u, 14808360341757365389u,
                4749263509373213952u,  9247018564633216290u,  11323675157921209402u,
                2859700462125450624u,  14882238070288950456u, 2192404833132388154u,
                593871733331593377u,   8778291414581187896u,  9617190707249721311u,
                13099102146860227022u, 7173304178017026190u,  188800643931297464u,
                14977219473281709286u, 5152272828145679687u,  4066525655875567756u,
                8824692270735996388u,  1308279908036033190u,  2838565433860722913u,
                1808377088858220390u,  6558709565158356186u,  1648771344345529694u,
                6949546038097145245u,  11929007907997256985u, 8397636325397273152u,
                7353033265757382049u,  15739596753942252066u, 8265029971140615336u,
                9278679942810108865u,  706229336943583572u,   18093024539576720327u,
                8356642386295377804u,  17019005168728692959u, 10745660387304040277u,
                14218442561674477269u, 5671304642231997436u,  17641249055521883096u,
                2481695319570886548u,  7192692656796588680u,  16278039446515725640u,
                4625663088087573665u,  3786786968548875789u,  8197811581050490133u,
                16529466968686500138u, 15400782843305873524u, 2400135546673933001u,
                13841784660623617090u, 11549540672964907004u, 9476252178893074228u,
                18205314639654215105u, 16379118860471226576u, 2389000856964185630u,
                12221355770088832760u, 17998616387856337904u, 16056706730289347493u,
                5300227297807876147u,  5207428187340024429u,  17428058523020785268u,
                16589238243032413462u, 15332338356199078564u, 10487644792012314600u,
                11829761622842905446u, 4417403317404803986u,  5554419482037121102u,
                8165902466320925460u,  8297865478600584233u,  17228171126324659157u,
                10399292866793828193u, 15736536043257570901u, 14740006186982764812u,
                14320587685121010665u, 10031463095315459401u, 7297283605813218793u,
                1489099108801110675u,  16583819269923384413u, 12512359484601065899u,
                5021256001343290469u,  2531900585268751432u,  9181931113760444130u,
                7651720467242209474u,  12220109040603751445u, 15668159815940814110u,
                8189732125765662222u,  12888129238151499083u, 4427242625667076769u,
                3274150487722006282u,  7942843347128085593u,  7003579340962719964u,
                9940722852374718637u,  1262045945453884357u,  12763759170156348415u,
                12478689987284848188u, 10482448043577337694u, 3966488015587306733u,
                11460641527186493844u, 6868228731909122983u,  15465397741467074217u,
                15295165298269976204u, 12147750758165112631u, 16138647801652631735u,
                16203797687755118508u, 4636972776252349972u,  17480179161334486720u,
                17518388547923427756u, 11577674163668004200u, 11848547475319652652u,
                5123804704029457567u,  5097669210588415856u,  1202834260335129493u,
                17282022855302389141u, 13034389804238642931u, 10211064799508399089u,
                10432026871399874019u, 16026755172712111374u, 6302158474829298170u,
                2260583935679101347u,  7001793786790109268u,  13516292111413788278u,
                4488179162024237572u,  3210434025964027265u,  14573580414431855104u,
                7365713933907296010u,  18003480115279504391u, 16154947381989993267u,
                14031886831280299731u, 10809801848161072381u, 15178910036772559806u,
                5989638100417143996u,  11587253027730107649u, 10294219825600332053u,
                8943931193848644945u,  16412103355447457900u, 830814054897026754u,
                4741264366937614131u,  9132819821668885553u,  15429069710275684325u,
                11853448098870414534u, 12652173959994609151u, 8268442247828559279u,
                12777866850548083178u, 16426542526361249470u, 3912192630192643044u,
                13392185919096853369u, 6012082827380234497u,  11957840334385793035u,
                14801593173195926112u, 15091947838930978355u, 7607362424232991178u,
                18436860322082134927u, 13871802228347606322u, 5534676620576843972u,
                9705812316409381282u,  7127501107294950542u,  12006341378644347076u,
                5236586458250640726u,  11042172239907660341u, 5759783405795756054u,
                6741188021946160720u,  9062861991168352058u,  2347957779168721077u,
                13637409664605039535u, 1238201813735746316u,  12231048212667504162u,
                5760642662739372587u,  10552987261048228617u, 105631500981943270u,
                13744063709213719364u, 15507579696550518045u, 9798337040960501650u,
                1532677441468290047u,  12641391569759670130u, 12418848153493822766u,
                11415559669711152846u, 2383698903925266947u,  1609377270837450886u,
                15873905051623017901u, 15517018713119955514u, 8107069303594950404u,
                10131966996927808381u, 15081522428949250076u, 1399222678079857520u,
                12249124256627612785u, 3357478422509701525u,  3034696069027470162u,
                18153998149616042231u, 16377598033441375827u, 5368335467060833304u,
                6044646628603547257u,  1479467778353524960u,  4236240858258845034u,
                3155890713018340186u,  3246414826460602346u,  5953667306733310472u,
                6362679177065133760u,  15946313723934743266u, 12256520446842734589u,
                9641859877507513078u,  6922663546619523408u,  7642050253596373439u,
                6276392818878942558u,  1278812256646018953u,  12741784025257524154u,
                18151567297633301791u, 7512654660927061552u,  4670214383887477212u,
                14563635726264587016u, 17130684498505520166u, 12325607660861620717u,
                12837517957394470910u, 1087902451704220421u,  275710593779638990u,
                9351141240618478552u,  14255629750696425608u, 8989397168933359459u,
                12827541113108290725u, 12577012022940511204u, 9313438271656148323u,
                17666029174801286206u, 9657735031127150232u,  16549144033821778816u,
                3436483321192130403u,  10176292006160822895u, 6817558803001867618u,
                5067237774995270030u,  60561280476022760u,    11201356538700003739u,
                14906041802142758355u, 17711328571032633319u, 13453135083561548366u,
                14964334724206345295u, 4354023135682264199u,  13284917788726551580u,
                14303499728793702035u, 3812301328388858911u,  3840948415193960920u,
                603312261738427772u,
            };

            cstrview s{
                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13"
                "\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27"
                "\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b"
                "\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f"
                "\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63"
                "\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77"
                "\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b"
                "\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f"
                "\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3"
                "\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7"
                "\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb"
                "\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef"
                "\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff"};
            snn_require(s.size() == 256);

            for (const usize hash : hashes)
            {
                snn_require(s.hash() == hash);
                s.drop_back_n(1);
            }
            snn_require(s.is_empty());

            snn_require(str{"One Two Three"}.hash() == 4047187161467679849u);
            snn_require(strbuf{}.hash() == 11160318154034397263u);

            snn_require(std::hash<str>{}(str{"One Two Three"}) == 4047187161467679849u);
            snn_require(std::hash<strbuf>{}(strbuf{}) == 11160318154034397263u);

            static_assert(cstrview{"One Two Three"}.hash() == 4047187161467679849u);
            static_assert(strview{}.hash() == 11160318154034397263u);

            static_assert(std::hash<cstrview>{}(cstrview{"One Two Three"}) == 4047187161467679849u);
            static_assert(std::hash<strview>{}(strview{}) == 11160318154034397263u);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        snn_static_require(app::test_strview_interface<cstrview>());
        snn_static_require(app::test_strview_interface<str>());
        snn_static_require(app::test_strview_interface<strbuf>());

        snn_static_require(app::test_str_interface<str>());
        snn_static_require(app::test_str_interface<strbuf>());

        snn_static_require(app::test_append_array());
        snn_static_require(app::test_comparison<str>());
        snn_static_require(app::test_comparison<strbuf>());
        snn_static_require(app::test_comparison<cstrview>());
        snn_static_require(app::test_concat());
        snn_static_require(app::test_hash());
        snn_static_require(app::test_str());
        snn_static_require(app::test_stream_append_array());

        snn_require(app::test_find_rnd());
        snn_require(app::test_find_byte_rnd());

        static_assert(sizeof(cstrview) == 16);
        static_assert(sizeof(str) == 24);
        static_assert(sizeof(strbuf) == 24);

        static_assert(is_strcore_v<str>);
        static_assert(is_strcore_v<const str>);
        static_assert(is_strcore_v<strbuf>);
        static_assert(is_strcore_v<const strbuf>);

        static_assert(!is_strcore_v<int>);
        static_assert(!is_strcore_v<cstrview>);
        static_assert(!is_strcore_v<strview>);
        static_assert(!is_strcore_v<str&>);

        static_assert(any_strcore<str>);
        static_assert(any_strcore<strbuf>);
        static_assert(!any_strcore<int>);
        static_assert(!any_strcore<cstrview>);
    }
}
