// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Shell command

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/fn/common.hh"

namespace snn::process
{
    // ## Classes

    // ### command

    class command final
    {
      public:
        template <same_as<const char> ConstChar, usize N>
        constexpr void append_command(ConstChar (&s)[N])
        {
            cmd_.append(s);
        }

        constexpr void append_command(const transient<cstrview> s, assume::is_valid_t)
        {
            cmd_.append(s.get());
        }

        constexpr void append_parameter(const transient<cstrview> s)
        {
            // The parameter can overlap, so use a temporary buffer (reserve minimum size).
            str tmp{init::reserve, s.get().size() + 2};

            tmp.append('\''); // Open single quoted string.

            auto rng = s.get().range();
            while (rng)
            {
                tmp.append(cstrview{rng.pop_front_while(fn::is{fn::not_equal_to{}, '\''})});

                if (rng)
                {
                    snn_should(rng.has_front('\''));

                    tmp.append('\''); // Close single quoted string.
                    while (rng.drop_front('\''))
                    {
                        tmp.append("\\'"); // Backslash escaped single-quote.
                    }
                    tmp.append('\''); // Open single quoted string.
                }
            }

            tmp.append('\''); // Close single quoted string.

            cmd_.append(tmp);
        }

        template <math::base Base = math::base::decimal, strict_integral Int>
        constexpr void append_integral(const Int i, const usize min_digits = 0)
        {
            cmd_.append_integral<Base>(i, min_digits);
        }

        constexpr void clear() noexcept
        {
            cmd_.clear();
        }

        [[nodiscard]] constexpr null_term<const char*> null_terminated() const
        {
            return cmd_.null_terminated();
        }

        template <typename String>
        [[nodiscard]] constexpr String to() const noexcept(noexcept(String{cmd_.view()}))
        {
            return String{cmd_.view()};
        }

        constexpr command& operator<<(const transient<cstrview> s)
        {
            append_parameter(s);
            return *this;
        }

        template <usize N>
        constexpr command& operator<<(const char (&s)[N])
        {
            append_command(s);
            return *this;
        }

        template <strict_integral Int>
        constexpr command& operator<<(const numeric<Int> n)
        {
            append_integral(n.get());
            return *this;
        }

      private:
        str cmd_;
    };
}
