// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Error (enum etc)

#pragma once

#include "snn-core/array.hh"
#include "snn-core/error_code.hh"

namespace snn::utf8
{
    // ## Enums

    // ### error

    enum class error : u8
    {
        no_error = 0,
        conversion,
        empty_pad_string,
        invalid,
        invalid_domain,
        library_initialization,
        unknown_encoding,
        out_of_memory, // Last (used below).
    };

    // ## Constants

    // ### error_count

    inline constexpr usize error_count = 8;
    static_assert(to_underlying(error::out_of_memory) == (error_count - 1));

    // ## Arrays

    // ### error_messages

    // clang-format off
    inline constexpr array<null_term<const char*>, error_count> error_messages{
        "No error",
        "Conversion failed",
        "Empty pad string",
        "Invalid input",
        "Invalid domain",
        "Library initialization",
        "Unknown encoding",
        "Out of memory",
    };
    // clang-format on

    // ## Constants

    // ### error_category

    inline constexpr error_category error_category{"snn::utf8", "UTF8", error_messages};

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
    struct is_error_code_enum_strict<utf8::error> : public std::true_type
    {
    };
}
