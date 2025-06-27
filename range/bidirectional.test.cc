// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/bidirectional.hh"

#include "snn-core/unittest.hh"
#include <set> // set

namespace snn::app
{
    namespace
    {
        bool example()
        {
            std::set<int> set;

            set.insert(456);
            set.insert(123);

            range::bidirectional rng{init::from, set.begin(), set.end()};

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(assume::not_empty) == 123);
            snn_require(rng.front().value() == 123);

            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(assume::not_empty) == 456);

            rng.drop_front(assume::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        // constexpr explicit bidirectional(BidirectionalIt first, BidirectionalIt last) noexcept
        // constexpr BidirectionalIt begin() const
        // constexpr BidirectionalIt end() const
        {
            std::set<int> set;

            range::bidirectional rng{init::from, set.begin(), set.end()};
            snn_require(set.begin() == rng.begin());
            snn_require(set.end() == rng.end());

            // Concepts
            using range_type = decltype(rng);
            static_assert(forward_range<range_type>);
            static_assert(bidirectional_range<range_type>);
            static_assert(!random_access_range<range_type>);
            static_assert(!contiguous_range<range_type>);
        }

        // constexpr explicit operator bool() const
        // constexpr bool is_empty() const
        // constexpr void drop_front(assume::not_empty_t)
        // constexpr optional<dereference_type> front()
        // constexpr optional<const_dereference_type> front() const
        // constexpr dereference_type front(assume::not_empty_t)
        // constexpr const_dereference_type front(assume::not_empty_t) const
        {
            std::set<int> set;
            set.insert(456);
            set.insert(123);

            range::bidirectional rng{init::from, set.begin(), set.end()};

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(assume::not_empty) == 123);
            snn_require(rng.front().value() == 123);
            snn_require(std::as_const(rng).front(assume::not_empty) == 123);
            snn_require(std::as_const(rng).front().value() == 123);

            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.front(assume::not_empty) == 456);

            rng.drop_front(assume::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            snn_require(!rng.front().has_value());
            snn_require(!std::as_const(rng).front().has_value());
        }

        // constexpr optional<dereference_type> back()
        // constexpr optional<const_dereference_type> back() const
        // constexpr dereference_type back(assume::not_empty_t)
        // constexpr const_dereference_type back(assume::not_empty_t) const
        // constexpr void drop_back(assume::not_empty_t)
        {
            std::set<int> set;
            set.insert(456);
            set.insert(123);

            range::bidirectional rng{init::from, set.begin(), set.end()};

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.back(assume::not_empty) == 456);
            snn_require(rng.back().value() == 456);
            snn_require(std::as_const(rng).back(assume::not_empty) == 456);
            snn_require(std::as_const(rng).back().value() == 456);

            rng.drop_back(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());

            snn_require(rng.back(assume::not_empty) == 123);

            rng.drop_back(assume::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            snn_require(!rng.back().has_value());
            snn_require(!std::as_const(rng).back().has_value());
        }
    }
}
