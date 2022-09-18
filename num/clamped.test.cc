// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/num/clamped.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                num::clamped<int, -8, 12> n = 0;
                snn_require(!n);
                snn_require(n.get() == 0);

                static_assert(n.min() == -8);
                static_assert(n.max() == 12);
                static_assert(n.can_be_zero());
            }
            {
                num::clamped<int, -8, 12> n = 5;
                snn_require(n);
                snn_require(n.get() == 5);

                static_assert(n.min() == -8);
                static_assert(n.max() == 12);
                static_assert(n.can_be_zero());
            }
            {
                num::clamped<int, 10, 38> n = 5;
                snn_require(n);
                snn_require(n.get() == 10);

                static_assert(n.min() == 10);
                static_assert(n.max() == 38);
                static_assert(!n.can_be_zero());
                snn_require(n.not_zero().get() == 10);
            }
            {
                num::clamped<int, 10, 38> n = 47;
                snn_require(n);
                snn_require(n.get() == 38);

                static_assert(n.min() == 10);
                static_assert(n.max() == 38);
                static_assert(!n.can_be_zero());
                snn_require(n.not_zero().get() == 38);
            }

            return true;
        }

        constexpr bool test_clamped()
        {
            static_assert(num::clamped<int, 0, 0>::can_be_zero());
            static_assert(num::clamped<int, -1, 0>::can_be_zero());
            static_assert(num::clamped<int, 0, 1>::can_be_zero());
            static_assert(num::clamped<int, -1, 1>::can_be_zero());
            static_assert(!num::clamped<int, 1, 1>::can_be_zero());
            static_assert(!num::clamped<int, 1, 2>::can_be_zero());
            static_assert(!num::clamped<int, -1, -1>::can_be_zero());
            static_assert(!num::clamped<int, -2, -1>::can_be_zero());

            // constexpr clamped(const Int value) noexcept : value_{value}
            // constexpr explicit operator bool() const noexcept
            // constexpr Int get() const noexcept
            // static constexpr Int min() noexcept
            // static constexpr Int max() noexcept
            {
                num::clamped<u8, 10, 38> n = 47;
                snn_require(n);
                snn_require(n.get() == 38);
                static_assert(n.min() == 10);
                static_assert(n.max() == 38);
                static_assert(!n.can_be_zero());
                snn_require(n.not_zero().get() == 38);
            }
            {
                num::clamped<usize, 1, 5'000> n = 0;
                snn_require(n);
                snn_require(n.get() == 1);
                static_assert(n.min() == 1);
                static_assert(n.max() == 5'000);
                static_assert(!n.can_be_zero());
                snn_require(n.not_zero().get() == 1);
            }
            {
                num::clamped<int, -42, 38> n{0};
                snn_require(!n);
                snn_require(n.get() == 0);
                static_assert(n.min() == -42);
                static_assert(n.max() == 38);
                static_assert(n.can_be_zero());
            }
            {
                num::clamped<int, -42, 38> n{14};
                snn_require(n);
                snn_require(n.get() == 14);
                static_assert(n.min() == -42);
                static_assert(n.max() == 38);
                static_assert(n.can_be_zero());
            }

            // constexpr clamped(const clamped<I, MinOther, MaxOther> other) noexcept
            {
                num::clamped<i32, -42, 38> n{-14};
                snn_require(num::clamped<i16, -20, 20>{n}.get() == -14);
                snn_require(num::clamped<i16, -40, -20>{n}.get() == -20);
                snn_require(num::clamped<i16, 20, 40>{n}.get() == 20);
                snn_require(num::clamped<i8, 0, 100>{n}.get() == 0);
            }
            {
                constexpr auto min = constant::limit<i32>::min;
                num::clamped<i32, min, 100> n{min};
                snn_require(num::clamped<i8, -100, 100>{n}.get() == -100);
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
        snn_static_require(app::test_clamped());
    }
}
