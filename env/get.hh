// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Get environment variable

#pragma once

#include "snn-core/null_term.hh"
#include "snn-core/optional.hh"
#include <cstdlib> // getenv

namespace snn::env
{
    // ## Functions

    // ### get

    // `std::getenv()` wrapper, "This function is thread-safe (...) as long as no other function
    // modifies the host environment.": https://en.cppreference.com/w/cpp/utility/program/getenv

    template <typename String>
    [[nodiscard]] optional<String> get(const transient<null_term<const char*>> name) noexcept(
        std::is_nothrow_constructible_v<String, not_null<const char*>, promise::null_terminated_t>)
    {
        const char* const val = std::getenv(name.get().get());
        if (val != nullptr)
        {
            return String{not_null{val}, promise::null_terminated};
        }
        return nullopt;
    }
}
