// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/file/standard/out.hh"

#include "snn-core/unittest.hh"
#include <unistd.h> // STDOUT_FILENO

namespace snn
{
    void unittest()
    {
        file::standard::out{} << "Unbuffered output to stdout.\n";
        static_assert(file::standard::out::number() == STDOUT_FILENO);
    }
}
