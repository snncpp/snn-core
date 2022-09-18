// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/tty/dimensions.hh"

#include "snn-core/unittest.hh"
#include "snn-core/file/reader_writer.hh"
#include <unistd.h> // STDOUT_FILENO

namespace snn
{
    void unittest()
    {
        {
            constexpr file::tty::window_dimensions d;
            static_assert(d.character_height() == 0);
            static_assert(d.character_width() == 0);
            static_assert(d.pixel_height() == 0);
            static_assert(d.pixel_width() == 0);
        }

        {
            // clang-format off
            constexpr file::tty::window_dimensions d{winsize{.ws_row = 60,
                                                             .ws_col = 80,
                                                             .ws_xpixel = 640,
                                                             .ws_ypixel = 480}};
            // clang-format on
            static_assert(d.character_height() == 60);
            static_assert(d.character_width() == 80);
            static_assert(d.pixel_height() == 480);
            static_assert(d.pixel_width() == 640);
        }

        {
            const auto res = file::tty::dimensions(STDOUT_FILENO);
            if (res)
            {
                const auto dimensions = res.value(promise::has_value);
                snn_require(dimensions.character_height() > 0);
                snn_require(dimensions.character_width() > 0);
            }
        }

        {
            file::reader_writer rw;
            snn_require(rw.open_for_reading("/dev/urandom"));

            const int fd = rw.descriptor().value();

            snn_require(!file::tty::dimensions(fd)); // Not a TTY.
        }
    }
}
