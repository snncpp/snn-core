// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Base part

#pragma once

#include "snn-core/file/path/split.hh"

namespace snn::file::path
{
    // ## Functions

    // ### base

    template <typename String = str>
    [[nodiscard]] constexpr result<String> base(const cstrview path) noexcept(
        std::is_nothrow_constructible_v<String, cstrview>)
    {
        return split<cstrview>(path).and_then([](auto&& parts) { return String{parts.base()}; });
    }
}
