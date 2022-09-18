// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Base (interface)

#pragma once

#include "snn-core/array.hh"
#include "snn-core/strcore.hh"
#include "snn-core/hex/encode.hh"

namespace snn::crypto::hash
{
    // ## Classes

    // ### base

    template <typename Derived, usize OutputSize>
        requires(OutputSize > 0 && OutputSize != constant::dynamic_count)
    class base
    {
      public:
        // #### Final digest

        // Call _one_ of these functions to get the final digest. The internal state after is
        // unspecified (depends on implementation). Call `reset()` before reuse.

        template <usize Count>
            requires(Count == OutputSize)
        void final(array_view<char, Count> digest) noexcept
        {
            derived_().final_(digest);
        }

        template <usize Count>
            requires(Count == OutputSize)
        void final(array<char, Count>& digest) noexcept
        {
            final(digest.template view<>());
        }

        template <typename Buf>
        void final(strcore<Buf>& append_to)
        {
            final(append_to.template append_uninitialized<OutputSize>());
        }

        template <any_strcore Str = str>
        [[nodiscard]] Str final()
        {
            Str digest;
            final(digest);
            return digest;
        }

        // Only `OutputSize` bytes will be written to the digest buffer.
        template <usize Count>
            requires(Count >= OutputSize && Count != constant::dynamic_count)
        void final_oversized(array_view<char, Count> digest) noexcept
        {
            derived_().final_(digest);
        }

        // Only `OutputSize` bytes will be written to the digest buffer.
        template <usize Count>
            requires(Count >= OutputSize)
        void final_oversized(array<char, Count>& digest) noexcept
        {
            final_oversized(digest.template view<>());
        }

        template <typename Buf>
        void final_hex(strcore<Buf>& append_to)
        {
            array<char, OutputSize> digest;
            final(digest);
            hex::encode(digest.view(), append_to, promise::no_overlap);
        }

        template <any_strcore Str = str>
        [[nodiscard]] Str final_hex()
        {
            Str digest_hex;
            final_hex(digest_hex);
            return digest_hex;
        }

        // #### Reset

        void reset() noexcept
        {
            derived_().init_();
        }

        // #### Update

        void update(const transient<cstrview> string) noexcept
        {
            derived_().update_(string.get());
        }

        Derived& operator<<(const transient<cstrview> string) noexcept
        {
            update(string);
            return derived_();
        }

      protected:
        base() = default;

      private:
        Derived& derived_() noexcept
        {
            return *static_cast<Derived*>(this);
        }
    };
}
