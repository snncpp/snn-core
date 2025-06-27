// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/forward.hh"

#include "snn-core/unittest.hh"
#include <forward_list> // forward_list

namespace snn::app
{
    namespace
    {
        bool example()
        {
            std::forward_list<int> list;

            list.push_front(456);
            list.push_front(123);

            range::forward rng{init::from, list.begin(), list.end()};

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

        // constexpr explicit forward(ForwardIt first, ForwardIt last) noexcept
        // constexpr ForwardIt begin() const
        // constexpr ForwardIt end() const
        {
            std::forward_list<int> list;

            range::forward rng{init::from, list.begin(), list.end()};
            snn_require(list.begin() == rng.begin());
            snn_require(list.end() == rng.end());

            // Concepts
            using range_type = decltype(rng);
            static_assert(forward_range<range_type>);
            static_assert(!bidirectional_range<range_type>);
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
            std::forward_list<int> list;
            list.push_front(456);
            list.push_front(123);

            range::forward rng{init::from, list.begin(), list.end()};

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
    }
}
