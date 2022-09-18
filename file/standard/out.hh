// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Unbuffered `stdout` writer

// Note: `stdout` is a macro in `<cstdio>`, so that name must be avoided.

#pragma once

#include "snn-core/file/standard/writer.hh"
#include <unistd.h> // STDOUT_FILENO

namespace snn::file::standard
{
    using out = writer<STDOUT_FILENO>;
}
