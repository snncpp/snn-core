// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/point.hh"

#include "snn-core/fuzz.hh"
#include "snn-core/time/zone/db/america/los_angeles.hh"
#include "snn-core/time/zone/db/europe/stockholm.hh"

namespace snn::app
{
    namespace
    {
        template <typename Str>
        void fuzz_format(time::point p, const cstrview format_string)
        {
            Str append_to;

            auto la    = time::zone::db::america::los_angeles.location(); // Negative offset.
            auto sthlm = time::zone::db::europe::stockholm.location();    // Positive offset.

            try
            {
                p.format(format_string, la, append_to, promise::no_overlap);
                p.format(format_string, sthlm, append_to, promise::no_overlap);
            }
            catch (exception& e)
            {
                const auto ec = e.error_code();
                snn_assert(ec == time::error::invalid_format_string ||
                           ec == time::error::unescaped_alpha_character);
            }
        }
    }
}

namespace snn
{
    int fuzz(cstrview input)
    {
        auto rng = input.range();

        const auto unix = rng.pop_front_n(13).pop_front_integral<i64>().value;
        const auto nano = rng.pop_front_n(11).pop_front_integral<i64>().value;

        const cstrview format_string = rng.view();

        // Allow year -9122 to year 9892.
        // A long format string just slows down the fuzzer, less than 200 should be reasonable.
        if (unix >= -350000000000 && unix <= 250000000000 && format_string.size() < 200)
        {
            time::point p{time::duration{unix, nano}};
            app::fuzz_format<str>(p, format_string);
            app::fuzz_format<strbuf>(p, format_string);
        }

        return constant::exit::success;
    }
}
