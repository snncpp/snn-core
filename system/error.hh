// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Error category

#pragma once

#include "snn-core/error_code.hh"
#if defined(__linux__)
    #include <cstring> // strerrordesc_np
#else
    #include <cstdio> // sys_errlist, sys_nerr
#endif

namespace snn::system
{
    namespace detail
    {
        inline null_term<const char*> message_lookup(const i32 value, //
                                                     const null_term<const char*>*, usize) noexcept
        {
            const char* message = nullptr;

#if defined(__linux__)
            message = ::strerrordesc_np(value);
#else
            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            // The use of sys_errlist is deprecated, but:
            // * strerror_r() might use locale.
            // * strerror() uses a static buffer and calls strerror_r().
            // * There is no reason to allocate memory here.
            if (value >= 0 && value < ::sys_nerr)
            {
                message = ::sys_errlist[value];
            }

            SNN_DIAGNOSTIC_POP
#endif

            if (message != nullptr)
            {
                return null_term{not_null{message}, assume::null_terminated};
            }

            return null_term{""};
        }
    }

    // ## Constants

    // ### error_category

    inline constexpr error_category error_category{"snn::system", "System", detail::message_lookup};
}
