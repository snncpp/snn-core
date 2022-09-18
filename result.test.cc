// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/result.hh"

#include "snn-core/unittest.hh"
#include "snn-core/vec.hh"
#include "snn-core/generic/error.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                result<int> res = 123;
                snn_require(res);
                snn_require(res.has_value());
                snn_require(res.value() == 123);
                snn_require(res.value(promise::has_value) == 123);
                snn_require(res.value_or(-1) == 123);
            }
            {
                result<int> res = generic::error::memory_allocation_failure;
                snn_require(!res);
                snn_require(!res.has_value());
                snn_require_throws_code(res.value(), generic::error::memory_allocation_failure);
                snn_require(res.value_or(-1) == -1);
                snn_require(res.error_code() == generic::error::memory_allocation_failure);
            }
            {
                int i            = 123;
                result<int&> res = i;
                snn_require(res);
                snn_require(res.has_value());
                snn_require(res.value() == 123);
                snn_require(std::addressof(res.value()) == std::addressof(i));
                snn_require(res.value(promise::has_value) == 123);
                snn_require(res.value_or(-1) == 123); // Returns by value.
            }
            {
                result<int&> res = generic::error::memory_allocation_failure;
                snn_require(!res);
                snn_require(!res.has_value());
                snn_require_throws_code(res.value(), generic::error::memory_allocation_failure);
                snn_require(res.value_or(-1) == -1);
                snn_require(res.error_code() == generic::error::memory_allocation_failure);
            }
            {
                result<void> res = {};
                snn_require(res);
                snn_require(res.has_value());
            }
            {
                result<void> res = generic::error::memory_allocation_failure;
                snn_require(!res);
                snn_require(!res.has_value());
            }

            return true;
        }
    }
}

namespace snn::app
{
    enum class error : u8
    {
        no_error = 0,
        first,
        second,
        third,
    };

    inline constexpr error_category error_category{"snn::app", "App"};

    [[nodiscard]] constexpr error_code make_error_code(const error e) noexcept
    {
        return error_code{static_cast<i32>(e), error_category};
    }
}

namespace snn
{
    template <>
    struct is_error_code_enum_strict<app::error> : public std::true_type
    {
    };
}

namespace snn::app
{
    class ref_test
    {
      public:
        constexpr ref_test()
            : s_{},
              b_{false}
        {
        }

        template <usize N>
        constexpr ref_test(const char (&s)[N])
            : s_{s},
              b_{true}
        {
        }

        [[nodiscard]] constexpr result<str&> get() noexcept
        {
            if (b_)
            {
                return s_;
            }

            return app::error::first;
        }

        [[nodiscard]] constexpr result<const str&> get() const noexcept
        {
            if (b_)
            {
                return s_;
            }

            return app::error::second;
        }

      private:
        str s_;
        bool b_;
    };

    [[nodiscard]] constexpr result<i32> test_int_failure()
    {
        return app::error::first;
    }

    [[nodiscard]] constexpr result<i32> test_int_success()
    {
        return 382;
    }

    [[nodiscard]] constexpr result<str> test_string_failure()
    {
        return app::error::second;
    }

    [[nodiscard]] constexpr result<str> test_string_success()
    {
        return str{"A long string, which goes on the heap."};
    }

    [[nodiscard]] constexpr result<void> test_void_failure()
    {
        return app::error::third;
    }

    [[nodiscard]] constexpr result<void> test_void_success()
    {
        return {};
    }

    template <typename T>
    constexpr void indirect_copy_assignment(T& a, T& b)
    {
        a = b;
    }

    template <typename T>
    constexpr void indirect_move_assignment(T& a, T& b)
    {
        a = std::move(b);
    }

