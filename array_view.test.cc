// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/array_view.hh"

#include "snn-core/unittest.hh"
#include "snn-core/fn/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            str src = "abcdef1234567890";

            auto v = src.view();
            static_assert(std::is_same_v<decltype(v), strview>);
            static_assert(std::is_same_v<decltype(v), array_view<char>>);

            snn_require(v == "abcdef1234567890");

            snn_require(v); // Not empty.
            snn_require(!v.is_empty());
            snn_require(v.count() == 16);
            snn_require(v.size() == 16);
            snn_require(v.byte_size().get() == 16);

            snn_require(v.at(0).value() == 'a');
            snn_require(v.at(1).value() == 'b');
            snn_require(v.at<char>(2).value() == 'c');

            snn_require(v.at(0, promise::within_bounds) == 'a');
            snn_require(v.at(1, promise::within_bounds) == 'b');
            snn_require(v.at(2, promise::within_bounds) == 'c');

            decltype(auto) opt = v.at(99);
            static_assert(std::is_same_v<decltype(opt), optional<char&>>);
            snn_require(!opt.has_value());
            snn_require(opt.error_code() == nullopt);

            decltype(auto) opt_val = v.at<char>(99);
            static_assert(std::is_same_v<decltype(opt_val), optional<char>>);
            snn_require(!opt_val.has_value());

            snn_require(v.all(chr::is_alphanumeric));

            snn_require(v.count_if(chr::is_digit) == 10);

            snn_require(v.view(6, 3) == "123");
            snn_require(v.view(6, 3).to<int>().value() == 123);

            snn_require(v.view_offset(-10) == "1234567890");

            v.view_offset(-10).fill("~-");

            snn_require(src == "abcdef~-~-~-~-~-");

            return true;
        }

        template <typename A, typename B>
        concept can_overwrite = requires(A& a, const B& b) { a.overwrite(b); };

        template <typename T, typename UInt>
        concept can_load = requires(const T& t) { t.template load<UInt>(); };

        template <typename T, typename UInt>
        concept can_load_swap = requires(const T& t) { t.template load_swap<UInt>(); };

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

        constexpr bool test_array_view()
        {
            static_assert(std::is_same_v<strview, array_view<char>>);
            static_assert(std::is_same_v<cstrview, array_view<const char>>);

            static_assert(std::is_same_v<array_view<char>::element_type, char>);
            static_assert(std::is_same_v<array_view<char>::value_type, char>);
            static_assert(std::is_same_v<array_view<const char>::element_type, const char>);
            static_assert(std::is_same_v<array_view<const char>::value_type, char>);

            // For an extensive test of cstrview, see strcore.test.cc.

            // The DANGERS of array_view, use with caution.
            {
                str src{"abc"};

                cstrview p{src};
                snn_require(p.size() == 3);
                snn_require(p == "abc");
                snn_require(p.data().get() == src.data().get());

                if (!std::is_constant_evaluated())
                {
                    snn_require(p.overlaps(src));
                    snn_require(src.overlaps(p));
                }

                src.append("... trigger an allocation.");
                snn_require(src.size() == 29);
                snn_require(src == "abc... trigger an allocation.");

                // The cstrview object still points to the old buffer. In this case most likely the
                // local string object on the stack, which now holds pointers. Reading from it would
                // return garbage. But it could also have been heap allocated memory, i.e. use after
                // free.
                snn_require(p.size() == 3);
                snn_require(p.data().get() != src.data().get());

                if (!std::is_constant_evaluated())
                {
                    snn_require(!p.overlaps(src));
                    snn_require(!src.overlaps(p));
                }
            }

            static_assert(is_trivially_relocatable_v<cstrview>);
            static_assert(std::is_nothrow_constructible_v<cstrview>);
            static_assert(std::is_nothrow_default_constructible_v<cstrview>);
            static_assert(std::is_trivially_copy_constructible_v<cstrview>);
            static_assert(std::is_nothrow_copy_constructible_v<cstrview>);
            static_assert(std::is_trivially_move_constructible_v<cstrview>);
            static_assert(std::is_nothrow_move_constructible_v<cstrview>);
            static_assert(std::is_trivially_copy_assignable_v<cstrview>);
            static_assert(std::is_nothrow_copy_assignable_v<cstrview>);
            static_assert(std::is_trivially_move_assignable_v<cstrview>);
            static_assert(std::is_nothrow_move_assignable_v<cstrview>);
            static_assert(std::is_trivially_destructible_v<cstrview>);

            static_assert(is_trivially_relocatable_v<strview>);
            static_assert(std::is_nothrow_constructible_v<strview>);
            static_assert(std::is_nothrow_default_constructible_v<strview>);
            static_assert(std::is_trivially_copy_constructible_v<strview>);
            static_assert(std::is_nothrow_copy_constructible_v<strview>);
            static_assert(std::is_trivially_move_constructible_v<strview>);
            static_assert(std::is_nothrow_move_constructible_v<strview>);
            static_assert(std::is_trivially_copy_assignable_v<strview>);
            static_assert(std::is_nothrow_copy_assignable_v<strview>);
            static_assert(std::is_trivially_move_assignable_v<strview>);
            static_assert(std::is_nothrow_move_assignable_v<strview>);
            static_assert(std::is_trivially_destructible_v<strview>);

            static_assert(std::is_default_constructible_v<array_view<int, 0>>);
            static_assert(!std::is_default_constructible_v<array_view<int, 1>>);

            // Deduction/Convertion
            {
                array arr{11, 22, 33};
                array_view v{arr};
                static_assert(std::is_same_v<decltype(v), array_view<int, 3>>);
                snn_require(v);
                snn_require(v.count() == 3);
                snn_require(v.begin() == arr.begin());
            }
            {
                const array arr{11, 22, 33};
                array_view v{arr};
                static_assert(std::is_same_v<decltype(v), array_view<const int, 3>>);
                snn_require(v);
                snn_require(v.count() == 3);
                snn_require(v.begin() == arr.begin());

                // Static to dynamic.
                array_view<const int> dyn{v};
                snn_require(dyn);
                snn_require(dyn.count() == 3);
                snn_require(dyn.begin() == arr.begin());
            }

            // .get<Index>()
            {
                array arr{11, 22, 33};
                array_view v{arr};
                static_assert(std::is_same_v<decltype(v), array_view<int, 3>>);
                snn_require(v.get<0>() == 11);
                snn_require(v.get<1>() == 22);
                snn_require(v.get<2>() == 33);
                v.get<2>() = 44;
                snn_require(arr.get<2>() == 44);
            }
            {
                const array arr{11, 22, 33};
                array_view v{arr};
                static_assert(std::is_same_v<decltype(v), array_view<const int, 3>>);
                snn_require(v.get<0>() == 11);
                snn_require(v.get<1>() == 22);
                snn_require(v.get<2>() == 33);
            }

            // constexpr array_view() noexcept requires(Count == 0) : data_{nullptr}
            {
                array_view<int, 0> v;
                static_assert(std::is_same_v<decltype(v), array_view<int, 0>>);
                snn_require(!v);
                snn_require(v.count() == 0);
                snn_require(v.data() == nullptr);
            }

            // constexpr array_view(array<U, C>& a) noexcept : data_{a.begin()}, count_
            {
                array arr{11, 22, 33};
                array_view<int> v{arr};
                snn_require(v);
                snn_require(v.count() == 3);
                snn_require(v.begin() == arr.begin());
            }

            // constexpr array_view(const array<U, C>& a) noexcept : data_{a.begin()}, count_
            {
                const array arr{11, 22, 33};
                array_view<const int> v{arr};
                snn_require(v);
                snn_require(v.count() == 3);
                snn_require(v.begin() == arr.begin());
            }

            // constexpr explicit array_view(val_or_ref<T&> ref) noexcept requires(Count == 1)
            {
                const int i = 123;
                array_view v{as_ref(i)};
                static_assert(std::is_same_v<decltype(v), array_view<const int, 1>>);
                snn_require(v.count() == 1);
                snn_require(v.front().value() == 123);
                snn_require(v.byte_size().get() == 4);
            }

            // constexpr explicit array_view(const char (&s)[N]) noexcept : data_{s}
            {
                array_view v{"One Two"};
                static_assert(std::is_same_v<decltype(v), array_view<const char, 7>>);
                snn_require(v.size() == 7);
                snn_require(v.front().value() == 'O');
                snn_require(v.back().value() == 'o');
            }

            // constexpr explicit array_view(const pointer data, promise::has_capacity_t) noexcept
            {
                array arr{'a', 'b', 'c'};
                array_view<char, 2> v{arr.begin(), promise::has_capacity};
                snn_require(v.size() == 2);
                snn_require(v.front().value() == 'a');
                snn_require(v.back().value() == 'b');
            }

            // constexpr explicit array_view(const not_null<pointer> data,
            //                               promise::has_capacity_t) noexcept
            {
                array arr{'a', 'b', 'c'};
                array_view<const char, 2> v{arr.data(), promise::has_capacity};
                snn_require(v.size() == 2);
                snn_require(v.front().value() == 'a');
                snn_require(v.back().value() == 'b');
            }

            // constexpr array_view(array_view<U, C> v) noexcept : data_{v.begin()}
            {
                array_view v{"One Two"};
                static_assert(std::is_same_v<decltype(v), array_view<const char, 7>>);

                array_view<const char, 3> v2 = v;
                snn_require(v2.size() == 3);
                snn_require(v2.front().value() == 'O');
                snn_require(v2.back().value() == 'e');
            }

            // constexpr explicit operator bool() const noexcept
            {
                array_view v{"One Two"};
                static_assert(std::is_same_v<decltype(v), array_view<const char, 7>>);
                snn_require(v);
            }
            {
                array_view v{""};
                static_assert(std::is_same_v<decltype(v), array_view<const char, 0>>);
                snn_require(!v);
            }

            // array_view<T, StaticCount>::contains()
            {
                const array arr{7, 3, 3, 2};
                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<const int, 4>>);
                snn_require(v.contains(7));
                snn_require(v.contains(3));
                snn_require(v.contains(2));
                snn_require(!v.contains(9));
            }

            // array_view<T, StaticCount>::count()
            {
                array arr{7, 3, 3, 2};
                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<int, 4>>);
                snn_require(v.count(7) == 1);
                snn_require(v.count(3) == 2);
                snn_require(v.count(2) == 1);
                snn_require(v.count(9) == 0);
            }

            // array_view<T, StaticCount>::count_if()
            {
                const array arr{7, 3, 3, 2};
                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<const int, 4>>);
                snn_require(v.count_if(fn::is{fn::less_than{}, 3}) == 1);
                snn_require(v.count_if(fn::is{fn::equal_to{}, 3}) == 2);
                snn_require(v.count_if(fn::is_zero{}) == 0);
            }

            // array_view<T, StaticCount>::find()
            {
                array arr{7, 3, 3, 2};
                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<int, 4>>);
                snn_require(v.find(7));
                snn_require(v.find(3));
                snn_require(v.find(2));
                snn_require(!v.find(9));
                snn_require(v.find(7).value_or_npos() == 0);
                snn_require(v.find(3).value_or_npos() == 1);
                snn_require(v.find(3, 1).value_or_npos() == 1);
                snn_require(v.find(3, 2).value_or_npos() == 2);
                snn_require(v.find(3, 3).value_or_npos() == constant::npos);
                snn_require(v.find(2).value_or_npos() == 3);
                snn_require(v.find(9).value_or_npos() == constant::npos);
            }

            // array_view<T, StaticCount>::find_if()
            {
                const array arr{7, 3, 3, 2};
                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<const int, 4>>);
                snn_require(v.find_if(fn::is{fn::less_than{}, 5}).value_or_npos() == 1);
                snn_require(v.find_if(fn::is{fn::less_than{}, 5}, 1).value_or_npos() == 1);
                snn_require(v.find_if(fn::is{fn::less_than{}, 5}, 2).value_or_npos() == 2);
                snn_require(v.find_if(fn::is{fn::less_than{}, 5}, 3).value_or_npos() == 3);
                snn_require(v.find_if(fn::is{fn::less_than{}, 2}).value_or_npos() ==
                            constant::npos);
            }

            // array_view<T, StaticCount>::find_in_reverse()
            {
                array arr{7, 3, 3, 2};
                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<int, 4>>);
                snn_require(v.find_in_reverse(7));
                snn_require(v.find_in_reverse(3));
                snn_require(v.find_in_reverse(2));
                snn_require(!v.find_in_reverse(9));
                snn_require(v.find_in_reverse(7).value_or_npos() == 0);
                snn_require(v.find_in_reverse(3).value_or_npos() == 2);
                snn_require(v.find_in_reverse(3, 2).value_or_npos() == 2);
                snn_require(v.find_in_reverse(3, 1).value_or_npos() == 1);
                snn_require(v.find_in_reverse(3, 0).value_or_npos() == constant::npos);
                snn_require(v.find_in_reverse(2).value_or_npos() == 3);
                snn_require(v.find_in_reverse(9).value_or_npos() == constant::npos);
            }

            // array_view<T, StaticCount>::find_in_reverse_if()
            {
                const array arr{7, 3, 3, 2};
                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<const int, 4>>);
                snn_require(v.find_in_reverse_if(fn::is{fn::lt{}, 5}).value_or_npos() == 3);
                snn_require(v.find_in_reverse_if(fn::is{fn::lt{}, 5}, 3).value_or_npos() == 3);
                snn_require(v.find_in_reverse_if(fn::is{fn::lt{}, 5}, 2).value_or_npos() == 2);
                snn_require(v.find_in_reverse_if(fn::is{fn::lt{}, 5}, 1).value_or_npos() == 1);
                snn_require(v.find_in_reverse_if(fn::is{fn::lt{}, 5}, 0).value_or_npos() ==
                            constant::npos);
                snn_require(v.find_in_reverse_if(fn::is{fn::lt{}, 2}).value_or_npos() ==
                            constant::npos);
            }

            // array_view<T, StaticCount>::has_front/has_back()
            {
                array arr{7, 3, 3, 2};
                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<int, 4>>);
                snn_require(v.has_front(7));
                snn_require(!v.has_front(3));
                snn_require(!v.has_front(2));
                snn_require(v.has_back(2));
                snn_require(!v.has_back(7));
                snn_require(!v.has_back(3));
            }
            {
                constexpr array_view<const int, 0> v;
                snn_require(!v.has_front(3));
                snn_require(!v.has_front(0));
                snn_require(!v.has_back(3));
                snn_require(!v.has_back(0));
            }

            // cstrview()
            {
                cstrview s{};
                snn_require(s.data().get() != nullptr);
                snn_require(s.count() == 0);
                snn_require(s.size() == 0);
                snn_require(s.byte_size().get() == 0);
                snn_require(s.is_empty());
                snn_require(!s);
            }

            // cstrview(val_or_ref<const char&>)
            // strview(val_or_ref<char&>)
            {
                char c = 'a';
                strview s{as_ref(c)};
                static_assert(std::is_same_v<decltype(s), strview>);
                snn_require(s.count() == 1);
                snn_require(s.data().get() == &c);
                snn_require(s.at(0).value() == 'a');
            }
            {
                const char c = 'a';
                cstrview s{as_ref(c)};
                static_assert(std::is_same_v<decltype(s), cstrview>);
                snn_require(s.count() == 1);
                snn_require(s.data().get() == &c);
                snn_require(s.at(0).value() == 'a');
            }
            {
                char c = 'a';
                strview s{as_ref(c)};
                snn_require(s.count() == 1);
                snn_require(s.data().get() == &c);
                snn_require(s.at(0).value() == 'a');
            }
            {
                char c = 'a';
                cstrview s{as_ref(c)}; // Conversion from non-const to const ref.
                snn_require(s.count() == 1);
                snn_require(s.data().get() == &c);
                snn_require(s.at(0).value() == 'a');
            }
            {
                const char c = 'a';
                cstrview s{as_ref(c)};
                snn_require(s.count() == 1);
                snn_require(s.data().get() == &c);
                snn_require(s.at(0).value() == 'a');
            }

            // strview()
            // cstrview(U&)
            {
                strview s{};
                snn_require(s.data().get() != nullptr);
                snn_require(s.count() == 0);
                snn_require(s.size() == 0);
                snn_require(s.byte_size().get() == 0);
                snn_require(s.is_empty());
                snn_require(!s);

                // strview is implicitly convertible to cstrview.
                cstrview cpy = s;
                snn_require(cpy.data().get() != nullptr);
                snn_require(cpy.data().get() == s.data().get());
                snn_require(cpy.count() == 0);
                snn_require(cpy.size() == 0);
                snn_require(cpy.is_empty());
                snn_require(!cpy);
            }

            // Static to strview/cstrview.
            {
                array abc{'a', 'b', 'c'};
                array_view v = abc;
                static_assert(std::is_same_v<decltype(v), array_view<char, 3>>);
                strview s = v;
                snn_require(s.size() == 3);
                snn_require(s == "abc");
            }
            {
                constexpr array_view v{"abcdef"};
                static_assert(std::is_same_v<decltype(v), const array_view<const char, 6>>);
                constexpr cstrview s = v;
                snn_require(s.size() == 6);
                snn_require(s == "abcdef");
            }

            // cstrview(const char (&)[N])
            {
                cstrview s{"Hello"};
                snn_require(s.data().get() != nullptr);
                snn_require(s.count() == 5);
                snn_require(s.size() == 5);
                snn_require(s.byte_size().get() == 5);
                snn_require(!s.is_empty());
                snn_require(s);
                snn_require(s.find('o').value_or(99) == 4);
                snn_require(s.find("ello").value_or(99) == 1);
                snn_require(s.find('E').value_or(99) == 99);
                snn_require(s.contains('e'));
                snn_require(s.contains("ll"));
                snn_require(s == "Hello");
                snn_require(s != "HELLO");
                snn_require(s != "Helloo");

                cstrview cpy{s};
                snn_require(cpy.data().get() != nullptr);
                snn_require(cpy.count() == 5);
                snn_require(cpy.size() == 5);
                snn_require(cpy.byte_size().get() == 5);
                snn_require(!cpy.is_empty());
                snn_require(cpy);
                snn_require(cpy == "Hello");
            }

            // strview(array)
            {
                array<char, 3> arr{'a', 'b', 'c'};
                strview s = arr;
                snn_require(s.count() == 3);
                snn_require(s.size() == 3);
                snn_require(s.begin() == arr.begin());
                snn_require(s == "abc");
            }

            // cstrview(array)
            {
                const array<char, 3> arr{'a', 'b', 'c'};
                cstrview s = arr;
                snn_require(s.size() == 3);
                snn_require(s.begin() == arr.begin());
                snn_require(s == "abc");
            }

            // strview(not_null<char*>, usize)
            {
                array<char, 3> arr{97, 98, 99};
                strview s{arr.writable(), 3};
                snn_require(s.data().get() != nullptr);
                snn_require(s.count() == 3);
                snn_require(s.size() == 3);
                snn_require(s.byte_size().get() == 3);
                snn_require(s == "abc");
            }

            // cstrview(const char*, promise::null_terminated_t)
            {
                const char* c_string = "Hello";
                cstrview s{c_string, promise::null_terminated};
                snn_require(s);
                snn_require(s.size() == 5);
                snn_require(s == "Hello");
            }
            {
                const char* c_string = "abc\0def";
                cstrview s{c_string, promise::null_terminated};
                snn_require(s);
                snn_require(s.size() == 3);
                snn_require(s == "abc");
            }
            {
                cstrview s{nullptr, promise::null_terminated};
                snn_require(!s);
                snn_require(s.size() == 0);
                snn_require(s.data().get() != nullptr);
                snn_require(s == "");
            }

            // cstrview(not_null<const char*>, promise::null_terminated_t)
            {
                const char* c_string = "Hello";
                cstrview s{not_null{c_string}, promise::null_terminated};
                snn_require(s);
                snn_require(s.size() == 5);
                snn_require(s == "Hello");
            }
            {
                const char* c_string = "abc\0def";
                cstrview s{not_null{c_string}, promise::null_terminated};
                snn_require(s);
                snn_require(s.size() == 3);
                snn_require(s == "abc");
            }
            {
                const char* c_string = "";
                cstrview s{not_null{c_string}, promise::null_terminated};
                snn_require(!s);
                snn_require(s.size() == 0);
                snn_require(s.data().get() != nullptr);
                snn_require(s == "");
            }

            // array_view<const int>(const int*, promise::null_terminated_t)
            {
                const array arr{27, 11, 98, 0};
                static_assert(std::is_same_v<decltype(arr.begin()), const int*>);
                array_view v{arr.begin(), promise::null_terminated};
                static_assert(std::is_same_v<decltype(v), array_view<const int>>);
                snn_require(v);
                snn_require(v.count() == 3);
                snn_require(v.at(0) == 27);
                snn_require(v.at(2) == 98);
            }
            {
                constexpr array_view<const int> v{nullptr, promise::null_terminated};
                snn_require(!v);
                snn_require(v.count() == 0);
            }

            // array_view<const int>(not_null<const int*>, promise::null_terminated_t)
            {
                const array arr{27, 11, 98, 0};
                static_assert(std::is_same_v<decltype(arr.data()), not_null<const int*>>);
                const array_view v{arr.data(), promise::null_terminated};
                static_assert(std::is_same_v<decltype(v), const array_view<const int>>);
                snn_require(v);
                snn_require(v.count() == 3);
                snn_require(v.at(0) == 27);
                snn_require(v.at(2) == 98);
            }

            // strview(char*, promise::null_terminated_t)
            {
                array arr{'H', 'e', 'l', 'l', 'o', '\0', 'F', 'o', 'o'};
                static_assert(std::is_same_v<decltype(arr.begin()), char*>);
                strview s{arr.begin(), promise::null_terminated};
                snn_require(s);
                snn_require(s.size() == 5);
                snn_require(s == "Hello");
            }
            {
                strview s{nullptr, promise::null_terminated};
                snn_require(!s);
                snn_require(s.size() == 0);
                snn_require(s.data().get() != nullptr);
                snn_require(s == "");
            }

            // strview(not_null<char*>, promise::null_terminated_t)
            {
                array arr{'H', 'e', 'l', 'l', 'o', '\0', 'F', 'o', 'o'};
                static_assert(std::is_same_v<decltype(arr.writable()), not_null<char*>>);
                strview s{arr.writable(), promise::null_terminated};
                snn_require(s);
                snn_require(s.size() == 5);
                snn_require(s == "Hello");
            }

            // strview(init::from_t, Container)
            {
                array arr{'a', 'b', 'c'};

                // The source container can be something like a `std::string`.
                string_like container{arr.begin(), arr.size()};
                snn_require(container.data() == arr.begin());
                snn_require(container.size() == arr.size());

                strview s{init::from, container};
                snn_require(s.begin() == arr.begin());
                snn_require(s.size() == arr.size());
            }
            {
                string_like<char> container{nullptr, 0};
                snn_require(container.data() == nullptr);
                snn_require(container.size() == 0);

                strview s{init::from, container};
                snn_require(s.begin() != nullptr);
                snn_require(s.size() == 0);
            }
            {
                using sl  = string_like<char>;
                using csl = string_like<const char>;

                static_assert(constructible_from<strview, init::from_t, sl&>);

                static_assert(!constructible_from<strview, init::from_t, csl&>);
                static_assert(!constructible_from<strview, init::from_t, const sl&>);
                static_assert(!constructible_from<strview, init::from_t, const csl&>);
                static_assert(!constructible_from<strview, init::from_t, sl&&>);
                static_assert(!constructible_from<strview, init::from_t, csl&&>);
                static_assert(!constructible_from<strview, init::from_t, const sl&&>);
                static_assert(!constructible_from<strview, init::from_t, const csl&&>);

                static_assert(!constructible_from<strview, init::from_t, int&>);
                static_assert(!constructible_from<strview, init::from_t, not_null<char*>&>);
            }

            // cstrview(init::from_t, Container)
            {
                const array arr{'a', 'b', 'c'};

                // The source container can be something like a `std::string[_view]`.
                const string_like container{arr.begin(), arr.size()};
                snn_require(container.data() == arr.begin());
                snn_require(container.size() == arr.size());

                cstrview s{init::from, container};
                snn_require(s.begin() == arr.begin());
                snn_require(s.size() == arr.size());
            }
            {
                string_like<char> container{nullptr, 0};
                snn_require(container.data() == nullptr);
                snn_require(container.size() == 0);

                cstrview s{init::from, container};
                snn_require(s.begin() != nullptr);
                snn_require(s.size() == 0);
            }
            {
                using sl  = string_like<char>;
                using csl = string_like<const char>;

                static_assert(constructible_from<cstrview, init::from_t, sl&>);
                static_assert(constructible_from<cstrview, init::from_t, csl&>);
                static_assert(constructible_from<cstrview, init::from_t, const sl&>);
                static_assert(constructible_from<cstrview, init::from_t, const csl&>);

                static_assert(!constructible_from<cstrview, init::from_t, sl&&>);
                static_assert(!constructible_from<cstrview, init::from_t, csl&&>);
                static_assert(!constructible_from<cstrview, init::from_t, const sl&&>);
                static_assert(!constructible_from<cstrview, init::from_t, const csl&&>);

                static_assert(!constructible_from<cstrview, init::from_t, int&>);
                static_assert(!constructible_from<cstrview, init::from_t, not_null<char*>&>);
            }

            // strview(init::from_t, ContiguousIt, ContiguousIt)
            {
                char* const data = nullptr;
                strview s{init::from, data, data};
                snn_require(s.data().get() != nullptr);
                snn_require(s.count() == 0);
            }
            {
                array<char, 3> arr{97, 98, 99};
                strview s = arr.view(1, 1);
                snn_require(s.data().get() != nullptr);
                snn_require(s.count() == 1);
                snn_require(!s.is_empty());
                snn_require(s);
                snn_require(s == "b");
            }

            // cstrview(init::from_t, ContiguousIt, ContiguousIt)
            {
                const char* const data = nullptr;
                cstrview s{init::from, data, data};
                snn_require(s.data().get() != nullptr);
                snn_require(s.count() == 0);
            }
            {
                const array<char, 3> arr{97, 98, 99};
                cstrview s = arr.view(1, 1);
                snn_require(s.data().get() != nullptr);
                snn_require(s.count() == 1);
                snn_require(s == "b");
            }

            // cstrview(ContiguousRange)
            {
                range::contiguous rng{"Hello"};
                cstrview s{rng};
                snn_require(s.size() == 5);
                snn_require(s == "Hello");
            }
            {
                cstrrng rng{init::from, nullptr, nullptr};
                cstrview s{rng};
                snn_require(s.size() == 0);
                snn_require(s == "");
            }

            // array_view<char, 0>()
            {
                array_view<char, 0> v;
                snn_require(v.count() == 0);
                snn_require(v.is_empty());
                snn_require(!v);
                snn_require(v.data() == nullptr);
                snn_require(v == "");
                snn_require(v != "abc");
            }

            // array_view()
            {
                array_view<const int> v;
                snn_require(v.count() == 0);
                snn_require(v.is_empty());
                snn_require(!v);
                snn_require(v.data() == nullptr);
            }

            // array_view(val_or_ref<T&>)
            {
                const int i = 123;
                array_view<const int> v{as_ref(i)};
                snn_require(v);
                snn_require(v.count() == 1);
                snn_require(!v.is_empty());
                snn_require(v.data() == &i);
                snn_require(v.at(0).value() == 123);
            }
            {
                int i = 123;
                array_view<const int> v{as_ref(i)};
                snn_require(v);
                snn_require(v.count() == 1);
                snn_require(!v.is_empty());
                snn_require(v.data() == &i);
                snn_require(v.at(0).value() == 123);
            }

            // array_view(...)
            {
                const array<int, 3> arr{3, 8, 2};
                array_view v = arr.view();

                snn_require(v.count() == 3);
                snn_require(v.byte_size().get() == 12);
                snn_require(!v.is_empty());
                snn_require(v);
                snn_require(v.data() == arr.data().get());
                snn_require(v.at(0).value() == 3);
                snn_require(v.at(1).value() == 8);
                snn_require(v.at(2).value() == 2);
                snn_require(v.front().value() == 3);
                snn_require(v.back().value() == 2);

                snn_require(!v.at(3).has_value());

                array_view<const int> cpy{v};
                snn_require(cpy.count() == 3);
                snn_require(v.at(0).value() == 3);
                snn_require(v.at(1).value() == 8);
                snn_require(v.at(2).value() == 2);

                if (!std::is_constant_evaluated())
                {
                    snn_require(v.overlaps(cpy));
                }
            }
            {
                array<int, 3> arr{3, 8, 2};
                array_view v = arr.view();

                snn_require(v.count() == 3);
                snn_require(v.data() == arr.data().get());

                v.drop_front_n(99);
                snn_require(v.count() == 0);
                snn_require(v.is_empty());
                snn_require(!v);
                snn_require(v.data() == arr.end());
                snn_require(!v.at(0).has_value());
                snn_require(!v.front().has_value());
                snn_require(!v.back().has_value());
            }

            // array_view(T*, usize)
            {
                array<int, 3> arr{3, 8, 2};
                array_view v{arr.data().get(), 2};
                snn_require(v.count() == 2);
                snn_require(v.byte_size().get() == 8);
                snn_require(!v.is_empty());
                snn_require(v);
                snn_require(v.data() != nullptr);
                snn_require(v.at(0).value() == 3);
                snn_require(v.at(1).value() == 8);
                snn_require(!v.at(2).has_value());
            }

            // array_view(not_null<T*>, usize)
            {
                array<int, 2> arr{3, 7};
                array_view v{arr.data(), 1};
                snn_require(v.count() == 1);
                snn_require(v.byte_size().get() == 4);
                snn_require(!v.is_empty());
                snn_require(v);
                snn_require(v.data() != nullptr);
                snn_require(v.at(0).value() == 3);
            }

            // array_view(init::from_t, ContiguousIt, ContiguousIt)
            {
                array<int, 3> arr{3, 8, 2};
                array_view<int> v{init::from, arr.begin(), arr.end()};
                snn_require(v.count() == 3);
                snn_require(!v.is_empty());
                snn_require(v);
                snn_require(v.data() != nullptr);
                snn_require(v.at(0).value() == 3);
                snn_require(v.at(1).value() == 8);
                snn_require(v.at(2).value() == 2);
            }
            {
                array_view<int> v{init::from, nullptr, nullptr};
                snn_require(v.count() == 0);
                snn_require(v.is_empty());
                snn_require(!v);
                snn_require(v.data() == nullptr);
            }

            // array_view(rng)
            {
                array<int, 3> arr{3, 8, 2};
                array_view<int> v{arr.range()};
                snn_require(v.count() == 3);
                snn_require(!v.is_empty());
                snn_require(v);
                snn_require(v.data() != nullptr);
                snn_require(v.at(0).value() == 3);
                snn_require(v.at(1).value() == 8);
                snn_require(v.at(2).value() == 2);
            }

            // begin(), end()
            {
                array_view<const int> v{};
                auto first = v.begin();
                auto last  = v.end();
                snn_require(last - first == 0);
            }
            {
                array<int, 3> arr{3, 8, 2};

                array_view<int> v = arr.view();
                for (auto& e : v)
                {
                    e += 10;
                }
                snn_require(v.count() == 3);
                snn_require(v.at(0).value() == 13);
                snn_require(v.at(1).value() == 18);
                snn_require(v.at(2).value() == 12);

                snn_require(arr.at(0).value() == 13);
                snn_require(arr.at(1).value() == 18);
                snn_require(arr.at(2).value() == 12);
            }

            // contains, find, find_in_reverse
            {
                const array<int, 4> arr{3, 8, 2, 3};
                array_view<const int> v = arr.view();
                snn_require(v.count() == 4);

                snn_require(v.contains(3));
                snn_require(v.contains(8));
                snn_require(v.contains(2));
                snn_require(!v.contains(1));

                snn_require(v.find(3).value_or_npos() == 0);
                snn_require(v.find(8).value_or_npos() == 1);
                snn_require(v.find(2).value_or_npos() == 2);
                snn_require(v.find(1).value_or_npos() == constant::npos);

                snn_require(v.find_in_reverse(3).value_or_npos() == 3);
                snn_require(v.find_in_reverse(8).value_or_npos() == 1);
                snn_require(v.find_in_reverse(2).value_or_npos() == 2);
                snn_require(v.find_in_reverse(1).value_or_npos() == constant::npos);
            }
            {
                array_view<const int> v;
                snn_require(v.is_empty());
                snn_require(!v.contains(3));
                snn_require(!v.find(3));
                snn_require(!v.find_in_reverse(3));
            }

            // each
            {
                array<int, 4> arr{3, 8, 2, 4};

                array_view<int> v = arr.view();
                snn_require(v.count() == 4);

                v.each([](int& i) { i = i * 2; });
                snn_require(v.count() == 4);
                snn_require(v.at(0).value() == 6);
                snn_require(v.at(1).value() == 16);
                snn_require(v.at(2).value() == 4);
                snn_require(v.at(3).value() == 8);

                snn_require(arr.at(0).value() == 6);
                snn_require(arr.at(1).value() == 16);
                snn_require(arr.at(2).value() == 4);
                snn_require(arr.at(3).value() == 8);

                v = array_view<int>{};
                snn_require(v.count() == 0);
                v.each([](int& i) { i = i * 2; });
                snn_require(v.count() == 0);
            }

            // fill
            {
                array<int, 3> arr{3, 8, 2};
                snn_require(arr.none(fn::is{fn::equal_to{}, 7}));

                array_view<int> v = arr.view();
                snn_require(v.count() == 3);
                snn_require(v.at(1).value() == 8);
                v.fill(7);

                snn_require(arr.all(fn::is{fn::equal_to{}, 7}));

                v.view(1).fill(9);

                snn_require(arr.at(0).value() == 7);
                snn_require(arr.at(1).value() == 9);
                snn_require(arr.at(2).value() == 9);
            }
            {
                array<int, 3> arr{3, 8, 2};
                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<int, 3>>);
                v.fill(98);
                snn_require(arr.at(0).value() == 98);
                snn_require(arr.at(1).value() == 98);
                snn_require(arr.at(2).value() == 98);
            }
            {
                str s;
                s.view().fill('a');
                snn_require(s.is_empty());

                s.resize(9, '.');
                snn_require(s == ".........");

                s.view().fill('a');
                snn_require(s == "aaaaaaaaa");

                s.view(3, 3).fill('b');
                snn_require(s == "aaabbbaaa");
            }
            {
                str s;
                s.view().fill("abc");
                snn_require(s.is_empty());

                s.resize(8, '_');
                snn_require(s == "________");

                s.view().fill("abc");
                snn_require(s == "abcabcab");

                s.view().fill(""); // Does nothing.
                snn_require(s == "abcabcab");

                s.view(3, 3).fill("d");
                snn_require(s == "abcdddab");

                s = "abcdefghijklmnopqrst";
                s.view(3, 14).fill("012");
                snn_require(s == "abc01201201201201rst");
            }
            {
                str s = "abcdefghijklmnopqrstuvwxyz";
                // Fill from self.
                s.view().fill(s.view(3, 6));
                snn_require(s == "defghidefghidefghidefghide");
            }

            // fill_front
            {
                array arr{'a', 'b', 'c', 'd', 'e', 'f'};
                snn_require(arr.view() == "abcdef");

                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<char, 6>>);

                snn_require(v.fill_front(array_view{"xyz"}) == 3); // Static
                static_assert(
                    std::is_same_v<decltype(array_view{"xyz"}), array_view<const char, 3>>);
                snn_require(arr.view() == "xyzdef");

                snn_require(v.fill_front(cstrview{"ab12"}) == 4); // Dynamic
                snn_require(arr.view() == "ab12ef");

                snn_require(v.fill_front(array_view{"abcDEF123"}) == 6); // Static
                snn_require(arr.view() == "abcDEF");

                snn_require(v.fill_front(cstrview{"0123456789"}) == 6); // Dynamic
                snn_require(arr.view() == "012345");
            }
            {
                array arr{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
                snn_require(arr.view() == "abcdefghi");

                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<char, 9>>);

                snn_require(v.fill_front(array_view{"xyz"}) == 3); // Static
                static_assert(
                    std::is_same_v<decltype(array_view{"xyz"}), array_view<const char, 3>>);
                snn_require(arr.view() == "xyzdefghi");

                snn_require(v.fill_front(cstrview{"ab12"}) == 4); // Dynamic
                snn_require(arr.view() == "ab12efghi");

                snn_require(v.fill_front(array_view{"abcDEF123"}) == 9); // Static
                snn_require(arr.view() == "abcDEF123");

                snn_require(v.fill_front(cstrview{"0123456789"}) == 9); // Dynamic
                snn_require(arr.view() == "012345678");
            }

            // overwrite
            {
                const array src{'A', 'B', 'C', 'D', 'E', 'F', '0', '1', '2', '3'};

                array dst{'a', 'b', 'c', 'd', 'e', 'f'};
                snn_require(dst.view() == "abcdef");

                // Counts have to match to overwrite.

                dst.view<>().overwrite(src.view<3, 6>()); // Counts match, can overwrite.
                snn_require(dst.view() == "DEF012");

                // dst.view<>().overwrite(src.view<>());  // Will not compile, counts do not match.
            }
            {
                static_assert(!app::can_overwrite<array_view<int, 4>, array_view<i8, 3>>);
                static_assert(app::can_overwrite<array_view<int, 4>, array_view<i8, 4>>);
                static_assert(!app::can_overwrite<array_view<int, 4>, array_view<i8, 5>>);
            }

            // as_bytes
            {
                const array a{11, 22};

                array_view v = a; // Static count.
                static_assert(std::is_same_v<decltype(v), array_view<const int, 2>>);

                if (!std::is_constant_evaluated())
                {
                    auto bytes = v.as_bytes<>();
                    static_assert(std::is_same_v<decltype(bytes), array_view<const byte, 8>>);

                    // Little-endian
                    snn_require(bytes.get<0>() == 11);
                    snn_require(bytes.get<1>() == 0);
                    snn_require(bytes.get<2>() == 0);
                    snn_require(bytes.get<3>() == 0);
                    snn_require(bytes.get<4>() == 22);
                    snn_require(bytes.get<5>() == 0);
                    snn_require(bytes.get<6>() == 0);
                    snn_require(bytes.get<7>() == 0);
                }

                if (!std::is_constant_evaluated())
                {
                    auto bytes = v.as_bytes();
                    static_assert(std::is_same_v<decltype(bytes), array_view<const byte>>);
                    snn_require(bytes.size() == 8);

                    // Little-endian
                    snn_require(bytes.at(0, promise::within_bounds) == 11);
                    snn_require(bytes.at(1, promise::within_bounds) == 0);
                    snn_require(bytes.at(2, promise::within_bounds) == 0);
                    snn_require(bytes.at(3, promise::within_bounds) == 0);
                    snn_require(bytes.at(4, promise::within_bounds) == 22);
                    snn_require(bytes.at(5, promise::within_bounds) == 0);
                    snn_require(bytes.at(6, promise::within_bounds) == 0);
                    snn_require(bytes.at(7, promise::within_bounds) == 0);
                }
            }
            {
                const array a{11, 22};

                auto v = a.view(); // Dynamic count.
                static_assert(std::is_same_v<decltype(v), array_view<const int>>);
                snn_require(v.count() == 2);

                if (!std::is_constant_evaluated())
                {
                    auto bytes = v.as_bytes();
                    static_assert(std::is_same_v<decltype(bytes), array_view<const byte>>);
                    snn_require(bytes.size() == 8);

                    // Little-endian
                    snn_require(bytes.at(0, promise::within_bounds) == 11);
                    snn_require(bytes.at(1, promise::within_bounds) == 0);
                    snn_require(bytes.at(2, promise::within_bounds) == 0);
                    snn_require(bytes.at(3, promise::within_bounds) == 0);
                    snn_require(bytes.at(4, promise::within_bounds) == 22);
                    snn_require(bytes.at(5, promise::within_bounds) == 0);
                    snn_require(bytes.at(6, promise::within_bounds) == 0);
                    snn_require(bytes.at(7, promise::within_bounds) == 0);
                }
            }
            {
                array a{11, 22};

                array_view v = a; // Static count.
                static_assert(std::is_same_v<decltype(v), array_view<int, 2>>);

                if (!std::is_constant_evaluated())
                {
                    auto bytes = v.as_bytes<>();
                    static_assert(std::is_same_v<decltype(bytes), array_view<byte, 8>>);

                    // Little-endian
                    snn_require(bytes.get<0>() == 11);
                    snn_require(bytes.get<1>() == 0);
                    snn_require(bytes.get<2>() == 0);
                    snn_require(bytes.get<3>() == 0);
                    snn_require(bytes.get<4>() == 22);
                    snn_require(bytes.get<5>() == 0);
                    snn_require(bytes.get<6>() == 0);
                    snn_require(bytes.get<7>() == 0);

                    auto const_bytes = std::as_const(v).as_bytes<>();
                    static_assert(std::is_same_v<decltype(const_bytes), array_view<const byte, 8>>);

                    snn_require(const_bytes.get<0>() == 11);
                    snn_require(const_bytes.get<7>() == 0);
                }

                if (!std::is_constant_evaluated())
                {
                    auto bytes = v.as_bytes();
                    static_assert(std::is_same_v<decltype(bytes), array_view<byte>>);
                    snn_require(bytes.size() == 8);

                    // Little-endian
                    snn_require(bytes.at(0, promise::within_bounds) == 11);
                    snn_require(bytes.at(1, promise::within_bounds) == 0);
                    snn_require(bytes.at(2, promise::within_bounds) == 0);
                    snn_require(bytes.at(3, promise::within_bounds) == 0);
                    snn_require(bytes.at(4, promise::within_bounds) == 22);
                    snn_require(bytes.at(5, promise::within_bounds) == 0);
                    snn_require(bytes.at(6, promise::within_bounds) == 0);
                    snn_require(bytes.at(7, promise::within_bounds) == 0);

                    auto const_bytes = std::as_const(v).as_bytes();
                    static_assert(std::is_same_v<decltype(const_bytes), array_view<const byte>>);
                    snn_require(const_bytes.size() == 8);

                    snn_require(const_bytes.at(0, promise::within_bounds) == 11);
                    snn_require(const_bytes.at(7, promise::within_bounds) == 0);
                }
            }
            {
                array a{11, 22};

                auto v = a.view(); // Dynamic count.
                static_assert(std::is_same_v<decltype(v), array_view<int>>);
                snn_require(v.count() == 2);

                if (!std::is_constant_evaluated())
                {
                    auto bytes = v.as_bytes();
                    static_assert(std::is_same_v<decltype(bytes), array_view<byte>>);
                    snn_require(bytes.size() == 8);

                    // Little-endian
                    snn_require(bytes.at(0, promise::within_bounds) == 11);
                    snn_require(bytes.at(1, promise::within_bounds) == 0);
                    snn_require(bytes.at(2, promise::within_bounds) == 0);
                    snn_require(bytes.at(3, promise::within_bounds) == 0);
                    snn_require(bytes.at(4, promise::within_bounds) == 22);
                    snn_require(bytes.at(5, promise::within_bounds) == 0);
                    snn_require(bytes.at(6, promise::within_bounds) == 0);
                    snn_require(bytes.at(7, promise::within_bounds) == 0);

                    auto const_bytes = std::as_const(v).as_bytes();
                    static_assert(std::is_same_v<decltype(const_bytes), array_view<const byte>>);
                    snn_require(const_bytes.size() == 8);

                    snn_require(const_bytes.at(0, promise::within_bounds) == 11);
                    snn_require(const_bytes.at(7, promise::within_bounds) == 0);
                }
            }
            {
                const array a{'a', 'b', 'c'};

                auto v = a.view();
                static_assert(std::is_same_v<decltype(v), cstrview>);
                snn_require(v.size() == 3);

                if (!std::is_constant_evaluated())
                {
                    auto bytes = v.as_bytes();
                    static_assert(std::is_same_v<decltype(bytes), array_view<const byte>>);
                    snn_require(bytes.size() == 3);

                    snn_require(bytes.at(0, promise::within_bounds) == 97);
                    snn_require(bytes.at(1, promise::within_bounds) == 98);
                    snn_require(bytes.at(2, promise::within_bounds) == 99);
                }
            }
            {
                array a{'a', 'b', 'c'};

                auto v = a.view();
                static_assert(std::is_same_v<decltype(v), strview>);
                snn_require(v.size() == 3);

                if (!std::is_constant_evaluated())
                {
                    auto bytes = v.as_bytes();
                    static_assert(std::is_same_v<decltype(bytes), array_view<byte>>);
                    snn_require(bytes.size() == 3);

                    snn_require(bytes.at(0, promise::within_bounds) == 97);
                    snn_require(bytes.at(1, promise::within_bounds) == 98);
                    snn_require(bytes.at(2, promise::within_bounds) == 99);

                    auto const_bytes = std::as_const(v).as_bytes();
                    static_assert(std::is_same_v<decltype(const_bytes), array_view<const byte>>);
                    snn_require(const_bytes.size() == 3);

                    snn_require(const_bytes.at(0, promise::within_bounds) == 97);
                    snn_require(const_bytes.at(1, promise::within_bounds) == 98);
                    snn_require(const_bytes.at(2, promise::within_bounds) == 99);
                }
            }

            // count(needle)
            {
                array<int, 4> arr{3, 8, 3, 2};
                snn_require(arr.view().count(3) == 2);
                snn_require(arr.view().count(8) == 1);
                snn_require(arr.view().count(7) == 0);
            }
            {
                cstrview s = "abcabcABCaaabc123";
                snn_require(s.count('a') == 5);
                snn_require(s.count('1') == 1);
                snn_require(s.count("") == 18);
                snn_require(s.count("a") == 5);
                snn_require(s.count("ab") == 3);
                snn_require(s.count("abc") == 3);
                snn_require(s.count("ABC") == 1);
                snn_require(s.count("abc123") == 1);

                snn_require(s.count('x') == 0);
                snn_require(s.count("x") == 0);
                snn_require(s.count("abcd") == 0);
                snn_require(s.count("abc123\0") == 0);
                snn_require(s.count("aBc") == 0);
            }
            {
                cstrview s;
                snn_require(s.is_empty());
                snn_require(s.count('a') == 0);
                snn_require(s.count("") == 1);
                snn_require(s.count("a") == 0);
                snn_require(s.count("ab") == 0);
                snn_require(s.count("abc") == 0);
            }
            {
                array<char, 7> arr{'a', 'b', 'c', 'a', 'a', 'b', 'c'};
                snn_require(arr.view() == "abcaabc");

                snn_require(arr.view().count('a') == 3);
                snn_require(arr.view().count("") == 8);
                snn_require(arr.view().count("a") == 3);
                snn_require(arr.view().count("abc") == 2);

                snn_require(arr.view().count('x') == 0);
                snn_require(arr.view().count("x") == 0);
                snn_require(arr.view().count("aBc") == 0);
                snn_require(arr.view().count("abcd") == 0);
                snn_require(arr.view().count("abc\0") == 0);
            }

            // transform
            {
                array<int, 4> arr{3, 8, 2, 4};

                array_view<int> v = arr.view();
                snn_require(v.count() == 4);

                v.transform([](const int i) { return i * 2; });
                snn_require(v.count() == 4);
                snn_require(v.at(0).value() == 6);
                snn_require(v.at(1).value() == 16);
                snn_require(v.at(2).value() == 4);
                snn_require(v.at(3).value() == 8);

                snn_require(arr.at(0).value() == 6);
                snn_require(arr.at(1).value() == 16);
                snn_require(arr.at(2).value() == 4);
                snn_require(arr.at(3).value() == 8);

                v = array_view<int>{};
                snn_require(v.count() == 0);
                v.transform([](const int i) { return i * 2; });
                snn_require(v.count() == 0);
            }
            {
                array<str, 3> arr{"123", "abc", "456"};

                array_view<str> v = arr.view();
                snn_require(v.count() == 3);

                v.transform([](const str& val) { return str{val} << "defghijklmnopqrstuvwxyz"; });
                snn_require(v.count() == 3);
                snn_require(v.at(0).value() == "123defghijklmnopqrstuvwxyz");
                snn_require(v.at(1).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(v.at(2).value() == "456defghijklmnopqrstuvwxyz");

                snn_require(arr.at(0).value() == "123defghijklmnopqrstuvwxyz");
                snn_require(arr.at(1).value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(arr.at(2).value() == "456defghijklmnopqrstuvwxyz");
            }

            // transform_with
            {
                array a{1, 3, 7};
                array b{2, 1, 3};
                a.view<>().transform_with(b.view<>(), fn::multiply{});
                snn_require(a.at(0).value() == 2);
                snn_require(a.at(1).value() == 3);
                snn_require(a.at(2).value() == 21);

                snn_require(b.at(0).value() == 2);
                snn_require(b.at(1).value() == 1);
                snn_require(b.at(2).value() == 3);
            }

            // range
            {
                const cstrview s{"Hello"};
                const auto rng = s.range();
                static_assert(std::is_same_v<front_value_t<decltype(rng)>, char>);
                snn_require(rng.count() == 5);
                snn_require(rng.begin() == s.begin());
                snn_require(rng.end() == s.end());
            }
            {
                const array<int, 3> arr{3, 8, 2};
                const array_view<const int> v = arr.view();
                auto rng                      = v.range();
                static_assert(std::is_same_v<front_value_t<decltype(rng)>, int>);
                snn_require(rng.begin() == v.begin());
                snn_require(rng.end() == v.end());
                snn_require(rng.count() == 3);
            }

            // view<>
            {
                const array arr{'a', 'b', 'c', 'd', 'e', 'f'};

                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<const char, 6>>);
                snn_require(v == "abcdef");

                {
                    auto v2 = v.view<>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 6>>);
                    snn_require(v2 == "abcdef");
                }
                {
                    auto v2 = v.view<2>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 4>>);
                    snn_require(v2 == "cdef");
                }
                {
                    auto v2 = v.view<2, constant::npos>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 4>>);
                    snn_require(v2 == "cdef");
                }
                {
                    auto v2 = v.view<2, 2>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 2>>);
                    snn_require(v2 == "cd");
                }
                {
                    auto v2 = v.view<2, 0>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 0>>);
                    snn_require(v2 == "");
                }
                {
                    auto v2 = v.view<5, 99>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 1>>);
                    snn_require(v2 == "f");
                }
                {
                    auto v2 = v.view<6>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 0>>);
                    snn_require(v2 == "");
                }
                {
                    auto v2 = v.view<99>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 0>>);
                    snn_require(v2 == "");
                }
            }
            {
                array arr{'a', 'b', 'c', 'd', 'e', 'f'};

                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<char, 6>>);
                snn_require(v == "abcdef");

                {
                    auto v2 = v.view<>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 6>>);
                    snn_require(v2 == "abcdef");
                }
                {
                    auto v2 = v.view<2>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 4>>);
                    snn_require(v2 == "cdef");
                }
                {
                    auto v2 = v.view<2, constant::npos>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 4>>);
                    snn_require(v2 == "cdef");
                }
                {
                    auto v2 = v.view<2, 2>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 2>>);
                    snn_require(v2 == "cd");
                }
                {
                    auto v2 = v.view<2, 0>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 0>>);
                    snn_require(v2 == "");
                }
                {
                    auto v2 = v.view<5, 99>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 1>>);
                    snn_require(v2 == "f");
                }
                {
                    auto v2 = v.view<6>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 0>>);
                    snn_require(v2 == "");
                }
                {
                    auto v2 = v.view<99>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 0>>);
                    snn_require(v2 == "");
                }
            }

            // view_exactly - Static
            {
                array a{'a', 'b', 'c', 'd', 'e', 'f'};

                auto src = a.view<>();
                static_assert(std::is_same_v<decltype(src), array_view<char, 6>>);

                {
                    auto v = src.view_exactly<0, 6>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 6>>);
                    snn_require(v == "abcdef");
                }
                {
                    auto v = src.view_exactly<0, 3>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 3>>);
                    snn_require(v == "abc");
                }
                {
                    auto v = src.view_exactly<2, 2>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 2>>);
                    snn_require(v == "cd");
                }
                {
                    auto v = src.view_exactly<2, 4>();
                    static_assert(std::is_same_v<decltype(v), array_view<char, 4>>);
                    snn_require(v == "cdef");
                }
                {
                    auto v = std::as_const(src).view_exactly<2, 4>();
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 4>>);
                    snn_require(v == "cdef");
                }
                {
                    auto v = src.view_exactly<0, 6>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<char, 6>>);
                    snn_require(v == "abcdef");
                }
                {
                    auto v = src.view_exactly<2, 4>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<char, 4>>);
                    snn_require(v == "cdef");
                }
                {
                    auto v = std::as_const(src).view_exactly<2, 4>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 4>>);
                    snn_require(v == "cdef");
                }
            }

            // view_exactly - Dynamic
            {
                array a{11, 22, 33, 44};

                auto src = a.view();
                static_assert(std::is_same_v<decltype(src), array_view<int>>);

                {
                    auto v = src.view_exactly<0, 4>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<int, 4>>);
                    snn_require(v.begin() == a.begin());
                    snn_require(v.end() == a.end());
                }
                {
                    auto v = std::as_const(src).view_exactly<0, 4>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<const int, 4>>);
                    snn_require(v.begin() == a.begin());
                    snn_require(v.end() == a.end());
                }
                {
                    auto v = src.view_exactly<1, 3>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<int, 3>>);
                    snn_require(v.begin() == a.view_exactly<1, 1>().begin());
                    snn_require(v.end() == a.end());
                }
                {
                    auto v = std::as_const(src).view_exactly<1, 3>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<const int, 3>>);
                    snn_require(v.begin() == a.view_exactly<1, 1>().begin());
                    snn_require(v.end() == a.end());
                }
            }

            // view_exactly - cstrview
            {
                const array a{'a', 'b', 'c', 'd'};

                auto src = a.view();
                static_assert(std::is_same_v<decltype(src), cstrview>);

                {
                    auto v = src.view_exactly<0, 4>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 4>>);
                    snn_require(v.begin() == a.begin());
                    snn_require(v.end() == a.end());
                }
                {
                    auto v = std::as_const(src).view_exactly<0, 4>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 4>>);
                    snn_require(v.begin() == a.begin());
                    snn_require(v.end() == a.end());
                }
                {
                    auto v = src.view_exactly<1, 3>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 3>>);
                    snn_require(v.begin() == a.view_exactly<1, 1>().begin());
                    snn_require(v.end() == a.end());
                }
                {
                    auto v = std::as_const(src).view_exactly<1, 3>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 3>>);
                    snn_require(v.begin() == a.view_exactly<1, 1>().begin());
                    snn_require(v.end() == a.end());
                }
            }

            // view_exactly - strview
            {
                array a{'a', 'b', 'c', 'd'};

                auto src = a.view();
                static_assert(std::is_same_v<decltype(src), strview>);

                {
                    auto v = src.view_exactly<0, 4>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<char, 4>>);
                    snn_require(v.begin() == a.begin());
                    snn_require(v.end() == a.end());
                }
                {
                    auto v = std::as_const(src).view_exactly<0, 4>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 4>>);
                    snn_require(v.begin() == a.begin());
                    snn_require(v.end() == a.end());
                }
                {
                    auto v = src.view_exactly<1, 3>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<char, 3>>);
                    snn_require(v.begin() == a.view_exactly<1, 1>().begin());
                    snn_require(v.end() == a.end());
                }
                {
                    auto v = std::as_const(src).view_exactly<1, 3>(promise::within_bounds);
                    static_assert(std::is_same_v<decltype(v), array_view<const char, 3>>);
                    snn_require(v.begin() == a.view_exactly<1, 1>().begin());
                    snn_require(v.end() == a.end());
                }
            }

            // auto view_offset(const isize pos, const isize count = constant::limit<isize>::max)
            {
                cstrview s{"One Two Three"};

                snn_require(s.count() == 13);

                snn_require(s.view_offset(0) == "One Two Three");
                snn_require(s.view_offset(0, 13) == "One Two Three");
                snn_require(s.view_offset(0, 99) == "One Two Three");
                snn_require(s.view_offset(0, 3) == "One");
                snn_require(s.view_offset(0, 0) == "");

                snn_require(s.view_offset(4) == "Two Three");
                snn_require(s.view_offset(4, 9) == "Two Three");
                snn_require(s.view_offset(4, 99) == "Two Three");
                snn_require(s.view_offset(4, 3) == "Two");
                snn_require(s.view_offset(4, 0) == "");

                snn_require(s.view_offset(-5) == "Three");
                snn_require(s.view_offset(-5, 5) == "Three");
                snn_require(s.view_offset(-5, 99) == "Three");
                snn_require(s.view_offset(-5, 3) == "Thr");
                snn_require(s.view_offset(-5, 0) == "");

                snn_require(s.view_offset(-9) == "Two Three");
                snn_require(s.view_offset(-9, -6) == "Two");
                snn_require(s.view_offset(-9, -7) == "Tw");
                snn_require(s.view_offset(-9, -8) == "T");
                snn_require(s.view_offset(-9, -9) == "");
                snn_require(s.view_offset(-9, -10) == "");
                snn_require(s.view_offset(-9, -99) == "");

                snn_require(s.view_offset(-16, 3) == "");
                snn_require(s.view_offset(-16, 4) == "O");
                snn_require(s.view_offset(-16, 6) == "One");

                snn_require(s.view_offset(-100) == "One Two Three");
                snn_require(s.view_offset(-100, 90) == "One");

                constexpr auto min = constant::limit<isize>::min;
                constexpr auto max = constant::limit<isize>::max;

                snn_require(s.view_offset(min, min) == "");
                snn_require(s.view_offset(max, max) == "");
                snn_require(s.view_offset(min, max) == "One Two Thre");
                snn_require(s.view_offset(max, min) == "");
            }

            // auto view_offset(const isize pos, const isize count = constant::limit<isize>::max)
            {
                cstrview s{"One Two Three"};

                snn_require(s.count() == 13);

                snn_require(s.view_offset(0) == "One Two Three");
                snn_require(s.view_offset(0, 13) == "One Two Three");
                snn_require(s.view_offset(0, 99) == "One Two Three");
                snn_require(s.view_offset(0, 3) == "One");
                snn_require(s.view_offset(0, 0) == "");

                snn_require(s.view_offset(4) == "Two Three");
                snn_require(s.view_offset(4, 9) == "Two Three");
                snn_require(s.view_offset(4, 99) == "Two Three");
                snn_require(s.view_offset(4, 3) == "Two");
                snn_require(s.view_offset(4, 0) == "");

                snn_require(s.view_offset(-5) == "Three");
                snn_require(s.view_offset(-5, 5) == "Three");
                snn_require(s.view_offset(-5, 99) == "Three");
                snn_require(s.view_offset(-5, 3) == "Thr");
                snn_require(s.view_offset(-5, 0) == "");

                snn_require(s.view_offset(-9) == "Two Three");
                snn_require(s.view_offset(-9, -6) == "Two");
                snn_require(s.view_offset(-9, -7) == "Tw");
                snn_require(s.view_offset(-9, -8) == "T");
                snn_require(s.view_offset(-9, -9) == "");
                snn_require(s.view_offset(-9, -10) == "");
                snn_require(s.view_offset(-9, -99) == "");

                snn_require(s.view_offset(-16, 3) == "");
                snn_require(s.view_offset(-16, 4) == "O");
                snn_require(s.view_offset(-16, 6) == "One");

                snn_require(s.view_offset(-100) == "One Two Three");
                snn_require(s.view_offset(-100, 90) == "One");

                constexpr auto min = constant::limit<isize>::min;
                constexpr auto max = constant::limit<isize>::max;

                snn_require(s.view_offset(min, min) == "");
                snn_require(s.view_offset(max, max) == "");
                snn_require(s.view_offset(min, max) == "One Two Thre");
                snn_require(s.view_offset(max, min) == "");
            }

            // view_offset<>
            {
                const array arr{'a', 'b', 'c', 'd', 'e', 'f'};

                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<const char, 6>>);
                snn_require(v == "abcdef");

                {
                    auto v2 = v.view_offset<0>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 6>>);
                    snn_require(v2 == "abcdef");
                }
                {
                    auto v2 = v.view_offset<-99>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 6>>);
                    snn_require(v2 == "abcdef");
                }
                {
                    auto v2 = v.view_offset<-9, 6>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 3>>);
                    snn_require(v2 == "abc");
                }
                {
                    auto v2 = v.view_offset<2>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 4>>);
                    snn_require(v2 == "cdef");
                }
                {
                    auto v2 = v.view_offset<-4>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 4>>);
                    snn_require(v2 == "cdef");
                }
                {
                    auto v2 = v.view_offset<2, 2>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 2>>);
                    snn_require(v2 == "cd");
                }
                {
                    auto v2 = v.view_offset<-4, -2>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 2>>);
                    snn_require(v2 == "cd");
                }
                {
                    auto v2 = v.view_offset<2, 0>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 0>>);
                    snn_require(v2 == "");
                }
                {
                    auto v2 = v.view_offset<-1, 99>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 1>>);
                    snn_require(v2 == "f");
                }
                {
                    auto v2 = v.view_offset<6>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 0>>);
                    snn_require(v2 == "");
                }
                {
                    auto v2 = v.view_offset<99>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 0>>);
                    snn_require(v2 == "");
                }
                {
                    auto v2 = v.view_offset<-10, -20>();
                    static_assert(std::is_same_v<decltype(v2), array_view<const char, 0>>);
                    snn_require(v2 == "");
                }
            }
            {
                array arr{'a', 'b', 'c', 'd', 'e', 'f'};

                array_view v = arr;
                static_assert(std::is_same_v<decltype(v), array_view<char, 6>>);
                snn_require(v == "abcdef");

                {
                    auto v2 = v.view_offset<0>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 6>>);
                    snn_require(v2 == "abcdef");
                }
                {
                    auto v2 = v.view_offset<-99>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 6>>);
                    snn_require(v2 == "abcdef");
                }
                {
                    auto v2 = v.view_offset<-9, 6>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 3>>);
                    snn_require(v2 == "abc");
                }
                {
                    auto v2 = v.view_offset<2>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 4>>);
                    snn_require(v2 == "cdef");
                }
                {
                    auto v2 = v.view_offset<-4>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 4>>);
                    snn_require(v2 == "cdef");
                }
                {
                    auto v2 = v.view_offset<2, 2>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 2>>);
                    snn_require(v2 == "cd");
                }
                {
                    auto v2 = v.view_offset<-4, -2>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 2>>);
                    snn_require(v2 == "cd");
                }
                {
                    auto v2 = v.view_offset<2, 0>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 0>>);
                    snn_require(v2 == "");
                }
                {
                    auto v2 = v.view_offset<-1, 99>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 1>>);
                    snn_require(v2 == "f");
                }
                {
                    auto v2 = v.view_offset<6>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 0>>);
                    snn_require(v2 == "");
                }
                {
                    auto v2 = v.view_offset<99>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 0>>);
                    snn_require(v2 == "");
                }
                {
                    auto v2 = v.view_offset<-10, -20>();
                    static_assert(std::is_same_v<decltype(v2), array_view<char, 0>>);
                    snn_require(v2 == "");
                }
            }

            // load/load_swap
            {
                array_view v{"abcdefg"};
                static_assert(std::is_same_v<decltype(v), array_view<const char, 7>>);

                snn_require(v.load<u16>() == 0x6261);
                snn_require(v.load<u32>() == 0x64636261);
                // v.load<u64>(); // Will not compile.

                static_assert(!app::can_load<array_view<char, 7>, u64>);
                static_assert(!app::can_load<array_view<const char, 7>, u64>);
                static_assert(app::can_load<array_view<char, 8>, u64>);
                static_assert(app::can_load<array_view<const char, 8>, u64>);

                snn_require(v.load_swap<u16>() == 0x6162);
                snn_require(v.load_swap<u32>() == 0x61626364);
                // v.load_swap<u64>(); // Will not compile.

                if (!std::is_constant_evaluated())
                {
                    snn_require(v.as_bytes<>().load<u16>() == 0x6261);
                    snn_require(v.as_bytes<>().load<u32>() == 0x64636261);
                    snn_require(v.as_bytes<>().load_swap<u16>() == 0x6162);
                    snn_require(v.as_bytes<>().load_swap<u32>() == 0x61626364);
                }

                static_assert(!app::can_load_swap<array_view<char, 7>, u64>);
                static_assert(!app::can_load_swap<array_view<const char, 7>, u64>);
                static_assert(app::can_load_swap<array_view<char, 8>, u64>);
                static_assert(app::can_load_swap<array_view<const char, 8>, u64>);

                // Only octet types support load/load_swap.
                static_assert(!app::can_load_swap<array_view<u16, 8>, u64>);
                static_assert(!app::can_load_swap<array_view<const u16, 8>, u64>);
            }

            // operator==(array_view), operator!=(array_view), operator<=>(array_view)
            {
                const array_view<int> a;
                const array_view<int> b;
                snn_require(a == b);
                snn_require(!(a != b));
            }
            {
                array<int, 3> src_a{3, 8, 2};
                array<int, 4> src_b{3, 8, 2, 3};

                array_view a{src_a.view()};
                array_view b{src_b.view()};

                snn_require(!(a == b));
                snn_require(a != b);
                snn_require(a < b);
                snn_require(b > a);

                b.drop_back_n(1);

                snn_require(a == b);
                snn_require(!(a != b));
                snn_require(!(a < b));
                snn_require(!(b > a));

                snn_require(src_a.at(1) == 8);
                a.at(1).value() = 9;
                snn_require(src_a.at(1) == 9);

                snn_require(!(a == b));
                snn_require(a != b);

                a.drop_back_n(99);
                b.drop_back_n(99);

                snn_require(a == b);
                snn_require(!(a != b));
            }

            // operator<(const char (&lhs)[N], cstrview rhs)
            {
                snn_require(!("" < cstrview{""}));
                snn_require(!("a" < cstrview{"A"}));
                snn_require("A" < cstrview{"a"});
                snn_require("a" < cstrview{"aa"});
            }

            // operator>(const char (&lhs)[N], cstrview rhs)
            {
                snn_require(!("" > cstrview{""}));
                snn_require(!("A" > cstrview{"a"}));
                snn_require("a" > cstrview{"A"});
                snn_require("aa" > cstrview{"a"});
            }

            // Structured binding.
            // get<Index>(...)
            // tuple_element
            // tuple_size
            {
                const array<char, 3> a{'a', 'b', 'c'};

                array_view v = a;
                static_assert(std::is_same_v<decltype(v), array_view<const char, 3>>);

                const auto [x, y, z] = v;
                snn_require(x == 'a');
                snn_require(y == 'b');
                snn_require(z == 'c');

                snn_require(get<0>(v) == 'a');
                snn_require(get<1>(v) == 'b');
                snn_require(get<2>(v) == 'c');

                static_assert(std::is_same_v<decltype(get<0>(v)), const char&>);
                static_assert(std::is_same_v<decltype(get<0>(std::as_const(v))), const char&>);

                static_assert(std::tuple_size_v<decltype(v)> == 3);
                static_assert(std::is_same_v<std::tuple_element_t<0, decltype(v)>, const char>);
            }
            {
                array<char, 3> a{'a', 'b', 'c'};

                array_view v = a;
                static_assert(std::is_same_v<decltype(v), array_view<char, 3>>);

                const auto [x, y, z] = v;
                snn_require(x == 'a');
                snn_require(y == 'b');
                snn_require(z == 'c');

                snn_require(get<0>(v) == 'a');
                snn_require(get<1>(v) == 'b');
                snn_require(get<2>(v) == 'c');

                get<1>(v) = 'B';
                snn_require(a.view() == "aBc");

                static_assert(std::is_same_v<decltype(get<0>(v)), char&>);
                static_assert(std::is_same_v<decltype(get<0>(std::as_const(v))), const char&>);

                static_assert(std::tuple_size_v<decltype(v)> == 3);
                static_assert(std::is_same_v<std::tuple_element_t<0, decltype(v)>, char>);
            }

            // as_bytes(val_or_ref<T&>)
            if (!std::is_constant_evaluated())
            {
                {
                    int i = 8080;

                    auto v = as_bytes(as_ref(i));
                    static_assert(std::is_same_v<decltype(v), array_view<byte, 4>>);

                    // Little-endian
                    snn_require(v.get<0>() == 144);
                    snn_require(v.get<1>() == 31);
                    snn_require(v.get<2>() == 0);
                    snn_require(v.get<3>() == 0);

                    v.get<0>() = 0;
                    v.get<1>() = 1;

                    snn_require(i == 256);
                }
                {
                    const int i = 8080;

                    auto v = as_bytes(as_ref(i));
                    static_assert(std::is_same_v<decltype(v), array_view<const byte, 4>>);

                    // Little-endian
                    snn_require(v.get<0>() == 144);
                    snn_require(v.get<1>() == 31);
                    snn_require(v.get<2>() == 0);
                    snn_require(v.get<3>() == 0);
                }
                {
                    const int i = 8080;

                    auto v = as_bytes(as_ref(i)).as_bytes(); // Dynamic count.
                    static_assert(std::is_same_v<decltype(v), array_view<const byte>>);
                    snn_require(v.size() == 4);

                    // Little-endian
                    snn_require(v.at(0, promise::within_bounds) == 144);
                    snn_require(v.at(1, promise::within_bounds) == 31);
                    snn_require(v.at(2, promise::within_bounds) == 0);
                    snn_require(v.at(3, promise::within_bounds) == 0);
                }
            }

            return true;
        }

        constexpr bool test_to()
        {
            {
                array arr{'a', 'b', 'c'};

                strview s = arr.view();

                auto p = s.to<pair::value_count<char*, usize>>();
                snn_require(p.value == arr.begin());
                snn_require(p.count == arr.size());
            }
            {
                array arr{'a', 'b', 'c'};

                const strview s = arr.view();

                auto p = s.to<pair::value_count<const char*, usize>>();
                snn_require(p.value == arr.begin());
                snn_require(p.count == arr.size());
            }
            {
                const array arr{'a', 'b', 'c'};

                cstrview s = arr.view();

                auto p = s.to<pair::value_count<const char*, usize>>();
                snn_require(p.value == arr.begin());
                snn_require(p.count == arr.size());
            }
            {
                using plain_ptr_pair = pair::value_count<char*, usize>;
                using const_ptr_pair = pair::value_count<const char*, usize>;

                static_assert(has_to<strview, plain_ptr_pair>);
                static_assert(has_to<strview, const_ptr_pair>);
                static_assert(!has_to<const strview, plain_ptr_pair>);
                static_assert(has_to<const strview, const_ptr_pair>);
                static_assert(!has_to<cstrview, plain_ptr_pair>);
                static_assert(has_to<cstrview, const_ptr_pair>);
                static_assert(!has_to<const cstrview, plain_ptr_pair>);
                static_assert(has_to<const cstrview, const_ptr_pair>);

                static_assert(same_as<decltype(strview{}.to<plain_ptr_pair>()), plain_ptr_pair>);
                static_assert(same_as<decltype(strview{}.to<const_ptr_pair>()), const_ptr_pair>);
                static_assert(same_as<decltype(cstrview{}.to<const_ptr_pair>()), const_ptr_pair>);

                static_assert(noexcept(strview{}.to<plain_ptr_pair>()));
                static_assert(noexcept(std::declval<const strview>().to<const_ptr_pair>()));
                static_assert(noexcept(cstrview{}.to<const_ptr_pair>()));

                static_assert(!noexcept(strview{}.to<strbuf>()));
                static_assert(!noexcept(std::declval<const strview>().to<strbuf>()));
                static_assert(!noexcept(cstrview{}.to<strbuf>()));
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
        snn_static_require(app::test_array_view());
        snn_static_require(app::test_to());
    }
}
