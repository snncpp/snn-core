// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/val_or_ref.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            int a = 123;
            int b = 456;

            // Reference to a.
            val_or_ref<int&> val{a};
            snn_require(val.get() == 123);
            a = -1;
            snn_require(val.get() == -1);

            // Rebind to b.
            val.assign_or_rebind(b);
            snn_require(val.get() == 456);
            b = 789;
            snn_require(val.get() == 789);
            val.get() = 999;
            snn_require(b == 999);

            snn_require(a == -1);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        {
            int a = 123;
            int b = 456;

            // Const reference to a.
            val_or_ref<const int&> val{a};
            snn_require(val.get() == 123);
            a = -1;
            snn_require(val.get() == -1);

            // Rebind to b.
            val.assign_or_rebind(b);
            snn_require(val.get() == 456);
            b = 789;
            snn_require(val.get() == 789);

            snn_require(a == -1);

            // Non-const to const conversion.
            val = as_ref(a);
            snn_require(val.get() == -1);
        }
        {
            static constexpr int i = 123;
            constexpr val_or_ref<const int&> val{i};
            static_assert(val.get() == 123);
        }
        {
            val_or_ref<int> val{123};
            snn_require(val.get() == 123);
            val.assign_or_rebind(456);
            snn_require(val.get() == 456);
        }
        {
            constexpr val_or_ref<int> val{123};
            static_assert(val.get() == 123);
        }

        // Callable
        {
            static_assert(val_or_ref{fn::add{}}(2, 5) == 7);
            static_assert(std::is_same_v<decltype(val_or_ref{fn::add{}}), val_or_ref<fn::add>>);

            static_assert(callable<val_or_ref<fn::add>&, int, int>);
            static_assert(callable<const val_or_ref<fn::add>&, int, int>);
            static_assert(callable<val_or_ref<fn::add>&&, int, int>);
            static_assert(callable<const val_or_ref<fn::add>&&, int, int>);

            static_assert(!callable<val_or_ref<fn::add>&, int>);
            static_assert(!callable<const val_or_ref<fn::add>&, int>);
            static_assert(!callable<val_or_ref<fn::add>&&, int>);
            static_assert(!callable<const val_or_ref<fn::add>&&, int>);

            static_assert(!callable<val_or_ref<fn::add>&>);
            static_assert(!callable<const val_or_ref<fn::add>&>);
            static_assert(!callable<val_or_ref<fn::add>&&>);
            static_assert(!callable<const val_or_ref<fn::add>&&>);
        }
        {
            struct which
            {
                constexpr int operator()() &
                {
                    return 1;
                }

                constexpr int operator()() const&
                {
                    return 2;
                }

                constexpr int operator()() &&
                {
                    return 3;
                }

                constexpr int operator()() const&&
                {
                    return 4;
                }
            };

            val_or_ref val{which{}};
            static_assert(std::is_same_v<decltype(val), val_or_ref<which>>);

            snn_require(val() == 1);
            snn_require(std::as_const(val)() == 2);
            snn_require(std::move(val)() == 3);
            snn_require(std::move(std::as_const(val))() == 4);
        }
        {
            constexpr auto add = fn::add{};
            static_assert(val_or_ref{add}(2, 5) == 7);
            static_assert(std::is_same_v<decltype(val_or_ref{add}), val_or_ref<const fn::add&>>);

            static_assert(callable<val_or_ref<fn::add&>&, int, int>);
            static_assert(callable<const val_or_ref<fn::add&>&, int, int>);

            static_assert(!callable<val_or_ref<fn::add&>&, int>);
            static_assert(!callable<const val_or_ref<fn::add&>&, int>);

            static_assert(!callable<val_or_ref<fn::add&>&>);
            static_assert(!callable<const val_or_ref<fn::add&>&>);
        }

        // Deduction
        {
            auto f = []() -> str { return str{}; };
            val_or_ref val{f()};
            static_assert(std::is_same_v<decltype(val), val_or_ref<str>>);
        }
        {
            auto f = []() -> const str { return str{}; };
            val_or_ref val{f()};
            static_assert(std::is_same_v<decltype(val), val_or_ref<str>>);
        }
        {
            str tmp;
            auto f = [](str& s) -> str&& { return std::move(s); };
            val_or_ref val{f(tmp)};
            static_assert(std::is_same_v<decltype(val), val_or_ref<str>>);
        }
        {
            const str tmp;
            auto f = [](const str& s) -> const str&& { return std::move(s); };
            val_or_ref val{f(tmp)};
            static_assert(std::is_same_v<decltype(val), val_or_ref<str>>);
        }
        {
            str tmp;
            auto f = [](str& s) -> str& { return s; };
            val_or_ref val{f(tmp)};
            static_assert(std::is_same_v<decltype(val), val_or_ref<str&>>);
        }
        {
            const str tmp;
            auto f = [](const str& s) -> const str& { return s; };
            val_or_ref val{f(tmp)};
            static_assert(std::is_same_v<decltype(val), val_or_ref<const str&>>);
        }

        // Deduction (copying is preferred over wrapping).
        {
            str s = "hello";
            val_or_ref val{s};
            static_assert(std::is_same_v<decltype(val), val_or_ref<str&>>);
            val_or_ref cpy{val};
            static_assert(std::is_same_v<decltype(cpy), val_or_ref<str&>>);
            snn_require(cpy.get() == "hello");
        }
        {
            val_or_ref val{123};
            static_assert(std::is_same_v<decltype(val), val_or_ref<int>>);
            val_or_ref cpy{val};
            static_assert(std::is_same_v<decltype(cpy), val_or_ref<int>>);
            snn_require(cpy.get() == 123);
        }

        // as_ref
        {
            str s              = "hello";
            decltype(auto) val = as_ref(s);
            static_assert(std::is_same_v<decltype(val), val_or_ref<str&>>);
            snn_require(val.get() == "hello");
        }
        {
            const str s        = "hello";
            decltype(auto) val = as_ref(s);
            static_assert(std::is_same_v<decltype(val), val_or_ref<const str&>>);
            snn_require(val.get() == "hello");
        }
        {
            constexpr decltype(auto) val = as_ref("abc");
            static_assert(std::is_same_v<decltype(val), const val_or_ref<const char(&)[4]>>);
        }

        // as_cref
        {
            str s              = "hello";
            decltype(auto) val = as_cref(s);
            static_assert(std::is_same_v<decltype(val), val_or_ref<const str&>>);
            snn_require(val.get() == "hello");
        }
        {
            const str s        = "hello";
            decltype(auto) val = as_cref(s);
            static_assert(std::is_same_v<decltype(val), val_or_ref<const str&>>);
            snn_require(val.get() == "hello");
        }
        {
            constexpr decltype(auto) val = as_cref("abc");
            static_assert(std::is_same_v<decltype(val), const val_or_ref<const char(&)[4]>>);
        }

        // as_val
        {
            constexpr decltype(auto) val = as_val(123);
            static_assert(std::is_same_v<decltype(val), const val_or_ref<int>>);
            static_assert(val.get() == 123);
        }
        {
            str s              = "hello";
            decltype(auto) val = as_val(s);
            static_assert(std::is_same_v<decltype(val), val_or_ref<str>>);
            snn_require(val.get() == "hello");
            snn_require(s == "hello");
        }
        {
            const str s        = "hello";
            decltype(auto) val = as_val(s);
            static_assert(std::is_same_v<decltype(val), val_or_ref<str>>);
            snn_require(val.get() == "hello");
            snn_require(s == "hello");
        }
        {
            str s              = "hello";
            decltype(auto) val = as_val(std::move(s));
            static_assert(std::is_same_v<decltype(val), val_or_ref<str>>);
            snn_require(val.get() == "hello");
            snn_require(s.is_empty()); // Moved
        }
        {
            const str s        = "hello";
            decltype(auto) val = as_val(std::move(s));
            static_assert(std::is_same_v<decltype(val), val_or_ref<str>>);
            snn_require(val.get() == "hello");
            snn_require(s == "hello"); // Not moved.
        }
    }
}
