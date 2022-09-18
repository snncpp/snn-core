// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Optional (forward declare)

#pragma once

#include "snn-core/core.hh"

namespace snn
{
    // ## Classes

    // ### result

    // Forward declare.

    template <typename, typename>
    class result;

    // ### optional_error_storage

    // Forward declare.

    class optional_error_storage;

    // ## Aliases

    // ### optional

    template <typename T>
    using optional = result<T, optional_error_storage>;
}
