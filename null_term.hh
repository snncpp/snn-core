// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Null-terminated non-null pointer wrapper

// Null-terminated version of `not_null<Ptr>`.

#pragma once

#include "snn-core/strcore.fwd.hh"
#include "snn-core/string/size.hh"

namespace snn
{
    // ## Classes

    // ### null_term

    template <pointer Ptr>
    class null_term final
    {
      public:
        // #### Explicit constructors

        // Construct with the implicit promise that the pointer is null-terminated.

        constexpr explicit null_term(const not_null<Ptr> ptr) noexcept
            : ptr_{ptr}
        {
            if constexpr (SNN_ADDRESS_SANITIZER_ENABLED)
            {
                snn_should(count_() < constant::limit<usize>::max);
            }
        }

        // Construct with the explicit promise that the pointer is null-terminated.

        constexpr explicit null_term(const not_null<Ptr> ptr, promise::null_terminated_t) noexcept
            : null_term{ptr}
        {
        }

        // #### as_not_null

        [[nodiscard]] constexpr not_null<Ptr> as_not_null() const noexcept
        {
            return ptr_;
        }

        // #### get

        [[nodiscard]] constexpr Ptr get() const noexcept
        {
            return ptr_.get();
        }

        // #### Conversion

        template <typename Other>
        [[nodiscard]] constexpr Other to() const
            noexcept(noexcept(Other{ptr_, promise::null_terminated}))
        {
            return Other{ptr_, promise::null_terminated};
        }

      private:
        not_null<Ptr> ptr_;

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        constexpr usize count_() const noexcept
        {
            usize i = 0;
            Ptr raw = ptr_.get();
            while (raw[i] != 0)
            {
                ++i;
            }
            return i;
        }

        SNN_DIAGNOSTIC_POP
    };

    // ### null_term<const char*> specialization

    template <>
    class null_term<const char*> final
    {
      public:
        // #### Explicit constructors

        // Construct with the implicit promise that the pointer is null-terminated.

        constexpr explicit null_term(const not_null<const char*> s) noexcept
            : s_{s}
        {
            if constexpr (SNN_ADDRESS_SANITIZER_ENABLED)
            {
                snn_should(string::size(s, promise::null_terminated) < constant::limit<usize>::max);
            }
        }

        // Construct with the explicit promise that the pointer is null-terminated.

        constexpr explicit null_term(const not_null<const char*> s,
                                     promise::null_terminated_t) noexcept
            : null_term{s}
        {
        }

        // #### Converting constructors

        template <same_as<const char> ConstChar, usize N>
        constexpr null_term(ConstChar (&s)[N]) noexcept
            : s_{not_null{s}}
        {
        }

        template <typename Buf>
            requires(strcore<Buf>::is_null_terminated())
        null_term(const strcore<Buf>& s)
            : s_{s.null_terminated().as_not_null()}
        {
        }

        template <typename Buf>
            requires(strcore<Buf>::is_null_terminated())
        null_term(const strcore<Buf>&&) = delete; // Temporary, use null_terminated() if safe.

        // #### as_not_null

        [[nodiscard]] constexpr not_null<const char*> as_not_null() const noexcept
        {
            return s_;
        }

        // #### get

        [[nodiscard]] constexpr const char* get() const noexcept
        {
            return s_.get();
        }

        // #### Conversion

        template <typename String>
        [[nodiscard]] constexpr String to() const
            noexcept(noexcept(String{s_, promise::null_terminated}))
        {
            return String{s_, promise::null_terminated};
        }

      private:
        not_null<const char*> s_;
    };

    // ## Deduction guides

    // ### null_term

    template <usize N>
    null_term(const char (&)[N]) -> null_term<const char*>;
}
