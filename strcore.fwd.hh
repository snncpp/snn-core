// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # String (forward declare), concepts and `str[buf]` aliases

#pragma once

#include "snn-core/core.hh"

namespace snn
{
    // ## Classes

    // ### strcore

    // Forward declare.

    template <typename>
    class strcore;

    // ## Aliases

    // ### str/strbuf

    namespace detail::strcore
    {
        class buf;
        class sso;
    }

    using str    = strcore<detail::strcore::sso>;
    using strbuf = strcore<detail::strcore::buf>;

    // ## Type traits

    // ### is_strcore

    namespace detail
    {
        template <typename>
        struct is_strcore_strict : public std::false_type
        {
        };

        template <typename B>
        struct is_strcore_strict<snn::strcore<B>> : public std::true_type
        {
        };
    }

    template <typename T>
    struct is_strcore : public detail::is_strcore_strict<std::remove_cv_t<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_strcore_v = is_strcore<T>::value;

    // ## Concepts

    // ### any_strcore

    template <typename T>
    concept any_strcore = is_strcore_v<T>;
}
