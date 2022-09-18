// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Blocking I/O

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/result.hh"
#include "snn-core/strcore.fwd.hh"
#include "snn-core/file/error.hh"
#include "snn-core/system/error.hh"
#include <cerrno>   // errno, EINTR
#include <unistd.h> // read, write

namespace snn::file::io
{
    // ## Classes

    // ### blocking

    class blocking final
    {
      public:
        static constexpr int open_flags = 0;

        blocking() noexcept = default; // Required to disable aggregate initialization.

        // #### read_some

        // Read up to `buffer.size()` bytes. Returns 0 on end-of-file (or if the buffer is empty).

        [[nodiscard]] result<usize> read_some(const int fd, strview buffer) noexcept
        {
            do
            {
                const isize bytes_read = ::read(fd, buffer.writable().get(), buffer.size());
                if (bytes_read >= 0)
                {
                    return to_usize(bytes_read);
                }
                snn_should(bytes_read == -1);
            } while (errno == EINTR);
            return error_code{errno, system::error_category};
        }

        // #### read_fill

        // Read until `buffer` is full or an error occurs (e.g. `error::no_more_data`).

        [[nodiscard]] result<void> read_fill(const int fd, strview buffer) noexcept
        {
            while (buffer)
            {
                const result<usize> res = read_some(fd, buffer);
                if (res)
                {
                    const usize bytes_read = res.value(promise::has_value);
                    if (bytes_read > 0)
                    {
                        buffer.drop_front_n(bytes_read);
                    }
                    else
                    {
                        return file::error::no_more_data;
                    }
                }
                else
                {
                    return res.error_code();
                }
            }
            return {};
        }

        // #### write_some

        // Write up to `buffer.size()` bytes. Returns the number of bytes written.

        [[nodiscard]] result<usize> write_some(const int fd,
                                               const transient<cstrview> buffer) noexcept
        {
            const cstrview buf = buffer.get();
            do
            {
                const isize bytes_written = ::write(fd, buf.data().get(), buf.size());
                if (bytes_written >= 0)
                {
                    return to_usize(bytes_written);
                }
                snn_should(bytes_written == -1);
            } while (errno == EINTR);
            return error_code{errno, system::error_category};
        }

        // #### write_all

        // Write until `buffer` is empty or an error occurs.

        [[nodiscard]] result<void> write_all(const int fd,
                                             const transient<cstrview> buffer) noexcept
        {
            cstrview buf = buffer.get();
            while (buf)
            {
                const result<usize> res = write_some(fd, buf);
                if (res)
                {
                    buf.drop_front_n(res.value(promise::has_value));
                }
                else
                {
                    return res.error_code();
                }
            }
            return {};
        }

        // #### swap

        void swap(blocking&) noexcept
        {
            // Nothing to swap.
        }
    };
}
