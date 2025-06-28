// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Abbreviation

#pragma once

#include "snn-core/array.hh"
#include "snn-core/array_view.hh"
#include "snn-core/exception.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/time/error.hh"

namespace snn::time::zone
{
    // ## Classes

    // ### abbreviation

    class abbreviation final
    {
      public:
        // #### Constants

        static constexpr usize min_size = 3;
        static constexpr usize max_size = 6;

        // #### Explicit constructors

        constexpr explicit abbreviation(const transient<cstrview> abbr)
            : buf_{},
              size_{0}
        {
            if (is_valid(abbr.get()))
            {
                size_ = static_cast<u8>(buf_.fill_front(abbr.get()));
            }
            else
            {
                throw_or_abort(error::invalid_zone_abbreviation);
            }
        }

        constexpr explicit abbreviation(const transient<cstrview> abbr, assume::is_valid_t) noexcept
            : buf_{},
              size_{0}
        {
            snn_should(is_valid(abbr.get()));
            size_ = static_cast<u8>(buf_.fill_front(abbr.get()));
        }

        // #### Converting constructors

        template <same_as<const char> ConstChar, usize N>
        consteval abbreviation(ConstChar (&abbr)[N]) noexcept
            : buf_{},
              size_{N - 1}
        {
            // The abbreviation is assumed to only contain valid characters when provided as a
            // static string in a consteval context.
            static_assert(N > min_size && (N - 1) <= max_size); // `N` includes trailing zero.
            snn_should(is_valid(cstrview{abbr}));
            buf_.fill_front(array_view{abbr});
        }

        // #### Range

        [[nodiscard]] constexpr cstrrng range() const noexcept
        {
            return cstrrng{view()}; // Convert from view to skip nullptr check.
        }

        // #### View

        [[nodiscard]] constexpr cstrview view() const noexcept
        {
            return cstrview{buf_.data(), usize{size_}};
        }

        // #### Validation

        [[nodiscard]] static constexpr bool is_valid(const transient<cstrview> abbr) noexcept
        {
            const cstrview s = abbr.get();

            if (s.size() >= min_size && s.size() <= max_size)
            {
                // +/-hh[mm]
                if (s.has_front('+') || s.has_front('-'))
                {
                    return (s.size() == 3 || s.size() == 5) && s.view(1).all(chr::is_digit);
                }

                // GMT/CEST etc.
                return s.all(chr::is_alpha);
            }

            return false;
        }

        // #### Comparison operators

        // Compare with another `abbreviation`. Implicit conversion disabled.

        constexpr bool operator==(const same_as<abbreviation> auto& abbr) const noexcept
        {
            return view() == abbr.view();
        }

        constexpr auto operator<=>(const same_as<abbreviation> auto& abbr) const noexcept
        {
            return view() <=> abbr.view();
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

        // #### Factories

        [[nodiscard]] static consteval abbreviation utc() noexcept
        {
            return abbreviation{init::internal, 'U', 'T', 'C'};
        }

      private:
        array<char, max_size> buf_;
        u8 size_;

        consteval abbreviation(init::internal_t, const char a, const char b, const char c) noexcept
            : buf_{a, b, c},
              size_{3}
        {
        }
    };

    // #### Alias

    using abbr = abbreviation;
}
