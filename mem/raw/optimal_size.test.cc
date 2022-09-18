// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/mem/raw/optimal_size.hh"

#include "snn-core/unittest.hh"
#include "snn-core/range/step.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool is_optimal_size(range::step<int> rng, const usize optimal)
        {
            for (const auto i : rng)
            {
                if (mem::raw::optimal_size(not_zero{to_usize(i)}).get() != optimal)
                {
                    return false;
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
        static_assert(mem::raw::optimal_size(not_zero<usize>{513}).get() == 640);
        static_assert(mem::raw::optimal_size(not_zero<usize>{999}).get() == 1024);

        static_assert(mem::raw::optimal_size(not_zero<usize>{640}).get() == 640);
        static_assert(mem::raw::optimal_size(not_zero<usize>{1024}).get() == 1024);
        static_assert(mem::raw::optimal_size(not_zero<usize>{4096}).get() == 4096);
        static_assert(mem::raw::optimal_size(not_zero<usize>{8192}).get() == 8192);
        static_assert(mem::raw::optimal_size(not_zero<usize>{16384}).get() == 16384);

        static_assert(mem::raw::optimal_size(not_zero<usize>{123'456}).get() ==
                      128 * constant::size::kibibyte<usize>); // 131 072 B
        static_assert(mem::raw::optimal_size(not_zero<usize>{49'000'000}).get() ==
                      48 * constant::size::mebibyte<usize>); // 50 331 648 B

        static_assert(mem::raw::optimal_size(not_zero{constant::limit<usize>::max}).get() ==
                      constant::limit<usize>::max);

        // 1 byte to 2 KiB.
        static_assert(app::is_optimal_size(range::step{1, 17}, 16));
        static_assert(app::is_optimal_size(range::step{17, 33}, 32));
        static_assert(app::is_optimal_size(range::step{33, 49}, 48));
        static_assert(app::is_optimal_size(range::step{49, 65}, 64));
        static_assert(app::is_optimal_size(range::step{65, 81}, 80));
        static_assert(app::is_optimal_size(range::step{81, 97}, 96));
        static_assert(app::is_optimal_size(range::step{97, 113}, 112));
        static_assert(app::is_optimal_size(range::step{113, 129}, 128));
        static_assert(app::is_optimal_size(range::step{129, 161}, 160));
        static_assert(app::is_optimal_size(range::step{161, 193}, 192));
        static_assert(app::is_optimal_size(range::step{193, 225}, 224));
        static_assert(app::is_optimal_size(range::step{225, 257}, 256));
        static_assert(app::is_optimal_size(range::step{257, 321}, 320));
        static_assert(app::is_optimal_size(range::step{321, 385}, 384));
        static_assert(app::is_optimal_size(range::step{385, 449}, 448));
        static_assert(app::is_optimal_size(range::step{449, 513}, 512));
        static_assert(app::is_optimal_size(range::step{513, 641}, 640));
        static_assert(app::is_optimal_size(range::step{641, 769}, 768));
        static_assert(app::is_optimal_size(range::step{769, 897}, 896));
        static_assert(app::is_optimal_size(range::step{897, 1025}, 1024));
        static_assert(app::is_optimal_size(range::step{1025, 1281}, 1280));
        static_assert(app::is_optimal_size(range::step{1281, 1537}, 1536));
        static_assert(app::is_optimal_size(range::step{1537, 1793}, 1792));
        static_assert(app::is_optimal_size(range::step{1793, 2049}, 2048));

        static_assert(!app::is_optimal_size(range::step{1, 17}, 8));
        static_assert(!app::is_optimal_size(range::step{17, 33}, 48));
    }
}