    namespace
    {
        constexpr bool test_assign_or_rebind()
        {
            // assign_or_rebind() - trivial
            {
                result<int> res{123};
                snn_require(res && res.value() == 123);

                int other = 456;

                // Has value - copy assign.
                res.assign_or_rebind(other);
                snn_require(res && res.value() == 456);
                snn_require(other == 456);

                other = 123456789;

                // Has value - move assign.
                res.assign_or_rebind(std::move(other));
                snn_require(res && res.value() == 123456789);
                snn_require(other == 123456789);

                // Has value - error assign.
                res.assign_or_rebind(app::error::first);
                snn_require(!res && res.error_code() == app::error::first);

                // Has no value - error assign.
                res.assign_or_rebind(make_error_code(app::error::second));
                snn_require(!res && res.error_code() == app::error::second);

                // Has no value - copy assign.
                res.assign_or_rebind(other);
                snn_require(res && res.value() == 123456789);
                snn_require(other == 123456789);

                res.assign_or_rebind(make_error_code(app::error::second));
                snn_require(!res && res.error_code() == app::error::second);

                // Has no value - move assign.
                res.assign_or_rebind(std::move(other));
                snn_require(res && res.value() == 123456789);
                snn_require(other == 123456789);

                // Must support `error_code` with 0 value.

                res.assign_or_rebind(app::error::no_error);
                snn_require(!res && res.error_code() == app::error::no_error);

                res.assign_or_rebind(make_error_code(app::error::no_error));
                snn_require(!res && res.error_code() == app::error::no_error);
            }

            // assign_or_rebind() - non-trivial
            {
                result<str> res{"abc"};
                snn_require(res && res.value() == "abc");

                str other{"one two"};

                // Has value - copy assign.
                res.assign_or_rebind(other);
                snn_require(res && res.value() == "one two");
                snn_require(other == "one two");

                other = "abcdefghijklmnopqrstuvwxyz";

                // Has value - move assign.
                res.assign_or_rebind(std::move(other));
                snn_require(res && res.value() == "abcdefghijklmnopqrstuvwxyz");
                snn_require(other == "one two");

                // Has value - error assign.
                res.assign_or_rebind(app::error::first);
                snn_require(!res && res.error_code() == app::error::first);

                // Has no value - error assign.
                res.assign_or_rebind(make_error_code(app::error::second));
                snn_require(!res && res.error_code() == app::error::second);

                // Has no value - copy assign.
                res.assign_or_rebind(other);
                snn_require(res && res.value() == "one two");
                snn_require(other == "one two");

                res.assign_or_rebind(make_error_code(app::error::second));
                snn_require(!res && res.error_code() == app::error::second);

                // Has no value - move assign.
                res.assign_or_rebind(std::move(other));
                snn_require(res && res.value() == "one two");
                snn_require(other == "");

                // Must support `error_code` with 0 value.

                res.assign_or_rebind(app::error::no_error);
                snn_require(!res && res.error_code() == app::error::no_error);

                res.assign_or_rebind(make_error_code(app::error::no_error));
                snn_require(!res && res.error_code() == app::error::no_error);
            }

            // assign_or_rebind() - reference
            {
                int a = 123;
                int b = 456;

                result<int&> res{a};
                snn_require(res && res.value() == 123);

                a = -123;
                snn_require(res.value() == -123);

                res.assign_or_rebind(b);
                snn_require(res && res.value() == 456);

                b = -456;
                snn_require(res.value() == -456);

                res.assign_or_rebind(app::error::first);
                snn_require(!res && res.error_code() == app::error::first);

                res.assign_or_rebind(b);
                snn_require(res && res.value() == -456);

                res.assign_or_rebind(app::error::first);
                snn_require(!res && res.error_code() == app::error::first);

                res.assign_or_rebind(make_error_code(app::error::second));
                snn_require(!res && res.error_code() == app::error::second);

                // Must support `error_code` with 0 value.

                res.assign_or_rebind(app::error::no_error);
                snn_require(!res && res.error_code() == app::error::no_error);

                res.assign_or_rebind(make_error_code(app::error::no_error));
                snn_require(!res && res.error_code() == app::error::no_error);
            }

            // assign_or_rebind() - void
            {
                result<void> res;
                snn_require(res);

                res.assign_or_rebind();
                snn_require(res);

                res.assign_or_rebind(app::error::first);
                snn_require(!res && res.error_code() == app::error::first);

                res.assign_or_rebind(make_error_code(app::error::second));
                snn_require(!res && res.error_code() == app::error::second);

                res.assign_or_rebind();
                snn_require(res);

                // Must support `error_code` with 0 value.

                res.assign_or_rebind(app::error::no_error);
                snn_require(!res && res.error_code() == app::error::no_error);

                res.assign_or_rebind(make_error_code(app::error::no_error));
                snn_require(!res && res.error_code() == app::error::no_error);
            }

            return true;
        }

