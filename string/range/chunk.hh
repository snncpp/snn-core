// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Split a string into byte size chunks

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/optional.hh"
#include "snn-core/math/common.hh"
#include "snn-core/range/iter/forward.hh"

namespace snn::string::range
{
    // ## Classes

    // ### chunk

    class chunk final
    {
      public:
        constexpr explicit chunk(const cstrview string, const usize size) noexcept
            : string_{string},
              size_{math::max(size, 1)}
        {
        }

        constexpr explicit operator bool() const noexcept
        {
            return !is_empty();
        }

        [[nodiscard]] constexpr auto begin() const noexcept
        {
            return snn::range::iter::forward{*this};
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return snn::range::iter::forward_end{};
        }

        constexpr void drop_front(promise::not_empty_t) noexcept
        {
            string_.drop_front_n(size_);
        }

        [[nodiscard]] constexpr optional<cstrview> front() const noexcept
        {
            if (!is_empty())
            {
                return front(promise::not_empty);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr cstrview front(promise::not_empty_t) const noexcept
        {
            snn_should(!is_empty());
            return string_.view(0, size_);
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return string_.is_empty();
        }

      private:
        cstrview string_;
        usize size_;
    };
}
