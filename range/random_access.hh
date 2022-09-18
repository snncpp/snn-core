// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Random access range

#pragma once

#include "snn-core/optional.hh"

namespace snn::range
{
    // ## Classes

    // ### random_access

    template <typename RandomAccessIt>
    class random_access final
    {
      private:
        RandomAccessIt first_;
        RandomAccessIt last_;

        using dereference_type       = decltype(*first_);
        using const_dereference_type = deep_const_t<dereference_type>;

      public:
        // #### Types

        using iterator       = RandomAccessIt;
        using const_iterator = deep_const_t<iterator>;

        // #### Constructor

        constexpr explicit random_access(meta::iterators_t, iterator first, iterator last) noexcept
            : first_{std::move(first)},
              last_{std::move(last)}
        {
            snn_should(first_ <= last_);
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

        constexpr void drop_front(promise::not_empty_t)
        {
            snn_assert(first_ != last_);
            ++first_;
        }

        [[nodiscard]] constexpr optional<dereference_type> front()
        {
            if (first_ != last_)
            {
                return *first_;
            }
            return nullopt;
        }

        [[nodiscard]] constexpr optional<const_dereference_type> front() const
        {
            if (first_ != last_)
            {
                return *first_;
            }
            return nullopt;
        }

        [[nodiscard]] constexpr dereference_type front(promise::not_empty_t)
        {
            snn_assert(first_ != last_);
            return *first_;
        }

        [[nodiscard]] constexpr const_dereference_type front(promise::not_empty_t) const
        {
            snn_assert(first_ != last_);
            return *first_;
        }

        // #### Bidirectional

        [[nodiscard]] constexpr optional<dereference_type> back()
        {
            if (first_ != last_)
            {
                return *(last_ - 1);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr optional<const_dereference_type> back() const
        {
            if (first_ != last_)
            {
                return *(last_ - 1);
            }
            return nullopt;
        }

        [[nodiscard]] constexpr dereference_type back(promise::not_empty_t)
        {
            snn_assert(first_ != last_);
            return *(last_ - 1);
        }

        [[nodiscard]] constexpr const_dereference_type back(promise::not_empty_t) const
        {
            snn_assert(first_ != last_);
            return *(last_ - 1);
        }

        constexpr void drop_back(promise::not_empty_t)
        {
            snn_assert(first_ != last_);
            --last_;
        }

        // #### Random access

        // Some containers expect a signed difference type for pointer arithmetic, so the position
        // is cast to `isize` before addition. This is safe with a 57-bit (or smaller) virtual
        // address space (57-bit-virtual-address-space).

        [[nodiscard]] constexpr optional<dereference_type> at(const usize pos)
        {
            if (pos < count())
            {
                return *(first_ + to_isize(pos));
            }
            return nullopt;
        }

        [[nodiscard]] constexpr optional<const_dereference_type> at(const usize pos) const
        {
            if (pos < count())
            {
                return *(first_ + to_isize(pos));
            }
            return nullopt;
        }

        [[nodiscard]] constexpr dereference_type at(const usize pos, promise::within_bounds_t)
        {
            snn_assert(pos < count());
            return *(first_ + to_isize(pos));
        }

        [[nodiscard]] constexpr const_dereference_type at(const usize pos,
                                                          promise::within_bounds_t) const
        {
            snn_assert(pos < count());
            return *(first_ + to_isize(pos));
        }

        [[nodiscard]] constexpr usize count() const
        {
            snn_should(first_ <= last_);
            return static_cast<usize>(last_ - first_);
        }
    };
}
