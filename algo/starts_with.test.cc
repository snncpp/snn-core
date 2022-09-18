// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/starts_with.hh"

#include "snn-core/unittest.hh"
#include "snn-core/ascii/fn/equal_to_icase.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::starts_with(cstrrng{""}, cstrrng{""}));
            snn_require(algo::starts_with(cstrrng{"a"}, cstrrng{""}));
            snn_require(algo::starts_with(cstrrng{"a"}, cstrrng{"a"}));
            snn_require(algo::starts_with(cstrrng{"abc"}, cstrrng{"ab"}));
            snn_require(algo::starts_with(cstrrng{"abc"}, cstrrng{"abc"}));

            snn_require(!algo::starts_with(cstrrng{""}, cstrrng{"a"}));
            snn_require(!algo::starts_with(cstrrng{""}, cstrrng{"ab"}));
            snn_require(!algo::starts_with(cstrrng{"a"}, cstrrng{"b"}));
            snn_require(!algo::starts_with(cstrrng{"ab"}, cstrrng{"ac"}));
            snn_require(!algo::starts_with(cstrrng{"abc"}, cstrrng{"ABC"}));
            snn_require(!algo::starts_with(cstrrng{"abc"}, cstrrng{"abcd"}));
            snn_require(!algo::starts_with(cstrrng{"abc"}, cstrrng{"abcde"}));
            snn_require(!algo::starts_with(cstrrng{"abc"}, cstrrng{"abcdef"}));

            snn_require(algo::starts_with(cstrrng{"abc"}, cstrrng{"AB"},
                                          ascii::fn::equal_to_icase{}));
            snn_require(!algo::starts_with(cstrrng{"abc"}, cstrrng{"AC"},
                                           ascii::fn::equal_to_icase{}));

            return true;
        }

        constexpr bool test_starts_with()
        {
            auto starts_with_without_count = [](cstrview a, cstrview b) {
                auto ra = range::forward{meta::iterators, a.begin(), a.end()};
                auto rb = range::forward{meta::iterators, b.begin(), b.end()};
                static_assert(!has_count<decltype(ra)>);
                return algo::starts_with(ra, rb);
            };

            snn_require(starts_with_without_count("", ""));
            snn_require(starts_with_without_count("a", ""));
            snn_require(starts_with_without_count("a", "a"));
            snn_require(starts_with_without_count("abc", "ab"));
            snn_require(starts_with_without_count("abc", "abc"));

            snn_require(!starts_with_without_count("", "a"));
            snn_require(!starts_with_without_count("", "ab"));
            snn_require(!starts_with_without_count("a", "b"));
            snn_require(!starts_with_without_count("ab", "ac"));
            snn_require(!starts_with_without_count("abc", "ABC"));
            snn_require(!starts_with_without_count("abc", "abcd"));
            snn_require(!starts_with_without_count("abc", "abcde"));
            snn_require(!starts_with_without_count("abc", "abcdef"));

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
        snn_static_require(app::test_starts_with());
    }
}
