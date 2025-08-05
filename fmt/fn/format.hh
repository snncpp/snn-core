// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Format

#pragma once

#include "snn-core/fmt/format.hh"

namespace snn::fmt::fn
{
    // ## Classes

    // ### `format`

    template <any_strcore Str = str>
    class format final
    {
      public:
        // #### Explicit constructors

        constexpr explicit format(const cstrview string) noexcept
            : string_{string}
        {
        }

        // #### Operations

        template <typename... Args>
        [[nodiscard]] constexpr auto operator()(const Args&... args) const
        {
            return fmt::format<Str>(string_, args...);
        }

      private:
        cstrview string_;
    };

    // ### `format_append`

    template <any_strcore Str>
        requires(!std::is_const_v<Str>)
    class format_append final
    {
      public:
        // #### Explicit constructors

        template <same_as<const char> ConstChar, usize N>
        constexpr explicit format_append(ConstChar (&string)[N], Str& append_to) noexcept
            : string_{string},
              append_to_{append_to}
        {
        }

        constexpr explicit format_append(const cstrview string, Str& append_to,
                                         assume::no_overlap_t) noexcept
            : string_{string},
              append_to_{append_to}
        {
        }

        // #### Operations

        template <typename... Args>
        constexpr void operator()(const Args&... args) const
        {
            fmt::format_append(string_, append_to_, assume::no_overlap, args...);
        }

      private:
        cstrview string_;
        Str& append_to_;
    };
}
