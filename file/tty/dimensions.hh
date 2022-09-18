// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Terminal dimensions

#pragma once

#include "snn-core/result.hh"
#include "snn-core/system/error.hh"
#include <cerrno>      // errno, E*
#include <sys/ioctl.h> // ioctl, winsize, TIOCGWINSZ

namespace snn::file::tty
{
    // ## Classes

    // ### window_dimensions

    // `struct winsize` wrapper.

    class window_dimensions final
    {
      public:
        constexpr explicit window_dimensions() noexcept
            : ws_{}
        {
        }

        constexpr explicit window_dimensions(const struct ::winsize ws) noexcept
            : ws_{ws}
        {
        }

        [[nodiscard]] constexpr u16 character_height() const noexcept
        {
            return ws_.ws_row;
        }

        [[nodiscard]] constexpr u16 character_width() const noexcept
        {
            return ws_.ws_col;
        }

        [[nodiscard]] constexpr u16 pixel_height() const noexcept
        {
            return ws_.ws_ypixel;
        }

        [[nodiscard]] constexpr u16 pixel_width() const noexcept
        {
            return ws_.ws_xpixel;
        }

      private:
        struct ::winsize ws_;
    };

    // ## Functions

    // ### dimensions

    // Get the dimensions of the terminal (tty) window. The file descriptor must refer to a
    // terminal (tty) i.e. `is_terminal(...)`.

    [[nodiscard]] inline result<window_dimensions> dimensions(const int fd) noexcept
    {
        struct ::winsize ws = {};
        const int ret       = ::ioctl(fd, TIOCGWINSZ, &ws);
        if (ret != -1)
        {
            return window_dimensions{ws};
        }
        return error_code{errno, system::error_category};
    }
}
