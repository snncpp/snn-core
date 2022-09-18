// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Call a function on destruction

#pragma once

#include "snn-core/core.hh"

namespace snn
{
    // ## Classes

    // ### defer

    template <typename ZeroArgFn>
    class defer final
    {
      public:
        constexpr explicit defer(ZeroArgFn f) noexcept
            : f_{std::move(f)}
        {
        }

        // Non-copyable
        defer(const defer&)            = delete;
        defer& operator=(const defer&) = delete;

        // Non-movable
        defer(defer&&)            = delete;
        defer& operator=(defer&&) = delete;

        constexpr ~defer()
        {
            f_();
        }

      private:
        ZeroArgFn f_;
    };
}
