// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Country code

// Two-letter uppercase country/territory code.
// https://en.wikipedia.org/wiki/ISO_3166-1_alpha-2

#pragma once

#include "snn-core/array.hh"
#include "snn-core/array_view.hh"
#include "snn-core/exception.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/country/error.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::country
{
    // ## Classes

    // ### code

    class code final
    {
      public:
        // #### Explicit constructors

        constexpr explicit code(const transient<cstrview> cc)
            : code_{}
        {
            if (is_valid(cc))
            {
                code_.fill_front(cc.get());
            }
            else
            {
                throw_or_abort(error::invalid_two_letter_code);
            }
        }

        // #### Converting constructors

        consteval code(const char (&s)[3]) noexcept
            : code_{s[0], s[1]}
        {
            snn_should(chr::is_alpha_upper(s[0]) && chr::is_alpha_upper(s[1]));
        }

        // #### Range

        [[nodiscard]] constexpr cstrrng range() const noexcept
        {
            return cstrrng{view()}; // Convert from view to skip nullptr check.
        }

        // #### View

        [[nodiscard]] constexpr cstrview view() const noexcept
        {
            return code_.view();
        }

        // #### Validation

        [[nodiscard]] static constexpr bool is_valid(const transient<cstrview> cc) noexcept
        {
            const cstrview s = cc.get();
            return s.size() == 2 && s.all(chr::is_alpha_upper);
        }

        // #### Comparison operators

        // Compare with another `code`. Implicit conversion disabled.

        constexpr bool operator==(const same_as<code> auto& c) const noexcept
        {
            return code_ == c.code_;
        }

        constexpr auto operator<=>(const same_as<code> auto& c) const noexcept
        {
            return code_ <=> c.code_;
        }

        // Compare with a string.

        constexpr bool operator==(const transient<cstrview> s) const noexcept
        {
            return code_.view() == s.get();
        }

        constexpr auto operator<=>(const transient<cstrview> s) const noexcept
        {
            return code_.view() <=> s.get();
        }

      private:
        array<char, 2> code_;
    };
}
