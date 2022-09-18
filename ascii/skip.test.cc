// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/ascii/skip.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            cstrrng rng{"abcåäö123"};
            snn_require(rng.size() == 12); // Bytes
            rng = ascii::skip(rng);
            snn_require(rng.size() == 9); // Bytes
            snn_require(rng.view() == "åäö123");

            // Empty or only ASCII, all characters will be skipped.
            snn_require(ascii::skip(cstrrng{""}).size() == 0);
            snn_require(ascii::skip(cstrrng{"abc"}).size() == 0);
            snn_require(ascii::skip(cstrrng{"abc\n123"}).size() == 0);
            snn_require(ascii::skip(cstrrng{"abcdefghijklmnopqrstuvwxyz"}).size() == 0);

            // Includes non-ASCII, not all characters will be skipped.
            snn_require(ascii::skip(cstrrng{"abcdefghijklmnopqrstuvwxyzåäö"}).size() == 6);
            snn_require(ascii::skip(cstrrng{"abcådefghijklmnopqrstuvwxyz"}).size() == 25);

            // Only non-ASCII, no characters will be skipped.
            snn_require(ascii::skip(cstrrng{"åäö"}).size() == 6);

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