        constexpr bool test_comparison()
        {
            // constexpr bool operator==(const result<T, Err>& left, const result<U, E>& right)
            // constexpr auto operator<=>(const result<T, Err>& left, const result<U, E>& right)
            {
                constexpr result<i32> a{123};
                constexpr result<i32> b{456};
                constexpr result<i32> e1{app::error::first};
                constexpr result<i32> e2{app::error::second};

                static_assert(a == a);
                static_assert(!(a == b));
                static_assert(e1 == e1);
                static_assert(!(e1 == e2));
                static_assert(!(e1 == a));
                static_assert(!(b == e2));

                static_assert(a != b);
                static_assert(!(a != a));
                static_assert(e1 != e2);
                static_assert(!(e1 != e1));
                static_assert(e1 != a);
                static_assert(a != e2);

                static_assert(a < b);
                static_assert(!(b < a));
                static_assert(!(a < a));
                static_assert(e1 < e2);
                static_assert(!(e1 < e1));
                static_assert(!(e2 < e1));
                static_assert(e1 < a);

                static_assert(b > a);
                static_assert(!(a > b));
                static_assert(!(a > a));
                static_assert(!(e1 > e2));
                static_assert(!(e1 > e1));
                static_assert(e2 > e1);
                static_assert(a > e1);

                static_assert(a <= b);
                static_assert(!(b <= a));
                static_assert(a <= a);
                static_assert(e1 <= e1);
                static_assert(e1 <= e2);
                static_assert(!(e2 <= e1));
                static_assert(e1 <= a);

                static_assert(b >= a);
                static_assert(!(a >= b));
                static_assert(a >= a);
                static_assert(e1 >= e1);
                static_assert(!(e1 >= e2));
                static_assert(e2 >= e1);
                static_assert(a >= e1);
            }

            // constexpr bool operator==(const result<T, Err>& left, const V& right)
            {
                constexpr result<i32> a{123};
                constexpr result<i32> e{generic::error::no_value};

                static_assert(a == 123);
                static_assert(123 == a);
                static_assert(!(a == 124));
                static_assert(!(124 == a));
                static_assert(!(e == 0));
                static_assert(!(0 == e));
                static_assert(!(e == 123));
                static_assert(!(123 == e));

                static_assert(a != 124);
                static_assert(124 != a);
                static_assert(!(a != 123));
                static_assert(!(123 != a));
                static_assert(e != 0);
                static_assert(0 != e);
                static_assert(e != 123);
                static_assert(123 != e);
            }

            // constexpr auto operator<=>(const result<T, Err>& left, const V& right)
            {
                constexpr result<i32> a{123};
                constexpr result<i32> e{generic::error::no_value};

                static_assert(a < 124);
                static_assert(122 < a);
                static_assert(!(a < 123));
                static_assert(!(123 < a));
                static_assert(e < 0);
                static_assert(!(0 < e));
                static_assert(e < 123);
                static_assert(!(123 < e));

                static_assert(a > 122);
                static_assert(124 > a);
                static_assert(!(a > 123));
                static_assert(!(123 > a));
                static_assert(!(e > 0));
                static_assert(0 > e);
                static_assert(!(e > 123));
                static_assert(123 > e);

                static_assert(a <= 123);
                static_assert(a <= 124);
                static_assert(123 <= a);
                static_assert(122 <= a);
                static_assert(!(a <= 122));
                static_assert(!(124 <= a));
                static_assert(e <= 0);
                static_assert(!(0 <= e));
                static_assert(e <= 123);
                static_assert(!(123 <= e));

                static_assert(a >= 123);
                static_assert(a >= 122);
                static_assert(123 >= a);
                static_assert(124 >= a);
                static_assert(!(a >= 124));
                static_assert(!(122 >= a));
                static_assert(!(e >= 0));
                static_assert(0 >= e);
                static_assert(!(e >= 123));
                static_assert(123 >= e);
            }

            return true;
        }

