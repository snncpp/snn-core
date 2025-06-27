// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Pad to code point count

#pragma once

#include "snn-core/exception.hh"
#include "snn-core/strcore.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/num/safe.hh"
#include "snn-core/unicode/core.hh"
#include "snn-core/utf8/count.hh"
#include "snn-core/utf8/encode.hh"
#include "snn-core/utf8/error.hh"
#include "snn-core/utf8/status.hh"

namespace snn::utf8
{
    namespace detail
    {
        constexpr usize pad_fill_size(const usize count, const cstrview pad_string,
                                      assume::not_empty_t)
        {
            snn_should(!pad_string.is_empty());

            if (count == 0)
            {
                return 0;
            }

            const auto pad_string_info = utf8::status(pad_string).value();

            if (pad_string_info.is_ascii)
            {
                return count;
            }

            snn_should(pad_string_info.count > 0);

            const usize repeat_times = count / pad_string_info.count;
            usize remaining_count    = count % pad_string_info.count;

            usize size = num::safe{repeat_times}.multiply(pad_string.size()).value();

            auto rng = pad_string.range();
            while (remaining_count > 0 && rng)
            {
                const auto p = rng.pop_front_codepoint();
                size += p.count;
                --remaining_count;
            }

            return size;
        }
    }

    // ## Functions

    // ### pad_inplace

    template <typename Buf>
    constexpr void pad_inplace(strcore<Buf>& s, const usize count, const same_as<u32> auto cp)
    {
        const usize current_count = utf8::count(s).value();
        if (count > current_count)
        {
            const usize fill_count  = count - current_count;
            const usize left_count  = fill_count / 2;
            const usize right_count = fill_count - left_count;

            if (unicode::is_ascii(cp))
            {
                s.reserve_append(fill_count);
                s.insert_for_overwrite(0, left_count).fill(static_cast<char>(cp));
                s.append_for_overwrite(right_count).fill(static_cast<char>(cp));
            }
            else
            {
                array<char, 4> buffer;
                const cstrview encoded = utf8::encode(cp, buffer);

                // If the first doesn't overflow the second can't overflow.
                const usize right_fill_size =
                    num::safe{encoded.size()}.multiply(right_count).value();
                const usize left_fill_size = encoded.size() * left_count;

                // An overflow here is harmless.
                s.reserve_append(left_fill_size + right_fill_size);

                s.insert_for_overwrite(0, left_fill_size).fill(encoded);
                s.append_for_overwrite(right_fill_size).fill(encoded);
            }
        }
    }

    template <typename Buf>
    constexpr void pad_inplace(strcore<Buf>& s, const usize count, const same_as<char> auto c)
    {
        pad_inplace(s, count, u32{to_byte(c)});
    }

    template <typename Buf>
    constexpr void pad_inplace(strcore<Buf>& s, const usize count,
                               const transient<cstrview> pad_string, assume::no_overlap_t)
    {
        const cstrview ps = pad_string.get();

        snn_should(std::is_constant_evaluated() || !ps.overlaps(s));

        if (ps.is_empty())
        {
            throw_or_abort(error::empty_pad_string);
        }

        const usize current_count = utf8::count(s).value();
        if (count > current_count)
        {
            const usize fill_count  = count - current_count;
            const usize left_count  = fill_count / 2;
            const usize right_count = fill_count - left_count;

            const usize left_fill_size = detail::pad_fill_size(left_count, ps, assume::not_empty);
            const usize right_fill_size =
                detail::pad_fill_size(right_count, ps, assume::not_empty);

            // An overflow here is harmless.
            s.reserve_append(left_fill_size + right_fill_size);

            s.insert_for_overwrite(0, left_fill_size).fill(ps);
            s.append_for_overwrite(right_fill_size).fill(ps);
        }
    }

    // ### pad_left_inplace

    template <typename Buf>
    constexpr void pad_left_inplace(strcore<Buf>& s, const usize count, const same_as<u32> auto cp)
    {
        const usize current_count = utf8::count(s).value();
        if (count > current_count)
        {
            const usize fill_count = count - current_count;

            if (unicode::is_ascii(cp))
            {
                s.insert_for_overwrite(0, fill_count).fill(static_cast<char>(cp));
            }
            else
            {
                array<char, 4> buffer;
                const cstrview encoded = utf8::encode(cp, buffer);

                const usize fill_size = num::safe{encoded.size()}.multiply(fill_count).value();
                s.insert_for_overwrite(0, fill_size).fill(encoded);
            }
        }
    }

    template <typename Buf>
    constexpr void pad_left_inplace(strcore<Buf>& s, const usize count, const same_as<char> auto c)
    {
        pad_left_inplace(s, count, u32{to_byte(c)});
    }

    template <typename Buf>
    constexpr void pad_left_inplace(strcore<Buf>& s, const usize count,
                                    const transient<cstrview> pad_string, assume::no_overlap_t)
    {
        const cstrview ps = pad_string.get();

        snn_should(std::is_constant_evaluated() || !ps.overlaps(s));

        if (ps.is_empty())
        {
            throw_or_abort(error::empty_pad_string);
        }

        const usize current_count = utf8::count(s).value();
        if (count > current_count)
        {
            const usize fill_count = count - current_count;
            const usize fill_size  = detail::pad_fill_size(fill_count, ps, assume::not_empty);
            s.insert_for_overwrite(0, fill_size).fill(ps);
        }
    }

    // ### pad_right_inplace

    template <typename Buf>
    constexpr void pad_right_inplace(strcore<Buf>& s, const usize count, const same_as<u32> auto cp)
    {
        const usize current_count = utf8::count(s).value();
        if (count > current_count)
        {
            const usize fill_count = count - current_count;

            if (unicode::is_ascii(cp))
            {
                s.append_for_overwrite(fill_count).fill(static_cast<char>(cp));
            }
            else
            {
                array<char, 4> buffer;
                const cstrview encoded = utf8::encode(cp, buffer);

                const usize fill_size = num::safe{encoded.size()}.multiply(fill_count).value();
                s.append_for_overwrite(fill_size).fill(encoded);
            }
        }
    }

    template <typename Buf>
    constexpr void pad_right_inplace(strcore<Buf>& s, const usize count, const same_as<char> auto c)
    {
        pad_right_inplace(s, count, u32{to_byte(c)});
    }

    template <typename Buf>
    constexpr void pad_right_inplace(strcore<Buf>& s, const usize count,
                                     const transient<cstrview> pad_string, assume::no_overlap_t)
    {
        const cstrview ps = pad_string.get();

        snn_should(std::is_constant_evaluated() || !ps.overlaps(s));

        if (ps.is_empty())
        {
            throw_or_abort(error::empty_pad_string);
        }

        const usize current_count = utf8::count(s).value();
        if (count > current_count)
        {
            const usize fill_count = count - current_count;
            const usize fill_size  = detail::pad_fill_size(fill_count, ps, assume::not_empty);
            s.append_for_overwrite(fill_size).fill(ps);
        }
    }
}
