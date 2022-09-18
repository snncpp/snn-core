// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "snn-core/crypto/hash/base.hh"
#include <bearssl_hash.h> // [#lib:bearssl]

namespace snn::crypto::hash
{
    static_assert(br_sha1_SIZE == 20);

    class sha1 final : public base<sha1, br_sha1_SIZE>
    {
      public:
        static constexpr usize block_size  = 64;
        static constexpr usize output_size = br_sha1_SIZE;

        sha1() noexcept
        {
            init_();
        }

      private:
        friend class base<sha1, br_sha1_SIZE>;

        ::br_sha1_context ctx_;

        void init_() noexcept
        {
            ::br_sha1_init(&ctx_);
        }

        template <usize Count>
            requires(Count >= output_size && Count != constant::dynamic_count)
        void final_(array_view<char, Count> digest) const noexcept
        {
            ::br_sha1_out(&ctx_, digest.begin());
        }

        void update_(const cstrview string) noexcept
        {
            ::br_sha1_update(&ctx_, string.cbegin(), string.size());
        }
    };
}
