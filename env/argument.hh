// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Environment argument

#pragma once

#include "snn-core/null_term.hh"

namespace snn::env
{
    // ## Classes

    // ### argument

    class argument final
    {
      public:
        constexpr argument() noexcept = default;

        template <same_as<const char> ConstChar, usize N>
        constexpr argument(ConstChar (&s)[N]) noexcept
            : arg_{s}
        {
        }

        template <typename String>
        [[nodiscard]] constexpr String to() const noexcept(
            std::is_nothrow_constructible_v<String, const char*, promise::null_terminated_t>)
        {
            // `arg_` can be `nullptr`.
            return String{arg_, promise::null_terminated};
        }

        [[nodiscard]] constexpr null_term<const char*> null_terminated() const noexcept
        {
            if (arg_ != nullptr)
            {
                return null_term<const char*>{not_null{arg_}, promise::null_terminated};
            }
            return null_term<const char*>{""};
        }

      private:
        const char* arg_{""};
    };
}
