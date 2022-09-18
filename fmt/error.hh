// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Error (enum etc)

#pragma once

#include "snn-core/array.hh"
#include "snn-core/error_code.hh"

namespace snn::fmt
{
    // ## Enums

    // ### error

    enum class error : u8
    {
        no_error = 0,
        argument_index_out_of_bounds,
        integral_argument_is_negative,
        integral_argument_is_too_large,
        invalid_digits_per_group_in_format_string,
        invalid_index,
        invalid_minimum_digits_in_format_string,
        invalid_printf_format_string,
        invalid_width_in_format_string,
        printf_error,
        referenced_argument_must_be_a_string,
        referenced_argument_must_be_an_integral,
        unexpected_character_in_format_string,
        unexpected_end_of_format_string, // Last (used below).
    };

    // ## Constants

    // ### error_count

    inline constexpr usize error_count = 14;
    static_assert(to_underlying(error::unexpected_end_of_format_string) == (error_count - 1));

    // ## Arrays

    // ### error_messages

    inline constexpr array<null_term<const char*>, error_count> error_messages{
        "No error",
        "Argument index out of bounds",
        "Integral argument is negative",
        "Integral argument is too large",
        "Invalid digits per group in format string",
        "Invalid index",
        "Invalid minimum digits in format string",
        "Invalid printf() format string",
        "Invalid width in format string",
        "printf() error",
        "Referenced argument must be a string",
        "Referenced argument must be an integral",
        "Unexpected character in format string",
        "Unexpected end of format string",
    };

    // ## Constants

    // ### error_category

    inline constexpr snn::error_category error_category{"snn::fmt", "Format", error_messages};

    // ## Functions

    // ### make_error_code

    [[nodiscard]] constexpr snn::error_code make_error_code(const error e) noexcept
    {
        return snn::error_code{i32{to_underlying(e)}, error_category};
    }
}

namespace snn
{
    // ## Specializations

    // ### is_error_code_enum_strict

    template <>
    struct is_error_code_enum_strict<fmt::error> : public std::true_type
    {
    };
}
