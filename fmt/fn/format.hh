// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Format and append

#pragma once

#include "snn-core/fmt/format.hh"

namespace snn::fmt::fn
{
    // ## Classes

    // ### format

    template <any_strcore Str>
        requires(!std::is_const_v<Str>)
    class format final
    {
      public:
        // #### Constructors

        template <same_as<const char> ConstChar, usize N>
        constexpr explicit format(ConstChar (&string)[N], Str& append_to) noexcept
            : string_{string},
              append_to_{append_to}
        {
        }

        constexpr explicit format(const cstrview string, Str& append_to,
                                  promise::no_overlap_t) noexcept
            : string_{string},
              append_to_{append_to}
        {
        }

        // #### Operator()

        template <typename... Args>
        constexpr void operator()(const Args&... args) const
        {
            fmt::format_append(string_, append_to_, promise::no_overlap, args...);
        }

      private:
        cstrview string_;
        Str& append_to_;
    };
}
