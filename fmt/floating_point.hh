// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Floating point formatting

#pragma once

#include "snn-core/array.hh"
#include "snn-core/exception.hh"
#include "snn-core/optional.hh"
#include "snn-core/strcore.hh"
#include "snn-core/fmt/error.hh"
#include "snn-core/fmt/integral.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/math/common.hh"
#include "snn-core/math/fp/common.hh"
#include "snn-core/num/bounded.hh"
#include "snn-core/num/error.hh"
#include <cstdio> // snprintf

namespace snn::fmt
{
    // ## Functions

    // ### floating_point

    // Format to a fixed number of decimal places, rounding halfway cases away from zero.

    // `char` decimal point and thousands separator.

    // Note: This implementation is not ideal, it will throw on precision loss.

    template <typename Buf>
    void floating_point(const double num, const num::bounded<u64, 0, 12> decimal_places,
                        const char decimal_point, const optional<char> thousands_separator,
                        strcore<Buf>& append_to)
    {
        const u64 multiplier = math::pow10(decimal_places).get();

        const double n = math::fp::round(math::fp::abs(num) * static_cast<double>(multiplier));

        // Double has 15 significant decimal digits.
        static_assert(sizeof(double) == 8);
        // This also catches not-a-number (NaN), as NaN comparisons always return false.
        if (!(n < 1e15))
        {
            throw_or_abort(num::error::precision_loss);
        }

        u64 rounded = static_cast<u64>(n);

        const u64 remainder = rounded % multiplier;
        rounded /= multiplier;

        if (math::fp::is_negative(num))
        {
            append_to.append('-');
        }

        if (thousands_separator)
        {
            fmt::integral(rounded, thousands_separator.value(promise::has_value), append_to);
        }
        else
        {
            // Without thousands separator.
            append_to.append_integral(rounded);
        }

        if (decimal_places)
        {
            append_to.append(decimal_point);
            const usize min_digits = decimal_places.get();
            append_to.append_integral(remainder, min_digits);
        }
    }

    template <any_strcore Str = str>
    [[nodiscard]] Str floating_point(const double num,
                                     const num::bounded<u64, 0, 12> decimal_places,
                                     const char decimal_point                 = '.',
                                     const optional<char> thousands_separator = nullopt)
    {
        Str append_to;
        floating_point(num, decimal_places, decimal_point, thousands_separator, append_to);
        return append_to;
    }

    // ### floating_point

    // Format to a fixed number of decimal places, rounding halfway cases away from zero.

    // `cstrview` decimal point and thousands separator.

    // Note: This implementation is not ideal, it will throw on precision loss.

    template <typename Buf>
    void floating_point(const double num, const num::bounded<u64, 0, 12> decimal_places,
                        const transient<cstrview> decimal_point,
                        const transient<cstrview> thousands_separator, strcore<Buf>& append_to,
                        promise::no_overlap_t)
    {
        snn_should(!decimal_point.get().overlaps(append_to));
        snn_should(!thousands_separator.get().overlaps(append_to));

        const u64 multiplier = math::pow10(decimal_places).get();

        const double n = math::fp::round(math::fp::abs(num) * static_cast<double>(multiplier));

        // Double has 15 significant decimal digits.
        static_assert(sizeof(double) == 8);
        // This also catches not-a-number (NaN), as NaN comparisons always return false.
        if (!(n < 1e15))
        {
            throw_or_abort(num::error::precision_loss);
        }

        u64 rounded = static_cast<u64>(n);

        const u64 remainder = rounded % multiplier;
        rounded /= multiplier;

        if (math::fp::is_negative(num))
        {
            append_to.append('-');
        }

        if (thousands_separator.get())
        {
            fmt::integral(rounded, thousands_separator.get(), append_to, promise::no_overlap);
        }
        else
        {
            // Without thousands separator.
            append_to.append_integral(rounded);
        }

        if (decimal_places)
        {
            append_to.append(decimal_point.get());
            const usize min_digits = decimal_places.get();
            append_to.append_integral(remainder, min_digits);
        }
    }

