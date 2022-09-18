// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/string/range/chunk.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            snn_require(algo::is_equal(string::range::chunk{"one two three", 4},
                                       init_list<cstrview>{"one ", "two ", "thre", "e"}));

            return true;
        }

        constexpr bool test_chunk()
        {
            {
                string::range::chunk rng{"one two three", 4};

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == "one ");
                snn_require(rng.front().value() == "one ");

                rng.drop_front(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front().value() == "two ");

                rng.drop_front(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == "thre");

                rng.drop_front(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == "e");

                rng.drop_front(promise::not_empty);

                snn_require(!rng);
                snn_require(rng.is_empty());
                snn_require(!rng.front().has_value());
            }

            {
                string::range::chunk rng{"abc", 0}; // Minimal chunk size is 1.

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == "a");

                rng.drop_front(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front().value() == "b");

                rng.drop_front(promise::not_empty);

                snn_require(rng);
                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == "c");

                rng.drop_front(promise::not_empty);

                snn_require(!rng);
                snn_require(rng.is_empty());
            }

            {
                string::range::chunk rng{"abcdef", 4};

                str combined;
                for (const auto s : rng)
                {
                    combined << s << ",";
                }
                snn_require(combined == "abcd,ef,");

                snn_require(!rng.is_empty());
                snn_require(rng.front(promise::not_empty) == "abcd");

                rng.drop_front(promise::not_empty);

                snn_require(!rng.is_empty());
                snn_require(rng.front().value() == "ef");

                rng.drop_front(promise::not_empty);

                snn_require(!rng);
                snn_require(rng.is_empty());
            }

            {
                string::range::chunk rng{"", 0};
                snn_require(!rng);
                snn_require(rng.is_empty());
            }
            {
                string::range::chunk rng{"", 1};
                snn_require(!rng);
                snn_require(rng.is_empty());
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
        snn_static_require(app::test_chunk());

        // Concepts
        static_assert(!contiguous_range<string::range::chunk>);
        static_assert(!random_access_range<string::range::chunk>);
        static_assert(!bidirectional_range<string::range::chunk>);
        static_assert(forward_range<string::range::chunk>);
    }
}
