// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Encode string

// Percent-encode all characters except "unreserved characters":
// https://www.rfc-editor.org/rfc/rfc3986#section-2.3
// unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"

#pragma once

#include "snn-core/array.hh"
#include "snn-core/strcore.hh"
#include "snn-core/chr/fn/lookup.hh"
#include "snn-core/hex/table/common.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::url
{
    // ## Functions

    // ### encode

    namespace detail
    {
        // Generated with: detail/encode.gen.cc
        inline constexpr array<bool, 256> encode_unreserved_lookup{
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        };

        template <typename Buf>
        constexpr void encode(const cstrview s, const array<char, 16>& hex_table,
                              strcore<Buf>& append_to)
        {
            append_to.reserve_append(s.size()); // Reserve minimal size.

            constexpr auto is_unreserved = chr::fn::lookup{detail::encode_unreserved_lookup};

            auto rng = s.range();
            while (rng)
            {
                const auto unreserved_rng = rng.pop_front_while(is_unreserved);
                if (unreserved_rng)
                {
                    append_to.append(unreserved_rng.view());
                }

                if (rng)
                {
                    const char c = rng.pop_front(assume::not_empty);
                    snn_should(!is_unreserved(c));

                    const auto b = to_byte(c);

                    array_view<char, 3> v = append_to.template append_for_overwrite<3>();
                    v.get<0>()            = '%';                                 // Prefix
                    v.get<1>()            = hex_table.at(b >> 4u, bounds::mask); // High bits.
                    v.get<2>()            = hex_table.at(b, bounds::mask);       // Low bits.
                }
            }
        }
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str encode(const transient<cstrview> s)
    {
        Str append_to;
        detail::encode(s.get(), hex::table::upper, append_to);
        return append_to;
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str encode(const transient<cstrview> s,
                                       const array<char, 16>& hex_table)
    {
        Str append_to;
        detail::encode(s.get(), hex_table, append_to);
        return append_to;
    }

    template <typename Buf>
    constexpr void encode(const transient<cstrview> s, strcore<Buf>& append_to,
                          assume::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !s.get().overlaps(append_to));
        detail::encode(s.get(), hex::table::upper, append_to);
    }

    template <typename Buf>
    constexpr void encode(const transient<cstrview> s, const array<char, 16>& hex_table,
                          strcore<Buf>& append_to, assume::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !s.get().overlaps(append_to));
        detail::encode(s.get(), hex_table, append_to);
    }
}
