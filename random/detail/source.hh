// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "snn-core/array_view.hh"
#if defined(__linux__)
#include "snn-core/exception.hh"
#include "snn-core/math/common.hh"
#include "snn-core/system/error.hh"
#include <unistd.h> // getentropy
#else
#include <cstdlib> // arc4random, arc4random_buf
#endif

// arc4random(3) never fails and uses ChaCha20 as of FreeBSD 12.

namespace snn::random::detail
{
    struct source final
    {
        static void fill(strview buffer)
        {
#if defined(__linux__)
            while (buffer)
            {
                // getentropy() can only fill 256 bytes.
                const usize chunk_size = math::min(buffer.size(), 256);
                if (::getentropy(buffer.writable().get(), chunk_size) != 0)
                {
                    throw_or_abort(error_code{errno, system::error_category});
                }
                buffer.drop_front_n(chunk_size);
            }
#else
            ::arc4random_buf(buffer.writable().get(), buffer.size());
#endif
        }

        static u32 number()
        {
#if defined(__linux__)
            u32 n;
            fill(strview{not_null{reinterpret_cast<char*>(&n)}, sizeof(n)});
            return n;
#else
            return ::arc4random();
#endif
        }
    };
}
