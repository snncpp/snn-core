// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Step range

// Forward range that steps from an initial value to a last value (exclusive) by a custom amount.

// `front()` returns a **copy** of the front element.

// `fn::less_than` is used for comparison.

#pragma once

#include "snn-core/fn/common.hh"
#include "snn-core/range/iter/forward.hh"

namespace snn::range
{
    // ## Classes

    // ### step

    template <typename T, typename By = T>
    class step final
    {
      public:
        // #### Constructors

        constexpr explicit step(T from, T to_excl, By by) noexcept
            : current_{std::move(from)},
              last_{std::move(to_excl)},
              by_{std::move(by)}
        {
            // Incrementing?
            snn_should(is_empty() ||
                       (fn::less_than{}(current_, current_ + by_) && "Infinite loop?"));
        }

        constexpr explicit step(const T from, const T to_excl) noexcept
            requires(snn::integral<T>)
            : current_{from},
              last_{to_excl},
              by_{1}
        {
        }

        // #### Explicit conversion operators

        constexpr explicit operator bool() const
        {
            return !is_empty();
        }

        // #### Iterators

        [[nodiscard]] constexpr auto begin() const
        {
            if constexpr (std::is_copy_constructible_v<step>)
            {
                return iter::forward<step>{*this};
            }
            else
            {
                return iter::forward_reference<step>{*this};
            }
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return iter::forward_end{};
        }

        // #### Single element access

        [[nodiscard]] constexpr T front(promise::not_empty_t) const
        {
            snn_should(!is_empty());
            return current_;
        }

        // #### Operations

        constexpr void drop_front(promise::not_empty_t)
        {
            snn_should(!is_empty());
            current_ += by_;
        }

        // #### Status

        [[nodiscard]] constexpr bool is_empty() const
        {
            return !fn::less_than{}(current_, last_);
        }

      private:
        T current_;
        T last_;
        By by_;
    };
}
