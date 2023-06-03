// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Size prefixed string literal

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/range/contiguous.hh"

namespace snn
{
    // ## Classes

    // ### size_prefixed_string_literal

    class size_prefixed_string_literal final
    {
      public:
        // #### Converting constructors

        template <same_as<const char> ConstChar, usize N>
            requires(N >= 2 && N <= 257) // 0-255 (not counting size prefix char + trailing '\0').
        consteval size_prefixed_string_literal(ConstChar (&s)[N]) noexcept
            : string_{s}
        {
            snn_should(size_() == N - 2);
        }

        // #### Implicit conversion operators

        constexpr operator cstrview() const noexcept
        {
            return view();
        }

        // #### Range

        [[nodiscard]] constexpr cstrrng range() const noexcept
        {
            return cstrrng{view()}; // Convert from view to skip nullptr check.
        }

        // #### View

        [[nodiscard]] constexpr cstrview view() const noexcept
        {
            return cstrview{not_null{data_()}, size_()};
        }

        // #### Comparison operators

        // Compare with another `size_prefixed_string_literal`. Implicit conversion disabled.

        constexpr bool operator==(
            const same_as<size_prefixed_string_literal> auto other) const noexcept
        {
            return view() == other.view();
        }

        constexpr auto operator<=>(
            const same_as<size_prefixed_string_literal> auto other) const noexcept
        {
            return view() <=> other.view();
        }

        // Compare with a string.

        constexpr bool operator==(const transient<cstrview> s) const noexcept
        {
            return view() == s.get();
        }

        constexpr auto operator<=>(const transient<cstrview> s) const noexcept
        {
            return view() <=> s.get();
        }

      private:
        const char* string_;

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        constexpr const char* data_() const noexcept
        {
            return &string_[1];
        }

        constexpr usize size_() const noexcept
        {
            return usize{to_byte(string_[0])};
        }

        SNN_DIAGNOSTIC_POP
    };
}
