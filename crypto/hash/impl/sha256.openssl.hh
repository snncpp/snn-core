// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "snn-core/crypto/hash/base.hh"
#include <openssl/sha.h> // [#lib:crypto]

// The `SHA256_*` functions are deprecated starting with OpenSSL 3.0, but switching to allocating
// functions (`EVP_MD_CTX_new()`) that can fail is not what we want here. Note that the OpenSSL man
// page example for `EVP_DIGESTINIT` does not take allocation failure into account.

SNN_DIAGNOSTIC_PUSH
SNN_DIAGNOSTIC_IGNORE_DEPRECATED_DECLARATIONS

namespace snn::crypto::hash
{
    static_assert(SHA256_CBLOCK == 64);
    static_assert(SHA256_DIGEST_LENGTH == 32);

    class sha256 final : public base<sha256, SHA256_DIGEST_LENGTH>
    {
      public:
        static constexpr usize block_size  = SHA256_CBLOCK;
        static constexpr usize output_size = SHA256_DIGEST_LENGTH;

        sha256() noexcept
        {
            init_();
        }

      private:
        friend class base<sha256, SHA256_DIGEST_LENGTH>;

        ::SHA256_CTX ctx_;

        void init_() noexcept
        {
            ::SHA256_Init(&ctx_);
        }

        template <usize Count>
            requires(Count >= output_size && Count != constant::dynamic_count)
        void final_(array_view<char, Count> digest) noexcept
        {
            ::SHA256_Final(digest.view().as_bytes().begin(), &ctx_);
        }

        void update_(const cstrview string) noexcept
        {
            ::SHA256_Update(&ctx_, string.cbegin(), string.size());
        }
    };
}

SNN_DIAGNOSTIC_POP
