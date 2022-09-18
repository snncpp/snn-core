// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/null_term.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                array arr{123, 456, 0};
                null_term nt{arr.writable()};
                static_assert(std::is_same_v<decltype(nt), null_term<int*>>);
                snn_require(nt.get() == arr.begin());
                snn_require(nt.not_null().get() == arr.begin());
                snn_require(nt.to<array_view<int>>().count() == 2);
            }
            {
                null_term nt{"Foo\0Bar"};
                static_assert(std::is_same_v<decltype(nt), null_term<const char*>>);
                snn_require(nt.to<cstrview>() == "Foo");
            }

            return true;
        }

        constexpr bool test_null_term()
        {
            static_assert(sizeof(null_term<int*>) == sizeof(int*));
            static_assert(sizeof(null_term<const char*>) == sizeof(const char*));

            static_assert(is_trivially_relocatable_v<null_term<const char*>>);
            static_assert(std::is_trivially_copy_constructible_v<null_term<const char*>>);
            static_assert(std::is_nothrow_copy_constructible_v<null_term<const char*>>);
            static_assert(std::is_trivially_move_constructible_v<null_term<const char*>>);
            static_assert(std::is_nothrow_move_constructible_v<null_term<const char*>>);
            static_assert(std::is_trivially_copy_assignable_v<null_term<const char*>>);
            static_assert(std::is_nothrow_copy_assignable_v<null_term<const char*>>);
            static_assert(std::is_trivially_move_assignable_v<null_term<const char*>>);
            static_assert(std::is_nothrow_move_assignable_v<null_term<const char*>>);
            static_assert(std::is_trivially_destructible_v<null_term<const char*>>);

            {
                null_term nt{not_null{"Foo\0Bar"}};
                static_assert(std::is_same_v<decltype(nt), null_term<const char*>>);
                snn_require(nt.to<cstrview>() == "Foo");
            }
            {
                null_term nt{not_null{"Foo\0Bar"}, promise::null_terminated};
                static_assert(std::is_same_v<decltype(nt), null_term<const char*>>);
                snn_require(nt.to<cstrview>() == "Foo");
            }
            {
                array<const char*, 3> arr{"foo", "bar", nullptr};
                null_term nt{arr.writable(), promise::null_terminated};
                static_assert(std::is_same_v<decltype(nt), null_term<const char**>>);
                snn_require(nt.get() == arr.begin());
                snn_require(nt.not_null().get() == arr.begin());
                snn_require(nt.to<array_view<const char*>>().count() == 2);
                static_assert(noexcept(nt.to<array_view<const char*>>()));
            }
            {
                array arr{0, 456, 0};
                null_term nt{arr.writable()};
                static_assert(std::is_same_v<decltype(nt), null_term<int*>>);
                snn_require(nt.get() == arr.begin());
                snn_require(nt.not_null().get() == arr.begin());
            }
            {
                array arr{'F', 'o', 'o', '\0', 'B', 'a', 'r'};
                null_term nt{arr.writable()};
                static_assert(std::is_same_v<decltype(nt), null_term<char*>>);
                snn_require(nt.get() == arr.begin());
                snn_require(nt.not_null().get() == arr.begin());
                snn_require(nt.to<strview>() == "Foo");
                static_assert(noexcept(nt.to<strview>()));
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
        snn_static_require(app::test_null_term());

        constexpr auto is_abc = [](const transient<null_term<const char*>> ts) {
            return ts.get().to<cstrview>() == "abc";
        };

        static_assert(is_abc("abc"));
        static_assert(is_abc("abc\0def"));
        snn_require(is_abc(str{"abc"})); // Temporary
        snn_require_throws_code(is_abc(str{"abc\0def"}), generic::error::unexpected_null_character);

        static_assert(callable<decltype(is_abc), str>);
        static_assert(callable<decltype(is_abc), str&>);
        static_assert(!callable<decltype(is_abc), strbuf>);  // Not null-terminated.
        static_assert(!callable<decltype(is_abc), strbuf&>); // Not null-terminated.

        static_assert(!is_abc("ABC"));
        static_assert(!is_abc("ab"));
        static_assert(!is_abc("abcd"));
    }
}
