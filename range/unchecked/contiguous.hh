// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Unchecked contiguous range

#pragma once

#include "snn-core/core.hh"

namespace snn::range::unchecked
{
    // ## Classes

    // ### contiguous

    template <pointer ContiguousIt>
    class contiguous final
    {
      private:
        ContiguousIt first_;
        ContiguousIt last_;

      public:
        // #### Types

        using iterator       = ContiguousIt;
        using const_iterator = deep_const_t<iterator>;

        using pointer       = iterator;
        using const_pointer = const_iterator;

        using element_type = std::remove_pointer_t<pointer>;
        using value_type   = std::remove_cv_t<element_type>;

        using reference       = element_type&;
        using const_reference = const element_type&;

        // #### Constructors

        constexpr explicit contiguous(init::from_t, const iterator first,
                                      const iterator last) noexcept
            : first_{first},
              last_{last}
        {
            snn_should(first_ <= last_);
        }

        // #### Status

        constexpr explicit operator bool() const noexcept
        {
            return first_ != last_;
        }

        [[nodiscard]] constexpr bool is_empty() const noexcept
        {
            return first_ == last_;
        }

        // #### Iterators

        [[nodiscard]] constexpr iterator begin() noexcept
        {
            return first_;
        }

        [[nodiscard]] constexpr iterator end() noexcept
        {
            return last_;
        }

        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return first_;
        }

        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return last_;
        }

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

        // #### Forward

        constexpr void drop_front(promise::not_empty_t) noexcept
        {
            snn_should(first_ != last_);
            ++first_;
        }

        [[nodiscard]] constexpr reference front(promise::not_empty_t) noexcept
        {
            snn_should(first_ != last_);
            return *first_;
        }

        [[nodiscard]] constexpr const_reference front(promise::not_empty_t) const noexcept
        {
            snn_should(first_ != last_);
            return *first_;
        }

        // #### Bidirectional

        [[nodiscard]] constexpr reference back(promise::not_empty_t) noexcept
        {
            snn_should(first_ != last_);
            return *(last_ - 1);
        }

        [[nodiscard]] constexpr const_reference back(promise::not_empty_t) const noexcept
        {
            snn_should(first_ != last_);
            return *(last_ - 1);
        }

        constexpr void drop_back(promise::not_empty_t) noexcept
        {
            snn_should(first_ != last_);
            --last_;
        }

        // #### Random access

        [[nodiscard]] constexpr reference at(const usize pos, promise::within_bounds_t) noexcept
        {
            snn_should(pos < count());
            return *(first_ + pos);
        }

        [[nodiscard]] constexpr const_reference at(const usize pos,
                                                   promise::within_bounds_t) const noexcept
        {
            snn_should(pos < count());
            return *(first_ + pos);
        }

        [[nodiscard]] constexpr usize count() const noexcept
        {
            snn_should(first_ <= last_);
            return static_cast<usize>(last_ - first_);
        }

        // #### Contiguous

        [[nodiscard]] constexpr reference pop_back(promise::not_empty_t) noexcept
        {
            snn_should(first_ != last_);
            return *(--last_);
        }

        constexpr contiguous pop_back_n(const usize count) noexcept
        {
            const auto last = last_;
            if (this->count() >= count)
            {
                last_ -= count;
            }
            else
            {
                last_ = first_;
            }
            return contiguous{init::from, last_, last};
        }

        [[nodiscard]] constexpr reference pop_front(promise::not_empty_t) noexcept
        {
            snn_should(first_ != last_);
            return *(first_++);
        }

        constexpr contiguous pop_front_n(const usize count) noexcept
        {
            const auto first = first_;
            if (this->count() >= count)
            {
                first_ += count;
            }
            else
            {
                first_ = last_;
            }
            return contiguous{init::from, first, first_};
        }

        SNN_DIAGNOSTIC_POP
    };
}
