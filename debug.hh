// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Debug functions and macros

// `SNN_DEBUG` must be defined for `snn_debug*(...)` function macros to do anything.

#pragma once

#include "snn-core/array_view.fwd.hh"
#include "snn-core/strcore.fwd.hh"

#ifdef SNN_DEBUG
#include "snn-core/hex/table/common.hh"
#include <iostream>
namespace snn
{
    // ## Functions

    // ### debug

    // Don't call these functions directly, use `snn_debug*(...)` function macros.

    inline void debug()
    {
    }

    template <same_as<bool> Bool>
    void debug(const Bool b)
    {
        std::cerr << (b ? "true" : "false");
    }

    template <character Char>
    void debug(const Char c)
    {
        std::cerr << c;
    }

    template <strict_integral Int>
    void debug(const Int i)
    {
        std::cerr << promote(i);
    }

    template <floating_point Fp>
    void debug(const Fp f)
    {
        std::cerr << f;
    }

    template <character Char, usize Count>
    void debug(const array_view<Char, Count> string)
    {
        for (const char c : string)
        {
            if (c >= ' ' && c <= '~')
            {
                std::cerr << c;
                if (c == '\\')
                {
                    std::cerr << c;
                }
            }
            else
            {
                std::cerr << "\\x";
                std::cerr << hex::table::upper.at(to_byte(c) >> 4, bounds::mask); // High bits.
                std::cerr << hex::table::upper.at(to_byte(c), bounds::mask);      // Low bits.
            }
        }
    }

    template <typename Buf>
    void debug(const strcore<Buf>& string)
    {
        debug(string.view());
    }

    template <same_as<const char> ConstChar, usize N>
    void debug(ConstChar (&string)[N])
    {
        std::cerr << string;
    }

    template <typename T, typename U, typename... Args>
    void debug(T&& t, U&& u, Args&&... args)
    {
        snn::debug(std::forward<T>(t));
        snn::debug(std::forward<U>(u), std::forward<Args>(args)...);
    }

    // ### debug_line

    // Don't call this function directly, use `snn_debug*(...)` function macros.

    template <typename... Args>
    void debug_line(Args&&... args)
    {
        snn::debug(std::forward<Args>(args)...);
        std::cerr << std::endl;
    }
}

// ## Macros

// ### snn_debug[_line]

#define snn_debug(...)      snn::debug(__VA_ARGS__)
#define snn_debug_line(...) snn::debug_line(__VA_ARGS__)
#else
#define snn_debug(...)      ((void)0)
#define snn_debug_line(...) ((void)0)
#endif
