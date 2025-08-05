// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fmt/fn/format.hh"

#include "snn-core/unittest.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/range/generate.hh"
#include "snn-core/range/view/take.hh"
#include "snn-core/range/view/transform.hh"

namespace snn::app
{
    namespace
    {
        constexpr bool test_format()
        {
            {
                fmt::fn::format f{"{}, {}, {}\n"};
                snn_require(f('a', 123, "abc") == "a, 123, abc\n");
                snn_require(f(1, 2, 3) == "1, 2, 3\n");
                static_assert(std::is_same_v<decltype(f(1, 2, 3)), str>);
            }
            {
                fmt::fn::format<strbuf> f{"{}: {}"};
                snn_require(f('a', 'b') == "a: b");
                snn_require(f(7, 8) == "7: 8");
                static_assert(std::is_same_v<decltype(f(7, 8)), strbuf>);
            }
            {
                snn_require(algo::is_equal(range::generate{fn::incrementing{1'000}} |
                                               range::v::take{3} |
                                               range::v::transform{fmt::fn::format{"{:d }"}},
                                           init_list<cstrview>{"1 000", "1 001", "1 002"}));
            }

            return true;
        }

        constexpr bool test_format_append()
        {
            {
                str out;
                fmt::fn::format_append f{"{}, {}, {}\n", out};
                f('a', 123, "abc");
                f(1, 2, 3);
                snn_require(out == "a, 123, abc\n1, 2, 3\n");
            }
            {
                strbuf out;
                constexpr cstrview format_string{"{}: {}, "};
                fmt::fn::format_append f{format_string, out, assume::no_overlap};
                f('a', 'b');
                f(7, 8);
                snn_require(out == "a: b, 7: 8, ");
            }

            return true;
        }
    }
}

namespace snn
{
    void unittest()
    {
        snn_static_require(app::test_format());
        snn_static_require(app::test_format_append());
    }
}
