// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # String size

#pragma once

#include "snn-core/array_view.fwd.hh"
#include "snn-core/strcore.fwd.hh"

namespace snn::string
{
    // ## Functions

    // ### size

    template <character Char, usize Count>
    [[nodiscard]] constexpr usize size(const array_view<Char, Count> s) noexcept
    {
        return s.size();
    }

    template <typename Buf>
    [[nodiscard]] constexpr usize size(const strcore<Buf>& s) noexcept
    {
        return s.size();
    }

    template <character Char>
    [[nodiscard]] constexpr usize size(const Char) noexcept
    {
        return 1;
    }

    // ### size (null terminated)

    // Null-terminated string size in bytes, excluding terminating null character.

    [[nodiscard]] constexpr usize size(const char* const first, promise::null_terminated_t) noexcept
    {
        if (first != nullptr)
        {
            return __builtin_strlen(first);
        }
        return 0;
    }

    [[nodiscard]] constexpr usize size(const not_null<const char*> first,
                                       promise::null_terminated_t) noexcept
    {
        return __builtin_strlen(first.get());
    }
}
