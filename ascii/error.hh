// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Error (enum etc)

#pragma once

#include "snn-core/array.hh"
#include "snn-core/error_code.hh"

namespace snn::ascii
{
    // ## Enums

    // ### error

    enum class error : u8
    {
        no_error = 0,
        pad_string_can_not_be_empty, // Last (used below).
    };

    // ## Constants

    // ### error_count

    inline constexpr usize error_count = 2;
    static_assert(to_underlying(error::pad_string_can_not_be_empty) == (error_count - 1));

    // ## Arrays

    // ### error_messages

    inline constexpr array<null_term<const char*>, error_count> error_messages{
        "No error",
        "Pad string can not be empty",
    };

    // ## Constants

    // ### error_category

    inline constexpr error_category error_category{"snn::ascii", "ASCII", error_messages};

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
    struct is_error_code_enum_strict<ascii::error> : public std::true_type
    {
    };
}
