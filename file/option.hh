// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Option flags (enum)

#pragma once

#include "snn-core/core.hh"
#include <fcntl.h> // O_*

namespace snn::file
{
    // ## Enums

    // ### option

    enum class option : int
    {
        none = 0,

        // Individual options.

        append        = O_APPEND,
        close_on_exec = O_CLOEXEC,
        create        = O_CREAT,
        direct        = O_DIRECT,
        exclusive     = O_EXCL,
        no_follow     = O_NOFOLLOW,
        non_block     = O_NONBLOCK,
        sync          = O_SYNC,
        truncate      = O_TRUNC,

        // Access modes (`close_on_exec` is on by default).

        read_only  = O_RDONLY | O_CLOEXEC,
        write_only = O_WRONLY | O_CLOEXEC,
        read_write = O_RDWR | O_CLOEXEC,

        // Combined

        create_or_append   = O_CREAT | O_APPEND,
        create_or_fail     = O_CREAT | O_EXCL,
        create_or_truncate = O_CREAT | O_TRUNC,
    };

    // ## Functions

    // ### Bitwise operators

    [[nodiscard]] constexpr option operator&(const option left, const option right) noexcept
    {
        using UInt = std::make_unsigned_t<std::underlying_type_t<option>>;
        return static_cast<option>(static_cast<UInt>(left) & static_cast<UInt>(right));
    }

    [[nodiscard]] constexpr option operator|(const option left, const option right) noexcept
    {
        using UInt = std::make_unsigned_t<std::underlying_type_t<option>>;
        return static_cast<option>(static_cast<UInt>(left) | static_cast<UInt>(right));
    }

    [[nodiscard]] constexpr option operator~(const option o) noexcept
    {
        using UInt = std::make_unsigned_t<std::underlying_type_t<option>>;
        return static_cast<option>(~static_cast<UInt>(o));
    }
}
