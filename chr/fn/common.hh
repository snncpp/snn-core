// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Common function objects

#pragma once

#include "snn-core/chr/common.hh"

namespace snn::chr::fn
{
    // ## Classes

    // ### is_base

    // When base is not known at compile time.

    class is_base final
    {
      public:
        constexpr explicit is_base(const math::base base) noexcept
            : f_{func_(base)}
        {
        }

        [[nodiscard]] constexpr bool operator()(const char c) const noexcept
        {
            return f_(c);
        }

      private:
        using func_type = bool (*)(char);

        func_type f_;

        static constexpr func_type func_(const math::base base) noexcept
        {
            // Clang will warn if a new base is not included here (`-Wswitch`).
            switch (base)
            {
                case math::base::binary:
                    return chr::is_binary;
                case math::base::octal:
                    return chr::is_octal;
                case math::base::decimal:
                    return chr::is_digit;
                case math::base::hex:
                    return chr::is_hex;
            }

            __builtin_trap();
        }
    };

    // ### xor_bits

    struct xor_bits final
    {
        explicit xor_bits() = default;

        [[nodiscard]] constexpr char operator()(const char a, const char b) const noexcept
        {
            return to_char(to_byte(a) ^ to_byte(b));
        }
    };
}
