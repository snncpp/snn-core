// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/limit.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            time::limit<time::milliseconds, 50, 10'000> l{time::seconds{15}};

            snn_require(l.get() == 10'000); // Clamped

            static_assert(l.min() == 50);
            static_assert(l.max() == 10'000);

            decltype(auto) u = l.unit();
            static_assert(std::is_same_v<decltype(u), time::milliseconds>);
            snn_require(u.value() == 10'000);

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
            constexpr time::limit<time::milliseconds, 1, 10'000> l{time::seconds{2}};
            static_assert(l.get() == 2'000);
            static_assert(l.min() == 1);
            static_assert(l.max() == 10'000);

            constexpr decltype(auto) u = l.unit();
            static_assert(std::is_same_v<decltype(u), const time::milliseconds>);
            static_assert(u.value() == 2'000);

            static_assert(l.unit() == time::seconds{2});
            static_assert(l.unit() == time::milliseconds{2'000});
            static_assert(l.unit() != time::microseconds{2});
        }

        {
            constexpr auto ms_limit = [](time::limit<time::milliseconds, 500, 10'000> l) {
                return l.get();
            };

            static_assert(ms_limit(time::seconds{2}) == 2'000);
            static_assert(ms_limit(time::milliseconds{100}) == 500); // Clamped
            static_assert(ms_limit(time::seconds{12}) == 10'000);    // Clamped
        }

        {
            constexpr time::limit<time::milliseconds, 1, 10'000> l{time::hours{2}};
            static_assert(l.get() == 10'000);
            static_assert(l.unit() == time::milliseconds{10'000});
            static_assert(l.unit() != time::milliseconds{10'001});
        }

        {
            constexpr time::limit<time::milliseconds, 1, 10'000> l{time::seconds{-2}};
            static_assert(l.get() == 1);
            static_assert(l.unit() == time::milliseconds{1});
        }

        {
            constexpr time::limit<time::milliseconds, 1, 10'000> l = time::seconds{2};
            static_assert(l.get() == 2'000);
            constexpr time::limit<time::seconds, 3, 30> l2 = l;
            static_assert(l2.get() == 3);
            constexpr time::limit<time::milliseconds, 1'000, 15'000> l3 = l2;
            static_assert(l3.get() == 3'000);
        }

        {
            constexpr time::limit<time::milliseconds, 0, 100> l = {{}};
            static_assert(l.get() == 0);
            static_assert(l.unit() == time::milliseconds{0});
        }

        {
            constexpr time::limit<time::milliseconds, 5, 100> l = {{}};
            static_assert(l.get() == 5);
            static_assert(l.unit() == time::milliseconds{5});
        }
    }
}
