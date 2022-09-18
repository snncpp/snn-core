// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Error (enum etc)

#pragma once

#include "snn-core/array.hh"
#include "snn-core/error_code.hh"

namespace snn::generic
{
    // ## Enums

    // ### error

    enum class error : u8
    {
        no_error = 0,
        capacity_would_exceed_max_capacity,
        insufficient_capacity,
        invalid_position_for_replace_or_insert,
        invalid_value,
        memory_allocation_failure,
        no_value,
        size_would_exceed_max_size,
        unexpected_null_character,
        unexpected_zero_value_error_code, // Last (used below).
    };

    // ## Constants

    // ### error_count

    inline constexpr usize error_count = 10;
    static_assert(to_underlying(error::unexpected_zero_value_error_code) == (error_count - 1));

    // ## Arrays

    // ### error_messages

    inline constexpr array<null_term<const char*>, error_count> error_messages{
        "No error",
        "Capacity would exceed max capacity",
        "Insufficient capacity",
        "Invalid position for replace/insert",
        "Invalid value",
        "Memory allocation failure",
        "No value",
        "Size would exceed max size",
        "Unexpected null character",
        "Unexpected 0 value error code",
    };

    // ## Constants

    // ### error_category

    inline constexpr error_category error_category{"snn::generic", "Generic", error_messages};

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
    struct is_error_code_enum_strict<generic::error> : public std::true_type
    {
    };
}
