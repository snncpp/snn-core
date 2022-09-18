// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "snn-core/crypto/hash/base.hh"
#include <bearssl_hash.h> // [#lib:bearssl]

namespace snn::crypto::hash
{
    static_assert(br_md5_SIZE == 16);

    class md5 final : public base<md5, br_md5_SIZE>
    {
      public:
        static constexpr usize block_size  = 64;
        static constexpr usize output_size = br_md5_SIZE;

        md5() noexcept
        {
            init_();
        }

      private:
        friend class base<md5, br_md5_SIZE>;

        ::br_md5_context ctx_;

        void init_() noexcept
        {
            ::br_md5_init(&ctx_);
        }

        template <usize Count>
            requires(Count >= output_size && Count != constant::dynamic_count)
        void final_(array_view<char, Count> digest) const noexcept
        {
            ::br_md5_out(&ctx_, digest.begin());
        }

        void update_(const cstrview string) noexcept
        {
            ::br_md5_update(&ctx_, string.cbegin(), string.size());
        }
    };
}
