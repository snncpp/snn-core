// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Format `byte_size`

#pragma once

#include "snn-core/formatter.hh"
#include "snn-core/size_prefixed_string_literal.hh"
#include "snn-core/strcore.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/fmt/context.hh"
#include "snn-core/fmt/error.hh"
#include "snn-core/fn/common.hh"

namespace snn::fmt::table::byte_size
{
    // ## Arrays

    // ### si[_short]

    // Use power-of-two sized arrays so the index can be masked.

    // SI/decimal (powers of 1000): B, kB, MB, GB, TB, PB, EB
    inline constexpr array<size_prefixed_string_literal, 8> si{
        "\001B", "\002kB", "\002MB", "\002GB", "\002TB", "\002PB", "\002EB", "\000",
    };

    // SI/decimal (powers of 1000): B, k, M, G, T, P, E
    inline constexpr array<size_prefixed_string_literal, 8> si_short{
        "\001B", "\001k", "\001M", "\001G", "\001T", "\001P", "\001E", "\000",
    };

    // ### iec[_short]

    // IEC/binary (powers of 1024): B, KiB, MiB, GiB, TiB, PiB, EiB
    inline constexpr array<size_prefixed_string_literal, 8> iec{
        "\001B", "\003KiB", "\003MiB", "\003GiB", "\003TiB", "\003PiB", "\003EiB", "\000",
    };

    // IEC/binary (powers of 1024): B, K, M, G, T, P, E
    inline constexpr array<size_prefixed_string_literal, 8> iec_short{
        "\001B", "\001K", "\001M", "\001G", "\001T", "\001P", "\001E", "\000",
    };
}

namespace snn::fmt
{
    // ## Functions

    // ### byte_size

    template <usize Divisor, typename StringLike, typename Buf>
        requires(Divisor == 1000 || Divisor == 1024)
    constexpr void byte_size(const snn::byte_size bs, const array<StringLike, 8>& suffixes,
                             const transient<cstrview> decimal_point,
                             const transient<cstrview> spacing, strcore<Buf>& append_to)
    {
        usize size = bs.get();

        usize unit_index = 0;

        // 0-999 bytes.
        if (size <= 999)
        {
            append_to.append_integral(size);
        }
        // 1000 bytes and up.
        else
        {
            usize remainder = 0;

            do
            {
                remainder = size % Divisor;
                size      = size / Divisor;
                ++unit_index;
            } while (size >= Divisor);

            // Convert remainder to a percentage (does nothing when `Divisor` is 1000).
            remainder = (remainder * 1000) / Divisor;

            // Round
            if (size <= 9)
            {
                remainder += 50;
            }
            else
            {
                remainder += 500;
            }

            // Round up?
            if (remainder >= 1000)
            {
                ++size;
                remainder = 0;
            }

            // Use a larger unit if size is four digits.
            if (size >= 1000)
            {
                size      = size / 1000;
                remainder = 0;
                ++unit_index;
            }

            append_to.append_integral(size);

            // Add single decimal for small size.
            if (size <= 9)
            {
                const usize decimal = remainder / 100;
                append_to.append(decimal_point.get());
                append_to.append_integral(decimal);
            }
        }

        append_to.append(spacing.get());
        append_to.append(cstrview{suffixes.at(unit_index, bounds::mask)});
    }

    template <usize Divisor, any_strcore Str = str, typename StringLike>
    [[nodiscard]] constexpr Str byte_size(const snn::byte_size bs,
                                          const array<StringLike, 8>& suffixes,
                                          const transient<cstrview> decimal_point = ".",
                                          const transient<cstrview> spacing       = " ")
    {
        Str append_to;
        fmt::byte_size<Divisor>(bs, suffixes, decimal_point, spacing, append_to);
        return append_to;
    }

    template <typename Buf>
    constexpr void byte_size(const snn::byte_size bs, strcore<Buf>& append_to)
    {
        fmt::byte_size<1000>(bs, fmt::table::byte_size::si, ".", " ", append_to);
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str byte_size(const snn::byte_size bs)
    {
        Str append_to;
        fmt::byte_size(bs, append_to);
        return append_to;
    }
}

namespace snn
{
    // ## Specializations

    // ### formatter<byte_size>

    template <>
    struct formatter<byte_size>
    {
        using type = byte_size;

        template <typename Buf>
        constexpr void format(const byte_size bs, const cstrview format_string, const fmt::context&,
                              strcore<Buf>& append_to, promise::no_overlap_t)
        {
            snn_should(std::is_constant_evaluated() || !format_string.overlaps(append_to));

            if (format_string.is_empty()) [[likely]]
            {
                fmt::byte_size<1000>(bs, fmt::table::byte_size::si, ".", " ", append_to);
            }
            else
            {
                // Syntax: [<decimal-point>0]<spacing>[<unit>]

                auto rng = format_string.range();

                auto decimal_point_rng = rng.pop_front_while(fn::_not{chr::is_digit});

                cstrview decimal_point{"."};

                if (decimal_point_rng && rng.drop_front('0'))
                {
                    decimal_point = decimal_point_rng.view();
                }
                else if (rng.is_empty())
                {
                    // No decimal point in format string.
                    rng = decimal_point_rng;
                }
                else
                {
                    throw_or_abort(fmt::error::unexpected_character_in_format_string);
                }

                bool si_unit                                        = true;
                const array<size_prefixed_string_literal, 8>* table = &fmt::table::byte_size::si;

                if (rng.drop_back('k'))
                {
                    table = &fmt::table::byte_size::si_short;
                }
                else if (rng.drop_back('K'))
                {
                    si_unit = false;
                    table   = &fmt::table::byte_size::iec_short;
                }
                else if (rng.drop_back("kB"))
                {
                    // Default
                }
                else if (rng.drop_back("KiB"))
                {
                    si_unit = false;
                    table   = &fmt::table::byte_size::iec;
                }

                // What's left in `rng` is spacing.
                const auto spacing = rng.view();

                if (si_unit)
                {
                    fmt::byte_size<1000>(bs, *table, decimal_point, spacing, append_to);
                }
                else
                {
                    fmt::byte_size<1024>(bs, *table, decimal_point, spacing, append_to);
                }
            }
        }
    };
}
