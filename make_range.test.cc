// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/make_range.hh"

#include "snn-core/strcore.hh"
#include "snn-core/unittest.hh"
#include <deque>        // deque
#include <forward_list> // forward_list
#include <set>          // set

namespace snn::app
{
    namespace
    {
        bool example()
        {
            // range::forward
            {
                std::forward_list<i32> list;
                auto rng            = make_range(list);
                using iterator_type = decltype(list)::iterator;
                static_assert(std::is_same_v<decltype(rng), range::forward<iterator_type>>);
            }

            // range::bidirectional
            {
                std::set<i32> set;
                auto rng            = make_range(set);
                using iterator_type = decltype(set)::iterator;
                static_assert(std::is_same_v<decltype(rng), range::bidirectional<iterator_type>>);
            }

            // range::random_access
            {
                std::deque<i32> queue;
                auto rng            = make_range(queue);
                using iterator_type = decltype(queue)::iterator;
                static_assert(std::is_same_v<decltype(rng), range::random_access<iterator_type>>);
            }

            // range::contiguous
            {
                const i32 num[] = {7, 6, 42, 9, 11, 8};
                auto rng        = make_range(num);
                static_assert(std::is_same_v<decltype(rng), range::contiguous<const i32*>>);
                snn_require(rng.count() == 6);
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_require(app::example());

        // range::contiguous
        {
            str s;
            auto rng = make_range(s);
            static_assert(std::is_same_v<decltype(rng), range::contiguous<char*>>);
        }
        {
            const str s;
            auto rng = make_range(s);
            static_assert(std::is_same_v<decltype(rng), range::contiguous<const char*>>);
        }
        {
            constexpr auto rng = make_range("");
            static_assert(std::is_same_v<decltype(rng), const range::contiguous<const char*>>);
            static_assert(!rng);
            static_assert(rng.is_empty());
            static_assert(rng.count() == 0);
        }
        {
            constexpr auto rng = make_range("abc");
            static_assert(std::is_same_v<decltype(rng), const range::contiguous<const char*>>);
            static_assert(rng);
            static_assert(!rng.is_empty());
            static_assert(rng.count() == 3);
            static_assert(rng.front(promise::not_empty) == 'a');
            static_assert(rng.at(1, promise::within_bounds) == 'b');
            static_assert(rng.back(promise::not_empty) == 'c');
        }
    }
}
