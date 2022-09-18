// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/pair/core.hh"

#include "snn-core/strcore.hh"
#include "snn-core/unittest.hh"
#include "snn-core/pair/common.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool test_is_any()
        {
            static_assert(pair::is_any_v<pair::first_second<int, int>>);
            static_assert(pair::is_any_v<pair::key_value<int, int>>);
            static_assert(!pair::is_any_v<int>);
            static_assert(!pair::is_any_v<int[2]>);
            static_assert(!pair::is_any_v<array_view<int, 2>>);

            return true;
        }

        constexpr bool test_any()
        {
            static_assert(pair::any<pair::first_second<int, int>>);
            static_assert(pair::any<pair::key_value<int, int>>);
            static_assert(!pair::any<int>);
            static_assert(!pair::any<int[2]>);
            static_assert(!pair::any<array_view<int, 2>>);

            return true;
        }

        constexpr bool test_get()
        {
            {
                pair::key_value<cstrview, usize> p{"Hello", 123};

                // This uses ADL to find `get` in the `pair` namespace.
                snn_require(get<0>(p) == "Hello");
                snn_require(get<1>(p) == 123);

                static_assert(std::is_same_v<decltype(get<0>(p)), cstrview&>);
                static_assert(std::is_same_v<decltype(get<0>(std::as_const(p))), const cstrview&>);
                static_assert(std::is_same_v<decltype(get<0>(std::move(p))), cstrview&&>);
                static_assert(std::is_same_v<decltype(get<0>(std::move(std::as_const(p)))),
                                             const cstrview&&>);

                static_assert(std::is_same_v<decltype(get<1>(p)), usize&>);
                static_assert(std::is_same_v<decltype(get<1>(std::as_const(p))), const usize&>);
                static_assert(std::is_same_v<decltype(get<1>(std::move(p))), usize&&>);
                static_assert(
                    std::is_same_v<decltype(get<1>(std::move(std::as_const(p)))), const usize&&>);
            }
            {
                cstrview s = "Hello";
                usize i    = 123;

                pair::key_value<cstrview&, usize&> p = {s, i};

                // This uses ADL to find `get` in the `pair` namespace.
                snn_require(get<0>(p) == "Hello");
                snn_require(get<1>(p) == 123);

                // Does not propagate const (would break structured binding).
                // If the return types here are unexpected look up "reference collapsing".

                static_assert(std::is_same_v<decltype(get<0>(p)), cstrview&>);
                static_assert(std::is_same_v<decltype(get<0>(std::as_const(p))), cstrview&>);
                static_assert(std::is_same_v<decltype(get<0>(std::move(p))), cstrview&>);
                static_assert(
                    std::is_same_v<decltype(get<0>(std::move(std::as_const(p)))), cstrview&>);

                static_assert(std::is_same_v<decltype(get<1>(p)), usize&>);
                static_assert(std::is_same_v<decltype(get<1>(std::as_const(p))), usize&>);
                static_assert(std::is_same_v<decltype(get<1>(std::move(p))), usize&>);
                static_assert(
                    std::is_same_v<decltype(get<1>(std::move(std::as_const(p)))), usize&>);

                // Structured binding (uses `get`).

                const auto [a, b] = p;

                snn_require(a == "Hello");
                snn_require(b == 123);

                // Does not propagate const.
                a = "ELLO";
                b = 456;

                snn_require(s == "ELLO");
                snn_require(i == 456);
            }

            return true;
        }

        constexpr bool test_swap()
        {
            pair::key_value<cstrview, int> one{"one", 1};
            pair::key_value<cstrview, int> two{"two", 2};

            snn_require(one.key == "one");
            snn_require(one.value == 1);

            snn_require(two.key == "two");
            snn_require(two.value == 2);

            swap(one, two);

            snn_require(one.key == "two");
            snn_require(one.value == 2);

            snn_require(two.key == "one");
            snn_require(two.value == 1);

            swap(one, one); // Swap with self.

            snn_require(one.key == "two");
            snn_require(one.value == 2);

            return true;
        }

        constexpr bool test_comparison()
        {
            pair::key_value<int> p{123, 456};

            snn_require(p.key == 123);
            snn_require(p.value == 456);

            snn_require(p == pair::key_value{123, 456});
            snn_require(p == pair::first_second<isize>{123, 456});

            snn_require(!(p == pair::key_value{0, 456}));
            snn_require(!(p == pair::key_value{123, 0}));
            snn_require(p != pair::key_value{0, 456});
            snn_require(p != pair::key_value{123, 0});
            snn_require(!(p != pair::key_value{123, 456}));

            auto i = p <=> pair::key_value{999, 456};
            static_assert(std::is_same_v<decltype(i), std::strong_ordering>);
            snn_require(i < 0);

            snn_require(p <=> pair::first_second<isize>{999, 456} < 0);

            snn_require(p < pair::key_value{999, 0});
            snn_require(p < pair::key_value{999, 999});
            snn_require(p < pair::key_value{123, 999});

            snn_require(!(p < pair::key_value{123, 456}));
            snn_require(!(p < pair::key_value{123, 0}));
            snn_require(!(p < pair::key_value{0, 0}));
            snn_require(!(p < pair::key_value{0, 999}));

            snn_require(p > pair::key_value{0, 0});
            snn_require(p > pair::key_value{0, 999});
            snn_require(p > pair::key_value{123, 0});

            snn_require(!(p > pair::key_value{123, 456}));
            snn_require(!(p > pair::key_value{123, 999}));
            snn_require(!(p > pair::key_value{999, 0}));
            snn_require(!(p > pair::key_value{999, 999}));

            return true;
        }

        constexpr bool test_tuple_element()
        {
            pair::key_value p{123, 456.0};
            static_assert(std::is_same_v<std::tuple_element_t<0, decltype(p)>, int>);
            static_assert(std::is_same_v<std::tuple_element_t<1, decltype(p)>, double>);

            return true;
        }

        constexpr bool test_tuple_size()
        {
            static_assert(std::tuple_size_v<pair::key_value<int>> == 2);
            static_assert(std::tuple_size_v<pair::key_value<int, int>> == 2);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::test_is_any());
        snn_static_require(app::test_any());
        snn_static_require(app::test_get());
        snn_static_require(app::test_swap());
        snn_static_require(app::test_comparison());
        snn_static_require(app::test_tuple_element());
        snn_static_require(app::test_tuple_size());
    }
}
