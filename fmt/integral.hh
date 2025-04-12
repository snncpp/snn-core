// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Format integral

#pragma once

#include "snn-core/formatter.hh"
#include "snn-core/strcore.hh"
#include "snn-core/fmt/context.hh"
#include "snn-core/fmt/error.hh"
#include "snn-core/hex/table/common.hh"
#include "snn-core/math/common.hh"
#include "snn-core/mem/raw/copy.hh"
#include "snn-core/num/safe.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::fmt
{
    namespace detail
    {
        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        template <math::base Base, strict_unsigned_integral UInt, typename Buf>
        constexpr void integral(UInt num, const cstrview group_separator,
                                const usize digits_per_group, const usize min_digits,
                                const array<char, 16>& hex_table, strcore<Buf>& append_to,
                                promise::no_overlap_t)
        {
            const usize digit_count = math::max(math::count_digits<Base>(num).get(), min_digits);
            usize combined_size     = digit_count;

            if (digits_per_group > 0)
            {
                const usize separator_count = (digit_count - 1) / digits_per_group;
                combined_size               = num::safe{separator_count}
                                    .multiply(group_separator.size())
                                    .add(digit_count)
                                    .value();
            }

            strview dest      = append_to.append_uninitialized(combined_size);
            char* const first = dest.begin();
            char* cur         = dest.end();

            // Don't insert any separators if digits per group is zero.
            // A starting group count of one will never be equal to zero digits per group.
            usize group_count = (digits_per_group == 0) ? 1 : 0;

            while (cur > first)
            {
                if (group_count == digits_per_group)
                {
                    if (group_separator.size() == 1) [[likely]]
                    {
                        *(--cur) = group_separator.front(promise::not_empty);
                    }
                    else
                    {
                        cur -= group_separator.size();
                        mem::raw::copy(group_separator.data(), not_null{cur},
                                       group_separator.byte_size(), promise::no_overlap);
                    }
                    group_count = 0;
                }
                ++group_count;

                if constexpr (Base == math::base::decimal)
                {
                    const u64 digit = num % 10;
                    num /= 10;
                    *(--cur) = static_cast<char>(digit + '0');
                }
                else if constexpr (Base == math::base::hex)
                {
                    *(--cur) = hex_table.at(num, bounds::mask);
                    num >>= 4;
                }
                else if constexpr (Base == math::base::binary)
                {
                    *(--cur) = static_cast<char>((num & 0b1) + '0');
                    num >>= 1;
                }
                else if constexpr (Base == math::base::octal)
                {
                    *(--cur) = static_cast<char>((num & 0b111) + '0');
                    num >>= 3;
                }
                else
                {
                    static_assert(meta::always_false<decltype(Base)>, "Unsupported base.");
                }
            }
            snn_should(cur == first);
        }

        SNN_DIAGNOSTIC_POP

        template <math::base Base, strict_signed_integral SInt, typename Buf>
        constexpr void integral(const SInt num, const cstrview group_separator,
                                const usize digits_per_group, const usize min_digits,
                                const array<char, 16>& hex_table, strcore<Buf>& append_to,
                                promise::no_overlap_t)
        {
            auto u = force_unsigned(num);
            if (num < 0)
            {
                append_to.append('-');
                u = math::negate_with_overflow(u);
            }
            integral<Base>(u, group_separator, digits_per_group, min_digits, hex_table, append_to,
                           promise::no_overlap);
        }
    }

    // ## integral

    // Format decimal number with `char` thousands separator.

    template <strict_integral Int, typename Buf>
    constexpr void integral(const Int num, const same_as<char> auto thousands_separator,
                            strcore<Buf>& append_to)
    {
        const cstrview group_separator{as_ref(thousands_separator)};
        constexpr usize digits_per_group = 3;
        constexpr usize min_digits       = 0;
        // Promote to limit template instantiations.
        detail::integral<math::base::decimal>(promote<64>(num), group_separator, digits_per_group,
                                              min_digits, hex::table::lower, append_to,
                                              promise::no_overlap);
    }

    template <any_strcore Str = str, strict_integral Int>
    [[nodiscard]] constexpr Str integral(const Int num,
                                         const same_as<char> auto thousands_separator)
    {
        Str append_to;
        // Promote to limit template instantiations.
        integral(promote<64>(num), thousands_separator, append_to);
        return append_to;
    }

    // ## integral

    // Format decimal number with `cstrview` thousands separator.

    template <strict_integral Int, typename Buf>
    constexpr void integral(const Int num, const transient<cstrview> thousands_separator,
                            strcore<Buf>& append_to, promise::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !thousands_separator.get().overlaps(append_to));
        constexpr usize digits_per_group = 3;
        constexpr usize min_digits       = 0;
        // Promote to limit template instantiations.
        detail::integral<math::base::decimal>(promote<64>(num), thousands_separator.get(),
                                              digits_per_group, min_digits, hex::table::lower,
                                              append_to, promise::no_overlap);
    }

    template <any_strcore Str = str, strict_integral Int>
    [[nodiscard]] constexpr Str integral(const Int num,
                                         const transient<cstrview> thousands_separator)
    {
        Str append_to;
        constexpr usize digits_per_group = 3;
        constexpr usize min_digits       = 0;
        // Promote to limit template instantiations.
        detail::integral<math::base::decimal>(promote<64>(num), thousands_separator.get(),
                                              digits_per_group, min_digits, hex::table::lower,
                                              append_to, promise::no_overlap);
        return append_to;
    }

    // ## integral

    // Extended integral formatting.

    template <math::base Base = math::base::decimal, strict_integral Int, typename Buf>
    constexpr void integral(const Int num, const transient<cstrview> group_separator,
                            const usize digits_per_group, const usize min_digits,
                            const array<char, 16>& hex_table, strcore<Buf>& append_to,
                            promise::no_overlap_t)
    {
        snn_should(std::is_constant_evaluated() || !group_separator.get().overlaps(append_to));
        // Promote to limit template instantiations.
        detail::integral<Base>(promote<64>(num), group_separator.get(), digits_per_group,
                               min_digits, hex_table, append_to, promise::no_overlap);
    }

    template <any_strcore Str = str, math::base Base = math::base::decimal, strict_integral Int>
    [[nodiscard]] constexpr Str integral(const Int num, const transient<cstrview> group_separator,
                                         const usize digits_per_group, const usize min_digits = 0,
                                         const array<char, 16>& hex_table = hex::table::lower)
    {
        Str append_to;
        // Promote to limit template instantiations.
        detail::integral<Base>(promote<64>(num), group_separator.get(), digits_per_group,
                               min_digits, hex_table, append_to, promise::no_overlap);
        return append_to;
    }
}

