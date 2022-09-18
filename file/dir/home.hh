// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Home directory (from environment)

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/env/get.hh"

namespace snn::file::dir
{
    // ## Functions

    // ### home

    template <typename String>
    [[nodiscard]] optional<String> home() noexcept(noexcept(env::get<String>("HOME")))
    {
        return env::get<String>("HOME");
    }
}
