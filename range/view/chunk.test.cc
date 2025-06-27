// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/range/view/chunk.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/count.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/range/integral.hh"
#include "snn-core/range/step.hh"
#include "snn-core/range/view/take.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool example()
        {
            auto rng = range::view::chunk{cstrrng{"abcde"}, 2};

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(assume::not_empty).view() == "ab");
            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(assume::not_empty).view() == "cd");
            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(rng.front(assume::not_empty).view() == "e");
            rng.drop_front(assume::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());

            return true;
        }

        template <typename Rng>
        constexpr usize chunks(Rng rng, usize count)
        {
            return algo::count(range::view::chunk{std::move(rng), count});
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::example());

        // Ranges without pop_front_n(usize).
        {
            static_assert(app::chunks(range::integral<u32>{} | range::v::take{0}, 1) == 0);
            static_assert(app::chunks(range::integral<u32>{} | range::v::take{1}, 1) == 1);
            static_assert(app::chunks(range::integral<u32>{} | range::v::take{2}, 1) == 2);
            static_assert(app::chunks(range::integral<u32>{} | range::v::take{3}, 1) == 3);

            static_assert(app::chunks(range::integral<u32>{} | range::v::take{0}, 2) == 0);
            static_assert(app::chunks(range::integral<u32>{} | range::v::take{1}, 2) == 1);
            static_assert(app::chunks(range::integral<u32>{} | range::v::take{2}, 2) == 1);
            static_assert(app::chunks(range::integral<u32>{} | range::v::take{3}, 2) == 2);

            auto rng = range::step<i32>{10, 99} | range::v::take{10} | range::v::chunk{3};

            usize count = 0;
            for (auto sub_rng : rng)
            {
                if (count == 0)
                {
                    snn_require(algo::is_equal(sub_rng, init_list<int>{10, 11, 12}));
                }
                else if (count == 1)
                {
                    snn_require(algo::is_equal(sub_rng, init_list<int>{13, 14, 15}));
                }
                else if (count == 2)
                {
                    snn_require(algo::is_equal(sub_rng, init_list<int>{16, 17, 18}));
                }
                else
                {
                    snn_require(algo::is_equal(sub_rng, init_list<int>{19}));
                }
                ++count;
            }
            snn_require(count == 4);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(algo::is_equal(rng.front(assume::not_empty), init_list<int>{10, 11, 12}));
            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(algo::is_equal(rng.front(assume::not_empty), init_list<int>{13, 14, 15}));
            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(algo::is_equal(rng.front(assume::not_empty), init_list<int>{16, 17, 18}));
            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(algo::is_equal(rng.front(assume::not_empty), init_list<int>{19}));
            rng.drop_front(assume::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());
        }

        // Ranges with pop_front_n(usize).
        {
            static_assert(app::chunks(cstrview{}.range(), 1) == 0);
            static_assert(app::chunks(cstrview{"a"}.range(), 1) == 1);
            static_assert(app::chunks(cstrview{"ab"}.range(), 1) == 2);
            static_assert(app::chunks(cstrview{"abc"}.range(), 1) == 3);

            static_assert(app::chunks(cstrview{}.range(), 2) == 0);
            static_assert(app::chunks(cstrview{"a"}.range(), 2) == 1);
            static_assert(app::chunks(cstrview{"ab"}.range(), 2) == 1);
            static_assert(app::chunks(cstrview{"abc"}.range(), 2) == 2);

            range::view::chunk rng{cstrview{"abcde"}.range(), 2};

            usize count = 0;
            for (auto sub_rng : rng)
            {
                if (count == 0)
                {
                    snn_require(cstrview{sub_rng} == "ab");
                }
                else if (count == 1)
                {
                    snn_require(cstrview{sub_rng} == "cd");
                }
                else
                {
                    snn_require(cstrview{sub_rng} == "e");
                }
                ++count;
            }
            snn_require(count == 3);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(cstrview{rng.front(assume::not_empty)} == "ab");
            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(cstrview{rng.front(assume::not_empty)} == "cd");
            rng.drop_front(assume::not_empty);

            snn_require(rng);
            snn_require(!rng.is_empty());
            snn_require(cstrview{rng.front(assume::not_empty)} == "e");
            rng.drop_front(assume::not_empty);

            snn_require(!rng);
            snn_require(rng.is_empty());
        }
    }
}
