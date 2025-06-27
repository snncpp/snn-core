// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/optional.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                array a{123, 456, 789};

                auto opt = a.at(1);
                static_assert(std::is_same_v<decltype(opt), optional<int&>>);

                snn_require(opt); // Has value.
                snn_require(opt.has_value());
                snn_require(opt.value() == 456);
                snn_require(opt.value(assume::has_value) == 456);
                snn_require(opt.value_or(-1) == 456);
            }
            {
                array a{123, 456, 789};

                auto opt = a.at(3); // Out of bounds.
                static_assert(std::is_same_v<decltype(opt), optional<int&>>);

                snn_require(!opt); // Has no value.
                snn_require(!opt.has_value());
                snn_require_throws_code(opt.value(), generic::error::no_value);
                snn_require(opt.value_or(-1) == -1);
            }

            return true;
        }

        constexpr optional<i32> test_int_failure()
        {
            return nullopt;
        }

        constexpr optional<i32> test_int_success()
        {
            return 382;
        }

        inline optional<i32&> test_int_ref_failure()
        {
            return nullopt;
        }

        inline optional<i32&> test_int_ref_success(i32& i)
        {
            return i;
        }

        constexpr optional<void> test_void_failure()
        {
            return nullopt;
        }

        constexpr optional<void> test_void_success()
        {
            return {};
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        static_assert(sizeof(optional<u8>) == 2);
        static_assert(sizeof(optional<u16>) == 4);
        static_assert(sizeof(optional<i32>) == 8);
        static_assert(sizeof(optional<usize>) == 16);
        static_assert(sizeof(optional<str>) == 32);

        static_assert(sizeof(optional<i32&>) == 8);

        static_assert(sizeof(optional<void>) == 1);

        static_assert(is_result_v<optional<int>>);
        static_assert(is_result_v<optional<int&>>);
        static_assert(is_result_v<optional<const int&>>);
        static_assert(is_result_v<optional<void>>);
        static_assert(is_result_v<const optional<int>>);
        static_assert(is_result_v<const optional<int&>>);
        static_assert(is_result_v<const optional<const int&>>);
        static_assert(is_result_v<const optional<void>>);

        static_assert(std::is_same_v<result_wrap_t<optional<int>>, optional<int>>);
        static_assert(std::is_same_v<result_wrap_t<optional<int&>>, optional<int&>>);
        static_assert(std::is_same_v<result_wrap_t<optional<const int&>>, optional<const int&>>);
        static_assert(std::is_same_v<result_wrap_t<optional<void>>, optional<void>>);

        static_assert(std::is_same_v<result_wrap_t<int, optional_error_storage>, optional<int>>);
        static_assert(std::is_same_v<result_wrap_t<int&, optional_error_storage>, optional<int&>>);
        static_assert(std::is_same_v<result_wrap_t<const int&, optional_error_storage>,
                                     optional<const int&>>);
        static_assert(std::is_same_v<result_wrap_t<void, optional_error_storage>, optional<void>>);

        static_assert(
            std::is_same_v<result_wrap_t<result<int>, optional_error_storage>, result<int>>);

        static_assert(is_trivially_relocatable_v<optional<int>>);
        static_assert(is_trivially_relocatable_v<optional<int&>>);
        static_assert(is_trivially_relocatable_v<optional<const int&>>);
        static_assert(is_trivially_relocatable_v<optional<void>>);
        static_assert(is_trivially_relocatable_v<optional<str>>);

        static_assert(optional_error_storage::has_boolean_state());
        static_assert(optional_error_storage::default_error_code() == nullopt);

        {
            constexpr auto opt = app::test_int_failure();
            static_assert(!opt);
            static_assert(!opt.has_value());
            snn_require_throws_code(opt.value(), generic::error::no_value);
        }
        {
            auto [i, ec] = app::test_int_failure().unpack();
            snn_require(ec);
            snn_require(i == 0);
        }

        {
            auto opt = app::test_int_ref_failure();
            snn_require(!opt);
            snn_require(!opt.has_value());
            snn_require_throws_code(opt.value(), generic::error::no_value);
        }

        {
            int i = 472;

            auto opt = app::test_int_ref_success(i);
            snn_require(opt);
            snn_require(opt.has_value());
            snn_require(opt.value() == 472);
            snn_require(&opt.value() == &i);
            snn_require(opt.value(assume::has_value) == 472);
            snn_require(&opt.value(assume::has_value) == &i);

            auto ec = opt.error_code();
            snn_require(!ec);

            snn_require(app::test_int_ref_success(i).value() == 472);
            snn_require(&app::test_int_ref_success(i).value() == &i);
        }

        {
            constexpr auto opt = app::test_int_success();
            static_assert(opt);
            static_assert(opt.has_value());
            static_assert(opt.value() == 382);
            static_assert(opt.value(assume::has_value) == 382);

            constexpr auto ec = opt.error_code();
            static_assert(!ec);

            static_assert(app::test_int_success().value() == 382);
        }
        {
            auto [i, ec] = app::test_int_success().unpack();
            snn_require(!ec);
            snn_require(i == 382);
        }

        {
            constexpr auto opt = app::test_void_failure();
            static_assert(!opt);
            static_assert(!opt.has_value());
            static_assert(opt.error_code() == generic::error::no_value);

            snn_require_throws_code(app::test_void_failure().or_throw(), generic::error::no_value);
        }

        {
            constexpr auto opt = app::test_void_success();
            static_assert(opt);
            static_assert(opt.has_value());

            constexpr auto ec = opt.error_code();
            static_assert(!ec);
        }

        {
            constexpr auto opt =
                app::test_void_failure().and_then([]() -> optional<int> { return 33; });
            static_assert(!opt);
        }
    }
}