    template <any_strcore Str = str>
    [[nodiscard]] Str floating_point(const double num,
                                     const num::bounded<u64, 0, 12> decimal_places,
                                     const transient<cstrview> decimal_point,
                                     const transient<cstrview> thousands_separator)
    {
        Str append_to;
        floating_point(num, decimal_places, decimal_point, thousands_separator, append_to,
                       promise::no_overlap);
        return append_to;
    }
}

namespace snn
{
    // ## Classes

    // Default formatters, can be overridden with formatter_override<T>.

    // ### formatter<double>

    // The `double` formatter currently only supports `snprintf()` and requires a format string that
    // begins with `'%'`.

    // TODO
    // Use `std::to_chars()` by default when it is available.

    template <>
    struct formatter<double>
    {
        using type = double;

        template <typename Buf>
        void format(const double d, const cstrview format_string, const fmt::context&,
                    strcore<Buf>& append_to, promise::no_overlap_t)
        {
            snn_should(!format_string.overlaps(append_to));

            if (!is_valid_printf_format(format_string))
            {
                throw_or_abort(fmt::error::invalid_printf_format_string);
            }

            array<char, 12> fmt_buf; // Zero-initialized.

            snn_should(format_string.size() < fmt_buf.size());
            fmt_buf.fill_front(format_string);
            snn_should(fmt_buf.back(promise::not_empty) == '\0');
            fmt_buf.back(promise::not_empty) = '\0'; // Redundant, just to be absolutely safe.

            array<char, 64> fixed_buf;

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif

            int ret = std::snprintf(fixed_buf.writable().get(), fixed_buf.size(),
                                    fmt_buf.data().get(), d);
            if (ret >= 0)
            {
                const auto size_needed = to_usize(ret);
                if (size_needed < fixed_buf.size()) [[likely]]
                {
                    append_to.append(fixed_buf.view(0, size_needed));
                }
                else
                {
                    auto dyn_buf = append_to.append_uninitialized(size_needed + sizeof('\0'));
                    ret          = std::snprintf(dyn_buf.writable().get(), dyn_buf.size(),
                                                 fmt_buf.data().get(), d);
                    if (ret >= 0 && to_usize(ret) == size_needed)
                    {
                        append_to.drop_back_n(sizeof('\0'));
                    }
                    else
                    {
                        throw_or_abort(fmt::error::printf_error);
                    }
                }
            }
            else
            {
                throw_or_abort(fmt::error::printf_error);
            }

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
        }

        template <typename Buf>
        void format(const float f, const cstrview format_string, const fmt::context& ctx,
                    strcore<Buf>& append_to, promise::no_overlap_t)
        {
            format(static_cast<double>(f), format_string, ctx, append_to, promise::no_overlap);
        }

        [[nodiscard]] static constexpr bool is_valid_printf_format(
            const cstrview format_string) noexcept
        {
            constexpr usize min_size = string_size("%g");
            // Longest supported (four flags should be enough, some are mutually exclusive).
            constexpr usize max_size = string_size("%#0+'99.99g");

            static_assert(min_size == 2);
            static_assert(max_size == 11);

            if (format_string.size() >= min_size && format_string.size() <= max_size)
            {
                auto rng = format_string.range();

                // Introductory '%' character (required).
                if (rng.drop_front('%'))
                {
                    // Flags (optional).
                    rng.pop_front_while(fn::in_array{'#', '0', '-', ' ', '+', '\''});

                    // Width (optional).
                    rng.pop_front_integral<usize, math::base::decimal, 2>();
                    if (rng.has_front_if(chr::is_digit))
                    {
                        return false;
                    }

                    // Precision (optional).
                    rng.drop_front('.');

                    // Precision digits (optional).
                    rng.pop_front_integral<usize, math::base::decimal, 2>();
                    if (rng.has_front_if(chr::is_digit))
                    {
                        return false;
                    }

                    // Conversion specifier (required).
                    if (rng.drop_front_if(fn::in_array{'a', 'A', 'e', 'E', 'f', 'F', 'g', 'G'}))
                    {
                        return rng.is_empty();
                    }
                }
            }

            return false;
        }
    };

    // ### formatter_format_as<float>

    template <>
    struct formatter_format_as<float>
    {
        using type = double;
    };
}
