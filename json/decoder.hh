// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Decoder

#pragma once

#include "snn-core/result.hh"
#include "snn-core/json/document.hh"
#include "snn-core/json/node.hh"
#include "snn-core/json/detail/decoder.hh"
#include "snn-core/pool/append_only.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::json
{
    // ## Classes

    // ### decoder

    class decoder final
    {
      public:
        static constexpr u16 default_depth_limit = 16;
        static constexpr u32 default_node_limit  = constant::limit<u32>::max;

        [[nodiscard]] constexpr usize byte_position() const noexcept
        {
            return byte_position_;
        }

        [[nodiscard]] constexpr result<document> decode_inplace(strrng rng)
        {
            pool::append_only<node> pool;
            detail::decoder d{rng, pool, node_limit_, depth_limit_};
            result<node&> root = d.decode_inplace();
            byte_position_     = d.byte_position();
            if (root)
            {
                return document{std::move(pool), root.value(assume::has_value)};
            }
            return root.error_code();
        }

        [[nodiscard]] constexpr u16 depth_limit() const noexcept
        {
            return depth_limit_;
        }

        [[nodiscard]] constexpr u32 node_limit() const noexcept
        {
            return node_limit_;
        }

        constexpr void set_depth_limit(const u16 i) noexcept
        {
            depth_limit_ = i;
        }

        constexpr void set_node_limit(const u32 i) noexcept
        {
            node_limit_ = i;
        }

      private:
        usize byte_position_ = 0;
        u32 node_limit_      = default_node_limit;
        u16 depth_limit_     = default_depth_limit;
    };
}
