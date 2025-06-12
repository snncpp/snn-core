// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/is_less.hh"

#include "snn-core/unittest.hh"
#include "snn-core/ascii/fn/less_than_icase.hh"
#include "snn-core/range/forward.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::is_less(cstrrng{""}, cstrrng{"a"}));
            snn_require(algo::is_less(cstrrng{"a"}, cstrrng{"b"}));
            snn_require(algo::is_less(cstrrng{"a"}, cstrrng{"ab"}));
            snn_require(algo::is_less(cstrrng{"A"}, cstrrng{"a"}));
            snn_require(algo::is_less(cstrrng{"ab"}, cstrrng{"ac"}));
            snn_require(algo::is_less(cstrrng{"ab"}, cstrrng{"abc"}));
            snn_require(algo::is_less(cstrrng{"AB"}, cstrrng{"ab"}));

            snn_require(!algo::is_less(cstrrng{""}, cstrrng{""}));
            snn_require(!algo::is_less(cstrrng{"a"}, cstrrng{""}));
            snn_require(!algo::is_less(cstrrng{"a"}, cstrrng{"a"}));
            snn_require(!algo::is_less(cstrrng{"b"}, cstrrng{"a"}));
            snn_require(!algo::is_less(cstrrng{"b"}, cstrrng{"abc"}));
            snn_require(!algo::is_less(cstrrng{"ab"}, cstrrng{"aa"}));
            snn_require(!algo::is_less(cstrrng{"ab"}, cstrrng{"ab"}));
            snn_require(!algo::is_less(cstrrng{"abc"}, cstrrng{"ab"}));
            snn_require(!algo::is_less(cstrrng{"ab"}, cstrrng{"ABC"}));

            snn_require(algo::is_less(cstrrng{"ab"}, cstrrng{"ABC"}, ascii::fn::less_than_icase{}));
            snn_require(!algo::is_less(cstrrng{"AB"}, cstrrng{"ab"}, ascii::fn::less_than_icase{}));

            return true;
        }

        constexpr bool test_is_less()
        {
            auto is_less_without_count = [](cstrview a, cstrview b) {
                auto ra = range::forward{init::from, a.begin(), a.end()};
                auto rb = range::forward{init::from, b.begin(), b.end()};
                static_assert(!has_count<decltype(ra)>);
                return algo::is_less(ra, rb);
            };

            snn_require(is_less_without_count("", "a"));
            snn_require(is_less_without_count("a", "b"));
            snn_require(is_less_without_count("a", "ab"));
            snn_require(is_less_without_count("A", "a"));
            snn_require(is_less_without_count("ab", "ac"));
            snn_require(is_less_without_count("ab", "abc"));
            snn_require(is_less_without_count("AB", "ab"));

            snn_require(!is_less_without_count("", ""));
            snn_require(!is_less_without_count("a", ""));
            snn_require(!is_less_without_count("a", "a"));
            snn_require(!is_less_without_count("b", "a"));
            snn_require(!is_less_without_count("b", "abc"));
            snn_require(!is_less_without_count("ab", "ab"));
            snn_require(!is_less_without_count("abc", "ab"));
            snn_require(!is_less_without_count("ab", "ABC"));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        // With `.count()`.
        static_assert(has_count<cstrrng>);
        snn_static_require(app::example());

        // Without `.count()`.
        snn_static_require(app::test_is_less());
    }
}