        constexpr bool test_copy_move()
        {
            // Copy constructor.
            {
                result<str> res = str{"abcdefghijklmnopqrstuvwxyz"};
                result<str> res2{res};

                snn_require(res);
                snn_require(res.has_value());
                snn_require(res.value() == "abcdefghijklmnopqrstuvwxyz");

                snn_require(res2);
                snn_require(res2.has_value());
                snn_require(res2.value() == "abcdefghijklmnopqrstuvwxyz");
            }
            {
                result<str> res = app::error::first;
                result<str> res2{res};

                snn_require(!res);
                snn_require(!res.has_value());
                snn_require(res.error_code() == app::error::first);

                snn_require(!res2);
                snn_require(!res2.has_value());
                snn_require(res2.error_code() == app::error::first);
            }

            // Copy assignment operator.
            {
                result<str> res  = str{"one two three"};
                result<str> res2 = str{"abcdefghijklmnopqrstuvwxyz"};

                res = res2;

                snn_require(res);
                snn_require(res.has_value());
                snn_require(res.value() == "abcdefghijklmnopqrstuvwxyz");

                snn_require(res2);
                snn_require(res2.has_value());
                snn_require(res2.value() == "abcdefghijklmnopqrstuvwxyz");
            }
            {
                result<str> res  = str{"abcdefghijklmnopqrstuvwxyz"};
                result<str> res2 = app::error::first;

                res = res2;

                snn_require(!res);
                snn_require(!res.has_value());
                snn_require(res.error_code() == app::error::first);

                snn_require(!res2);
                snn_require(!res2.has_value());
                snn_require(res2.error_code() == app::error::first);
            }
            {
                result<str> res  = app::error::first;
                result<str> res2 = str{"abcdefghijklmnopqrstuvwxyz"};

                res = res2;

                snn_require(res);
                snn_require(res.has_value());
                snn_require(res.value() == "abcdefghijklmnopqrstuvwxyz");

                snn_require(res2);
                snn_require(res2.has_value());
                snn_require(res2.value() == "abcdefghijklmnopqrstuvwxyz");
            }
            {
                result<str> res  = app::error::first;
                result<str> res2 = app::error::second;

                res = res2;

                snn_require(!res);
                snn_require(!res.has_value());
                snn_require(res.error_code() == app::error::second);

                snn_require(!res2);
                snn_require(!res2.has_value());
                snn_require(res2.error_code() == app::error::second);
            }
            {
                result<str> res = str{"abcdefghijklmnopqrstuvwxyz"};

                app::indirect_copy_assignment(res, res);

                snn_require(res);
                snn_require(res.has_value());
                snn_require(res.value() == "abcdefghijklmnopqrstuvwxyz");
            }
            {
                result<str> res = app::error::first;

                app::indirect_copy_assignment(res, res);

                snn_require(!res);
                snn_require(!res.has_value());
                snn_require(res.error_code() == app::error::first);
            }

            // Move constructor.
            {
                result<str> res = str{"abcdefghijklmnopqrstuvwxyz"};
                result<str> res2{std::move(res)};

                snn_require(res);
                snn_require(res.has_value());
                snn_require(res.value() == "");

                snn_require(res2);
                snn_require(res2.has_value());
                snn_require(res2.value() == "abcdefghijklmnopqrstuvwxyz");
            }
            {
                result<str> res = app::error::first;
                result<str> res2{std::move(res)};

                snn_require(!res);
                snn_require(!res.has_value());
                snn_require(res.error_code() == app::error::first);

                snn_require(!res2);
                snn_require(!res2.has_value());
                snn_require(res2.error_code() == app::error::first);
            }

            // Move assignment operator.
            {
                result<str> res  = str{"one two three"};
                result<str> res2 = str{"abcdefghijklmnopqrstuvwxyz"};

                res = std::move(res2);

                snn_require(res);
                snn_require(res.has_value());
                snn_require(res.value() == "abcdefghijklmnopqrstuvwxyz");

                snn_require(res2);
                snn_require(res2.has_value());
                snn_require(res2.value() == "one two three");
            }
            {
                result<str> res  = str{"abcdefghijklmnopqrstuvwxyz"};
                result<str> res2 = app::error::first;

                res = std::move(res2);

                snn_require(!res);
                snn_require(!res.has_value());
                snn_require(res.error_code() == app::error::first);

                snn_require(!res2);
                snn_require(!res2.has_value());
                snn_require(res2.error_code() == app::error::first);
            }
            {
                result<str> res  = app::error::first;
                result<str> res2 = str{"abcdefghijklmnopqrstuvwxyz"};

                res = std::move(res2);

                snn_require(res);
                snn_require(res.has_value());
                snn_require(res.value() == "abcdefghijklmnopqrstuvwxyz");

                snn_require(res2);
                snn_require(res2.has_value());
                snn_require(res2.value() == "");
            }
            {
                result<str> res  = app::error::first;
                result<str> res2 = app::error::second;

                res = std::move(res2);

                snn_require(!res);
                snn_require(!res.has_value());
                snn_require(res.error_code() == app::error::second);

                snn_require(!res2);
                snn_require(!res2.has_value());
                snn_require(res2.error_code() == app::error::second);
            }
            {
                result<str> res = str{"abcdefghijklmnopqrstuvwxyz"};

                app::indirect_move_assignment(res, res);

                snn_require(res);
                snn_require(res.has_value());
                snn_require(res.value() == "abcdefghijklmnopqrstuvwxyz");
            }
            {
                result<str> res = app::error::first;

                app::indirect_move_assignment(res, res);

                snn_require(!res);
                snn_require(!res.has_value());
                snn_require(res.error_code() == app::error::first);
            }

            return true;
        }

