// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0 AND Apache-2.0 WITH LLVM-exception
// This is a constexpr version of `__murmur2_or_cityhash<..., 64>` from libc++ (see LICENSE.md).

#pragma once

#include "snn-core/mem/raw/load.hh"

namespace snn::detail::cityhash64
{
    struct u64pair
    {
        u64 first;
        u64 second;
    };

    inline constexpr usize k0 = 0xc3a5c85c97cb3127;
    inline constexpr usize k1 = 0xb492b66fbe98f273;
    inline constexpr usize k2 = 0x9ae16a3b2f90404f;
    inline constexpr usize k3 = 0xc949d7c7509e6557;

    template <typename Int>
    constexpr Int loadword(const char* const s) noexcept
    {
        return mem::raw::load<Int>(s);
    }

    // clang-format off

    __attribute__((__no_sanitize__("unsigned-shift-base")))
    constexpr usize rotate(const usize val, const usize shift) noexcept
    {
        return (val >> shift) | (val << (64u - shift));
    }

    constexpr usize shift_mix(const usize val) noexcept
    {
        return val ^ (val >> 47u);
    }

    __attribute__((__no_sanitize__("unsigned-integer-overflow")))
    constexpr usize hash_len_16(const usize u, const usize v) noexcept
    {
        constexpr usize mul = 0x9ddfea08eb382d69;
        usize a             = (u ^ v) * mul;
        a ^= (a >> 47u);
        usize b = (v ^ a) * mul;
        b ^= (b >> 47u);
        b *= mul;
        return b;
    }

    SNN_DIAGNOSTIC_PUSH
    SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

    __attribute__((__no_sanitize__("unsigned-integer-overflow", "unsigned-shift-base")))
    constexpr usize hash_len_0_to_16(const char* const s, const usize len) noexcept
    {
        if (len > 8)
        {
            const auto a = loadword<usize>(s);
            const auto b = loadword<usize>(s + len - 8);
            return hash_len_16(a, rotate(b + len, len)) ^ b;
        }
        if (len >= 4)
        {
            const auto a = loadword<u32>(s);
            const auto b = loadword<u32>(s + len - 4);
            return hash_len_16(len + (a << 3u), b);
        }
        if (len > 0)
        {
            const u8 a  = static_cast<u8>(s[0]);
            const u8 b  = static_cast<u8>(s[len >> 1u]);
            const u8 c  = static_cast<u8>(s[len - 1]);
            const u32 y = static_cast<u32>(a) + (static_cast<u32>(b) << 8u);
            const u32 z = static_cast<u32>(len) + (static_cast<u32>(c) << 2u);
            return shift_mix(y * k2 ^ z * k3) * k2;
        }
        return k2;
    }

    __attribute__((__no_sanitize__("unsigned-integer-overflow")))
    constexpr usize hash_len_17_to_32(const char* const s, const usize len) noexcept
    {
        const auto a = loadword<usize>(s) * k1;
        const auto b = loadword<usize>(s + 8);
        const auto c = loadword<usize>(s + len - 8) * k2;
        const auto d = loadword<usize>(s + len - 16) * k0;
        return hash_len_16(rotate(a - b, 43) + rotate(c, 30) + d, a + rotate(b ^ k3, 20) - c + len);
    }

    __attribute__((__no_sanitize__("unsigned-integer-overflow")))
    constexpr usize hash_len_33_to_64(const char* const s, const usize len) noexcept
    {
        auto z  = loadword<usize>(s + 24);
        auto a  = loadword<usize>(s) + (len + loadword<usize>(s + len - 16)) * k0;
        usize b = rotate(a + z, 52);
        usize c = rotate(a, 37);
        a += loadword<usize>(s + 8);
        c += rotate(a, 7);
        a += loadword<usize>(s + 16);
        const usize vf = a + z;
        const usize vs = b + rotate(a, 31) + c;
        a        = loadword<usize>(s + 16) + loadword<usize>(s + len - 32);
        z += loadword<usize>(s + len - 8);
        b = rotate(a + z, 52);
        c = rotate(a, 37);
        a += loadword<usize>(s + len - 24);
        c += rotate(a, 7);
        a += loadword<usize>(s + len - 16);
        const usize wf = a + z;
        const usize ws = b + rotate(a, 31) + c;
        const usize r  = shift_mix((vf + ws) * k2 + (wf + vs) * k0);
        return shift_mix(r * k0 + vs) * k2;
    }

    // "Return a 16-byte hash for 48 bytes. Quick and dirty. Callers do best to use
    // "random-looking" values for a and b."
    __attribute__((__no_sanitize__("unsigned-integer-overflow")))
    constexpr u64pair weak_hash_len_32_with_seeds(const usize w, const usize x, const usize y,
                                                  const usize z, usize a, usize b) noexcept
    {
        a += w;
        b             = rotate(b + a + z, 21);
        const usize c = a;
        a += x;
        a += y;
        b += rotate(a, 44);
        return u64pair{a + z, b + c};
    }

    // "Return a 16-byte hash for s[0] ... s[31], a, and b. Quick and dirty."
    constexpr u64pair weak_hash_len_32_with_seeds(const char* const s, const usize a,
                                                  const usize b) noexcept
    {
        return weak_hash_len_32_with_seeds(loadword<usize>(s), loadword<usize>(s + 8),
                                           loadword<usize>(s + 16), loadword<usize>(s + 24), a, b);
    }

    __attribute__((__no_sanitize__("unsigned-integer-overflow")))
    constexpr usize hash(const char* s, usize len) noexcept
    {
        if (len <= 32)
        {
            if (len <= 16)
            {
                return hash_len_0_to_16(s, len);
            }

            return hash_len_17_to_32(s, len);
        }

        if (len <= 64)
        {
            return hash_len_33_to_64(s, len);
        }

        // "For strings over 64 bytes we hash the end first, and then as we loop we keep 56
        // bytes of state: v, w, x, y, and z."
        auto x    = loadword<usize>(s + len - 40);
        auto y    = loadword<usize>(s + len - 16) + loadword<usize>(s + len - 56);
        usize z   = hash_len_16(loadword<usize>(s + len - 48) + len, loadword<usize>(s + len - 24));
        u64pair v = weak_hash_len_32_with_seeds(s + len - 64, len, z);
        u64pair w = weak_hash_len_32_with_seeds(s + len - 32, y + k1, x);
        x         = x * k1 + loadword<usize>(s);

        // "Decrease len to the nearest multiple of 64, and operate on 64-byte chunks."
        len = (len - 1) & ~static_cast<usize>(63);
        do
        {
            x = rotate(x + y + v.first + loadword<usize>(s + 8), 37) * k1;
            y = rotate(y + v.second + loadword<usize>(s + 48), 42) * k1;
            x ^= w.second;
            y += v.first + loadword<usize>(s + 40);
            z = rotate(z + w.first, 33) * k1;
            v = weak_hash_len_32_with_seeds(s, v.second * k1, x + w.first);
            w = weak_hash_len_32_with_seeds(s + 32, z + w.second, y + loadword<usize>(s + 16));
            std::swap(z, x);
            s += 64;
            len -= 64;
        } while (len != 0);
        return hash_len_16(hash_len_16(v.first, w.first) + shift_mix(y) * k1 + z,
                           hash_len_16(v.second, w.second) + x);
    }

    SNN_DIAGNOSTIC_POP

    // clang-format on
}
