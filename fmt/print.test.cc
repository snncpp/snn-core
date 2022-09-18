// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fmt/print.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            constexpr cstrview stdout_description = "standard output (stdout)";
            fmt::print("This is sent to {}.\n", stdout_description);
            fmt::print_line("This is sent to {}.", stdout_description);

            constexpr cstrview stderr_description = "standard error (stderr)";
            fmt::print_error("This is sent to {}.\n", stderr_description);
            fmt::print_error_line("This is sent to {}.", stderr_description);

            fmt::print("");       // Does nothing.
            fmt::print_error(""); // Does nothing.

            fmt::print_line();       // Prints a single '\n' to stdout.
            fmt::print_error_line(); // Prints a single '\n' to stderr.

            snn_require_throws_code(fmt::print_to(-1, "Hello\n"),
                                    make_error_code(EBADF, system::error_category));

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());
    }
}
