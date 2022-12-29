// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is terminal

#pragma once

#include "snn-core/core.hh"
#include <unistd.h> // isatty

namespace snn::file::tty
{
    // ## Functions

    // ### is_terminal

    [[nodiscard]] inline bool is_terminal(const int fd) noexcept
    {
        return ::isatty(fd) == 1;
    }
}
