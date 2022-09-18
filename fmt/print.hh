// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Format and print

#pragma once

#include "snn-core/file/io/blocking.hh"
#include "snn-core/fmt/format.hh"
#include <unistd.h> // STDERR_FILENO, STDOUT_FILENO

namespace snn::fmt
{
    // ## print_to

    // Format and print to an open file descriptor.

    template <typename... Args>
    void print_to(const int fd, const transient<cstrview> format_string, const Args&... args)
    {
        const str formatted = fmt::format(format_string, args...);
        if (formatted)
        {
            file::io::blocking io;
            io.write_all(fd, formatted).or_throw();
        }
    }

    // ## print

    // Format and print to stdout.

    template <typename... Args>
    void print(const transient<cstrview> format_string, const Args&... args)
    {
        print_to(STDOUT_FILENO, format_string, args...);
    }

    // ## print_error

    // Format and print to stderr.

    template <typename... Args>
    void print_error(const transient<cstrview> format_string, const Args&... args)
    {
        print_to(STDERR_FILENO, format_string, args...);
    }

    // ## print_line_to

    // Format and print a line to an open file descriptor.

    template <typename... Args>
    void print_line_to(const int fd, const transient<cstrview> format_string, const Args&... args)
    {
        str formatted = fmt::format(format_string, args...);
        formatted.append('\n');

        file::io::blocking io;
        io.write_all(fd, formatted).or_throw();
    }

    inline void print_line_to(const int fd)
    {
        file::io::blocking io;
        io.write_all(fd, "\n").or_throw();
    }

    // ## print_line

    // Format and print a line to stdout.

    template <typename... Args>
    void print_line(const transient<cstrview> format_string, const Args&... args)
    {
        print_line_to(STDOUT_FILENO, format_string, args...);
    }

    inline void print_line()
    {
        print_line_to(STDOUT_FILENO);
    }

    // ## print_error_line

    // Format and print a line to stderr.

    template <typename... Args>
    void print_error_line(const transient<cstrview> format_string, const Args&... args)
    {
        print_line_to(STDERR_FILENO, format_string, args...);
    }

    inline void print_error_line()
    {
        print_line_to(STDERR_FILENO);
    }
}
