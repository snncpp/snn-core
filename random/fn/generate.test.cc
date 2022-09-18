// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/random/fn/generate.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            // strict_integral
            {
                random::fn::generate<u64> gen;
                static_assert(std::is_same_v<decltype(gen()), u64>);
                const auto a = gen();
                const auto b = gen();
                snn_require(a != b);
            }

            // str
            {
                random::fn::generate<str> gen{16};
                static_assert(std::is_same_v<decltype(gen()), str>);
                const auto a = gen();
                const auto b = gen();
                snn_require(a.size() == 16);
                snn_require(b.size() == 16);
                snn_require(a != b);
            }

            // strbuf
            {
                random::fn::generate<strbuf> gen{20};
                static_assert(std::is_same_v<decltype(gen()), strbuf>);
                const auto a = gen();
                const auto b = gen();
                snn_require(a.size() == 20);
                snn_require(b.size() == 20);
                snn_require(a != b);
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());
    }
}
