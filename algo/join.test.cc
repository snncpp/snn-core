// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/join.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/vec.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/range/step.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::join<str>(range::step<char>{'a', 'g'}) == "abcdef");
            snn_require(algo::join<str>(range::step<char>{'a', 'g'}, ',') == "a,b,c,d,e,f");

            array strings{"One", "Two", "Three"};
            strbuf buffer;
            algo::join(strings.range(), ", ", buffer, promise::no_overlap);
            snn_require(buffer == "One, Two, Three");

            return true;
        }

        constexpr bool test_join()
        {
            // void join(Rng rng, Container& append_to, promise::no_overlap_t)
            {
                strbuf s;
                array strings{"One", "Two", "Three"};
                algo::join(strings.range(), s, promise::no_overlap);
                snn_require(s == "OneTwoThree");
                algo::join(strings.view_offset(-1).range(), s, promise::no_overlap);
                snn_require(s == "OneTwoThreeThree");
                algo::join(strings.view(99).range(), s, promise::no_overlap);
                snn_require(s == "OneTwoThreeThree");
            }
            {
                vec<i32> v;
                algo::join(range::step{0, 5}, v, promise::no_overlap);
                snn_require(algo::is_equal(v.range(), {0, 1, 2, 3, 4}));
            }

            // Container join(Rng rng)
            {
                const auto s = algo::join<str>(range::step<char>{'a', 'g'});
                snn_require(s == "abcdef");
            }
            {
                const auto v = algo::join<vec<int>>(range::step{0, 5});
                snn_require(algo::is_equal(v.range(), {0, 1, 2, 3, 4}));
            }

            // void join(Rng rng, const T& delimiter, Container& append_to, promise::no_overlap_t)
            {
                strbuf s;
                array strings{"One", "Two", "Three"};
                algo::join(strings.range(), ", ", s, promise::no_overlap);
                snn_require(s == "One, Two, Three");
                algo::join(strings.range(), "", s, promise::no_overlap);
                snn_require(s == "One, Two, ThreeOneTwoThree");
            }
            {
                vec<i32> v;
                algo::join(range::step{0, 3}, 99, v, promise::no_overlap);
                snn_require(algo::is_equal(v.range(), {0, 99, 1, 99, 2}));
            }

            // Container join(Rng rng, const T& delimiter)
            {
                array<cstrview, 0> strings;
                snn_require(strings.is_empty());
                snn_require(algo::join<str>(strings.range(), ", ") == "");
                snn_require(algo::join<str>(strings.range(), "") == "");
            }
            {
                array strings{"One"};
                snn_require(algo::join<str>(strings.range(), ", ") == "One");
                snn_require(algo::join<str>(strings.range(), "") == "One");
            }
            {
                array strings{"One", "Two"};
                snn_require(algo::join<str>(strings.range(), ", ") == "One, Two");
                snn_require(algo::join<str>(strings.range(), ',') == "One,Two");
                snn_require(algo::join<str>(strings.range(), "") == "OneTwo");
            }
            {
                const auto v = algo::join<vec<int>>(range::step{0, 3}, 99);
                snn_require(algo::is_equal(v.range(), {0, 99, 1, 99, 2}));
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
        snn_static_require(app::test_join());
    }
}
