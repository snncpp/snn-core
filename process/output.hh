// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Command output

// Wrapper around `popen()` function (`man 3 popen`).

// Returned by `execute_and_consume_output()` function.

#pragma once

#include "snn-core/result.hh"
#include "snn-core/strcore.hh"
#include "snn-core/process/error.hh"
#include "snn-core/process/command.hh"
#include "snn-core/system/error.hh"
#include <stdio.h>  // ferror, FILE, getline, pclose, popen
#include <stdlib.h> // free

namespace snn::process
{
    // ## Classes

    // ### output

    class output final
    {
      public:
        // Non-copyable
        output(const output&)            = delete;
        output& operator=(const output&) = delete;

        // Non-movable
        output(output&&)            = delete;
        output& operator=(output&&) = delete;

        ~output()
        {
            if (file_ != nullptr)
            {
                ::pclose(file_);
            }

            ::free(getline_buffer_);
        }

        explicit operator bool() const noexcept
        {
            return file_ != nullptr;
        }

        [[nodiscard]] int exit_status() const noexcept
        {
            return exit_status_;
        }

        template <typename String = str>
        [[nodiscard]] result<String> read_line()
            noexcept(std::is_nothrow_constructible_v<String, cstrview>)
        {
            if (file_ == nullptr)
            {
                return error_code{EBADF, system::error_category};
            }

            const isize line_size = ::getline(&getline_buffer_, &getline_buffer_size_, file_);
            if (line_size >= 0)
            {
                // Line size is excluding the trailing null byte.
                snn_should(to_usize(line_size) < getline_buffer_size_);
                const cstrview line{getline_buffer_, to_usize(line_size)};
                return String{line};
            }

            const int saved_errno = errno;
            const int is_eof      = ::feof(file_);
            const int has_error   = ::ferror(file_);

            exit_status_ = ::pclose(file_);
            file_        = nullptr;

            if (is_eof)
            {
                return process::error::no_more_data;
            }

            exit_status_ = constant::exit::failure;

            if (has_error)
            {
                return error_code{EIO, system::error_category};
            }

            return error_code{saved_errno, system::error_category};
        }

      private:
        ::FILE* file_;
        char* getline_buffer_      = nullptr;
        usize getline_buffer_size_ = 0;
        int exit_status_           = constant::exit::failure;

        friend output execute_and_consume_output(const command&);

        output(const command& cmd)
            : file_{::popen(cmd.null_terminated().get(), "r")}
        {
        }
    };
}
