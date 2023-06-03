// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Unchecked forward range

#pragma once

#include "snn-core/core.hh"

namespace snn::range::unchecked
{
    // ## Classes

    // ### forward

    template <typename ForwardIt>
    class forward final
    {
      private:
        ForwardIt first_;
        ForwardIt last_;

        using dereference_type       = decltype(*first_);
        using const_dereference_type = deep_const_t<dereference_type>;

      public:
        // #### Types

        using iterator       = ForwardIt;
        using const_iterator = deep_const_t<iterator>;

        // #### Constructors

        constexpr explicit forward(meta::iterators_t, iterator first, iterator last) noexcept
            : first_{std::move(first)},
              last_{std::move(last)}
        {
        }

        // #### Status

        constexpr explicit operator bool() const
        {
            return first_ != last_;
        }

        [[nodiscard]] constexpr bool is_empty() const
        {
            return first_ == last_;
        }

        // #### Iterators

        [[nodiscard]] constexpr iterator begin()
        {
            return first_;
        }

        [[nodiscard]] constexpr iterator end()
        {
            return last_;
        }

        [[nodiscard]] constexpr const_iterator begin() const
        {
            return first_;
        }

        [[nodiscard]] constexpr const_iterator end() const
        {
            return last_;
        }

        // #### Forward

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        constexpr void drop_front(promise::not_empty_t)
        {
            snn_should(first_ != last_);
            ++first_;
        }

        [[nodiscard]] constexpr dereference_type front(promise::not_empty_t)
        {
            snn_should(first_ != last_);
            return *first_;
        }

        [[nodiscard]] constexpr const_dereference_type front(promise::not_empty_t) const
        {
            snn_should(first_ != last_);
            return *first_;
        }

        SNN_DIAGNOSTIC_POP
    };
}
