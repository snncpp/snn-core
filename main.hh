// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Application entry point

#include "snn-core/array_view.hh"
#include "snn-core/strcore.hh"
#include "snn-core/env/argument.hh"
#include "snn-core/file/standard/error.hh"

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

        // `argc` is guaranteed to be non-negative.
        return snn::main(snn::array_view{reinterpret_cast<const snn::env::argument*>(&argv[0]),
                                         snn::to_usize(argc)});
    }
    catch (const snn::exception& e)
    {
        snn::file::standard::error{} << "Error: " << e.error_code().format() << "\n";
    }
    catch (const std::exception& e)
    {
        snn::file::standard::error{}
            << "Error: " << snn::cstrview{e.what(), snn::assume::null_terminated} << "\n";
    }
    catch (...)
    {
        snn::file::standard::error{} << "Error: Unknown exception\n";
    }

    return snn::constant::exit::failure;
}
