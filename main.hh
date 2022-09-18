// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Application entry point

#include "snn-core/array_view.hh"
#include "snn-core/env/argument.hh"
#include "snn-core/fmt/exception.hh"
#include "snn-core/fmt/print.hh"

namespace snn
{
    // ## Functions

    // ### main

    // Forward declare, this function must be defined in each application.

    int main(array_view<const env::argument>);
}

int main(int argc, char* argv[])
{
    try
    {
        // `env::argument` has a single `const char*` member.
        static_assert(std::is_standard_layout_v<snn::env::argument>);
        static_assert(sizeof(snn::env::argument) == sizeof(char*));
        static_assert(alignof(snn::env::argument) == alignof(char*));

        return snn::main(snn::array_view{reinterpret_cast<const snn::env::argument*>(&argv[0]),
                                         snn::to_usize(argc)});
    }
    catch (const snn::exception& e)
    {
        snn::fmt::print_error("Error: {}\n", e);
    }
    catch (const std::exception& e)
    {
        snn::fmt::print_error("Error: {}\n", e);
    }
    catch (...)
    {
        snn::fmt::print_error("Error: Unknown exception\n");
    }

    return snn::constant::exit::failure;
}
