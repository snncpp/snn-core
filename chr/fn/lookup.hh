// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Table lookup

#pragma once

#include "snn-core/array.hh"

namespace snn::chr::fn
{
    // ## Classes

    // ### lookup

    template <integral Int>
    class lookup final
    {
      public:
        constexpr explicit lookup(const array<Int, 256>& table) noexcept
            : data_{table.begin()}
        {
        }

        lookup(const array<Int, 256>&&) = delete;

        [[nodiscard]] constexpr Int operator()(const char c) const noexcept
        {
            static_assert(constant::limit<byte>::min == 0);
            static_assert(constant::limit<byte>::max == 255);
            return data_[to_byte(c)]; // Can't overflow.
        }

      private:
        const Int* data_;
    };
}
