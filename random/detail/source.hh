// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "snn-core/array_view.hh"
#include <stdlib.h> // arc4random, arc4random_buf

// arc4random(3) never fails and uses ChaCha20 as of FreeBSD 12.

namespace snn::random::detail
{
    struct source final
    {
        static void fill(strview buffer)
        {
            ::arc4random_buf(buffer.writable().get(), buffer.size());
        }

        static u32 number()
        {
            return ::arc4random();
        }
    };
}
