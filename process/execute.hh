// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Execute a shell command

#pragma once

#include "snn-core/process/command.hh"
#include "snn-core/process/output.hh"
#include <cstdlib> // system

namespace snn::process
{
    // ## Functions

    // ### execute

    // Execute a shell (/bin/sh) command and return exit status.
    // Wrapper around `system()` function (`man 3 system`).

    [[nodiscard]] inline int execute(const command& cmd)
    {
        return std::system(cmd.null_terminated().get());
    }

    // ### execute_and_consume_output

    // Execute a shell (/bin/sh) command and consume output.
    // Wrapper around `popen()` function (`man 3 popen`).

    [[nodiscard]] inline output execute_and_consume_output(const command& cmd)
    {
        return output{cmd};
    }
}
