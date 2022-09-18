// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Format C++ type name

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/strcore.fwd.hh"

namespace snn::fmt
{
    // ## Functions

    // ### type_name

    template <typename T>
    [[nodiscard]] constexpr cstrview type_name() noexcept
    {
#if defined(__clang__)
        constexpr cstrview prefix{"snn::cstrview snn::fmt::type_name() [T = "};
        constexpr cstrview suffix{"]"};
        constexpr cstrview s{__PRETTY_FUNCTION__};
#elif defined(__GNUC__) && __GNUC__ >= 10
        constexpr cstrview prefix{"constexpr snn::cstrview snn::fmt::type_name() [with T = "};
        constexpr cstrview suffix{"; snn::cstrview = snn::array_view<const char>]"};
        constexpr cstrview s{__PRETTY_FUNCTION__};
#else
#warning "type_name<T> is not implemented for this compiler."
        constexpr cstrview prefix;
        constexpr cstrview suffix;
        constexpr cstrview s;
#endif
        static_assert(s.has_front(prefix));
        static_assert(s.has_back(suffix));
        static_assert(s.size() >= (prefix.size() + suffix.size()));
        return s.view(prefix.size(), s.size() - (prefix.size() + suffix.size()));
    }

    // ## Functions

    // ### type

    template <typename T, typename Buf>
    constexpr void type(strcore<Buf>& append_to)
    {
        constexpr cstrview name = type_name<T>();
        append_to.append(name);
    }

    template <typename T, any_strcore Str = str>
    [[nodiscard]] constexpr Str type()
    {
        Str append_to;
        type<T>(append_to);
        return append_to;
    }
}
