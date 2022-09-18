// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Formatter primary template

#pragma once

#include "snn-core/core.hh"

namespace snn
{
    // ## Classes

    // ### formatter

    // Must be specialized for a type to be formattable.

    template <typename T, typename = void>
    struct formatter
    {
        using type = T;

        static_assert(meta::always_false<T>,
                      "Include the required header file or add a snn::formatter<...> "
                      "specialization to format this type.");
    };

    // ### formatter_override

    // Can be specialized to override formatter<T>.

    template <typename, typename = void>
    struct formatter_override
    {
        using type = void;
    };

    // ## Type traits

    // ### formatter_format_as

    // Can be specialized to delegate to another formatter.
    // The type must be implicitly convertible to the delegated type.

    template <typename T>
    struct formatter_format_as
    {
        using type = T;
    };

    template <typename T>
    using formatter_format_as_t = typename formatter_format_as<T>::type;

    // ### formatter_select

    // Selects the formatter to use for a type. Should never be specialized.

    template <typename T>
    struct formatter_select_strict
    {
        using U    = formatter_format_as_t<T>;
        using type = std::conditional_t<std::is_same_v<U, typename formatter_override<U>::type>,
                                        formatter_override<U>, formatter<U>>;

        static_assert(std::is_same_v<typename type::type, U>,
                      "The selected formatter's type member is incorrect.");
    };

    template <typename T>
    struct formatter_select : public formatter_select_strict<std::remove_cvref_t<T>>
    {
    };

    template <typename T>
    using formatter_select_t = typename formatter_select<T>::type;
}
