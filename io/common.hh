// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Common concepts (readable/writable)

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/result.hh"

namespace snn::io
{
    // ## Concepts

    // ### readable

    template <typename T>
    concept readable = requires(T& t) {
        { t.read_some(int{}, strview{}) } -> same_as<result<usize>>;
    };

    // ### writable

    template <typename T>
    concept writable = requires(T& t) {
        { t.write_some(int{}, cstrview{}) } -> same_as<result<usize>>;
        { t.write_all(int{}, cstrview{}) } -> same_as<result<void>>;
    };

}
