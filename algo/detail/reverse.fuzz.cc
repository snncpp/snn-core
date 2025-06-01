// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/algo/reverse.hh"

#include "snn-core/fuzz.hh"
#include "snn-core/strcore.hh"
#include "snn-core/range/view/drop_last.hh"
#include <algorithm>

namespace snn::app
{
    template <typename Str>
    void reverse_as(cstrview input)
    {
        Str s1{input};
        Str s2{input};

        // Random access range & legacy iterable.
        algo::reverse(s1.range());
        std::reverse(s2.begin(), s2.end());
        assert(s1 == s2);

        // Non-random access range & non-legacy iterable.
        algo::reverse(s1.range() | range::v::drop_last{0});
        std::reverse(s2.begin(), s2.end());
        assert(s1 == s2);
    }
}

namespace snn
{
    int fuzz(cstrview input)
    {
        app::reverse_as<str>(input);
        app::reverse_as<strbuf>(input);

        return constant::exit::success;
    }
}
