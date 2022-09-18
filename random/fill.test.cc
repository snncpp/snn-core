// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/random/fill.hh"

#include "snn-core/array.hh"
#include "snn-core/unittest.hh"
#include "snn-core/fn/common.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            array<char, 8> buf; // Zero initiated.
            snn_require(buf.all(fn::is_zero{}));

            random::fill(buf.view());
            snn_require(!buf.all(fn::is_zero{}));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        {
            array<char, 8> buf; // Zero initiated.

            snn_require(buf.all(fn::is_zero{}));

            random::fill(buf.view());

            snn_require(!buf.all(fn::is_zero{})); // Very unlikely.

            buf.fill('\0');

            random::fill(buf.view(0, 0)); // Empty view, does nothing.

            snn_require(buf.all(fn::is_zero{}));
        }
        {
            // Fill more than 256 bytes (some implementations will only provide 256 bytes of random
            // data at a time).

            str s{container::fill, 300, '\0'};

            snn_require(s.size() == 300);
            snn_require(s.all(fn::is_zero{}));

            str needle{container::fill, 8, '\0'}; // 64-bit needle.
            snn_require(needle.size() == 8);
            snn_require(needle.all(fn::is_zero{}));

            snn_require(s.contains(needle));

            random::fill(s.view());

            snn_require(!s.contains(needle)); // Very unlikely.
        }
    }
}
