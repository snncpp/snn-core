// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Error (enum etc)

#pragma once

#include "snn-core/array.hh"
#include "snn-core/error_code.hh"

namespace snn::time
{
    // ## Enums

    // ### error

    enum class error : u8
    {
        no_error = 0,
        invalid_hour,
        invalid_minute,
        invalid_second,
        invalid_day,
        invalid_format_string,
        invalid_fraction,
        invalid_offset,
        invalid_offset_hour,
        invalid_offset_minute,
        invalid_month,
        invalid_month_name,
        invalid_nanosecond,
        invalid_tzif_data,
        invalid_week,
        invalid_weekday_name,
        invalid_year,
        invalid_zone_abbreviation,
        invalid_zone_name,
        string_exceeds_max_size,
        trailing_characters,
        unescaped_alpha_character, // Last (used below).
    };

    // ## Constants

    // ### error_count

    inline constexpr usize error_count = 22;
    static_assert(to_underlying(error::unescaped_alpha_character) == (error_count - 1));

    // ## Arrays

    // ### error_messages

    inline constexpr array<null_term<const char*>, error_count> error_messages{
        "No error",
        "Invalid hour",
        "Invalid minute",
        "Invalid second",
        "Invalid day",
        "Invalid format string",
        "Invalid fraction",
        "Invalid offset",
        "Invalid offset hour",
        "Invalid offset minute",
        "Invalid month",
        "Invalid month name",
        "Invalid nanosecond",
        "Invalid TZif data",
        "Invalid week",
        "Invalid weekday name",
        "Invalid year",
        "Invalid zone abbreviation",
        "Invalid zone name",
        "String exceeds maximum size",
        "Trailing characters",
        "Unescaped alpha character",
    };

    // ## Constants

    // ### error_category

    inline constexpr error_category error_category{"snn::time", "Time", error_messages};

    // ## Functions

    // ### make_error_code

    [[nodiscard]] constexpr error_code make_error_code(const error e) noexcept
    {
        return error_code{i32{to_underlying(e)}, error_category};
    }
}

namespace snn
{
    // ## Specializations

    // ### is_error_code_enum_strict

    template <>
    struct is_error_code_enum_strict<time::error> : public std::true_type
    {
    };
}
