// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fmt/integral.hh"

#include "snn-core/fuzz.hh"
#include "snn-core/chr/common.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        auto rng = input.range();

        const auto num = rng.pop_front_integral<i64>().value;

        rng.pop_front().discard();

        const cstrview group_separator{rng.pop_front_while(chr::is_ascii_printable)};

        rng.pop_front().discard();

        const usize digits_per_group = to_byte(rng.pop_front().value_or(3));

        const usize min_digits = to_byte(rng.pop_front().value_or(0));

        try
        {
            str s;
            s = fmt::integral<str, math::base::decimal>(num, group_separator, digits_per_group,
                                                        min_digits);
            s = fmt::integral<str, math::base::hex>(num, group_separator, digits_per_group,
                                                    min_digits);
            s = fmt::integral<str, math::base::binary>(num, group_separator, digits_per_group,
                                                       min_digits);
            s = fmt::integral<str, math::base::octal>(num, group_separator, digits_per_group,
                                                      min_digits);
        }
        catch (const exception&)
        {
            // Ignore
        }

        return constant::exit::success;
    }
}
