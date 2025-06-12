// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Join path components

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/file/path/append.hh"

namespace snn::file::path
{
    namespace detail::join
    {
        constexpr usize size(char) noexcept
        {
            return 1;
        }

        constexpr usize size(const cstrview s) noexcept
        {
            return s.size();
        }

        template <strict_integral Int>
        constexpr usize size(numeric<Int>) noexcept
        {
            return sizeof(Int) * 2; // Should be a reasonable guess.
        }
    }

    // ## Functions

    // ### join

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str join()
    {
        return Str{};
    }

    template <any_strcore Str = str, typename... Args>
    [[nodiscard]] constexpr Str join(const Args&... args)
    {
        Str append_to{init::reserve, (detail::join::size(args) + ...)};
        (path::detail::append::append(args, append_to), ...);
        return append_to;
    }
}
