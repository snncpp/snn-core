// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/random_access.hh"

#include "snn-core/unittest.hh"
#include <deque> // deque

namespace snn::app
{
    namespace
    {
        bool example()
        {
            std::deque<int> deque;

            deque.push_back(123);
            deque.push_back(456);

            range::random_access rng{meta::iterators, deque.begin(), deque.end()};

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.count() == 2);

            snn_require(rng.front(promise::not_empty) == 123);
            snn_require(rng.front().value() == 123);

            snn_require(rng.back(promise::not_empty) == 456);
            snn_require(rng.back().value() == 456);

            snn_require(rng.at(0, promise::within_bounds) == 123);
            snn_require(rng.at(0).value() == 123);

            snn_require(rng.at(1, promise::within_bounds) == 456);
            snn_require(rng.at(1).value() == 456);

            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.count() == 1);

            snn_require(rng.front(promise::not_empty) == 456);
            snn_require(rng.front().value() == 456);

            snn_require(rng.back(promise::not_empty) == 456);
            snn_require(rng.back().value() == 456);

            snn_require(rng.at(0, promise::within_bounds) == 456);
            snn_require(rng.at(0).value() == 456);

            rng.drop_back(promise::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());
            snn_require(rng.count() == 0);

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        // constexpr explicit random_access(RandomAccessIt first, RandomAccessIt last) noexcept
        // constexpr RandomAccessIt begin() const
        // constexpr RandomAccessIt end() const
        {
            std::deque<int> deque;

            range::random_access rng{meta::iterators, deque.begin(), deque.end()};
            snn_require(deque.begin() == rng.begin());
            snn_require(deque.end() == rng.end());

            // Concepts
            using range_type = decltype(rng);
            static_assert(forward_range<range_type>);
            static_assert(bidirectional_range<range_type>);
            static_assert(random_access_range<range_type>);
            static_assert(!contiguous_range<range_type>);
        }

        // constexpr explicit operator bool() const
        // constexpr bool is_empty() const
        // constexpr void drop_front(promise::not_empty_t)
        // constexpr optional<dereference_type> front()
        // constexpr optional<const_dereference_type> front() const
        // constexpr dereference_type front(promise::not_empty_t)
        // constexpr const_dereference_type front(promise::not_empty_t) const
        {
            std::deque<int> deque;
            deque.push_back(123);
            deque.push_back(456);

            range::random_access rng{meta::iterators, deque.begin(), deque.end()};

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(promise::not_empty) == 123);
            snn_require(rng.front().value() == 123);
            snn_require(std::as_const(rng).front(promise::not_empty) == 123);
            snn_require(std::as_const(rng).front().value() == 123);

            rng.drop_front(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(promise::not_empty) == 456);

            rng.drop_front(promise::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            snn_require(!rng.front().has_value());
            snn_require(!std::as_const(rng).front().has_value());
        }

        // constexpr optional<dereference_type> back()
        // constexpr optional<const_dereference_type> back() const
        // constexpr dereference_type back(promise::not_empty_t)
        // constexpr const_dereference_type back(promise::not_empty_t) const
        // constexpr void drop_back(promise::not_empty_t)
        {
            std::deque<int> deque;
            deque.push_back(123);
            deque.push_back(456);

            range::random_access rng{meta::iterators, deque.begin(), deque.end()};

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.back(promise::not_empty) == 456);
            snn_require(rng.back().value() == 456);
            snn_require(std::as_const(rng).back(promise::not_empty) == 456);
            snn_require(std::as_const(rng).back().value() == 456);

            rng.drop_back(promise::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.back(promise::not_empty) == 123);

            rng.drop_back(promise::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            snn_require(!rng.back().has_value());
            snn_require(!std::as_const(rng).back().has_value());
        }

        // constexpr optional<dereference_type> at(const usize pos)
        // constexpr optional<const_dereference_type> at(const usize pos) const
        // constexpr dereference_type at(const usize pos, promise::within_bounds_t)
        // constexpr const_dereference_type at(const usize pos, promise::within_bounds_t) const
        // constexpr usize count() const
        {
            std::deque<int> deque;
            deque.push_back(123);
            deque.push_back(456);

            range::random_access rng{meta::iterators, deque.begin(), deque.end()};

            snn_require(rng.count() == 2);

            snn_require(rng.at(0, promise::within_bounds) == 123);
            snn_require(rng.at(0).value() == 123);
            snn_require(std::as_const(rng).at(0, promise::within_bounds) == 123);
            snn_require(std::as_const(rng).at(0).value() == 123);

            snn_require(rng.at(1, promise::within_bounds) == 456);
            snn_require(rng.at(1).value() == 456);
            snn_require(std::as_const(rng).at(1, promise::within_bounds) == 456);
            snn_require(std::as_const(rng).at(1).value() == 456);

            rng.drop_front(promise::not_empty);

            snn_require(rng.count() == 1);

            rng.drop_front(promise::not_empty);

            snn_require(rng.count() == 0);

            snn_require(!rng);
            snn_require(rng.is_empty());

            snn_require(!rng.at(0).has_value());
            snn_require(!std::as_const(rng).at(0).has_value());
        }
    }
}
