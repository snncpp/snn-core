// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/status.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            {
                const utf8::info info = utf8::status("").value();
                snn_require(info.count == 0);
                snn_require(info.is_ascii);
            }
            {
                const utf8::info info = utf8::status("abcdef").value();
                snn_require(info.count == 6);
                snn_require(info.is_ascii);
            }
            {
                const utf8::info info = utf8::status("aoäö").value();
                snn_require(info.count == 4);
                snn_require(!info.is_ascii);
            }
            {
                // Invalid
                snn_require(!utf8::status("\x80"));
                snn_require(!utf8::status("\x80\x80"));
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