namespace snn
{
    // Default formatters, can be overridden with formatter_override<T>.

    template <>
    struct formatter<u64>
    {
        using type = u64;

        template <strict_unsigned_integral UInt, typename Buf>
            requires(sizeof(UInt) >= 8)
        constexpr void format(const UInt i, const cstrview format_string, const fmt::context&,
                              strcore<Buf>& append_to, promise::no_overlap_t)
        {
            if (format_string.is_empty()) [[likely]]
            {
                append_to.append_integral(i);
            }
            else
            {
                snn_should(std::is_constant_evaluated() || !format_string.overlaps(append_to));

                auto rng = format_string.range();

                // Sign
                if (rng.drop_front(' '))
                {
                    append_to.append(' ');
                }
                else if (rng.drop_front('+'))
                {
                    append_to.append('+');
                }

                // Show prefix.
                const bool show_prefix = rng.drop_front('#');

                // Minimum digits.
                const auto min_digits_value_count =
                    rng.pop_front_integral<usize, math::base::decimal, 4>();
                const usize min_digits = min_digits_value_count.value;
                if (rng.has_front_if(chr::is_digit))
                {
                    throw_or_abort(fmt::error::invalid_minimum_digits_in_format_string);
                }

                // Base & prefix.

                enum class format_base : u8
                {
                    decimal   = 0,
                    hex_lower = 1,
                    hex_upper = 2,
                    binary    = 3,
                    octal     = 4,
                };

                cstrview prefix;
                format_base base = format_base::decimal;

                if (rng.drop_front('d'))
                {
                    // Nothing changes.
                }
                else if (rng.drop_front('x'))
                {
                    prefix = "0x";
                    base   = format_base::hex_lower;
                }
                else if (rng.drop_front('X'))
                {
                    prefix = "0x";
                    base   = format_base::hex_upper;
                }
                else if (rng.drop_front('b'))
                {
                    prefix = "0b";
                    base   = format_base::binary;
                }
                else if (rng.drop_front('o'))
                {
                    prefix = "0o";
                    base   = format_base::octal;
                }

                // Group separator.

                const auto group_separator = cstrview{rng.pop_front_while(fn::_not{chr::is_digit})};

                // Digits per group.

                const auto digits_per_group_value_count =
                    rng.pop_front_integral<usize, math::base::decimal, 2>();
                usize digits_per_group = digits_per_group_value_count.value;
                if (!digits_per_group_value_count.count)
                {
                    if (group_separator)
                    {
                        digits_per_group = 3;
                    }
                }
                else if (rng.has_front_if(chr::is_digit))
                {
                    throw_or_abort(fmt::error::invalid_digits_per_group_in_format_string);
                }

                if (rng)
                {
                    throw_or_abort(fmt::error::unexpected_character_in_format_string);
                }

                if (show_prefix && prefix)
                {
                    append_to.append(prefix);
                }

                switch (base)
                {
                    case format_base::decimal:
                        fmt::detail::integral<math::base::decimal>(i, group_separator,
                                                                   digits_per_group, min_digits,
                                                                   hex::table::lower, append_to,
                                                                   promise::no_overlap);
                        break;

                    case format_base::hex_lower:
                        fmt::detail::integral<math::base::hex>(i, group_separator, digits_per_group,
                                                               min_digits, hex::table::lower,
                                                               append_to, promise::no_overlap);
                        break;

                    case format_base::hex_upper:
                        fmt::detail::integral<math::base::hex>(i, group_separator, digits_per_group,
                                                               min_digits, hex::table::upper,
                                                               append_to, promise::no_overlap);
                        break;

                    case format_base::binary:
                        fmt::detail::integral<math::base::binary>(i, group_separator,
                                                                  digits_per_group, min_digits,
                                                                  hex::table::lower, append_to,
                                                                  promise::no_overlap);
                        break;

                    case format_base::octal:
                        fmt::detail::integral<math::base::octal>(i, group_separator,
                                                                 digits_per_group, min_digits,
                                                                 hex::table::lower, append_to,
                                                                 promise::no_overlap);
                        break;
                }
            }
        }

