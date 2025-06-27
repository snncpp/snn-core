// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/unchecked/forward.hh"

#include "snn-core/array.hh"
#include "snn-core/strcore.hh"
#include "snn-core/unittest.hh"
#include <unordered_map> // unordered_map

namespace snn
{
    void unittest()
    {
        // Concepts
        static_assert(forward_range<range::unchecked::forward<int*>>);
        static_assert(!bidirectional_range<range::unchecked::forward<int*>>);
        static_assert(!random_access_range<range::unchecked::forward<int*>>);
        static_assert(!contiguous_range<range::unchecked::forward<int*>>);
        static_assert(sizeof(range::unchecked::forward<int*>) == (sizeof(int*) * 2));

        {
            array<int, 4> arr{99, 23, 3, 8};
            range::unchecked::forward rng{init::from, arr.begin(), arr.end()};

            snn_require(rng);

            snn_require(rng.front(assume::not_empty) == 99);

            rng.drop_front(assume::not_empty);

            snn_require(rng);

            snn_require(rng.front(assume::not_empty) == 23);

            while (rng)
            {
                auto& i = rng.front(assume::not_empty);
                i       = i * 2;
                rng.drop_front(assume::not_empty);
            }

            snn_require(arr.at(0).value() == 99);
            snn_require(arr.at(1).value() == 46);
            snn_require(arr.at(2).value() == 6);
            snn_require(arr.at(3).value() == 16);
        }

        {
            std::unordered_map<str, int> m;

            m.try_emplace("One", 1);
            m.try_emplace("Two", 22);

            range::unchecked::forward rng{init::from, m.begin(), m.end()};

            while (rng)
            {
                auto& p = rng.front(assume::not_empty);
                if (p.first == "One")
                {
                    p.second = 11;
                }
                else if (p.first == "Two")
                {
                    p.second = 2222;
                }
                rng.drop_front(assume::not_empty);
            }

            rng = range::unchecked::forward{init::from, m.begin(), m.end()};
            str buf;
            while (rng)
            {
                auto& p = rng.front(assume::not_empty);
                buf << p.first << ':' << as_num(p.second) << '\n';
                rng.drop_front(assume::not_empty);
            }
            snn_require(buf.size() == 16);
            snn_require(buf.contains("One:11\n"));
            snn_require(buf.contains("Two:2222\n"));
        }
    }
}
