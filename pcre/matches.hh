// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Matches

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/optional.hh"
#include "snn-core/math/common.hh"
#include "snn-core/pcre/core.hh"
#include "snn-core/pcre/error.hh"
#include "snn-core/pcre/match_view.hh"

namespace snn::pcre
{
    // ## Classes

    // ### matches

    class matches final
    {
      public:
        // #### Types

        using trivially_relocatable_type = matches;

        // #### Constructor

        explicit matches(const not_null<pcre2_code_8*> code, const cstrview subject) noexcept
            : match_data_{::pcre2_match_data_create_from_pattern_8(code.get(), nullptr)},
              subject_{subject}
        {
        }

        // #### Non-copyable

        matches(const matches&)            = delete;
        matches& operator=(const matches&) = delete;

        // #### Movable

        matches(matches&& other) noexcept
            : match_data_{std::exchange(other.match_data_, nullptr)},
              subject_{std::exchange(other.subject_, cstrview{})},
              count_{std::exchange(other.count_, 0)},
              error_number_{std::exchange(other.error_number_, 0)}
        {
        }

        matches& operator=(matches&& other) noexcept
        {
            std::swap(match_data_, other.match_data_);
            std::swap(subject_, other.subject_);
            std::swap(count_, other.count_);
            std::swap(error_number_, other.error_number_);
            return *this;
        }

        // #### Destructor

        ~matches()
        {
            ::pcre2_match_data_free_8(match_data_); // Does nothing if nullptr.
        }

        // #### Explicit conversion operators

        explicit operator bool() const noexcept
        {
            return !is_empty();
        }

        // #### Single element access

        [[nodiscard]] optional<match_view> at(const usize pos) const noexcept
        {
            if (pos < count())
            {
                return at(pos, promise::within_bounds);
            }
            return nullopt;
        }

        [[nodiscard]] match_view at(const usize pos, promise::within_bounds_t) const noexcept
        {
            snn_assert(pos < count());
            const usize* const ovector = ::pcre2_get_ovector_pointer_8(match_data_);
            const usize index          = pos * 2;

            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            const usize start_pos = ovector[index];
            const usize end_pos   = ovector[index + 1];

            SNN_DIAGNOSTIC_POP

            // Do not assume that the end position is greater than the start position (pcre2demo.c).
            return match_view{subject_, math::min(start_pos, end_pos),
                              math::abs_diff(start_pos, end_pos)};
        }

        // #### Count

        [[nodiscard]] u32 count() const noexcept
        {
            return count_;
        }

        [[nodiscard]] bool is_empty() const noexcept
        {
            return count_ == 0;
        }

        // #### Status

        [[nodiscard]] int error_number() const noexcept
        {
            return error_number_;
        }

        [[nodiscard]] bool is_valid() const noexcept
        {
            return match_data_ != nullptr;
        }

        // #### Data

        [[nodiscard]] not_null<pcre2_match_data_8*> data(promise::is_valid_t) const noexcept
        {
            snn_assert(is_valid());
            return not_null{match_data_};
        }

        // #### Setters

        void set_count(const u32 c) noexcept
        {
            if (is_valid())
            {
                count_ = math::min(c, ::pcre2_get_ovector_count_8(match_data_));
            }
        }

        void set_error_number(const int n) noexcept
        {
            error_number_ = n;
        }

      private:
        pcre2_match_data_8* match_data_;
        cstrview subject_;
        u32 count_{0};
        int error_number_{0};
    };
}
