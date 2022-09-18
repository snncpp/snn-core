// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Common functionality (concepts)

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/result.hh"

namespace snn::stream
{
    // ## Concepts

    // clang-format off

    // ### readable

    template <typename T>
    concept readable = requires(T& t)
    {
        {t.read_some(strview{})} -> same_as<result<usize>>;
    };

    // ### writable

    template <typename T>
    concept writable = requires(T& t)
    {
        {t.write_some(cstrview{})} -> same_as<result<usize>>;
        {t.write_all(cstrview{})}  -> same_as<result<void>>;
    };

    // clang-format on
}
