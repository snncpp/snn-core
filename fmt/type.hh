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

    namespace detail
    {
        constexpr cstrview clang_type_name(const cstrview func) noexcept
        {
            constexpr cstrview prefix{"snn::cstrview snn::fmt::type_name() [T = "};
            constexpr cstrview suffix{"]"};
            if (func.has_front(prefix) && func.has_back(suffix))
            {
                return func.view(prefix.size(), func.size() - (prefix.size() + suffix.size()));
            }
            return cstrview{};
        }

        constexpr cstrview clang17_type_name(const cstrview func) noexcept
        {
            constexpr cstrview prefix{"cstrview snn::fmt::type_name() [T = "};
            constexpr cstrview suffix{"]"};
            if (func.has_front(prefix) && func.has_back(suffix))
            {
                return func.view(prefix.size(), func.size() - (prefix.size() + suffix.size()));
            }
            return cstrview{};
        }

        constexpr cstrview gcc_type_name(const cstrview func) noexcept
        {
            constexpr cstrview prefix{"constexpr snn::cstrview snn::fmt::type_name() [with T = "};
            constexpr cstrview suffix{"; snn::cstrview = snn::array_view<const char>]"};
            if (func.has_front(prefix) && func.has_back(suffix))
            {
                return func.view(prefix.size(), func.size() - (prefix.size() + suffix.size()));
            }
            return cstrview{};
        }
    }

    template <typename T>
    [[nodiscard]] constexpr cstrview type_name() noexcept
    {
        constexpr cstrview func{__PRETTY_FUNCTION__};
        if constexpr (constexpr auto clang_type_name = detail::clang_type_name(func))
        {
            return clang_type_name;
        }
        else if constexpr (constexpr auto clang17_type_name = detail::clang17_type_name(func))
        {
            return clang17_type_name;
        }
        else if constexpr (constexpr auto gcc_type_name = detail::gcc_type_name(func))
        {
            return gcc_type_name;
        }
        else
        {
            static_assert(meta::always_false<T>, "Type formatting is not supported.");
        }
        return cstrview{};
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