        constexpr bool test_call_if()
        {
            // call_if
            {
                constexpr auto r = call_if([](int a, int b) { return a + b; }, test_int_success(),
                                           test_int_success());
                static_assert(r);
                static_assert(r.value() == 764);
            }
            {
                auto r = call_if(
                    [](str&& s, int&& i) {
                        s.append_integral(i);
                        return std::move(s);
                    },
                    test_string_success(), test_int_success());
                snn_require(r);
                snn_require(r.has_value());
                snn_require(r.value() == "A long string, which goes on the heap.382");
            }
            {
                auto r = call_if(
                    [](str&& s, int&& i) {
                        s.append_integral(i);
                        return std::move(s);
                    },
                    test_string_success(), test_int_failure());
                snn_require(!r);
                snn_require(r.error_code() == app::error::first);
            }
            {
                auto r = call_if(
                    [](str&& s, int&& i) {
                        s.append_integral(i);
                        return std::move(s);
                    },
                    test_string_failure(), test_int_success());
                snn_require(!r);
                snn_require(r.error_code() == app::error::second);
            }
            {
                auto r = call_if(
                    [](str&& s, int&& i) {
                        s.append_integral(i);
                        return std::move(s);
                    },
                    test_string_failure(), test_int_failure());
                snn_require(!r);
                snn_require(r.error_code() == app::error::second);
            }

            return true;
        }

