// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Append path components

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/file/path/is_absolute.hh"

namespace snn::file::path
{
    namespace detail::append
    {
        constexpr bool is_absolute(const char c) noexcept
        {
            return file::path::is_absolute(cstrview{as_ref(c)});
        }

        constexpr bool is_absolute(const cstrview s) noexcept
        {
            return file::path::is_absolute(s);
        }

        template <strict_integral Int>
        constexpr bool is_absolute(numeric<Int>) noexcept
        {
            return false;
        }

        template <typename T, typename Buf>
        constexpr void append(const T& part, strcore<Buf>& to)
        {
            if (detail::append::is_absolute(part))
            {
                to.clear();
            }
            else if (to && !to.has_back('/'))
            {
                to.append('/');
            }
            to << part;
        }
    }

    // ## Functions

    // ### append

    template <character Char, typename Buf>
    constexpr void append(const Char part, strcore<Buf>& to)
    {
        detail::append::append(part, to);
    }

    template <strict_integral Int, typename Buf>
    constexpr void append(const numeric<Int> part, strcore<Buf>& to)
    {
        detail::append::append(part, to);
    }

    template <same_as<const char> ConstChar, usize N, typename Buf>
    constexpr void append(ConstChar (&part)[N], strcore<Buf>& to)
    {
        detail::append::append(cstrview{part}, to);
    }

    template <typename Buf>
    constexpr void append(const transient<cstrview> part, strcore<Buf>& to, promise::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !part.get().overlaps(to));
        detail::append::append(part.get(), to);
    }
}
