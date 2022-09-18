// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Pattern

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/exception.hh"
#include "snn-core/pcre/core.hh"
#include "snn-core/pcre/error.hh"
#include "snn-core/pcre/matches.hh"

namespace snn::pcre
{
    // ## Constants

    // ### Pattern options

    //                   Constant         PCRE2 constant           Modifier
    inline constexpr u32 dollar_endonly = PCRE2_DOLLAR_ENDONLY; // D
    inline constexpr u32 dotall         = PCRE2_DOTALL;         // s
    inline constexpr u32 icase          = PCRE2_CASELESS;       // i
    inline constexpr u32 multiline      = PCRE2_MULTILINE;      // m
    inline constexpr u32 ucp            = PCRE2_UCP;            // p
    inline constexpr u32 ungreedy       = PCRE2_UNGREEDY;       // U
    inline constexpr u32 utf            = PCRE2_UTF;            // u

    // ## Classes

    // ### pattern

    class pattern final
    {
      public:
        // #### Types

        using trivially_relocatable_type = pattern;

        // #### Constructors

        explicit pattern() noexcept
            : code_{nullptr},
              error_number_{0},
              error_position_{0}
        {
        }

        explicit pattern(const transient<cstrview> pat, const u32 options) noexcept
            : pattern{}
        {
            usize error_offset                         = 0;
            constexpr pcre2_compile_context_8* context = nullptr;
            const cstrview p                           = pat.get();
            code_ = ::pcre2_compile_8(reinterpret_cast<const u8*>(p.cbegin()), p.size(), options,
                                      &error_number_, &error_offset, context);
            error_position_ = static_cast<u32>(math::min(error_offset, constant::limit<u32>::max));
        }

        explicit pattern(const transient<cstrview> pat, const transient<cstrview> modifiers)
            : pattern{pat, modifiers_to_options_(modifiers.get())}
        {
        }

        explicit pattern(const transient<cstrview> pat) noexcept
            : pattern{pat, u32{}}
        {
        }

        // #### Non-copyable

        pattern(const pattern&)            = delete;
        pattern& operator=(const pattern&) = delete;

        // #### Movable

        pattern(pattern&& other) noexcept
            : code_{std::exchange(other.code_, nullptr)},
              error_number_{std::exchange(other.error_number_, 0)},
              error_position_{std::exchange(other.error_position_, 0)}
        {
        }

        pattern& operator=(pattern&& other) noexcept
        {
            std::swap(code_, other.code_);
            std::swap(error_number_, other.error_number_);
            std::swap(error_position_, other.error_position_);
            return *this;
        }

        // #### Destructor

        ~pattern()
        {
            ::pcre2_code_free_8(code_); // Does nothing if nullptr.
        }

        // #### Explicit conversion operators

        explicit operator bool() const noexcept
        {
            return is_valid();
        }

        // #### Match

        [[nodiscard]] matches match_first(const cstrview subject)
        {
            if (is_valid())
            {
                matches ma{not_null{code_}, subject};
                if (ma.is_valid())
                {
                    const not_null<pcre2_match_data_8*> match_data = ma.data(promise::is_valid);

                    constexpr usize start_offset             = 0;
                    constexpr u32 options                    = 0;
                    constexpr pcre2_match_context_8* context = nullptr;

                    const int ret =
                        ::pcre2_match_8(code_, reinterpret_cast<const u8*>(subject.data().get()),
                                        subject.size(), start_offset, options, match_data.get(),
                                        context);

                    if (ret > 0)
                    {
                        ma.set_count(to_usize(ret));
                    }
                    else if (ret != PCRE2_ERROR_NOMATCH)
                    {
                        if (ret != 0)
                        {
                            ma.set_error_number(ret);
                        }
                        else
                        {
                            // Should never happen (match_data is sized for the pattern).
                            throw_or_abort(error::undersized_storage);
                        }
                    }

                    return ma;
                }
                throw_or_abort(error::out_of_memory);
            }
            throw_or_abort(error::invalid_pattern);
        }

        // #### Status

        [[nodiscard]] bool is_valid() const noexcept
        {
            return code_ != nullptr;
        }

        [[nodiscard]] int error_number() const noexcept
        {
            return error_number_;
        }

        [[nodiscard]] u32 error_position() const noexcept
        {
            return error_position_;
        }

      private:
        pcre2_code_8* code_;
        int error_number_;
        u32 error_position_;

        static constexpr u32 modifiers_to_options_(const cstrview modifiers)
        {
            u32 options = 0;
            for (const char c : modifiers)
            {
                switch (c)
                {
                    case 'D':
                        options |= PCRE2_DOLLAR_ENDONLY;
                        break;

                    case 'U':
                        options |= PCRE2_UNGREEDY;
                        break;

                    case 'i':
                        options |= PCRE2_CASELESS;
                        break;

                    case 'm':
                        options |= PCRE2_MULTILINE;
                        break;

                    case 'p':
                        options |= PCRE2_UCP;
                        break;

                    case 's':
                        options |= PCRE2_DOTALL;
                        break;

                    case 'u':
                        options |= PCRE2_UTF;
                        break;

                    default:
                        throw_or_abort(error::invalid_pattern_modifier);
                }
            }
            return options;
        }
    };
}
