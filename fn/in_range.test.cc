// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fn/in_range.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            const array words{"one", "three", "two"};
            const auto is_word = fn::in_range{words, promise::is_sorted};

            snn_require(is_word("one"));
            snn_require(is_word("two"));
            snn_require(is_word("three"));

            snn_require(!is_word(""));
            snn_require(!is_word("abc"));
            snn_require(!is_word("xyz"));
            snn_require(!is_word("One"));
            snn_require(!is_word("one1"));

            static_assert(std::is_same_v<decltype(words), const array<cstrview, 3>>);
            static_assert(std::is_same_v<decltype(is_word),
                                         const fn::in_range<range::contiguous<const cstrview*>>>);

            // This is just an example, probably not the most efficient.
            const cstrview chars = "!()+-<=>?";
            const auto is_symbol = fn::in_range{chars.range(), promise::is_sorted};

            snn_require(is_symbol('!'));
            snn_require(is_symbol('('));
            snn_require(is_symbol('>'));
            snn_require(is_symbol('?'));

            snn_require(!is_symbol(' '));
            snn_require(!is_symbol('.'));
            snn_require(!is_symbol('3'));
            snn_require(!is_symbol('A'));

            static_assert(std::is_same_v<decltype(is_symbol),
                                         const fn::in_range<range::contiguous<const char*>>>);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());
    }
}