        constexpr bool test_result()
        {
            {
                constexpr auto r = test_int_failure();
                static_assert(!r);
                static_assert(!r.has_value());
                static_assert(r.error_code() == app::error::first);
                snn_require_throws_code(r.value(), app::error::first);

                constexpr auto r2 = r;
                static_assert(!r2);
                static_assert(!r2.has_value());
                static_assert(r2.error_code() == app::error::first);
                snn_require_throws_code(r2.value(), app::error::first);
            }
            {
                snn_require_throws_code(test_int_failure().value(), app::error::first);
            }
            {
                constexpr auto r = test_int_failure().and_then([](const int i) { return i + i; });
                static_assert(!r);
                static_assert(!r.has_value());
                static_assert(r.error_code() == app::error::first);
                snn_require_throws_code(r.value(), app::error::first);
            }
            {
                auto [i, ec] = test_int_failure().unpack();
                snn_require(ec);
                snn_require(ec == app::error::first);
                snn_require(i == 0);
            }
            {
                constexpr i32 i = test_int_failure().value_or_default();
                static_assert(i == 0);
            }
            {
                constexpr i32 i = test_int_failure().value_or(0);
                static_assert(i == 0);
            }
            {
                constexpr i32 i = test_int_failure().value_or(123);
                static_assert(i == 123);
            }
            {
                constexpr i32 def = 123;
                constexpr i32 i   = test_int_failure().value_or(def);
                static_assert(i == 123);
            }
            {
                constexpr auto r = test_int_success();
                static_assert(r);
                static_assert(r.has_value());
                static_assert(r.value() == 382);
                static_assert(r.value(promise::has_value) == 382);

                static_assert(test_int_success().value() == 382);

                constexpr auto ec = r.error_code();
                static_assert(!ec);

                constexpr auto r2 = r;
                static_assert(r2);
                static_assert(r2.has_value());
                static_assert(r2.value() == 382);
            }
            {
                constexpr auto r =
                    test_int_success().and_then([](const int i) -> result<int> { return i + i; });
                static_assert(r);
                static_assert(r.has_value());
                static_assert(r.value() == 764);
            }
            {
                constexpr auto r = test_int_success().and_then([](const int i) { return i + i; });
                static_assert(r);
                static_assert(r.has_value());
                static_assert(r.value() == 764);
            }
            {
                constexpr auto r = test_int_success().and_then([](const int) -> result<int> {
                    return app::error::second;
                });
                static_assert(!r);
                static_assert(!r.has_value());
                static_assert(r.error_code() == app::error::second);
                snn_require_throws_code(r.value(), app::error::second);
            }
            {
                auto [i, ec] = test_int_success().unpack();
                snn_require(!ec);
                snn_require(i == 382);
            }
            {
                constexpr i32 i = test_int_success().value_or_default();
                static_assert(i == 382);
            }
            {
                constexpr i32 i = test_int_success().value_or(0);
                static_assert(i == 382);
            }
            {
                constexpr i32 i = test_int_success().value_or(123);
                static_assert(i == 382);
            }
            {
                constexpr i32 def = 123;
                constexpr i32 i   = test_int_success().value_or(def);
                static_assert(i == 382);
            }
            {
                snn_require_throws_code(test_string_failure().value(), app::error::second);
            }
            {
                auto r = test_string_failure();
                snn_require(!r);
                snn_require(!r.has_value());
                snn_require(r.error_code() == app::error::second);
                snn_require_throws_code(r.value(), app::error::second);
            }
            {
                auto [s, ec] = test_string_failure().unpack();
                snn_require(ec);
                snn_require(ec == app::error::second);
                snn_require(s == "");
            }
            {
                const str s = test_string_failure().value_or_default();
                snn_require(s == "");
            }
            {
                const str s = test_string_failure().value_or("");
                snn_require(s == "");
            }
            {
                const str s = test_string_failure().value_or("abc");
                snn_require(s == "abc");
            }
            {
                const str s = test_string_failure().value_or("A long string, goes on the heap.");
                snn_require(s == "A long string, goes on the heap.");
            }
            {
                const str def = "A long string, goes on the heap.";
                const str s   = test_string_failure().value_or(def);
                snn_require(s == "A long string, goes on the heap.");
                snn_require(def == "A long string, goes on the heap.");
            }
            {
                str def     = "A long string, goes on the heap.";
                const str s = test_string_failure().value_or(def);
                snn_require(s == "A long string, goes on the heap.");
                snn_require(def == "A long string, goes on the heap.");
            }
            {
                str def     = "A long string, goes on the heap.";
                const str s = test_string_failure().value_or(std::move(def));
                snn_require(s == "A long string, goes on the heap.");
                snn_require(def == "");
            }
            {
                auto r = test_string_success();
                snn_require(r);
                snn_require(r.has_value());
                snn_require(r.value() == "A long string, which goes on the heap.");
                snn_require(r.value(promise::has_value) ==
                            "A long string, which goes on the heap.");
                snn_require(r.value(promise::has_value).size() == 38);

                auto ec = r.error_code();
                snn_require(!ec);

                snn_require(test_string_success().value() ==
                            "A long string, which goes on the heap.");
            }
            {
                auto [s, ec] = test_string_success().unpack();
                snn_require(!ec);
                snn_require(s == "A long string, which goes on the heap.");
            }
            {
                const str s = test_string_success().value_or_default();
                snn_require(s == "A long string, which goes on the heap.");
            }
            {
                const str s = test_string_success().value_or("abc");
                snn_require(s == "A long string, which goes on the heap.");
            }
            {
                const str s = test_string_success().value_or("abcdefghijklmnopqrstuvwxyz");
                snn_require(s == "A long string, which goes on the heap.");
            }
            {
                const str def = "abcdefghijklmnopqrstuvwxyz";
                const str s   = test_string_success().value_or(def);
                snn_require(s == "A long string, which goes on the heap.");
                snn_require(def == "abcdefghijklmnopqrstuvwxyz");
            }
            {
                str def     = "abcdefghijklmnopqrstuvwxyz";
                const str s = test_string_success().value_or(def);
                snn_require(s == "A long string, which goes on the heap.");
                snn_require(def == "abcdefghijklmnopqrstuvwxyz");
            }
            {
                str def     = "abcdefghijklmnopqrstuvwxyz";
                const str s = test_string_success().value_or(std::move(def));
                snn_require(s == "A long string, which goes on the heap.");
                snn_require(def == "abcdefghijklmnopqrstuvwxyz");
            }
            {
                constexpr auto r = test_void_failure();
                static_assert(!r);
                static_assert(!r.has_value());
                static_assert(r.error_code() == app::error::third);
                snn_require_throws_code(r.or_throw(), app::error::third);

                snn_require_throws_code(test_void_failure().or_throw(), app::error::third);
            }
            {
                constexpr auto r = test_void_failure().and_then([]() { return 33; });
                static_assert(!r);
                static_assert(!r.has_value());
                static_assert(r.error_code() == app::error::third);
            }
            {
                constexpr auto r = test_void_success();
                static_assert(r);
                snn_require_nothrow(r.or_throw());

                constexpr auto ec = r.error_code();
                static_assert(!ec);

                snn_require_nothrow(test_void_success().or_throw());
            }
            {
                constexpr auto r = test_void_success().and_then([]() -> result<int> { return 33; });
                static_assert(r);
                static_assert(r.has_value());
                static_assert(r.value() == 33);
            }
            {
                ref_test cont;
                auto r = cont.get();
                snn_require(!r);
                snn_require(!r.has_value());
                snn_require(r.error_code() == app::error::first);
                snn_require_throws_code(r.value(), app::error::first);

                auto r2 = r;
                snn_require(!r2);
                snn_require(!r2.has_value());
            }
            {
                ref_test cont;
                auto r = cont.get().and_then([](str& s) -> result<str&> {
                    s.append('x');
                    return s;
                });
                snn_require(!r);
                snn_require(!r.has_value());
                snn_require(r.error_code() == app::error::first);
                snn_require_throws_code(r.value(), app::error::first);
            }
            {
                const ref_test cont;
                const auto r = cont.get();
                snn_require(!r);
                snn_require(!r.has_value());
                snn_require(r.error_code() == app::error::second);
                snn_require_throws_code(r.value(), app::error::second);
            }
            {
                ref_test cont{"This should go on the heap."};
                auto r = cont.get();
                snn_require(r);
                snn_require(r.has_value());
                snn_require(r.value() == "This should go on the heap.");
                snn_require(r.value(promise::has_value) == "This should go on the heap.");
                snn_require(r.value(promise::has_value).size() == 27);

                auto ec = r.error_code();
                snn_require(!ec);

                auto& s = r.value(promise::has_value);
                s.append(" This should trigger another memory allocation.");

                auto r2 = cont.get();
                snn_require(r2);
                snn_require(r2.has_value());
                snn_require(&r.value(promise::has_value) == &r2.value(promise::has_value));
                snn_require(r2.value(promise::has_value) ==
                            "This should go on the heap."
                            " This should trigger another memory allocation.");
                snn_require(r2.value(promise::has_value).size() == 74);

                auto r3 = r2;
                snn_require(r3);
                snn_require(r3.has_value());
                snn_require(r3.value(promise::has_value) ==
                            "This should go on the heap."
                            " This should trigger another memory allocation.");
            }
            {
                ref_test cont{"This should go on the heap."};
                auto r = cont.get().and_then([](str& s) -> result<str&> {
                    s.append("..");
                    return s;
                });
                snn_require(r);
                snn_require(r.has_value());
                snn_require(r.value() == "This should go on the heap...");
            }
            {
                const ref_test cont{"This should go on the heap."};
                auto r = cont.get();
                snn_require(r);
                snn_require(r.has_value());
                snn_require(r.value() == "This should go on the heap.");
                snn_require(r.value(promise::has_value) == "This should go on the heap.");
                snn_require(r.value(promise::has_value).size() == 27);

                auto r2 = cont.get();
                snn_require(r2);
                snn_require(r2.has_value());
                snn_require(r2.value() == "This should go on the heap.");
                snn_require(r2.value(promise::has_value) == "This should go on the heap.");
                snn_require(r2.value(promise::has_value).size() == 27);
                snn_require(&r.value(promise::has_value) == &r2.value(promise::has_value));
            }

            // An `error_code` with a value of 0 (no error) should not be passed to a `result`, but
            // if it happens it must still be treated as an error.
            {
                static_assert(to_underlying(app::error::no_error) == 0);
            }
            {
                constexpr result<int> res = app::error::no_error;
                static_assert(!res);
                static_assert(res.error_code() == app::error::no_error);
            }
            {
                const result<str> res = app::error::no_error;
                snn_require(!res);
                snn_require(res.error_code() == app::error::no_error);
            }
            {
                constexpr result<int&> res = app::error::no_error;
                static_assert(!res);
                static_assert(res.error_code() == app::error::no_error);
            }
            {
                constexpr result<void> res = app::error::no_error;
                static_assert(!res);
                static_assert(res.error_code() == app::error::no_error);
            }
            {
                // `unpack()` promotes the error if needed.
                const auto [val, ec] = result<int>{app::error::no_error}.unpack();
                snn_require(ec);
                snn_require(ec == generic::error::unexpected_zero_value_error_code);
            }

            // result<T&>::value_or
            {
                u16 src          = 123;
                result<u16&> r   = src;
                decltype(auto) i = r.value_or(456);
                static_assert(std::is_same_v<decltype(i), u16>);
                snn_require(i == 123);
            }
            {
                result<u16&> r   = nullopt;
                decltype(auto) i = r.value_or(456);
                static_assert(std::is_same_v<decltype(i), u16>);
                snn_require(i == 456);
            }
            {
                str src          = "abc";
                result<str&> r   = src;
                decltype(auto) s = r.value_or("def");
                static_assert(std::is_same_v<decltype(s), str>);
                snn_require(s == "abc");
            }
            {
                result<str&> r   = nullopt;
                decltype(auto) s = r.value_or("def");
                static_assert(std::is_same_v<decltype(s), str>);
                snn_require(s == "def");
            }

            // result<T&>::value_or_default
            {
                str src          = "abc";
                result<str&> r   = src;
                decltype(auto) s = r.value_or_default();
                static_assert(std::is_same_v<decltype(s), str>);
                snn_require(s == "abc");
            }
            {
                result<str&> r   = nullopt;
                decltype(auto) s = r.value_or_default();
                static_assert(std::is_same_v<decltype(s), str>);
                snn_require(s.is_empty());
            }

            return true;
        }

