// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Trim characters

// Trim by predicate, a single character or all ASCII control and whitespace.

#pragma once

#include "snn-core/chr/common.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/math/common.hh"

namespace snn::ascii
{
    // ## Functions

    // ### trim_left_inplace_if

    template <typename String, typename OneArgPred>
    constexpr void trim_left_inplace_if(String& s, OneArgPred p)
    {
        static_assert(std::is_same_v<front_value_t<String&>, char>);
        const usize pos = s.find_if(fn::_not{std::move(p)}).value_or_npos();
        if constexpr (requires { s.drop_front_n(pos); })
        {
            s.drop_front_n(pos);
        }
        else
        {
            s.drop_at(0, pos);
        }
    }

    // ### trim_left_inplace

    template <typename String>
    constexpr void trim_left_inplace(String& s, const char c) noexcept
    {
        trim_left_inplace_if(s, fn::is{fn::equal_to{}, c});
    }

    template <typename String>
    constexpr void trim_left_inplace(String& s) noexcept
    {
        trim_left_inplace_if(s, chr::is_ascii_control_or_space);
    }

    // ### trim_right_inplace_if

    template <typename String, typename OneArgPred>
    constexpr void trim_right_inplace_if(String& s, OneArgPred p)
    {
        static_assert(std::is_same_v<front_value_t<String&>, char>);
        const usize pos = s.find_in_reverse_if(fn::_not{std::move(p)}).value_or_npos();
        static_assert(constant::npos + 1 == 0);
        s.truncate(math::add_with_overflow(pos, 1));
    }

    // ### trim_right_inplace

    template <typename String>
    constexpr void trim_right_inplace(String& s, const char c) noexcept
    {
        trim_right_inplace_if(s, fn::is{fn::equal_to{}, c});
    }

    template <typename String>
    constexpr void trim_right_inplace(String& s) noexcept
    {
        trim_right_inplace_if(s, chr::is_ascii_control_or_space);
    }

    // ### trim_inplace_if

    template <typename String, typename OneArgPred>
    constexpr void trim_inplace_if(String& s, OneArgPred p)
    {
        trim_left_inplace_if(s, p);
        trim_right_inplace_if(s, p);
    }

    // ### trim_inplace

    template <typename String>
    constexpr void trim_inplace(String& s, const char c) noexcept
    {
        trim_left_inplace(s, c);
        trim_right_inplace(s, c);
    }

    template <typename String>
    constexpr void trim_inplace(String& s) noexcept
    {
        trim_left_inplace(s);
        trim_right_inplace(s);
    }

    // ### trim

    template <typename String>
    [[nodiscard]] constexpr String trim(String s, const char c) noexcept
    {
        trim_inplace(s, c);
        return s;
    }

    template <typename String>
    [[nodiscard]] constexpr String trim(String s) noexcept
    {
        trim_inplace(s);
        return s;
    }
}
