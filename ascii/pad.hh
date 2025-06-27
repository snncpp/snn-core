// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Pad to byte size

#pragma once

#include "snn-core/exception.hh"
#include "snn-core/strcore.hh"
#include "snn-core/ascii/error.hh"
#include "snn-core/math/common.hh"

namespace snn::ascii
{
    // ## Functions

    // ### pad

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str pad(const transient<cstrview> string, const usize size,
                                    const same_as<char> auto c)
    {
        const cstrview s = string.get();

        Str padded{init::reserve, math::max(s.size(), size)};
        if (size > s.size())
        {
            const usize diff        = size - s.size();
            const usize left_count  = diff / 2;
            const usize right_count = diff - left_count;
            padded.append_for_overwrite(left_count).fill(c);
            padded.append(s);
            padded.append_for_overwrite(right_count).fill(c);
        }
        else
        {
            padded.append(s);
        }
        return padded;
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str pad(const transient<cstrview> string, const usize size,
                                    const transient<cstrview> pad_string)
    {
        const cstrview s  = string.get();
        const cstrview ps = pad_string.get();

        if (ps.is_empty())
        {
            throw_or_abort(error::pad_string_can_not_be_empty);
        }

        Str padded{init::reserve, math::max(s.size(), size)};
        if (size > s.size())
        {
            const usize diff        = size - s.size();
            const usize left_count  = diff / 2;
            const usize right_count = diff - left_count;
            padded.append_for_overwrite(left_count).fill(ps);
            padded.append(s);
            padded.append_for_overwrite(right_count).fill(ps);
        }
        else
        {
            padded.append(s);
        }
        return padded;
    }

    // ### pad_inplace

    template <typename Buf>
    constexpr void pad_inplace(strcore<Buf>& string, const usize size, const same_as<char> auto c)
    {
        const usize current_size = string.size();
        if (size > current_size)
        {
            const usize diff        = size - current_size;
            const usize left_count  = diff / 2;
            const usize right_count = diff - left_count;
            string.reserve(size);
            string.insert_for_overwrite(0, left_count).fill(c);
            string.append_for_overwrite(right_count).fill(c);
        }
    }

    template <typename Buf>
    constexpr void pad_inplace(strcore<Buf>& string, const usize size,
                               const transient<cstrview> pad_string, assume::no_overlap_t)
    {
        const cstrview ps = pad_string.get();

        snn_should(std::is_constant_evaluated() || !ps.overlaps(string));

        if (ps.is_empty())
        {
            throw_or_abort(error::pad_string_can_not_be_empty);
        }

        const usize current_size = string.size();
        if (size > current_size)
        {
            const usize diff        = size - current_size;
            const usize left_count  = diff / 2;
            const usize right_count = diff - left_count;
            string.reserve(size);
            string.insert_for_overwrite(0, left_count).fill(ps);
            string.append_for_overwrite(right_count).fill(ps);
        }
    }

    // ### pad_left

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str pad_left(const transient<cstrview> string, const usize size,
                                         const same_as<char> auto c)
    {
        const cstrview s = string.get();

        Str padded{init::reserve, math::max(s.size(), size)};
        if (size > s.size())
        {
            padded.append_for_overwrite(size - s.size()).fill(c);
        }
        padded.append(s);
        return padded;
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str pad_left(const transient<cstrview> string, const usize size,
                                         const transient<cstrview> pad_string)
    {
        const cstrview s  = string.get();
        const cstrview ps = pad_string.get();

        if (ps.is_empty())
        {
            throw_or_abort(error::pad_string_can_not_be_empty);
        }

        Str padded{init::reserve, math::max(s.size(), size)};
        if (size > s.size())
        {
            const usize fill_size = size - s.size();
            padded.append_for_overwrite(fill_size).fill(ps);
        }
        padded.append(s);
        return padded;
    }

    // ### pad_left_inplace

    template <typename Buf>
    constexpr void pad_left_inplace(strcore<Buf>& string, const usize size,
                                    const same_as<char> auto c)
    {
        const usize current_size = string.size();
        if (size > current_size)
        {
            string.insert_for_overwrite(0, size - current_size).fill(c);
        }
    }

    template <typename Buf>
    constexpr void pad_left_inplace(strcore<Buf>& string, const usize size,
                                    const transient<cstrview> pad_string, assume::no_overlap_t)
    {
        const cstrview ps = pad_string.get();

        snn_should(std::is_constant_evaluated() || !ps.overlaps(string));

        if (ps.is_empty())
        {
            throw_or_abort(error::pad_string_can_not_be_empty);
        }

        const usize current_size = string.size();
        if (size > current_size)
        {
            const usize fill_size = size - current_size;
            string.insert_for_overwrite(0, fill_size).fill(ps);
        }
    }

    // ### pad_right

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str pad_right(const transient<cstrview> string, const usize size,
                                          const same_as<char> auto c)
    {
        const cstrview s = string.get();

        Str padded{init::reserve, math::max(s.size(), size)};
        padded.append(s);
        if (size > s.size())
        {
            padded.append_for_overwrite(size - s.size()).fill(c);
        }
        return padded;
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str pad_right(const transient<cstrview> string, const usize size,
                                          const transient<cstrview> pad_string)
    {
        const cstrview s  = string.get();
        const cstrview ps = pad_string.get();

        if (ps.is_empty())
        {
            throw_or_abort(error::pad_string_can_not_be_empty);
        }

        Str padded{init::reserve, math::max(s.size(), size)};
        padded.append(s);
        if (size > s.size())
        {
            const usize fill_size = size - s.size();
            padded.append_for_overwrite(fill_size).fill(ps);
        }
        return padded;
    }

    // ### pad_right_inplace

    template <typename Buf>
    constexpr void pad_right_inplace(strcore<Buf>& string, const usize size,
                                     const same_as<char> auto c)
    {
        const usize current_size = string.size();
        if (size > current_size)
        {
            string.append_for_overwrite(size - current_size).fill(c);
        }
    }

    template <typename Buf>
    constexpr void pad_right_inplace(strcore<Buf>& string, const usize size,
                                     const transient<cstrview> pad_string, assume::no_overlap_t)
    {
        const cstrview ps = pad_string.get();

        snn_should(std::is_constant_evaluated() || !ps.overlaps(string));

        if (ps.is_empty())
        {
            throw_or_abort(error::pad_string_can_not_be_empty);
        }

        const usize current_size = string.size();
        if (size > current_size)
        {
            const usize fill_size = size - current_size;
            string.append_for_overwrite(fill_size).fill(ps);
        }
    }
}
