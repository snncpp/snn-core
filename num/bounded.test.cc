// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/num/bounded.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                num::bounded<int, -8, 12> n = 0;
                snn_require(!n);
                snn_require(n.get() == 0);

                static_assert(n.min() == -8);
                static_assert(n.max() == 12);
                static_assert(n.can_be_zero());
            }
            {
                num::bounded<int, 10, 38> n = 15;
                snn_require(n);
                snn_require(n.get() == 15);

                static_assert(n.min() == 10);
                static_assert(n.max() == 38);
                static_assert(!n.can_be_zero());
                snn_require(n.not_zero().get() == 15);
            }

            snn_require_throws_code((num::bounded<int, 1, 5>{0}), num::error::out_of_range);
            snn_require_throws_code((num::bounded<int, 1, 5>{6}), num::error::out_of_range);
            snn_require_throws_code((num::bounded<int, -42, 38>{-43}), num::error::out_of_range);
            snn_require_throws_code((num::bounded<int, -42, 38>{39}), num::error::out_of_range);

            return true;
        }

        constexpr bool test_bounded()
        {
            static_assert(num::bounded<int, 0, 0>::can_be_zero());
            static_assert(num::bounded<int, -1, 0>::can_be_zero());
            static_assert(num::bounded<int, 0, 1>::can_be_zero());
            static_assert(num::bounded<int, -1, 1>::can_be_zero());
            static_assert(!num::bounded<int, 1, 1>::can_be_zero());
            static_assert(!num::bounded<int, 1, 2>::can_be_zero());
            static_assert(!num::bounded<int, -1, -1>::can_be_zero());
            static_assert(!num::bounded<int, -2, -1>::can_be_zero());

            // constexpr bounded(const Int value)
            // constexpr explicit operator bool() const noexcept
            // constexpr Int get() const noexcept
            // static constexpr Int min() noexcept
            // static constexpr Int max() noexcept
            {
                num::bounded<usize, 1, 5'000> n = 499;
                snn_require(n);
                snn_require(n.get() == 499);
                static_assert(n.min() == 1);
                static_assert(n.max() == 5'000);
                static_assert(!n.can_be_zero());
                snn_require(n.not_zero().get() == 499);
            }
            {
                num::bounded<u16, 0, 65'000> n = 0;
                snn_require(!n);
                snn_require(n.get() == 0);
                static_assert(n.can_be_zero());
            }
            {
                num::bounded<i32, -42, 38> n{-14};
                snn_require(n);
                snn_require(n.get() == -14);
                static_assert(n.can_be_zero());
            }

            // constexpr bounded(const Int value)
            {
                snn_require_throws_code((num::bounded<u32, 1, 50>{0}), num::error::out_of_range);
                snn_require_throws_code((num::bounded<u32, 1, 50>{51}), num::error::out_of_range);
                snn_require_throws_code((num::bounded<i32, -7, 5>{-8}), num::error::out_of_range);
                snn_require_throws_code((num::bounded<i32, -7, 5>{6}), num::error::out_of_range);
            }

            // constexpr bounded(const bounded<I, MinOther, MaxOther> other)
            {
                num::bounded<i32, -42, 38> n{-14};

                num::bounded<i16, -20, 20> n2{n};
                snn_require(n2);
                snn_require(n2.get() == -14);

                snn_require_throws_code((num::bounded<i16, 0, 100>{n}), num::error::out_of_range);
            }
            {
                constexpr auto min = constant::limit<i32>::min;
                num::bounded<i32, min, 100> n{min};
                snn_require_throws_code((num::bounded<i8, 0, 100>{n}), num::error::out_of_range);
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
        snn_static_require(app::test_bounded());
    }
}
