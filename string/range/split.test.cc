// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/string/range/split.hh"

#include "snn-core/make_range.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::is_equal(string::range::split{"one two three", ' '},
                                       init_list<cstrview>{"one", "two", "three"}));

            return true;
        }

        template <typename Delimiter>
        constexpr bool splits_to(const cstrview subject, Delimiter&& delimiter,
                                 const init_list<cstrview> expected) noexcept
        {
            usize index             = 0;
            const auto expected_rng = make_range(expected);
            for (const cstrview s :
                 string::range::split{subject, std::forward<Delimiter>(delimiter)})
            {
                if (index >= expected_rng.count() ||
                    expected_rng.at(index, assume::within_bounds) != s)
                {
                    return false;
                }
                ++index;
            }
            return (index == expected_rng.count());
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        // Concepts
        static_assert(!contiguous_range<string::range::split<cstrview>>);
        static_assert(!random_access_range<string::range::split<cstrview>>);
        static_assert(!bidirectional_range<string::range::split<cstrview>>);
        static_assert(forward_range<string::range::split<cstrview>>);

        {
            string::range::split rng{"one two three", " "};

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(promise::not_empty) == "one");
            snn_require(rng.front().value() == "one");
            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front().value() == "two");
            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(promise::not_empty) == "three");
            rng.drop_front(promise::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());
        }

        {
            string::range::split rng{"one two three", ' '};

            snn_require(rng.pop_front(promise::not_empty) == "one");
            snn_require(rng.pop_front().value() == "two");
            snn_require(rng.pop_front().value() == "three");

            snn_require(!rng.pop_front().has_value());
            snn_require(rng.is_empty());
        }

        {
            string::range::split rng{"", ""};
            snn_require(!rng);
            snn_require(rng.is_empty());
        }
        {
            string::range::split rng{"abc", ""};
            snn_require(!rng);
            snn_require(rng.is_empty());
        }
        {
            string::range::split rng{"", " "};
            snn_require(!rng);
            snn_require(rng.is_empty());
        }
        {
            string::range::split rng{"", ' '};
            snn_require(!rng);
            snn_require(rng.is_empty());
        }

        static_assert(app::splits_to("", "", {}));
        static_assert(app::splits_to("", " ", {}));
        static_assert(app::splits_to("", ' ', {}));

        // Can't split on empty needle (return bytes or UTF-8 code points?).
        static_assert(app::splits_to("abc", "", {}));

        static_assert(app::splits_to(" ", ' ', {"", ""}));
        static_assert(app::splits_to(" ", " ", {"", ""}));
        static_assert(app::splits_to("a", "a", {"", ""}));
        static_assert(app::splits_to("ab", "ab", {"", ""}));
        static_assert(app::splits_to("abc", "abc", {"", ""}));

        static_assert(app::splits_to("  ", ' ', {"", "", ""}));
        static_assert(app::splits_to("  ", " ", {"", "", ""}));
        static_assert(app::splits_to("aa", "a", {"", "", ""}));
        static_assert(app::splits_to("abab", "ab", {"", "", ""}));
        static_assert(app::splits_to("abcabc", "abc", {"", "", ""}));

        static_assert(app::splits_to("abc", '\n', {"abc"}));
        static_assert(app::splits_to("abc\n", '\n', {"abc", ""}));

        static_assert(app::splits_to("one two three", ' ', {"one", "two", "three"}));
        static_assert(!app::splits_to(" one two three", ' ', {"one", "two", "three"}));
        static_assert(!app::splits_to("one two three ", ' ', {"one", "two", "three"}));
        static_assert(!app::splits_to("one two", ' ', {"one", "two", "three"}));
        static_assert(!app::splits_to("one two three four", ' ', {"one", "two", "three"}));

        static_assert(app::splits_to("one two three", " ", {"one", "two", "three"}));
        static_assert(!app::splits_to(" one two three", " ", {"one", "two", "three"}));
        static_assert(!app::splits_to("one two three ", " ", {"one", "two", "three"}));
        static_assert(!app::splits_to("one two", " ", {"one", "two", "three"}));
        static_assert(!app::splits_to("one two three four", " ", {"one", "two", "three"}));

        static_assert(app::splits_to(" one two three", ' ', {"", "one", "two", "three"}));
        static_assert(app::splits_to(" one two three", " ", {"", "one", "two", "three"}));

        static_assert(app::splits_to("one two three ", ' ', {"one", "two", "three", ""}));
        static_assert(app::splits_to("one two three ", " ", {"one", "two", "three", ""}));

        static_assert(app::splits_to("one two three", "two", {"one ", " three"}));
        static_assert(!app::splits_to("one two three", "TWO", {"one ", " three"}));
    }
}
