// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Unit (forward declare), aliases and concepts

#pragma once

#include "snn-core/core.hh"

namespace snn::time
{
    // ## Classes

    // ### unit

    // Forward declare.

    template <i64, i64> // Numerator, Denominator.
    class unit;

    // ## Aliases

    // ### Various

    using nanoseconds  = unit<1, 1'000'000'000>;
    using microseconds = unit<1, 1'000'000>;
    using milliseconds = unit<1, 1'000>;
    using seconds      = unit<1, 1>;
    using minutes      = unit<60, 1>;
    using hours        = unit<3'600, 1>;
    using days         = unit<86'400, 1>;

    // ## Type traits

    // ### is_unit

    template <typename>
    struct is_unit_strict : public std::false_type
    {
    };

    template <i64 Num, i64 Den>
    struct is_unit_strict<unit<Num, Den>> : public std::true_type
    {
    };

    template <typename T>
    struct is_unit : public is_unit_strict<std::remove_cv_t<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_unit_v = is_unit<T>::value;

    // ## Concepts

    // ### any_unit

    template <typename T>
    concept any_unit = is_unit_v<T>;
}
