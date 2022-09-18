// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Open and read from or write to a file

// The internal file descriptor is implicitly closed on destruction but it is recommended to
// explicitly call `close()` and check the return value, especially when writing to a file.

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/error_code.hh"
#include "snn-core/null_term.hh"
#include "snn-core/result.hh"
#include "snn-core/file/descriptor.hh"
#include "snn-core/file/info.hh"
#include "snn-core/file/offset.hh"
#include "snn-core/file/option.hh"
#include "snn-core/file/perm.hh"
#include "snn-core/file/seek.hh"
#include "snn-core/file/status.hh"
#include "snn-core/file/truncate.hh"
#include "snn-core/file/io/blocking.hh"
#include "snn-core/io/common.hh"
#include "snn-core/system/error.hh"
#include <cerrno>  // errno, E*
#include <fcntl.h> // open

namespace snn::file
{
    // ## Classes

    // ### reader_writer

    template <typename Io = file::io::blocking>
        requires(snn::io::readable<Io> || snn::io::writable<Io>)
    class reader_writer final
    {
      public:
        // #### Constructors

        explicit reader_writer(Io io = Io{}) noexcept
            : io_{std::move(io)},
              fd_{}
        {
        }

        explicit reader_writer(file::descriptor fd, Io io = Io{}) noexcept
            : io_{std::move(io)},
              fd_{std::move(fd)}
        {
        }

        explicit reader_writer(const transient<null_term<const char*>> path, const option options,
                               const perm permissions = perm::regular_default, Io io = Io{})
            : io_{std::move(io)},
              fd_{}
        {
            open(path, options, permissions).or_throw();
        }

        // #### Explicit conversion operators

        explicit operator bool() const noexcept
        {
            return is_open();
        }

        // #### Operations

        [[nodiscard]] result<void> close() noexcept
        {
            return fd_.close();
        }

        [[nodiscard]] file::descriptor& descriptor() noexcept
        {
            return fd_;
        }

        [[nodiscard]] const file::descriptor& descriptor() const noexcept
        {
            return fd_;
        }

        [[nodiscard]] bool is_open() const noexcept
        {
            return fd_.has_value();
        }

        // #### Open

        // Open without any default options, the access mode (e.g. `option::read_write`) must be
        // included in options.

        [[nodiscard]] result<void> open(const transient<null_term<const char*>> path,
                                        const option options,
                                        const perm permissions = perm::regular_default) noexcept
        {
            const auto flags = static_cast<int>(options);
            const auto mode  = static_cast<mode_t>(permissions);
            return open_(path, flags, mode);
        }

        // Open for reading and writing, file is created by default.

        [[nodiscard]] result<void> open_for_reading_and_writing(
            const transient<null_term<const char*>> path, const option options = option::create,
            const perm permissions = perm::regular_default) noexcept
        {
            const auto flags = static_cast<int>(option::read_write | options);
            const auto mode  = static_cast<mode_t>(permissions);
            return open_(path, flags, mode);
        }

        // Open for appending only, file is created by default.

        [[nodiscard]] result<void> open_for_appending(
            const transient<null_term<const char*>> path, const option options = option::create,
            const perm permissions = perm::regular_default) noexcept
        {
            const auto flags = static_cast<int>(option::write_only | option::append | options);
            const auto mode  = static_cast<mode_t>(permissions);
            return open_(path, flags, mode);
        }

        // Open for reading only, file must exist by default.

        [[nodiscard]] result<void> open_for_reading(
            const transient<null_term<const char*>> path, const option options = option::none,
            const perm permissions = perm::regular_default) noexcept
        {
            const auto flags = static_cast<int>(option::read_only | options);
            const auto mode  = static_cast<mode_t>(permissions);
            return open_(path, flags, mode);
        }

        // Open for writing only, file is created or truncated by default.

        [[nodiscard]] result<void> open_for_writing(
            const transient<null_term<const char*>> path,
            const option options   = option::create_or_truncate,
            const perm permissions = perm::regular_default) noexcept
        {
            const auto flags = static_cast<int>(option::write_only | options);
            const auto mode  = static_cast<mode_t>(permissions);
            return open_(path, flags, mode);
        }

        // #### Read

        // Read up to `buffer.size()` bytes. Returns 0 on end-of-file (or if the buffer is empty).

        [[nodiscard]] result<usize> read_some(strview buffer) noexcept
            requires(snn::io::readable<Io>)
        {
            return io_.read_some(fd_.value_or(-1), buffer);
        }

        // Read until `buffer` is full or an error occurs (e.g. `error::no_more_data`).

        [[nodiscard]] result<void> read_fill(strview buffer) noexcept
            requires(snn::io::readable<Io>)
        {
            return io_.read_fill(fd_.value_or(-1), buffer);
        }

        // #### Write

        // Write up to `buffer.size()` bytes. Returns the number of bytes written.

        [[nodiscard]] result<usize> write_some(const transient<cstrview> buffer) noexcept
            requires(snn::io::writable<Io>)
        {
            return io_.write_some(fd_.value_or(-1), buffer);
        }

        // Write until `buffer` is empty or an error occurs.

        [[nodiscard]] result<void> write_all(const transient<cstrview> buffer) noexcept
            requires(snn::io::writable<Io>)
        {
            return io_.write_all(fd_.value_or(-1), buffer);
        }

        // #### Offset

        [[nodiscard]] result<usize> offset() noexcept
        {
            return file::offset(fd_.value_or(-1));
        }

        template <strict_integral Int>
        [[nodiscard]] result<usize> seek(const Int offset,
                                         const file::whence w = file::whence::set) noexcept
        {
            return file::seek(fd_.value_or(-1), offset, w);
        }

        // #### Status

        [[nodiscard]] result<file::info> status() noexcept
        {
            return file::status(fd_.value_or(-1));
        }

        [[nodiscard]] result<void> status(file::info& info) noexcept
        {
            return file::status(fd_.value_or(-1), info);
        }

        // #### Truncate

        [[nodiscard]] result<void> truncate(const usize size = 0) noexcept
        {
            return file::truncate(fd_.value_or(-1), size);
        }

        // #### Stream append

        reader_writer& operator<<(const transient<cstrview> s)
        {
            write_all(s).or_throw();
            return *this;
        }

      private:
        Io io_;
        file::descriptor fd_;

        [[nodiscard]] result<void> open_(const transient<null_term<const char*>> path,
                                         const int flags, const mode_t mode) noexcept
        {
            while (true)
            {
                const int fd = ::open(path.get().get(), flags | Io::open_flags, mode);
                if (fd >= 0)
                {
                    fd_ = file::descriptor{fd};
                    return {};
                }

                if (errno != EINTR)
                {
                    return error_code{errno, system::error_category};
                }
            }
        }
    };
}
