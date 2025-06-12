// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/compare.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/range/forward.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::compare(cstrrng{""}, cstrrng{"a"}) < 0);
            snn_require(algo::compare(cstrrng{"a"}, cstrrng{"b"}) < 0);
            snn_require(algo::compare(cstrrng{"a"}, cstrrng{"ab"}) < 0);
            snn_require(algo::compare(cstrrng{"A"}, cstrrng{"a"}) < 0);
            snn_require(algo::compare(cstrrng{"ab"}, cstrrng{"ac"}) < 0);
            snn_require(algo::compare(cstrrng{"ab"}, cstrrng{"abc"}) < 0);
            snn_require(algo::compare(cstrrng{"AB"}, cstrrng{"ab"}) < 0);

            snn_require(algo::compare(cstrrng{""}, cstrrng{""}) == 0);
            snn_require(algo::compare(cstrrng{"a"}, cstrrng{"a"}) == 0);
            snn_require(algo::compare(cstrrng{"ab"}, cstrrng{"ab"}) == 0);

            snn_require(algo::compare(cstrrng{"a"}, cstrrng{""}) > 0);
            snn_require(algo::compare(cstrrng{"a"}, cstrrng{"A"}) > 0);
            snn_require(algo::compare(cstrrng{"b"}, cstrrng{"a"}) > 0);
            snn_require(algo::compare(cstrrng{"b"}, cstrrng{"abc"}) > 0);
            snn_require(algo::compare(cstrrng{"ab"}, cstrrng{"aa"}) > 0);
            snn_require(algo::compare(cstrrng{"abc"}, cstrrng{"ab"}) > 0);
            snn_require(algo::compare(cstrrng{"ab"}, cstrrng{"AB"}) > 0);
            snn_require(algo::compare(cstrrng{"ab"}, cstrrng{"ABC"}) > 0);

            decltype(auto) i = algo::compare(cstrrng{}, cstrrng{});
            static_assert(std::is_same_v<decltype(i), std::strong_ordering>);
            snn_require(i == 0);

            return true;
        }

        constexpr bool test_compare()
        {
            auto compare_without_count = [](cstrview a, cstrview b) {
                auto ra = range::forward{init::from, a.begin(), a.end()};
                auto rb = range::forward{init::from, b.begin(), b.end()};
                static_assert(!random_access_range<decltype(ra)>);
                return algo::compare(ra, rb);
            };

            snn_require(compare_without_count("", "a") < 0);
            snn_require(compare_without_count("a", "b") < 0);
            snn_require(compare_without_count("a", "ab") < 0);
            snn_require(compare_without_count("A", "a") < 0);
            snn_require(compare_without_count("ab", "ac") < 0);
            snn_require(compare_without_count("ab", "abc") < 0);
            snn_require(compare_without_count("AB", "ab") < 0);

            snn_require(compare_without_count("", "") == 0);
            snn_require(compare_without_count("a", "a") == 0);
            snn_require(compare_without_count("ab", "ab") == 0);

            snn_require(compare_without_count("a", "") > 0);
            snn_require(compare_without_count("b", "a") > 0);
            snn_require(compare_without_count("a", "A") > 0);
            snn_require(compare_without_count("b", "abc") > 0);
            snn_require(compare_without_count("abc", "ab") > 0);
            snn_require(compare_without_count("ab", "AB") > 0);
            snn_require(compare_without_count("ab", "ABC") > 0);

            return true;
        }

        constexpr bool test_compare_partial_ordering()
        {
            array<int, 2> arri{3, 7};
            array<double, 2> arrd{2.7, 7.5};

            decltype(auto) i = algo::compare(arri.range(), arrd.range());
            static_assert(std::is_same_v<decltype(i), std::partial_ordering>);
            snn_require(i > 0);

            snn_require(algo::compare(arri.view(1).range(), arrd.view(1).range()) < 0);
            snn_require(algo::compare(arri.view(2).range(), arrd.view(2).range()) == 0);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        // Strong ordering - with `.count()`.
        static_assert(random_access_range<cstrrng>);
        snn_static_require(app::example());

        // Strong ordering - without `.count()`.
        snn_static_require(app::test_compare());

        // Partial ordering.
        snn_static_require(app::test_compare_partial_ordering());
    }
}
