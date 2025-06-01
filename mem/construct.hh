// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Construct a single object at a given address

#pragma once

#include "snn-core/core.hh"
#include <memory> // construct_at

namespace snn::mem
{
    // ## Functions

    // ### construct

    // `std::construct_at(...)` must be used for this function to work in a constant evaluated
    // context, but it uses parentheses instead of brace initialization, which isn't as strict and
    // narrows without warning. Using different initialization methods depending on context is
    // error-prone, so for now this function is constrained by the concept:
    // `brace_constructible_from<T, Args...>`

    template <typename T, typename... Args>
        requires brace_constructible_from<T, Args...>
    constexpr not_null<T*> construct(const not_null<T*> at, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<T, Args...>)
    {
        std::construct_at(at.get(), std::forward<Args>(args)...);
        return at;
    }
}
