// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Error (enum etc)

#pragma once

#include "snn-core/array.hh"
#include "snn-core/error_code.hh"

namespace snn::json
{
    // ## Enums

    // ### error

    enum class error : u8
    {
        no_error = 0,
        depth_limit,
        invalid_array,
        invalid_backslash_escape,
        invalid_codepoint_escape,
        invalid_number,
        invalid_object,
        invalid_surrogate_pair,
        invalid_utf8,
        node_limit,
        trailing_characters,
        unescaped_control_character,
        unexpected_character,
        unexpected_eof, // Last (used below).
    };

    // ## Constants

    // ### error_count

    inline constexpr usize error_count = 14;
    static_assert(to_underlying(error::unexpected_eof) == (error_count - 1));

    // ## Arrays

    // ### error_messages

    inline constexpr array<null_term<const char*>, error_count> error_messages{
        "No error",
        "Depth limit",
        "Invalid array",
        "Invalid backslash escape",
        "Invalid codepoint escape",
        "Invalid number",
        "Invalid object",
        "Invalid surrogate pair",
        "Invalid UTF-8",
        "Node limit",
        "Trailing characters",
        "Unescaped control character",
        "Unexpected character",
        "Unexpected end-of-file (EOF)",
    };

    // ## Constants

    // ### error_category

    inline constexpr error_category error_category{"snn::json", "JSON", error_messages};

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
    struct is_error_code_enum_strict<json::error> : public std::true_type
    {
    };
}