        constexpr bool test_type_traits()
        {
            static_assert(is_result_v<result<str>>);
            static_assert(is_result_v<const result<str>>);
            static_assert(is_result_v<result<const str>>);
            static_assert(is_result_v<result<int>>);
            static_assert(is_result_v<result<const int>>);
            static_assert(is_result_v<result<int&>>);
            static_assert(is_result_v<result<const int&>>);
            static_assert(is_result_v<result<void>>);

            static_assert(!is_result_v<str>);
            static_assert(!is_result_v<const str>);
            static_assert(!is_result_v<int>);
            static_assert(!is_result_v<const int>);
            static_assert(!is_result_v<int&>);
            static_assert(!is_result_v<const int&>);
            static_assert(!is_result_v<void>);

            static_assert(std::is_same_v<result_compat_t<int>, int>);
            static_assert(std::is_same_v<result_compat_t<int&>, int&>);
            static_assert(std::is_same_v<result_compat_t<int&&>, int>);
            static_assert(std::is_same_v<result_compat_t<const int>, int>);
            static_assert(std::is_same_v<result_compat_t<const int&>, const int&>);
            static_assert(std::is_same_v<result_compat_t<const int&&>, int>);

            static_assert(std::is_same_v<result_wrap_t<str>, result<str>>);
            static_assert(std::is_same_v<result_wrap_t<int>, result<int>>);
            static_assert(std::is_same_v<result_wrap_t<int&>, result<int&>>);
            static_assert(std::is_same_v<result_wrap_t<str&&>, result<str>>);
            static_assert(std::is_same_v<result_wrap_t<void>, result<void>>);

            static_assert(std::is_same_v<result_wrap_t<result<str>>, result<str>>);
            static_assert(std::is_same_v<result_wrap_t<result<int>>, result<int>>);
            static_assert(std::is_same_v<result_wrap_t<result<int&>>, result<int&>>);
            static_assert(std::is_same_v<result_wrap_t<result<void>>, result<void>>);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        static_assert(app::test_comparison());
        static_assert(app::test_type_traits());

        snn_static_require(app::test_assign_or_rebind());
        snn_static_require(app::test_copy_move());
        snn_static_require(app::test_call_if());
        snn_static_require(app::test_result());

        static_assert(sizeof(result<u8>) == 24);
        static_assert(sizeof(result<i32>) == 24);
        static_assert(sizeof(result<usize>) == 24);

        static_assert(sizeof(result<str>) == 32);

        static_assert(sizeof(result<i32&>) == 24);
        static_assert(sizeof(result<void>) == 16);

        static_assert(sane<result<int>>);
        static_assert(sane<result<int&>>);
        static_assert(sane<result<str>>);
        static_assert(sane<result<void>>);

        static_assert(std::is_copy_constructible_v<result<str>>);
        static_assert(std::is_copy_assignable_v<result<str>>);
        static_assert(std::is_nothrow_move_constructible_v<result<str>>);
        static_assert(std::is_nothrow_move_assignable_v<result<str>>);

        static_assert(std::is_same_v<result<str>::value_type, str>);
        static_assert(std::is_same_v<result<str&>::value_type, str>);
        static_assert(std::is_same_v<result<const str&>::value_type, str>);

        static_assert(is_trivially_relocatable_v<result<int>>);
        static_assert(is_trivially_relocatable_v<result<int&>>);
        static_assert(is_trivially_relocatable_v<result<const int&>>);
        static_assert(is_trivially_relocatable_v<result<void>>);
        static_assert(is_trivially_relocatable_v<result<str>>);
        static_assert(!is_trivially_relocatable_v<result<app::ref_test>>);

        // trivially_relocatable
        {
            vec<result<str>> v;

            v.append(app::error::first);
            v.append(str{"abcdefghijklmnopqrstuvwxyz"});
            v.append(str{"123"});
            v.append(app::error::second);

            // Make sure that the vector reallocates.
            snn_require(v.capacity() < 20);
            v.reserve(20);
            snn_require(v.capacity() >= 20);

            snn_require(v.count() == 4);
            snn_require(v.at(0, promise::within_bounds).error_code() == app::error::first);
            snn_require(v.at(1, promise::within_bounds) == "abcdefghijklmnopqrstuvwxyz");
            snn_require(v.at(2, promise::within_bounds) == "123");
            snn_require(v.at(3, promise::within_bounds).error_code() == app::error::second);
        }
    }
}
