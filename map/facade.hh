// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Facade (`std::` container wrapper)

// Wrapper around `std::map` or `std::unordered_map`, see [map/sorted.hh](sorted.hh) and
// [map/unsorted.hh](unsorted.hh).

#pragma once

#include "snn-core/optional.hh"
#include "snn-core/math/common.hh"
#include "snn-core/range/bidirectional.hh"
#include "snn-core/range/forward.hh"

namespace snn::map
{
    // ## Classes

    // ### facade

    template <typename Map>
    class facade final
    {
      public:
        // #### Types

        using key_type    = typename Map::key_type;
        using mapped_type = typename Map::mapped_type;
        using value_type  = typename Map::value_type;

        using iterator       = typename Map::iterator;
        using const_iterator = typename Map::const_iterator;

        using mapped_reference       = mapped_type&;
        using const_mapped_reference = const mapped_type&;

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
                return p_.first->first;
            }

            [[nodiscard]] auto& value() noexcept
            {
                return p_.first->second;
            }

            [[nodiscard]] const auto& value() const noexcept
            {
                return p_.first->second;
            }

            [[nodiscard]] bool was_inserted() const noexcept
            {
                return p_.second;
            }

          private:
            pair_type p_;
        };

        class insert_or_assign_result final
        {
          public:
            using pair_type = std::pair<facade::iterator, bool>;

            explicit insert_or_assign_result(pair_type&& p)
                : p_{std::move(p)}
            {
            }

            [[nodiscard]] auto iterator() const
            {
                return p_.first;
            }

            [[nodiscard]] const auto& key() const noexcept
            {
                return p_.first->first;
            }

            [[nodiscard]] auto& value() noexcept
            {
                return p_.first->second;
            }

            [[nodiscard]] const auto& value() const noexcept
            {
                return p_.first->second;
            }

            [[nodiscard]] bool was_inserted() const noexcept
            {
                return p_.second;
            }

            [[nodiscard]] bool was_assigned() const noexcept
            {
                return !was_inserted();
            }

          private:
            pair_type p_;
        };

      public:
        // #### Default constructor

        facade()
        {
        }

        // #### Explicit constructors

        // Generic container interface.

        explicit facade(container::reserve_t, const usize capacity)
        {
            reserve_if_supported_(capacity);
        }

        // #### Converting constructors

        facade(init_list<value_type> init)
            : map_{init}
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
            return map_.begin();
        }

        [[nodiscard]] decltype(auto) begin() const noexcept
        {
            return map_.cbegin();
        }

        [[nodiscard]] decltype(auto) cbegin() const noexcept
        {
            return map_.cbegin();
        }

        [[nodiscard]] decltype(auto) end() noexcept
        {
            return map_.end();
        }

        [[nodiscard]] decltype(auto) end() const noexcept
        {
            return map_.cend();
        }

        [[nodiscard]] decltype(auto) cend() const noexcept
        {
            return map_.cend();
        }

        // #### Count

        [[nodiscard]] usize count() const noexcept
        {
            return map_.size();
        }

        [[nodiscard]] bool is_empty() const noexcept
        {
            return map_.empty();
        }

        // #### Capacity

        // Generic container interface.

        void reserve(const usize capacity)
        {
            reserve_if_supported_(capacity);
        }

        void reserve_append(const usize capacity_increase)
        {
            reserve_if_supported_(math::add_with_saturation(map_.size(), capacity_increase));
        }

        // #### Insert

        // Insert if key doesn't exist or do nothing.
        template <typename K, typename V>
        insert_result insert(K&& key, V&& value)
        {
            return insert_result{map_.try_emplace(std::forward<K>(key), std::forward<V>(value))};
        }

        // Insert if key doesn't exist or do nothing.
        template <typename K, typename... Args>
        insert_result insert_inplace(K&& key, Args&&... args)
        {
            return insert_result{
                map_.try_emplace(std::forward<K>(key), std::forward<Args>(args)...)};
        }

        // Insert or assign value if key exists.
        template <typename K, typename V>
        insert_or_assign_result insert_or_assign(K&& key, V&& value)
        {
            return insert_or_assign_result{
                map_.insert_or_assign(std::forward<K>(key), std::forward<V>(value))};
        }

        // #### Append

        // Generic container interface.

        // Insert if key doesn't exist or do nothing.
        template <typename K, typename... Args>
        void append_inplace(K&& key, Args&&... args)
        {
            map_.try_emplace(std::forward<K>(key), std::forward<Args>(args)...);
        }

        // #### Lookup

        template <value_type_or<mapped_reference> R = mapped_reference, typename K>
        [[nodiscard]] optional<R> get(const K& key)
        {
            const auto it = map_.find(key);
            if (it == map_.cend())
            {
                return nullopt;
            }
            return it->second;
        }

        template <value_type_or<const_mapped_reference> R = const_mapped_reference, typename K>
        [[nodiscard]] optional<R> get(const K& key) const
        {
            const auto it = map_.find(key);
            if (it == map_.cend())
            {
                return nullopt;
            }
            return it->second;
        }

        template <typename K>
        [[nodiscard]] bool contains(const K& key) const
        {
            return map_.find(key) != map_.cend();
        }

        // #### Operations

        void clear() noexcept
        {
            map_.clear();
        }

        template <typename K>
        bool remove(const K& key)
        {
            // Don't use `erase(key)`, it will always convert to `key_type`, `find(key)` will not
            // under optimal conditions.
            const auto it = map_.find(key);
            if (it != map_.end())
            {
                map_.erase(it);
                return true;
            }
            return false;
        }

        template <typename OneArgPred>
        usize remove_if(OneArgPred p)
        {
            const usize size_before = map_.size();

            auto cur        = map_.begin();
            const auto last = map_.end();
            while (cur != last)
            {
                if (p(*cur))
                {
                    cur = map_.erase(cur);
                }
                else
                {
                    ++cur;
                }
            }

            return size_before - map_.size();
        }

        void swap(facade& other) noexcept
        {
            if (this != &other)
            {
                map_.swap(other.map_);
            }
        }

        // #### Range

        [[nodiscard]] auto range() noexcept
        {
            return range_(typename std::iterator_traits<iterator>::iterator_category{});
        }

        [[nodiscard]] auto range() const noexcept
        {
            return range_(typename std::iterator_traits<const_iterator>::iterator_category{});
        }

      private:
        Map map_{};

        auto range_(std::forward_iterator_tag) noexcept
        {
            return range::forward{meta::iterators, map_.begin(), map_.end()};
        }

        auto range_(std::forward_iterator_tag) const noexcept
        {
            return range::forward{meta::iterators, map_.cbegin(), map_.cend()};
        }

        auto range_(std::bidirectional_iterator_tag) noexcept
        {
            return range::bidirectional{meta::iterators, map_.begin(), map_.end()};
        }

        auto range_(std::bidirectional_iterator_tag) const noexcept
        {
            return range::bidirectional{meta::iterators, map_.cbegin(), map_.cend()};
        }

        void reserve_if_supported_(const usize capacity)
        {
            if constexpr (requires { map_.reserve(usize{}); })
            {
                map_.reserve(capacity);
            }
        }
    };

    // ## Functions

    // ### swap

    template <typename Map>
    void swap(facade<Map>& a, facade<Map>& b) noexcept
    {
        a.swap(b);
    }
}
