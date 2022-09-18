// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "snn-core/crypto/hash/base.hh"
#include <openssl/sha.h> // [#lib:crypto]

// The `SHA1_*` functions are deprecated starting with OpenSSL 3.0, but switching to allocating
// functions (`EVP_MD_CTX_new()`) that can fail is not what we want here. Note that the OpenSSL man
// page example for `EVP_DIGESTINIT` does not take allocation failure into account.

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

namespace snn::crypto::hash
{
    static_assert(SHA_CBLOCK == 64);
    static_assert(SHA_DIGEST_LENGTH == 20);

    class sha1 final : public base<sha1, SHA_DIGEST_LENGTH>
    {
      public:
        static constexpr usize block_size  = SHA_CBLOCK;
        static constexpr usize output_size = SHA_DIGEST_LENGTH;

        sha1() noexcept
        {
            init_();
        }

      private:
        friend class base<sha1, SHA_DIGEST_LENGTH>;

        ::SHA_CTX ctx_;

        void init_() noexcept
        {
            ::SHA1_Init(&ctx_);
        }

        template <usize Count>
            requires(Count >= output_size && Count != constant::dynamic_count)
        void final_(array_view<char, Count> digest) noexcept
        {
            ::SHA1_Final(digest.view().as_bytes().begin(), &ctx_);
        }

        void update_(const cstrview string) noexcept
        {
            ::SHA1_Update(&ctx_, string.cbegin(), string.size());
        }
    };
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
