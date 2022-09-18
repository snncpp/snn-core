// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/tty/is_terminal.hh"

#include "snn-core/unittest.hh"
#include <unistd.h> // STDOUT_FILENO

namespace snn
{
    void unittest()
    {
        const bool is_terminal = file::tty::is_terminal(STDOUT_FILENO);
        ignore_if_unused(is_terminal); // True if `stdout` is a terminal (tty).
    }
}
