// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Error (enum etc)

#pragma once

#include "snn-core/array.hh"
#include "snn-core/error_code.hh"

namespace snn::pcre
{
    // ## Enums

    // ### error

    enum class error : u8
    {
        no_error = 0,
        invalid_pattern,
        invalid_pattern_modifier,
        out_of_memory,
        undersized_storage, // Last (used below).
    };

    // ## Constants

    // ### error_count

    inline constexpr usize error_count = 5;
    static_assert(to_underlying(error::undersized_storage) == (error_count - 1));

    // ## Arrays

    // ### error_messages

    // clang-format off
    inline constexpr array<null_term<const char*>, error_count> error_messages{
        "No error",
        "Invalid pattern",
        "Invalid pattern modifier",
        "Out of memory",
        "Undersized storage",
    };
    // clang-format on

    // ## Constants

    // ### error_category

    inline constexpr error_category error_category{"snn::pcre", "PCRE", error_messages};

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
    struct is_error_code_enum_strict<pcre::error> : public std::true_type
    {
    };
}
