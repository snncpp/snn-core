// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/standard/error.hh"

#include "snn-core/unittest.hh"
#include <unistd.h> // STDERR_FILENO

namespace snn
{
    void unittest()
    {
        file::standard::error{} << "Unbuffered output to stderr.\n";
        static_assert(file::standard::error::number() == STDERR_FILENO);
    }
}
