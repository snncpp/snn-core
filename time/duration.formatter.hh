// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Duration formatter

#pragma once

#include "snn-core/formatter.hh"
#include "snn-core/strcore.hh"
#include "snn-core/ascii/trim.hh"
#include "snn-core/fmt/context.hh"
#include "snn-core/time/core.hh"
#include "snn-core/time/duration.hh"

namespace snn
{
    // ## Specializations

    // ### formatter<time::duration>

    template <>
    struct formatter<time::duration>
    {
        using type = time::duration;

        template <typename Buf>
        constexpr void format(const time::duration d, cstrview, const fmt::context&,
                              strcore<Buf>& append_to, assume::no_overlap_t)
        {
            const i64 sec = d.seconds();
            u32 nano      = d.nanoseconds();

            auto abs = to_u64(sec);

            if (sec < 0)
            {
                abs = math::negate_with_overflow(abs);
                if (nano > 0)
                {
                    --abs;
                    nano = 1'000'000'000 - nano;
                }
                append_to.append('-');
            }

            if (abs == 0)
            {
                if (nano == 0)
                {
                    append_to.append("0s");
                }
                else if (nano < 1'000)
                {
                    append_to.append_integral(nano);
                    append_to.append("ns");
                }
                else if (nano < 1'000'000)
                {
                    const auto us        = nano / 1'000;
                    const auto remainder = nano % 1'000;
                    append_to.append_integral(us);
                    if (remainder > 0)
                    {
                        append_to.append('.');
                        constexpr usize min_digits = 3;
                        append_to.append_integral(remainder, min_digits);
                        ascii::trim_right_inplace(append_to, '0');
                    }
                    append_to.append("µs"); // UTF-8 bytes: 0xC2 0xB5 0x73
                }
                else // nano < 1'000'000'000
                {
                    const auto ms        = nano / 1'000'000;
                    const auto remainder = nano % 1'000'000;
                    append_to.append_integral(ms);
                    if (remainder > 0)
                    {
                        append_to.append('.');
                        constexpr usize min_digits = 6;
                        append_to.append_integral(remainder, min_digits);
                        ascii::trim_right_inplace(append_to, '0');
                    }
                    append_to.append("ms");
                }
            }
            else
            {
                if (abs >= time::seconds_per_hour<u64>)
                {
                    const u64 hours = abs / time::seconds_per_hour<u64>;
                    abs             = abs % time::seconds_per_hour<u64>;
                    append_to.append_integral(hours);
                    append_to.append('h');
                }

                if (abs >= time::seconds_per_minute<u64>)
                {
                    const u64 minutes = abs / time::seconds_per_minute<u64>;
                    abs               = abs % time::seconds_per_minute<u64>;
                    append_to.append_integral(minutes);
                    append_to.append('m');
                }

                if (abs > 0 || nano > 0)
                {
                    append_to.append_integral(abs);
                    if (nano > 0)
                    {
                        append_to.append('.');
                        constexpr usize min_digits = 9;
                        append_to.append_integral(nano, min_digits);
                        ascii::trim_right_inplace(append_to, '0');
                    }
                    append_to.append('s');
                }
            }
        }
    };
}
