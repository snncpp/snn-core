// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Fuzzer entry point

#include "snn-core/array_view.hh"
#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

namespace snn
{
    // ## Functions

    // ### fuzz

    // Forward declare, this function must be defined in each fuzzer.

    int fuzz(cstrview);
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    return snn::fuzz(snn::cstrview{reinterpret_cast<const char*>(data), size});
}
