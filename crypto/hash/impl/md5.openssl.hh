// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "snn-core/crypto/hash/base.hh"
#include <openssl/md5.h> // [#lib:crypto]

// The `MD5_*` functions are deprecated starting with OpenSSL 3.0, but switching to allocating
// functions (`EVP_MD_CTX_new()`) that can fail is not what we want here. Note that the OpenSSL man
// page example for `EVP_DIGESTINIT` does not take allocation failure into account.

SNN_DIAGNOSTIC_PUSH
SNN_DIAGNOSTIC_IGNORE_DEPRECATED_DECLARATIONS

namespace snn::crypto::hash
{
    static_assert(MD5_CBLOCK == 64);
    static_assert(MD5_DIGEST_LENGTH == 16);

    class md5 final : public base<md5, MD5_DIGEST_LENGTH>
    {
      public:
        static constexpr usize block_size  = MD5_CBLOCK;
        static constexpr usize output_size = MD5_DIGEST_LENGTH;

        md5() noexcept
        {
            init_();
        }

      private:
        friend class base<md5, MD5_DIGEST_LENGTH>;

        ::MD5_CTX ctx_;

        void init_() noexcept
        {
            ::MD5_Init(&ctx_);
        }

        template <usize Count>
            requires(Count >= output_size && Count != constant::dynamic_count)
        void final_(array_view<char, Count> digest) noexcept
        {
            ::MD5_Final(digest.view().as_bytes().begin(), &ctx_);
        }

        void update_(const cstrview string) noexcept
        {
            ::MD5_Update(&ctx_, string.cbegin(), string.size());
        }
    };
}

SNN_DIAGNOSTIC_POP
