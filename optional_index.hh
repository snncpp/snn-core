// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Optional index

#pragma once

#include "snn-core/exception.hh"
#include "snn-core/generic/error.hh"

namespace snn
{
    // ## Classes

    // ### optional_index

    class optional_index final
    {
      public:
        constexpr explicit optional_index(const usize index, assume::within_bounds_t) noexcept
            : index_{index}
        {
            // Let the optimizer know that we're not holding a `constant::npos`.
            snn_assume(index_ != constant::npos);
        }

        constexpr optional_index(const usize index) noexcept
            : index_{index}
        {
        }

        // Optional assignment is error-prone and therefore disabled.
        // The implicit copy/move assignment operator can still be used.
        template <typename U>
        void operator=(const U&) = delete;

        constexpr explicit operator bool() const noexcept
        {
            return has_value();
        }

        [[nodiscard]] constexpr bool has_value() const noexcept
        {
            return index_ != constant::npos;
        }

        [[nodiscard]] constexpr usize value() const
        {
            if (has_value())
            {
                return index_;
            }
            throw_or_abort(generic::error::no_value);
        }

        [[nodiscard]] constexpr usize value(assume::has_value_t) const noexcept
        {
            snn_assert(has_value());
            return index_;
        }

        [[nodiscard]] constexpr usize value_or(const usize alt) const noexcept
        {
            if (has_value())
            {
                return index_;
            }
            return alt;
        }

        [[nodiscard]] constexpr usize value_or_npos() const noexcept
        {
            return index_;
        }

      private:
        usize index_;
    };
}
