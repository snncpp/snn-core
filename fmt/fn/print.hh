// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Format and print

#pragma once

#include "snn-core/fmt/print.hh"

namespace snn::fmt::fn
{
    // ## Classes

    // ### print

    class print final
    {
      public:
        explicit print(const cstrview string) noexcept
            : string_{string}
        {
        }

        template <typename... Args>
        void operator()(const Args&... args) const
        {
            fmt::print(string_, args...);
        }

      private:
        cstrview string_;
    };

    // ### print_error

    class print_error final
    {
      public:
        explicit print_error(const cstrview string) noexcept
            : string_{string}
        {
        }

        template <typename... Args>
        void operator()(const Args&... args) const
        {
            fmt::print_error(string_, args...);
        }

      private:
        cstrview string_;
    };
}
