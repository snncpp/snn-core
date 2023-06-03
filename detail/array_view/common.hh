// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "snn-core/array.hh"
#include "snn-core/optional_index.hh"
#include "snn-core/math/common.hh"
#include "snn-core/mem/raw/load.hh"
#include <string.h> // memrchr

namespace snn::detail::array_view
{
    // For `array_view<[const] char>` specializations that never hold null pointers.
    inline char single_char = '\xFF'; // Not null-terminated.

    struct position_count final
    {
        usize position;
        usize count;
    };

    constexpr position_count from_offset(const usize current_count, const isize pos,
                                         const isize count) noexcept
    {
        // This code is branchless (using conditional moves), don't change it without checking
        // the generated assembly. Tested with Clang 13.

        // The largest addressable memory block is always less than max(usize) / 2.
        const isize signed_count = to_isize(current_count);

        // Start position when pos is zero or positive.
        isize start_pos = pos;

        // Start position when pos is negative.
        if (pos < 0)
        {
            start_pos = signed_count + pos; // This can never overflow.
        }

        // End position when count is zero or positive.
        isize end_pos = 0;
        if (__builtin_add_overflow(start_pos, count, &end_pos))
        {
            end_pos = signed_count;
        }

        // End position when count is negative.
        if (count < 0)
        {
            end_pos = signed_count + count; // This can never overflow.
        }

        start_pos = math::max(start_pos, 0);

        isize diff = 0;
        if (__builtin_sub_overflow(end_pos, start_pos, &diff))
        {
            diff = 0;
        }
        diff = math::max(diff, 0);

        // Unsigned position and count (can still be invalid).
        return {to_usize(start_pos), to_usize(diff)};
    }

    // mask_calc() & mask_match()
    // https://www.agner.org/optimize/optimizing_assembly.pdf
    // "13.10 Vector operations in general purpose registers"
    // "The code examples in these manuals can be used without restrictions."

    constexpr u64 mask_calc(const char needle) noexcept
    {
        return to_byte(needle) * 0x0101010101010101u;
    }

    constexpr bool mask_match(const u64 block, const u64 mask) noexcept
    {
        const u64 m    = block ^ mask;
        const u64 diff = math::subtract_with_overflow(m, 0x0101010101010101u);
        return static_cast<bool>(diff & ~m & 0x8080808080808080u);
    }

    SNN_DIAGNOSTIC_PUSH
    SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

    // Boyer-Moore-Horspool
    // Uses a 256 byte lookup table on stack, so don't inline.
    __attribute__((noinline)) constexpr optional_index bmh(
        const char* const subject_data, const usize subject_size, const char* const needle_data,
        const usize needle_size, const usize start_pos, promise::is_valid_t) noexcept
    {
        snn_should(needle_size > 1 && needle_size < 256);
        snn_should(start_pos < subject_size && (subject_size - start_pos) >= needle_size);

        const usize needle_size_m_one = needle_size - 1;
        const usize needle_size_m_two = needle_size - 2;
        const char needle_back        = needle_data[needle_size_m_one];

        snn::array<u8, 256> skip_table;
        skip_table.fill(static_cast<u8>(needle_size));

        // Set skip count for all bytes in needle except the last byte.
        for (usize i = 0; i < needle_size_m_one; ++i)
        {
            skip_table.at(to_byte(needle_data[i]), promise::within_bounds) =
                static_cast<u8>(needle_size_m_one - i);
        }

        usize pos = start_pos + needle_size_m_one;
        while (pos < subject_size)
        {
            if (subject_data[pos] == needle_back)
            {
                // Try to match (reverse order).
                snn_should(pos > 0);
                usize i = pos - 1;
                usize j = needle_size_m_two;
                while (subject_data[i] == needle_data[j])
                {
                    if (j == 0)
                    {
                        return i;
                    }
                    snn_should(i > 0);
                    --i;
                    --j;
                }
            }

            pos += skip_table.at(to_byte(subject_data[pos]), promise::within_bounds);
        }

        return constant::npos;
    }

    __attribute__((noinline)) constexpr const char* custom_memchr(const char* const subject_data,
                                                                  const char needle,
                                                                  const usize subject_size) noexcept
    {
        const char* cur        = subject_data;
        const char* const last = subject_data + subject_size;

        constexpr int block_size = sizeof(u64);
        static_assert(block_size == 8);

        if ((last - cur) >= block_size)
        {
            const u64 mask = mask_calc(needle);
            do
            {
                const auto block = mem::raw::load<u64>(cur);
                if (mask_match(block, mask))
                {
                    break;
                }
                cur += block_size;
            } while ((last - cur) >= block_size);
        }

        while (cur < last)
        {
            if (*cur == needle)
            {
                return cur;
            }
            ++cur;
        }

        return nullptr;
    }

    constexpr optional_index find(const char* const subject_data, const usize subject_size,
                                  const char needle, const usize start_pos) noexcept
    {
        if (start_pos >= subject_size)
        {
            return constant::npos;
        }

        const char* location = nullptr;

#if defined(__FreeBSD__) && !defined(SNN_NATIVE_MEMCHR)
        // `custom_memchr()` is a bit faster than FreeBSD 13's `memchr()`.
        location = custom_memchr(subject_data + start_pos, needle, subject_size - start_pos);
#else
        if (std::is_constant_evaluated())
        {
            location = custom_memchr(subject_data + start_pos, needle, subject_size - start_pos);
        }
        else
        {
            location = static_cast<const char*>(__builtin_memchr(subject_data + start_pos, needle,
                                                                 subject_size - start_pos));
        }
#endif

        if (location == nullptr)
        {
            return constant::npos;
        }

        return to_usize(location - subject_data);
    }

