// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/error_code.hh"

#include "snn-core/unittest.hh"
#include "snn-core/generic/error.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                auto ec = make_error_code(generic::error::no_error);
                snn_require(!ec); // Zero value.
                snn_require(ec.value() == 0);
                snn_require(ec.message<cstrview>() == "No error");
            }
            {
                auto ec = make_error_code(generic::error::no_value);
                snn_require(ec); // Not zero.
                snn_require(ec.value() != 0);
                snn_require(ec.message<cstrview>() == "No value");
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

    constexpr array<null_term<const char*>, 4> error_message_lookup{
        "No error",
        "First",
        "Second",
        "Third",
    };

    inline constexpr error_category error_category{"snn::app", "App", error_message_lookup};

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
    namespace
    {
        constexpr bool test_error_code()
        {
            // error_category

            static_assert(sizeof(error_category) == 48);

            snn::error_category one{"snn::unittest_one", "one"};
            snn::error_category two{"snn::unittest_one\0two", "onetwo"};

            snn_require(one == one);
            snn_require(!(one == two));
            snn_require(one != two);
            snn_require(!(two != two));

            if (std::is_constant_evaluated())
            {
                snn_require(one < two); // Unspecified in non-constexpr context.
            }

            snn_require(one.id<cstrview>() == "snn::unittest_one");
            snn_require(one.name<cstrview>() == "one");
            snn_require(two.id<cstrview>() == "snn::unittest_one\0two");
            snn_require(two.name<cstrview>() == "onetwo");

            // error_code

            static_assert(sizeof(error_code) == 16);

            static_assert(std::is_trivially_copyable_v<error_code>);
            static_assert(is_trivially_relocatable_v<error_code>);

            static_assert(!error_code::has_boolean_state());

            auto zero = make_error_code(app::error::no_error);
            snn_require(!zero);
            snn_require(zero.value() == 0);

            auto first  = make_error_code(app::error::first);
            auto second = error_code{app::error::second};
            auto third  = error_code{3, app::error_category};

            snn_require(zero.non_zero_or(not_zero<i32>{1}, app::error_category) ==
                        app::error::first);
            snn_require(first.non_zero_or(not_zero<i32>{2}, app::error_category) ==
                        app::error::first);

            auto cpy = first;

            snn_require(first);
            snn_require(second);
            snn_require(third);
            snn_require(cpy);

            snn_require(first == cpy);
            snn_require(!(first != cpy));
            snn_require(first != second);
            snn_require(!(first == second));

            snn_require(cpy == app::error::first);
            snn_require(!(cpy != app::error::first));
            snn_require(cpy != app::error::second);
            snn_require(!(cpy == app::error::second));

            snn_require(third == app::error::third);

            snn_require(first.value() == 1);
            snn_require(second.value() == 2);
            snn_require(third.value() == 3);
            snn_require(cpy.value() == 1);

            snn_require(first.category() == second.category());
            snn_require(first.category() == cpy.category());
            snn_require(cpy.category() == app::error_category);
            snn_require(!(cpy.category() != app::error_category));
            snn_require(cpy.category() != one);
            snn_require(!(cpy.category() == one));

            snn_require(cpy.message<cstrview>() == "First");
            snn_require(second.message<cstrview>() == "Second");
            snn_require(third.message<cstrview>() == "Third");

            snn_require(cpy.category().id<cstrview>() == "snn::app");
            snn_require(cpy.category().name<cstrview>() == "App");

            auto fourth = error_code{4, app::error_category};
            snn_require(fourth.message<cstrview>() == "");

            auto other = make_error_code(4, one);
            snn_require(fourth != other);
            snn_require(fourth < other);

            auto empty = error_code{};
            snn_require(!empty);
            snn_require(empty.value() == 0);
            snn_require(empty != app::error::no_error);
            snn_require(!(empty == app::error::no_error));

            snn_require(empty != first);
            snn_require(!(empty == first));

            snn_require(empty.category().id<cstrview>() == "");
            snn_require(empty.category().name<cstrview>() == "");
            snn_require(empty.message<cstrview>() == "");
            snn_require(empty.category().message<cstrview>(0) == "");
            snn_require(empty.category().message<cstrview>(99) == "");

            auto empty2 = error_code{};
            snn_require(empty == empty2);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_error_code());
    }
}
