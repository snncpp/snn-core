// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Error (enum etc)

#pragma once

#include "snn-core/array.hh"
#include "snn-core/error_code.hh"

namespace snn::file
{
    // ## Enums

    // ### error

    enum class error : u8
    {
        no_error = 0,
        failed_to_create_unique_temporary_directory,
        invalid_temporary_directory_path,
        invalid_temporary_directory_prefix,
        no_more_data, // Last (used below).
    };

    // ## Constants

    // ### error_count

    inline constexpr usize error_count = 5;
    static_assert(to_underlying(error::no_more_data) == (error_count - 1));

    // ## Arrays

    // ### error_messages

    inline constexpr array<null_term<const char*>, error_count> error_messages{
        "No error",
        "Failed to create a unique temporary directory",
        "Invalid temporary directory path",
        "Invalid temporary directory prefix",
        "No more data",
    };

    // ## Constants

    // ### error_category

    inline constexpr error_category error_category{"snn::file", "File", error_messages};

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
    struct is_error_code_enum_strict<file::error> : public std::true_type
    {
    };
}
