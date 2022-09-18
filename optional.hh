// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Optional (`result` without error code)

#pragma once

#include "snn-core/result.hh"
#include "snn-core/generic/error.hh"

namespace snn
{
    // ## Constants

    // ### nullopt

    inline constexpr auto nullopt = make_error_code(generic::error::no_value);

    // ## Classes

    // ### optional_error_storage

    class optional_error_storage final
    {
      public:
        constexpr explicit optional_error_storage() noexcept
            : has_error_{false}
        {
        }

        constexpr explicit optional_error_storage(error_code) noexcept
            : has_error_{true}
        {
        }

        constexpr explicit operator bool() const noexcept
        {
            return has_error_;
        }

        constexpr operator error_code() const noexcept
        {
            snn_should(has_error_);
            return nullopt;
        }

        [[nodiscard]] static constexpr error_code default_error_code() noexcept
        {
            return nullopt;
        }

        [[nodiscard]] static constexpr bool has_boolean_state() noexcept
        {
            return true;
        }

      private:
        bool has_error_;
    };

    // ## Aliases

    // ### optional

    template <typename T>
    using optional = result<T, optional_error_storage>;
}
