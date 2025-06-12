// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Facade (`std::` container wrapper)

// Wrapper around `std::set` or `std::unordered_set`, see [set/sorted.hh](sorted.hh) and
// [set/unsorted.hh](unsorted.hh).

#pragma once

#include "snn-core/math/common.hh"
#include "snn-core/range/bidirectional.hh"
#include "snn-core/range/forward.hh"
#include <iterator> // iterator_traits, *_iterator_tag

namespace snn::set
{
    // ## Classes

    // ### facade

    template <typename Set>
    class facade final
    {
      public:
        // #### Types

        using key_type   = typename Set::key_type;
        using value_type = typename Set::value_type;

        using iterator       = typename Set::iterator;
        using const_iterator = typename Set::const_iterator;

      private:
        // #### Classes

        class insert_result final
        {
          public:
            using pair_type = std::pair<facade::iterator, bool>;

            explicit insert_result(pair_type&& p)
                : p_{std::move(p)}
            {
            }

            explicit operator bool() const noexcept
            {
                return was_inserted();
            }

            [[nodiscard]] auto iterator() const
            {
                return p_.first;
            }

            [[nodiscard]] const auto& key() const noexcept
            {
                return *p_.first;
            }

            [[nodiscard]] bool was_inserted() const noexcept
            {
                return p_.second;
            }

          private:
            pair_type p_;
        };

      public:
        // #### Default constructor

        facade() = default;

        // #### Explicit constructors

        // Generic container interface.

        explicit facade(container::reserve_t, const usize capacity)
        {
            reserve_if_supported_(capacity);
        }

        // #### Converting constructors

        facade(init_list<value_type> values)
            : set_{values}
        {
        }

        // #### Explicit conversion operators

        explicit operator bool() const noexcept
        {
            return !is_empty();
        }

        // #### Iterators

        [[nodiscard]] decltype(auto) begin() noexcept
        {
            return set_.begin();
        }

        [[nodiscard]] decltype(auto) begin() const noexcept
        {
            return set_.cbegin();
        }

        [[nodiscard]] decltype(auto) cbegin() const noexcept
        {
            return set_.cbegin();
        }

        [[nodiscard]] decltype(auto) end() noexcept
        {
            return set_.end();
        }

        [[nodiscard]] decltype(auto) end() const noexcept
        {
            return set_.cend();
        }

        [[nodiscard]] decltype(auto) cend() const noexcept
        {
            return set_.cend();
        }

        // #### Count

        [[nodiscard]] usize count() const noexcept
        {
            return set_.size();
        }

        [[nodiscard]] bool is_empty() const noexcept
        {
            return set_.empty();
        }

        // #### Capacity

        // Generic container interface.

        void reserve(const usize capacity)
        {
            reserve_if_supported_(capacity);
        }

        void reserve_append(const usize capacity_increase)
        {
            reserve_if_supported_(math::add_with_saturation(set_.size(), capacity_increase));
        }

        // #### Insert

        template <typename K>
        insert_result insert(K&& key)
        {
            return insert_result{set_.emplace(std::forward<K>(key))};
        }

        template <typename... Args>
        insert_result insert_inplace(Args&&... args)
        {
            return insert_result{set_.emplace(std::forward<Args>(args)...)};
        }

        // #### Append

        // Generic container interface.

        template <typename... Args>
        void append_inplace(Args&&... args)
        {
            set_.emplace(std::forward<Args>(args)...);
        }

        // #### Lookup

        template <typename K>
        [[nodiscard]] bool contains(const K& key) const
        {
            return set_.find(key) != set_.cend();
        }

        // #### Modifiers

        void clear() noexcept
        {
            set_.clear();
        }

        template <typename K>
        bool remove(const K& key)
        {
            // Don't use `erase(key)`, it will always convert to `key_type`, `find(key)` will not
            // under optimal conditions.
            const auto it = set_.find(key);
            if (it != set_.end())
            {
                set_.erase(it);
                return true;
            }
            return false;
        }

        void swap(facade& other) noexcept
        {
            if (this != &other)
            {
                set_.swap(other.set_);
            }
        }

        // #### Range

        [[nodiscard]] auto range() const noexcept
        {
            return range_(typename std::iterator_traits<const_iterator>::iterator_category{});
        }

      private:
        Set set_{};

        auto range_(std::forward_iterator_tag) const noexcept
        {
            return range::forward{meta::iterators, set_.cbegin(), set_.cend()};
        }

        auto range_(std::bidirectional_iterator_tag) const noexcept
        {
            return range::bidirectional{meta::iterators, set_.cbegin(), set_.cend()};
        }

        void reserve_if_supported_(const usize capacity)
        {
            if constexpr (requires { set_.reserve(usize{}); })
            {
                set_.reserve(capacity);
            }
        }
    };

    // ## Functions

    // ### swap

    template <typename Set>
    void swap(facade<Set>& a, facade<Set>& b) noexcept
    {
        a.swap(b);
    }
}
