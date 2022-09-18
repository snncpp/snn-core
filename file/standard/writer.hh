// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Unbuffered writer

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/result.hh"
#include "snn-core/file/io/blocking.hh"
#include "snn-core/file/tty/dimensions.hh"
#include "snn-core/file/tty/is_terminal.hh"

namespace snn::file::standard
{
    // ## Classes

    // ### writer

    template <int Fd, typename Io = file::io::blocking>
        requires(Fd >= 0)
    class writer final
    {
      public:
        // #### Constructor

        explicit writer(Io io = Io{}) noexcept
            : io_{std::move(io)}
        {
        }

        // #### Operations

        [[nodiscard]] result<void> write_all(const transient<cstrview> buffer) noexcept
        {
            return io_.write_all(Fd, buffer);
        }

        // #### Status

        [[nodiscard]] static result<tty::window_dimensions> dimensions() noexcept
        {
            return tty::dimensions(Fd);
        }

        [[nodiscard]] static bool is_terminal() noexcept
        {
            return tty::is_terminal(Fd);
        }

        [[nodiscard]] static constexpr int number() noexcept
        {
            return Fd;
        }

        // #### Stream append

        writer& operator<<(const transient<cstrview> buffer)
        {
            write_all(buffer).or_throw();
            return *this;
        }

      private:
        Io io_;
    };
}
