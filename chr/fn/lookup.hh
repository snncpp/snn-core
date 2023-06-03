// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Table lookup

#pragma once

#include "snn-core/array_view.hh"

namespace snn::chr::fn
{
    // ## Classes

    // ### lookup

    template <integral Int>
    class lookup final
    {
      public:
        constexpr explicit lookup(const array<Int, 256>& table) noexcept
            : v_{table.template view<>()}
        {
        }

        lookup(const array<Int, 256>&&) = delete;

        [[nodiscard]] constexpr Int operator()(const char c) const noexcept
        {
            return v_.at(to_byte(c), promise::within_bounds);
        }

      private:
        array_view<const Int, 256> v_;
    };
}
