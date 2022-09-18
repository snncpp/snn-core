// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Code points

#pragma once

#include "snn-core/optional.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/range/iter/forward.hh"

namespace snn::utf8::range
{
    // ## Classes

    // ### codepoint

    class codepoint final
    {
      public:
        constexpr explicit codepoint(const cstrview s) noexcept
            : rng_{s.range()}
        {
            next_();
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
            snn_should(!is_empty());
            next_();
        }

        [[nodiscard]] constexpr pair::value_count<u32> front(promise::not_empty_t) const noexcept
        {
            snn_should(!is_empty());
            return current_;
        }

        [[nodiscard]] constexpr optional<pair::value_count<u32>> front() const noexcept
        {
            if (!is_empty())
            {
                return front(promise::not_empty);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return current_.count == 0;
        }

      private:
        cstrrng rng_;
        pair::value_count<u32> current_{};

        constexpr void next_() noexcept
        {
            current_ = rng_.pop_front_codepoint();
        }
    };
}
