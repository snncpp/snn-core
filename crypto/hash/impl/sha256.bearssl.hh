// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "snn-core/crypto/hash/base.hh"
#include <bearssl_hash.h> // [#lib:bearssl]

namespace snn::crypto::hash
{
    static_assert(br_sha256_SIZE == 32);

    class sha256 final : public base<sha256, br_sha256_SIZE>
    {
      public:
        static constexpr usize block_size  = 64;
        static constexpr usize output_size = br_sha256_SIZE;

        sha256() noexcept
        {
            init_();
        }

      private:
        friend class base<sha256, br_sha256_SIZE>;

        ::br_sha256_context ctx_;

        void init_() noexcept
        {
            ::br_sha256_init(&ctx_);
        }

        template <usize Count>
            requires(Count >= output_size && Count != constant::dynamic_count)
        void final_(array_view<char, Count> digest) const noexcept
        {
            ::br_sha256_out(&ctx_, digest.begin());
        }

        void update_(const cstrview string) noexcept
        {
            ::br_sha256_update(&ctx_, string.cbegin(), string.size());
        }
    };
}
