// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/unpack_front.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                char first{};
                char third{};
                snn_require(algo::unpack_front(cstrrng{"abcdef"}, first, std::ignore, third));
                snn_require(first == 'a');
                snn_require(third == 'c');
            }
            {
                char x{};
                char y{};

                snn_require(x == '\0');
                snn_require(y == '\0');

                snn_require(algo::unpack_front(cstrrng{"abc"}));

                snn_require(algo::unpack_front(cstrrng{"abc"}, x));
                snn_require(x == 'a');

                snn_require(algo::unpack_front(cstrrng{"123"}, x, y));
                snn_require(x == '1');
                snn_require(y == '2');

                // Not enough elements.
                snn_require(!algo::unpack_front(cstrrng{""}, x));
                snn_require(x == '1'); // Not assigned.

                // Not enough elements, some variables are still assigned.
                snn_require(!algo::unpack_front(cstrrng{"A"}, x, y));
                snn_require(x == 'A');
                snn_require(y == '2'); // Not assigned.

                snn_require_throws_code(algo::unpack_front(cstrrng{""}, x).or_throw(),
                                        generic::error::no_value);
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
    }
}
