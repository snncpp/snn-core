// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Generate integral/str

#pragma once

#include "snn-core/random/number.hh"
#include "snn-core/random/string.hh"

namespace snn::random::fn
{
    // ## Classes

    // ### generate<T> (not implemented)

    template <typename>
    class generate;

    // ### generate<Int> specialization

    template <strict_integral Int>
    class generate<Int> final
    {
      public:
        explicit generate() = default;

        [[nodiscard]] Int operator()() const
        {
            return random::number<Int>();
        }
    };

    // ### generate<Str> specialization

    template <any_strcore Str>
    class generate<Str> final
    {
      public:
        constexpr explicit generate(const usize size) noexcept
            : size_{size}
        {
        }

        [[nodiscard]] Str operator()() const
        {
            return random::string<Str>(size_);
        }

      private:
        usize size_;
    };
}
