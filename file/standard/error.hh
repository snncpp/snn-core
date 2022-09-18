// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Unbuffered `stderr` writer

// Note: `stderr` is a macro in `<cstdio>`, so that name must be avoided.

#pragma once

#include "snn-core/file/standard/writer.hh"
#include <unistd.h> // STDERR_FILENO

namespace snn::file::standard
{
    using error = writer<STDERR_FILENO>;
}
