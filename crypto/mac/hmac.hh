// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # HMAC

// Hash-based message authentication code.
// https://en.wikipedia.org/wiki/HMAC

// Generate a message authentication code for some data using a cryptographic hash function
// (e.g. SHA-256) and a secret key.

#pragma once

#include "snn-core/array.hh"
#include "snn-core/strcore.hh"
#include "snn-core/chr/fn/common.hh"
#include "snn-core/hex/encode.hh"

namespace snn::crypto::mac
{
    // ## Classes

    // ### hmac

    template <typename Hash>
    class hmac final
    {
      public:
        static constexpr usize block_size  = Hash::block_size;
        static constexpr usize output_size = Hash::output_size;

        // #### Explicit constructor

        explicit hmac(const transient<cstrview> secret_key) noexcept
        {
            const cstrview sec_key = secret_key.get();

            array<char, Hash::block_size> key;

            if (sec_key.size() <= key.size())
            {
                key.fill_front(sec_key);
            }
            else
            {
                inner_.update(sec_key);
                inner_.final_oversized(key);
                inner_.reset();
            }

            array<char, Hash::block_size> padding;

            // Inner
            padding.fill('\x36');
            padding.transform(key, chr::fn::xor_bits{});
            inner_.update(padding.view());

            // Outer
            padding.fill('\x5c');
            padding.transform(key, chr::fn::xor_bits{});
            outer_.update(padding.view());
        }

        // #### Final MAC

        // Call _one_ of these functions to get the final MAC. The internal state after is
        // unspecified (depends on implementation).

        template <typename T>
        void final(T&& mac) noexcept(noexcept(final_(std::forward<T>(mac))))
        {
            final_(std::forward<T>(mac));
        }

        template <any_strcore Str = str>
        [[nodiscard]] Str final()
        {
            Str mac;
            final(mac);
            return mac;
        }

        template <typename Buf>
        void final_hex(strcore<Buf>& append_to)
        {
            array<char, Hash::output_size> mac;
            final(mac);
            hex::encode(mac.view(), append_to, promise::no_overlap);
        }

        template <any_strcore Str = str>
        [[nodiscard]] Str final_hex()
        {
            Str mac_hex;
            final_hex(mac_hex);
            return mac_hex;
        }

        // #### Update

        void update(const transient<cstrview> string) noexcept
        {
            inner_.update(string);
        }

        hmac& operator<<(const transient<cstrview> string) noexcept
        {
            update(string);
            return *this;
        }

      private:
        Hash inner_{};
        Hash outer_{};

        template <typename T>
        void final_(T&& mac) noexcept(noexcept(outer_.final(std::forward<T>(mac))))
        {
            array<char, Hash::output_size> inner_digest;
            inner_.final(inner_digest);
            outer_.update(inner_digest.view());
            outer_.final(std::forward<T>(mac));
        }
    };
}
