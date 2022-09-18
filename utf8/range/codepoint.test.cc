// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/range/codepoint.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            using pairs = init_list<pair::value_count<u32>>;

            snn_require(algo::is_equal(utf8::range::codepoint{"a"}, pairs{{0x61, 1}}));
            snn_require(algo::is_equal(utf8::range::codepoint{"å"}, pairs{{0xE5, 2}}));
            snn_require(algo::is_equal(utf8::range::codepoint{"å1"}, pairs{{0xE5, 2}, {0x31, 1}}));

            snn_require(algo::is_equal(utf8::range::codepoint{"abc"},
                                       pairs{{0x61, 1}, {0x62, 1}, {0x63, 1}}));

            snn_require(algo::is_equal(utf8::range::codepoint{"åäö"},
                                       pairs{{0xE5, 2}, {0xE4, 2}, {0xF6, 2}}));

            snn_require(utf8::range::codepoint{""}.is_empty());

            return true;
        }

        constexpr bool test_codepoint()
        {
            utf8::range::codepoint rng{"åa"};

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front().value() == pair::value_count<u32>{0xE5, 2});
            snn_require(rng.front(promise::not_empty) == pair::value_count<u32>{0xE5, 2});

            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front().value() == pair::value_count<u32>{0x61, 1});
            snn_require(rng.front(promise::not_empty) == pair::value_count<u32>{0x61, 1});

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
        snn_static_require(app::test_codepoint());
    }
}
