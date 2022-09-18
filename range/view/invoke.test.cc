// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/invoke.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/algo/sum.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            array<str, 3> arr{"One", "Two", "Three"};
            snn_require(algo::is_equal(arr.range() | range::v::invoke{&str::size}, {3u, 3u, 5u}));
            snn_require(algo::sum(arr.range() | range::v::invoke{&str::size}) == 11u);

            return true;
        }

        constexpr bool test_invoke()
        {
            struct person
            {
                int id;
                cstrview name;
            };

            array<person, 2> arr{{{123, "foo"}, {456, "bar"}}};

            auto rng = range::view::invoke{arr.range(), &person::id};

            // Concepts
            static_assert(!contiguous_range<decltype(rng)>);
            static_assert(!random_access_range<decltype(rng)>);
            static_assert(!bidirectional_range<decltype(rng)>);
            static_assert(forward_range<decltype(rng)>);

            // The range is copyable so the iterator holds a copy (multi-pass).
            using range_type = decltype(rng);
            static_assert(std::is_same_v<decltype(rng.begin()), range::iter::forward<range_type>>);

            static_assert(std::is_same_v<decltype(rng.front(promise::not_empty)), int&>);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(promise::not_empty) == 123);
            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(promise::not_empty) == 456);
            rng.drop_front(promise::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
        snn_static_require(app::test_invoke());
    }
}
