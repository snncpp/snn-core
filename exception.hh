// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Exception and `throw_or_abort(...)` function

#pragma once

#include "snn-core/error_code.hh"
#include "snn-core/strcore.fwd.hh"
#include <stdexcept> // exception, runtime_error

namespace snn
{
    // ## Classes

    // ### exception

    class exception final : public std::exception
    {
      public:
        explicit exception(const snn::error_code ec)
            : error_code_{ec}
        {
        }

        [[nodiscard]] snn::error_code error_code() const noexcept
        {
            return error_code_;
        }

        [[nodiscard]] const char* what() const noexcept override
        {
            return "snn::exception";
        }

      private:
        snn::error_code error_code_;
    };

    // ## Functions

    // ### throw_or_abort

    [[noreturn]] __attribute__((noinline)) inline void throw_or_abort(const error_code ec)
    {
        throw exception{ec};
    }

    template <same_as<const char> ConstChar, usize N>
    [[noreturn]] __attribute__((noinline)) void throw_or_abort(ConstChar (&error_message)[N])
    {
        throw std::runtime_error{error_message};
    }

    template <typename Buf>
        requires(strcore<Buf>::is_null_terminated())
    [[noreturn]] __attribute__((noinline)) void throw_or_abort(const strcore<Buf>& error_message)
    {
        throw std::runtime_error{error_message.cbegin()};
    }
}
