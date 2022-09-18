// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Match range

// **Important**: The `matches_view` returned by `front()` is invalidated by `drop_front()` (or the
// destruction of the range).

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/range/iter/forward.hh"
#include "snn-core/regex/matches_view.hh"
#include "snn-core/regex/pattern.hh"

namespace snn::regex::range
{
    // ## Classes

    // ### match

    class match final
    {
      public:
        explicit match(const cstrview subject, const pattern& pat)
            : first_{subject.cbegin(), subject.cend(), pat.internal()},
              last_{}
        {
        }

        match(cstrview, const pattern&&) = delete;

        // Non-copyable (inefficient).
        match(const match&)            = delete;
        match& operator=(const match&) = delete;

        // Movable
        match(match&&)            = default;
        match& operator=(match&&) = default;

        explicit operator bool() const
        {
            return !is_empty();
        }

        [[nodiscard]] auto begin() noexcept
        {
            return snn::range::iter::forward_reference{*this};
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return snn::range::iter::forward_end{};
        }

        void drop_front(promise::not_empty_t)
        {
            // A `snn_assert` here can not be not be optimized away by Clang 13.0.0.
            snn_should(!is_empty());
            ++first_;
        }

        [[nodiscard]] matches_view front(promise::not_empty_t) const
        {
            // A `snn_assert` here can not be not be optimized away by Clang 13.0.0.
            snn_should(!is_empty());
            return matches_view{*first_};
        }

        [[nodiscard]] bool is_empty() const
        {
            return first_ == last_;
        }

      private:
        std::cregex_iterator first_;
        std::cregex_iterator last_;
    };
}
