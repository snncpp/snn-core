// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Permission bits (enum)

#pragma once

#include "snn-core/core.hh"

namespace snn::file
{
    // ## Enums

    // ### perm

    enum class perm : u16
    {
        none = 0,

        all_read  = 0444,
        all_write = 0222,
        all_exec  = 0111,
        all_all   = 0777,

        owner_read  = 0400,
        owner_write = 0200,
        owner_exec  = 0100,
        owner_all   = 0700,

        group_read  = 0040,
        group_write = 0020,
        group_exec  = 0010,
        group_all   = 0070,

        other_read  = 0004,
        other_write = 0002,
        other_exec  = 0001,
        other_all   = 0007,

        setuid = 04000,
        setgid = 02000,
        sticky = 01000,

        directory_default = 0777,
        regular_default   = 0666,

        mask = 07777,
    };

    // ## Functions

    // ### Bitwise operators

    [[nodiscard]] constexpr perm operator&(const perm left, const perm right) noexcept
    {
        return static_cast<perm>(to_underlying(left) & to_underlying(right));
    }

    [[nodiscard]] constexpr perm operator|(const perm left, const perm right) noexcept
    {
        return static_cast<perm>(to_underlying(left) | to_underlying(right));
    }

    [[nodiscard]] constexpr perm operator~(const perm p) noexcept
    {
        return static_cast<perm>(~to_underlying(p));
    }
}
