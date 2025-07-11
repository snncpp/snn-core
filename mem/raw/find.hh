// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Find (`memchr`)

#pragma once

#include "snn-core/core.hh"

namespace snn::mem::raw
{
    // ## Functions

    // ### `find`

    namespace detail
    {
        template <typename T, octet Octet>
            requires same_as<const T, const Octet>
        constexpr T* find_slow(const not_null<T*> haystack_data,
                               const byte_size<usize> haystack_size, const Octet needle) noexcept
        {
            // This is up to 14 times slower than the SIMD (SSE2) optimized `memchr` on FreeBSD 14.

            T* const data    = haystack_data.get();
            const usize size = haystack_size.get();
            for (usize i = 0; i < size; ++i)
            {
                SNN_DIAGNOSTIC_PUSH
                SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

                if (data[i] == needle)
                {
                    return &data[i];
                }

                SNN_DIAGNOSTIC_POP
            }

            return nullptr;
        }
    }

    template <typename T, octet Octet>
        requires same_as<const T, const Octet>
    [[nodiscard]] constexpr T* find(const not_null<T*> haystack_data,
                                    const byte_size<usize> haystack_size,
                                    const Octet needle) noexcept
    {
        if (std::is_constant_evaluated())
        {
#if defined(__has_builtin) && __has_builtin(__builtin_char_memchr)
            if constexpr (std::is_same_v<Octet, char>)
            {
                return __builtin_char_memchr(haystack_data.get(), to_byte(needle),
                                             haystack_size.get());
            }
#endif
            return detail::find_slow(haystack_data, haystack_size, needle);
        }
        else
        {
            return static_cast<T*>(__builtin_memchr(haystack_data.get(), force_unsigned(needle),
                                                    haystack_size.get()));
        }
    }
}
