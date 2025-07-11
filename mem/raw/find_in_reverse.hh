// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Find in reverse (`memrchr`)

#pragma once

#include "snn-core/mem/raw/load.hh"
#include <string.h> // memrchr

namespace snn::mem::raw
{
    // ## Functions

    // ### `find_in_reverse`

    namespace detail
    {
        // Based on:
        // <https://www.agner.org/optimize/optimizing_assembly.pdf>
        // "13.10 Vector operations in general purpose registers"
        // "The code examples in these manuals can be used without restrictions."

        constexpr u64 mask_calc(const byte needle) noexcept
        {
            return needle * 0x0101010101010101u;
        }

        __attribute__((__no_sanitize__("unsigned-integer-overflow"))) //
        constexpr u64 mask_match(const u64 block, const u64 mask) noexcept
        {
            const u64 m = block ^ mask;
            return (m - 0x0101010101010101u) & ~m & 0x8080808080808080u;
        }

        template <typename T, octet Octet>
            requires same_as<const T, const Octet>
        constexpr T* find_in_reverse(const not_null<T*> haystack_data,
                                     const byte_size<usize> haystack_size,
                                     const Octet needle) noexcept
        {
            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            T* const first = haystack_data.get();
            T* cur         = first + haystack_size.get();

            constexpr int block_size = sizeof(u64);
            static_assert(block_size == 8);

            if ((cur - first) > block_size)
            {
                const u64 mask = detail::mask_calc(force_unsigned(needle));
                do
                {
                    cur -= block_size;
                    if (detail::mask_match(mem::raw::load<u64>(cur), mask))
                    {
                        cur += block_size;
                        break;
                    }
                } while ((cur - first) > block_size);
            }

            while (cur > first)
            {
                --cur;
                if (*cur == needle)
                {
                    return cur;
                }
            }

            return nullptr;

            SNN_DIAGNOSTIC_POP
        }
    }

    template <typename T, octet Octet>
        requires same_as<const T, const Octet>
    [[nodiscard]] constexpr T* find_in_reverse(const not_null<T*> haystack_data,
                                               const byte_size<usize> haystack_size,
                                               const Octet needle) noexcept
    {
        // FreeBSD's `amd64` `memrchr` is broken, it is always `O(n)`:
        // <https://github.com/freebsd/freebsd-src/blob/main/lib/libc/amd64/string/memrchr.S>

#if defined(__linux__)
        if (!std::is_constant_evaluated())
        {
            return static_cast<T*>(::memrchr(haystack_data.get(), force_unsigned(needle),
                                             haystack_size.get()));
        }
#endif

        return detail::find_in_reverse(haystack_data, haystack_size, needle);
    }
}
