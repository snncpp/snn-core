// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # PBKDF2-HMAC

// Derive a key from a password, a salt and an iteration count.

// https://en.wikipedia.org/wiki/PBKDF2

#pragma once

#include "snn-core/array.hh"
#include "snn-core/result.hh"
#include "snn-core/chr/fn/common.hh"
#include "snn-core/crypto/error.hh"
#include "snn-core/crypto/mac/hmac.hh"

namespace snn::crypto::kdf
{
    // ## Functions

    // ### pbkdf2_hmac

    // Password size must be between 0 and 4 096 bytes (inclusive).
    // 8 should be the absolute minimum, longer is strongly recommended.

    // Salt size must be between 8 and 32 bytes (inclusive).
    // 16 is recommended.

    // Iteration count must be between 1 000 and `constant::limit<u32>::max` (inclusive).
    // 100 000 or higher is recommended.

    template <typename Hash, any_strcore Str = str, usize DerivedKeySize = Hash::output_size>
        requires(DerivedKeySize > 0 && DerivedKeySize <= Hash::output_size)
    [[nodiscard]] result<Str> pbkdf2_hmac(const transient<cstrview> password,
                                          const transient<cstrview> salt,
                                          const usize iteration_count)
    {
        // Arbitrary max password size.
        if (password.get().size() > 4096)
        {
            return error::invalid_password_size;
        }

        // A 64-bit salt (8 bytes) is the absolute minimum to be considered unique.
        // Arbitrary max salt size.
        if (salt.get().size() < 8 || salt.get().size() > 32)
        {
            return error::invalid_salt_size;
        }

        // For the highest iteration count, `constant::limit<u32>::max` should be more than enough.
        // On Intel Xeon CPU E5-1650 v2 @ 3.50GHz with SHA-256:
        // 0.43 seconds/1 000 000 iterations, 4 294 967 295 iterations (u32 max) takes ~30 minutes.

        if (iteration_count < 1'000 || iteration_count > constant::limit<u32>::max)
        {
            return error::invalid_iteration_count;
        }

        array<char, Hash::output_size> digest_one;
        array<char, Hash::output_size> digest_two;

        const crypto::mac::hmac<Hash> hmac_init{password.get()};

        crypto::mac::hmac<Hash> hmac{hmac_init};

        hmac << salt.get() << "\x00\x00\x00\x01";
        hmac.final(digest_one);

        digest_two = digest_one;

        for (usize i = 2; i <= iteration_count; ++i)
        {
            hmac = hmac_init;
            hmac.update(digest_one.view());
            hmac.final(digest_one);

            digest_two.transform_with(digest_one, chr::fn::xor_bits{});
        }

        return Str{digest_two.view(0, DerivedKeySize)};
    }
}