        template <strict_unsigned_integral UInt, typename Buf>
            requires(sizeof(UInt) < 8)
        constexpr void format(const UInt i, const cstrview format_string, const fmt::context& ctx,
                              strcore<Buf>& append_to, promise::no_overlap_t)
        {
            // Promote to limit template instantiations.
            format(promote<64>(i), format_string, ctx, append_to, promise::no_overlap);
        }
    };

    template <>
    struct formatter_format_as<u32>
    {
        using type = u64;
    };

    template <>
    struct formatter_format_as<u16>
    {
        using type = u64;
    };

    template <>
    struct formatter_format_as<u8>
    {
        using type = u64;
    };

    template <>
    struct formatter<i64>
    {
        using type = i64;

        template <strict_signed_integral SInt, typename Buf>
            requires(sizeof(SInt) >= 8)
        constexpr void format(const SInt i, cstrview format_string, const fmt::context& ctx,
                              strcore<Buf>& append_to, promise::no_overlap_t)
        {
            if (format_string.is_empty()) [[likely]]
            {
                append_to.append_integral(i);
            }
            else
            {
                snn_should(std::is_constant_evaluated() || !format_string.overlaps(append_to));

                auto u = force_unsigned(i);

                if (i < 0)
                {
                    append_to.append('-');
                    u = math::negate_with_overflow(u);

                    if (format_string.has_front(' ') || format_string.has_front('+'))
                    {
                        format_string.drop_front_n(1);
                    }
                }

                formatter<u64> f;
                f.format(u, format_string, ctx, append_to, promise::no_overlap);
            }
        }

        template <strict_signed_integral SInt, typename Buf>
            requires(sizeof(SInt) < 8)
        constexpr void format(const SInt i, const cstrview format_string, const fmt::context& ctx,
                              strcore<Buf>& append_to, promise::no_overlap_t)
        {
            // Promote to limit template instantiations.
            format(promote<64>(i), format_string, ctx, append_to, promise::no_overlap);
        }
    };

    template <>
    struct formatter_format_as<i32>
    {
        using type = i64;
    };

    template <>
    struct formatter_format_as<i16>
    {
        using type = i64;
    };

    template <>
    struct formatter_format_as<i8>
    {
        using type = i64;
    };

#if SNN_INT128_ENABLED
    template <>
    struct formatter_format_as<u128>
    {
        using type = u64;
    };

    template <>
    struct formatter_format_as<i128>
    {
        using type = i64;
    };
#endif
}
