// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/random/number.hh"

#include "snn-core/unittest.hh"

namespace snn::app
{
    namespace
    {
        bool example()
        {
            const auto a = random::number<u64>();
            const auto b = random::number<u64>();
            snn_require(a != b);

            const auto i = random::number<int>(3, 10);
            snn_require(i >= 3 && i < 10);

            return true;
        }

        template <typename T>
        bool is_equal_or_one_off(const T mid)
        {
            const T min = mid - 1;
            const T max = mid + 1;

            bool min_found = false;
            bool mid_found = false;
            bool max_found = false;

            for (loop::count lc{1000}; lc--;)
            {
                const auto n = random::number<T>(min, max + 1);
                if (n == min)
                {
                    min_found = true;
                }
                else if (n == mid)
                {
                    mid_found = true;
                }
                else if (n == max)
                {
                    max_found = true;
                }
                else
                {
                    return false;
                }
            }

            return min_found && mid_found && max_found;
        }

        template <typename T>
        bool is_in_range(const T min_inclusive, const T max_exclusive)
        {
            for (loop::count lc{100}; lc--;)
            {
                const auto n = random::number<T>(min_inclusive, max_exclusive);
                if (n < min_inclusive || n >= max_exclusive)
                {
                    return false;
                }
            }
            return true;
        }

        template <typename Int>
        bool is_in_range_random()
        {
            for (usize lc = 0; lc < 100;)
            {
                const auto min_inclusive = random::number<Int>();
                const auto max_exclusive = random::number<Int>();
                if (min_inclusive < max_exclusive)
                {
                    const auto n = random::number<Int>(min_inclusive, max_exclusive);
                    if (n < min_inclusive || n >= max_exclusive)
                    {
                        return false;
                    }
                    ++lc;
                }
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

        {
            const auto i1 = random::number<u64>();
            const auto i2 = random::number<u64>();
            snn_require(i1 != i2);
        }
        {
            u8 i = 0;
            i    = i | random::number<u8>();
            i    = i | random::number<u8>();
            i    = i | random::number<u8>();
            snn_require(i > 0);
        }

        {
            const auto i = random::number<int>(-2, 3);
            snn_require(i >= -2 && i < 3);
        }

        snn_require(random::number<int>(0, 1) == 0);

        snn_require(random::number<int>(0, 0) == 0); // Invalid range, returns first argument.
        snn_require(random::number<int>(1, 0) == 1); // Invalid range, returns first argument.
        snn_require(random::number<int>(2, 0) == 2); // Invalid range, returns first argument.

        snn_require(app::is_equal_or_one_off<usize>(234728197));
        snn_require(app::is_equal_or_one_off<i8>(0));
        snn_require(app::is_equal_or_one_off<i8>(-120));

        snn_require(app::is_in_range<u8>(0, 1));
        snn_require(app::is_in_range<u8>(2, 7));
        snn_require(app::is_in_range<i16>(-30, 30));
        snn_require(app::is_in_range<i16>(-32768, 32767));

        snn_require(app::is_in_range_random<u8>());
        snn_require(app::is_in_range_random<u16>());
        snn_require(app::is_in_range_random<u32>());
        snn_require(app::is_in_range_random<u64>());

        snn_require(app::is_in_range_random<i8>());
        snn_require(app::is_in_range_random<i16>());
        snn_require(app::is_in_range_random<i32>());
        snn_require(app::is_in_range_random<i64>());
    }
}
