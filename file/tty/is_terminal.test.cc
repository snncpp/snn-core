// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/tty/is_terminal.hh"

#include "snn-core/unittest.hh"
#include <unistd.h> // isatty, STDOUT_FILENO

namespace snn
{
    void unittest()
    {
        const bool is_terminal = file::tty::is_terminal(STDOUT_FILENO);
        snn_require(is_terminal == static_cast<bool>(::isatty(STDOUT_FILENO)));
    }
}
