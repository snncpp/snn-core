// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/is_equal.hh"

#include "snn-core/unittest.hh"
#include "snn-core/ascii/fn/equal_to_icase.hh"
#include "snn-core/range/forward.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::is_equal(cstrrng{""}, cstrrng{""}));
            snn_require(algo::is_equal(cstrrng{"a"}, cstrrng{"a"}));
            snn_require(algo::is_equal(cstrrng{"ab"}, cstrrng{"ab"}));
            snn_require(algo::is_equal(cstrrng{"abc"}, cstrrng{"abc"}));

            snn_require(!algo::is_equal(cstrrng{""}, cstrrng{"a"}));
            snn_require(!algo::is_equal(cstrrng{"a"}, cstrrng{""}));

            snn_require(!algo::is_equal(cstrrng{"ab"}, cstrrng{"a"}));
            snn_require(!algo::is_equal(cstrrng{"a"}, cstrrng{"ab"}));

            snn_require(!algo::is_equal(cstrrng{"a"}, cstrrng{"A"}));
            snn_require(!algo::is_equal(cstrrng{"A"}, cstrrng{"a"}));
            snn_require(!algo::is_equal(cstrrng{"ab"}, cstrrng{"aB"}));
            snn_require(!algo::is_equal(cstrrng{"aB"}, cstrrng{"ab"}));

            snn_require(algo::is_equal(cstrrng{"abc"}, {'a', 'b', 'c'}));
            snn_require(!algo::is_equal(cstrrng{"abc"}, {'a', 'b'}));
            snn_require(!algo::is_equal(cstrrng{"abc"}, {'a', 'b', 'C'}));

            snn_require(algo::is_equal(cstrrng{"abc"}, cstrrng{"ABC"},
                                       ascii::fn::equal_to_icase{}));
            snn_require(!algo::is_equal(cstrrng{"abc"}, cstrrng{"ACB"},
                                        ascii::fn::equal_to_icase{}));

            return true;
        }

        constexpr bool test_is_equal()
        {
            auto is_eq_without_count = [](cstrview a, cstrview b) {
                auto ra = range::forward{meta::iterators, a.begin(), a.end()};
                auto rb = range::forward{meta::iterators, b.begin(), b.end()};
                static_assert(!has_count<decltype(ra)>);
                return algo::is_equal(ra, rb);
            };

            snn_require(is_eq_without_count("", ""));
            snn_require(is_eq_without_count("a", "a"));
            snn_require(is_eq_without_count("ab", "ab"));
            snn_require(is_eq_without_count("abc", "abc"));

            snn_require(!is_eq_without_count("", "a"));
            snn_require(!is_eq_without_count("a", ""));
            snn_require(!is_eq_without_count("a", "ab"));
            snn_require(!is_eq_without_count("ab", "a"));
            snn_require(!is_eq_without_count("ab", "abc"));
            snn_require(!is_eq_without_count("abc", "ab"));

            snn_require(!is_eq_without_count("a", "A"));
            snn_require(!is_eq_without_count("A", "a"));
            snn_require(!is_eq_without_count("ab", "aB"));
            snn_require(!is_eq_without_count("aB", "ab"));
            snn_require(!is_eq_without_count("abc", "abC"));
            snn_require(!is_eq_without_count("abC", "abc"));

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
        snn_static_require(app::test_is_equal());
    }
}
