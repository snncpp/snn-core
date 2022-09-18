// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/pair/common.hh"

#include "snn-core/strcore.hh"
#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            pair::key_value<cstrview, usize> kv{"Hello", 123};

            snn_require(kv.key == "Hello");
            snn_require(kv.value == 123);

            snn_require(kv.get<0>() == "Hello");
            snn_require(kv.get<1>() == 123);

            // This uses ADL to find `get` in the `pair` namespace.
            snn_require(get<0>(kv) == "Hello");
            snn_require(get<1>(kv) == 123);

            // Concept
            static_assert(pair::any<decltype(kv)>);

            return true;
        }

        struct non_trivial
        {
            [[maybe_unused]] ~non_trivial();
        };

        constexpr bool test_member_types()
        {
            static_assert(std::is_same_v<pair::first_second<int, char>::a_type, int>);
            static_assert(std::is_same_v<pair::first_second<int, char>::b_type, char>);

            static_assert(std::is_same_v<pair::first_second<int, char>::first_type, int>);
            static_assert(std::is_same_v<pair::first_second<int, char>::second_type, char>);

            static_assert(is_trivially_relocatable_v<pair::first_second<str>>);
            static_assert(is_trivially_relocatable_v<pair::first_second<str, int>>);
            static_assert(is_trivially_relocatable_v<pair::first_second<int, str>>);

            static_assert(!is_trivially_relocatable_v<pair::first_second<non_trivial>>);
            static_assert(!is_trivially_relocatable_v<pair::first_second<non_trivial, int>>);
            static_assert(!is_trivially_relocatable_v<pair::first_second<int, non_trivial>>);

            return true;
        }

        constexpr bool test_default_constructor()
        {
            {
                pair::first_second<int> p;
                snn_require(p.first == 0);
                snn_require(p.second == 0);
                static_assert(sizeof(p) == (sizeof(int) * 2));
            }
            {
                pair::first_second<cstrview, usize> p;
                snn_require(p.first == "");
                snn_require(p.second == 0);
                static_assert(sizeof(p) == (sizeof(cstrview) + sizeof(usize)));
            }
            {
                static_assert(std::is_default_constructible_v<pair::first_second<int, int>>);
                static_assert(implicitly_default_constructible<pair::first_second<int, int>>);

                static_assert(!std::is_default_constructible_v<pair::first_second<int&, int>>);
                static_assert(!std::is_default_constructible_v<pair::first_second<int, int&>>);
                static_assert(!std::is_default_constructible_v<pair::first_second<int&, int&>>);

                static_assert(std::is_default_constructible_v<pair::first_second<const char*>>);
                static_assert(implicitly_default_constructible<pair::first_second<const char*>>);

                static_assert(std::is_default_constructible_v<
                              pair::first_second<const char*, promise::null_terminated_t>>);
                static_assert(!implicitly_default_constructible<
                              pair::first_second<const char*, promise::null_terminated_t>>);

                static_assert(std::is_default_constructible_v<
                              pair::first_second<promise::null_terminated_t, const char*>>);
                static_assert(!implicitly_default_constructible<
                              pair::first_second<promise::null_terminated_t, const char*>>);

                static_assert(std::is_default_constructible_v<
                              pair::first_second<promise::null_terminated_t>>);
                static_assert(!implicitly_default_constructible<
                              pair::first_second<promise::null_terminated_t>>);
            }

            return true;
        }

        constexpr bool test_converting_constructor()
        {
            {
                pair::first_second p = {123, 'x'};
                static_assert(std::is_same_v<decltype(p), pair::first_second<int, char>>);
                snn_require(p.first == 123);
                snn_require(p.second == 'x');
            }
            {
                pair::first_second<cstrview> p = {"One", "Two"};
                snn_require(p.first == "One");
                snn_require(p.second == "Two");
            }
            {
                usize i    = 123;
                cstrview s = "abc";
                pair::first_second p{i, std::move(s)};
                static_assert(std::is_same_v<decltype(p), pair::first_second<usize, cstrview>>);
                snn_require(p.first == 123);
                snn_require(p.second == "abc");
            }
            {
                cstrview s = "abcdef";
                pair::first_second<usize, cstrview&> p{123, s};
                snn_require(p.first == 123);
                snn_require(p.second == "abcdef");
                p.second = "abc";
                snn_require(s == "abc");
            }
            {
                pair::first_second p = {"One", 456};
                static_assert(std::is_same_v<decltype(p), pair::first_second<const char*, int>>);
                snn_require(p.first != nullptr);
                snn_require(p.second == 456);
            }
            {
                static_assert(std::is_constructible_v<pair::first_second<int, int>, int, int>);

                // Lvalue references will not bind to rvalues.

                static_assert(std::is_constructible_v<pair::first_second<int&, int&>, int&, int&>);
                static_assert(!std::is_constructible_v<pair::first_second<int&, int&>, int, int>);
                static_assert(!std::is_constructible_v<pair::first_second<int&, int&>, int&, int>);
                static_assert(!std::is_constructible_v<pair::first_second<int&, int&>, int, int&>);

                static_assert(std::is_constructible_v<pair::first_second<int, int&>, int, int&>);
                static_assert(!std::is_constructible_v<pair::first_second<int, int&>, int, int>);

                static_assert(std::is_constructible_v<pair::first_second<int&, int>, int&, int>);
                static_assert(!std::is_constructible_v<pair::first_second<int&, int>, int, int>);
            }

            return true;
        }

        constexpr bool test_get()
        {
            usize i = 456;
            pair::first_second<isize, usize&> p{123, i};
            snn_require(p.get<0>() == 123);
            snn_require(p.get<1>() == 456);

            static_assert(std::is_same_v<decltype(p.get<0>()), isize&>);
            static_assert(std::is_same_v<decltype(std::as_const(p).get<0>()), const isize&>);
            static_assert(std::is_same_v<decltype(std::move(p).get<0>()), isize&&>);
            static_assert(
                std::is_same_v<decltype(std::move(std::as_const(p)).get<0>()), const isize&&>);

            // Reference collapsing.
            static_assert(std::is_same_v<decltype(p.get<1>()), usize&>);
            static_assert(std::is_same_v<decltype(std::as_const(p).get<1>()), usize&>);
            static_assert(std::is_same_v<decltype(std::move(p).get<1>()), usize&>);
            static_assert(std::is_same_v<decltype(std::move(std::as_const(p)).get<1>()), usize&>);

            return true;
        }

        constexpr bool test_pair()
        {
            static_assert(std::is_same_v<pair::first_second<int>, pair::first_second<int, int>>);

            static_assert(std::is_trivially_copyable_v<pair::first_second<int>>);

            static_assert(sane<pair::first_second<int>>);
            static_assert(sane<pair::first_second<int&>>);
            static_assert(sane<pair::first_second<int&&>>);
            static_assert(sane<pair::first_second<const int&>>);
            static_assert(sane<pair::first_second<const int&&>>);

            static_assert(std::is_nothrow_default_constructible_v<pair::first_second<int>>);
            static_assert(std::is_nothrow_default_constructible_v<pair::first_second<str>>);

            static_assert(std::is_nothrow_constructible_v<pair::first_second<int>, int&, int&>);
            static_assert(std::is_nothrow_constructible_v<pair::first_second<str>, str&&, str&&>);

            static_assert(!std::is_nothrow_constructible_v<pair::first_second<str>, str&, str&>);
            static_assert(
                !std::is_nothrow_constructible_v<pair::first_second<str, int>, str&, int&>);
            static_assert(
                !std::is_nothrow_constructible_v<pair::first_second<int, str>, int&, str&>);

            {
                pair::first_second<str> p{"abcdefghijklmnopqrstuvwxyz",
                                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

                snn_require(p.first == "abcdefghijklmnopqrstuvwxyz");
                snn_require(get<0>(p) == "abcdefghijklmnopqrstuvwxyz");

                auto s = get<0>(std::move(p));
                snn_require(s == "abcdefghijklmnopqrstuvwxyz");

                // First was moved.
                snn_require(p.first == "");
                snn_require(get<0>(p) == "");

                snn_require(p.second == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
                snn_require(get<1>(p) == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            }
            {
                auto mk = [] { return pair::first_second<str, int>{"abcd", 123}; };

                static_assert(std::is_same_v<decltype(mk().first), str>);
                static_assert(std::is_same_v<decltype(mk().second), int>);
                static_assert(std::is_same_v<decltype(get<0>(mk())), str&&>);
                static_assert(std::is_same_v<decltype(get<1>(mk())), int&&>);
            }
            {
                int i = 123;
                pair::first_second<str, int&> p{"abcd", i};

                static_assert(std::is_same_v<decltype(p.first), str>);
                static_assert(std::is_same_v<decltype(std::as_const(p).first), str>);
                static_assert(std::is_same_v<decltype(std::move(p).first), str>);
                static_assert(std::is_same_v<decltype(std::move(std::as_const(p)).first), str>);

                static_assert(std::is_same_v<decltype(get<0>(p)), str&>);
                static_assert(std::is_same_v<decltype(get<0>(std::as_const(p))), const str&>);
                static_assert(std::is_same_v<decltype(get<0>(std::move(p))), str&&>);
                static_assert(
                    std::is_same_v<decltype(get<0>(std::move(std::as_const(p)))), const str&&>);

                static_assert(std::is_same_v<decltype(p.second), int&>);
                static_assert(std::is_same_v<decltype(std::as_const(p).second), int&>);
                static_assert(std::is_same_v<decltype(std::move(p).second), int&>);
                static_assert(std::is_same_v<decltype(std::move(std::as_const(p)).second), int&>);

                static_assert(std::is_same_v<decltype(get<1>(p)), int&>);
                static_assert(std::is_same_v<decltype(get<1>(std::as_const(p))), int&>);
                static_assert(std::is_same_v<decltype(get<1>(std::move(p))), int&>);
                static_assert(std::is_same_v<decltype(get<1>(std::move(std::as_const(p)))), int&>);

                static_assert(std::tuple_size_v<decltype(p)> == 2);
                static_assert(std::is_same_v<std::tuple_element_t<0, decltype(p)>, str>);
                static_assert(std::is_same_v<std::tuple_element_t<1, decltype(p)>, int&>);
            }
            {
                int i = 123;
                pair::first_second<str, const int&> p{"abcd", i};

                static_assert(std::is_same_v<decltype(p.first), str>);
                static_assert(std::is_same_v<decltype(std::as_const(p).first), str>);
                static_assert(std::is_same_v<decltype(std::move(p).first), str>);
                static_assert(std::is_same_v<decltype(std::move(std::as_const(p)).first), str>);

                static_assert(std::is_same_v<decltype(get<0>(p)), str&>);
                static_assert(std::is_same_v<decltype(get<0>(std::as_const(p))), const str&>);
                static_assert(std::is_same_v<decltype(get<0>(std::move(p))), str&&>);
                static_assert(
                    std::is_same_v<decltype(get<0>(std::move(std::as_const(p)))), const str&&>);

                static_assert(std::is_same_v<decltype(p.second), const int&>);
                static_assert(std::is_same_v<decltype(std::as_const(p).second), const int&>);
                static_assert(std::is_same_v<decltype(std::move(p).second), const int&>);
                static_assert(
                    std::is_same_v<decltype(std::move(std::as_const(p)).second), const int&>);

                static_assert(std::is_same_v<decltype(get<1>(p)), const int&>);
                static_assert(std::is_same_v<decltype(get<1>(std::as_const(p))), const int&>);
                static_assert(std::is_same_v<decltype(get<1>(std::move(p))), const int&>);
                static_assert(
                    std::is_same_v<decltype(get<1>(std::move(std::as_const(p)))), const int&>);

                static_assert(std::tuple_size_v<decltype(p)> == 2);
                static_assert(std::is_same_v<std::tuple_element_t<0, decltype(p)>, str>);
                static_assert(std::is_same_v<std::tuple_element_t<1, decltype(p)>, const int&>);
            }
            {
                usize a = 123;
                isize b = 456;
                pair::first_second<usize&&, isize> p{std::move(a), b};

                snn_require(get<0>(p) == 123);
                snn_require(get<1>(p) == 456);

                static_assert(std::is_same_v<decltype(get<0>(p)), usize&>);
                static_assert(std::is_same_v<decltype(get<0>(std::as_const(p))), usize&>);
                static_assert(std::is_same_v<decltype(get<0>(std::move(p))), usize&&>);
                static_assert(
                    std::is_same_v<decltype(get<0>(std::move(std::as_const(p)))), usize&&>);

                static_assert(std::is_same_v<decltype(get<1>(p)), isize&>);
                static_assert(std::is_same_v<decltype(get<1>(std::as_const(p))), const isize&>);
                static_assert(std::is_same_v<decltype(get<1>(std::move(p))), isize&&>);
                static_assert(
                    std::is_same_v<decltype(get<1>(std::move(std::as_const(p)))), const isize&&>);
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
        snn_static_require(app::test_converting_constructor());
        snn_static_require(app::test_default_constructor());
        snn_static_require(app::test_get());
        snn_static_require(app::test_member_types());
        snn_static_require(app::test_pair());
    }
}