    constexpr optional_index find(const char* const subject_data, const usize subject_size,
                                  const char* const needle_data, const usize needle_size,
                                  const usize start_pos) noexcept
    {
        if (needle_size == 1)
        {
            return find(subject_data, subject_size, needle_data[0], start_pos);
        }

        if (start_pos > subject_size)
        {
            return constant::npos;
        }

        const usize remaining = subject_size - start_pos;
        if (remaining < needle_size)
        {
            return constant::npos;
        }

        if (needle_size == 0)
        {
            return start_pos;
        }

        // Needle size must be less than 256 but otherwise these values are arbitrary.
        if (remaining >= 1'000 && needle_size >= 4 && needle_size < 256)
        {
            return bmh(subject_data, subject_size, needle_data, needle_size, start_pos,
                       promise::is_valid);
        }

        const usize needle_size_m_one = needle_size - 1;
        const char needle_back        = needle_data[needle_size_m_one];

        usize pos = find(subject_data, subject_size, needle_back, start_pos + needle_size_m_one)
                        .value_or_npos();
        if (pos != constant::npos)
        {
            // Calculate how many bytes we can skip if a match fails.
            usize skip_count = 1;
            while (skip_count <= needle_size_m_one &&
                   needle_data[needle_size_m_one - skip_count] != needle_back)
            {
                ++skip_count;
            }

            const usize needle_size_m_two = needle_size - 2;
            do
            {
                // Try to match (reverse order).
                snn_should(pos > 0);
                usize i = pos - 1;
                usize j = needle_size_m_two;
                while (subject_data[i] == needle_data[j])
                {
                    if (j == 0)
                    {
                        return i;
                    }
                    snn_should(i > 0);
                    --i;
                    --j;
                }

                // Find next occurrence of last byte in needle.
                pos =
                    find(subject_data, subject_size, needle_back, pos + skip_count).value_or_npos();
            } while (pos != constant::npos);
        }

        return constant::npos;
    }

    __attribute__((noinline)) constexpr const char* custom_memrchr(
        const char* const subject_data, const char needle, const usize subject_size) noexcept
    {
        const char* const first = subject_data;
        const char* cur         = subject_data + subject_size;

        constexpr int block_size = sizeof(u64);
        static_assert(block_size == 8);

        if ((cur - first) >= block_size)
        {
            const u64 mask = mask_calc(needle);
            do
            {
                cur -= block_size;
                const auto block = mem::raw::load<u64>(cur);
                if (mask_match(block, mask))
                {
                    cur += block_size;
                    break;
                }
            } while ((cur - first) >= block_size);
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
    }

    constexpr optional_index find_in_reverse(const char* const subject_data,
                                             const usize subject_size, const char needle,
                                             const usize start_pos) noexcept
    {
        usize size = subject_size;
        if (start_pos < size)
        {
            size = start_pos + 1;
        }

        const char* location = nullptr;

#if defined(__FreeBSD__) && !defined(SNN_NATIVE_MEMRCHR)
        // `custom_memrchr()` is faster than FreeBSD 13's `memrchr()`.
        location = custom_memrchr(subject_data, needle, size);
#else
        if (std::is_constant_evaluated())
        {
            location = custom_memrchr(subject_data, needle, size);
        }
        else
        {
            location = static_cast<const char*>(::memrchr(subject_data, needle, size));
        }
#endif

        if (location == nullptr)
        {
            return constant::npos;
        }

        return to_usize(location - subject_data);
    }

    constexpr optional_index find_in_reverse(const char* const subject_data,
                                             const usize subject_size,
                                             const char* const needle_data, const usize needle_size,
                                             const usize start_pos) noexcept
    {
        if (needle_size == 1)
        {
            return find_in_reverse(subject_data, subject_size, needle_data[0], start_pos);
        }

        if (subject_size < needle_size)
        {
            return constant::npos;
        }

        usize pos = subject_size - needle_size;
        if (start_pos < pos)
        {
            pos = start_pos;
        }

        if (needle_size == 0)
        {
            return pos;
        }

        const char needle_front = needle_data[0];

        pos = find_in_reverse(subject_data, subject_size, needle_front, pos).value_or_npos();
        if (pos != constant::npos)
        {
            // Determine how many bytes we can skip if a match fails.
            usize skip_count = needle_size;
            for (usize i = 1; i < needle_size; ++i)
            {
                if (needle_data[i] == needle_front)
                {
                    skip_count = i;
                    break;
                }
            }

            do
            {
                // Try to match (forward).
                usize i = pos + 1;
                usize j = 1;
                while (subject_data[i] == needle_data[j])
                {
                    ++j;
                    if (j == needle_size)
                    {
                        return pos;
                    }
                    ++i;
                }

                if (pos < skip_count)
                {
                    break;
                }

                pos = find_in_reverse(subject_data, subject_size, needle_front, pos - skip_count)
                          .value_or_npos();
            } while (pos != constant::npos);
        }

        return constant::npos;
    }

    SNN_DIAGNOSTIC_POP
}
