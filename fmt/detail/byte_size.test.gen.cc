// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/main.hh"
#include "snn-core/file/standard/out.hh"
#include <libutil.h> // humanize_number [#lib:util]

namespace snn
{
    int main(array_view<const env::argument>)
    {
        array sizes{
            i64{},

            1,
            10,
            100,
            1000,
            10000,
            100000,
            1000000,
            10000000,
            100000000,
            1000000000,
            10000000000,
            100000000000,
            1000000000000,
            10000000000000,
            100000000000000,
            1000000000000000,
            10000000000000000,
            1000000000000000000,

            2,
            22,
            222,
            2222,
            22222,
            222222,
            2222222,
            22222222,
            222222222,
            2222222222,
            22222222222,
            222222222222,
            2222222222222,
            22222222222222,
            222222222222222,
            2222222222222222,
            22222222222222222,
            2222222222222222222,

            5,
            55,
            555,
            5555,
            55555,
            555555,
            5555555,
            55555555,
            555555555,
            5555555555,
            55555555555,
            555555555555,
            5555555555555,
            55555555555555,
            555555555555555,
            5555555555555555,
            55555555555555555,
            5555555555555555555,

            9,
            99,
            999,
            9999,
            99999,
            999999,
            9999999,
            99999999,
            999999999,
            9999999999,
            99999999999,
            999999999999,
            9999999999999,
            99999999999999,
            999999999999999,
            9999999999999999,
            99999999999999999,
            9223372036854775807, // Max

            1024,                // KiB
            1048576,             // MiB
            1073741824,          // GiB
            1099511627776,       // TiB
            1125899906842624,    // PiB
            1152921504606846976, // EiB

            1001,                 // Four digit remainder (powers of 1024).
            1023,                 // Four digit remainder (powers of 1024).
            1023 * 1024,          // Four digit size, zero remainder (powers of 1024).
            (1023 * 1024) + 1023, // Four digit size, four digit remainder (powers of 1024).
        };

        strbuf hh{init::reserve, 25 * constant::size::kibibyte<usize>};

        hh << "// 1000 B,kB,MB,GB...\n";
        hh << "// 1000 B,k,M,G...\n";

        hh << '\n';

        hh << "using fmt::table::byte_size::si;\n";
        hh << "using fmt::table::byte_size::si_short;\n";

        hh << '\n';

        i64 last = 0;
        for (const auto i : sizes)
        {
            if (i < last)
            {
                hh << '\n';
            }

            array<char, 6> buf;
            int ret = ::humanize_number(buf.begin(), buf.size(), i, "", HN_AUTOSCALE,
                                        HN_DECIMAL | HN_DIVISOR_1000 | HN_B);
            if (ret > 0)
            {
                const auto formatted_size = to_usize(ret);

                const cstrview value = buf.view(0, formatted_size - 1);
                const cstrview unit  = buf.view(formatted_size - 1, 1);

                // si

                hh << "snn_require(fmt::byte_size<1000>(byte_size{" << as_num(i) << "}, si) == \""
                   << value << unit;

                if (!hh.has_back('B'))
                {
                    hh << 'B';
                }

                hh << "\");\n";

                // si_short

                hh << "snn_require(fmt::byte_size<1000>(byte_size{" << as_num(i)
                   << "}, si_short) == \"" << value << unit << "\");\n";
            }
            else
            {
                fmt::print_error_line("Error: humanize_number() failed.");
                return constant::exit::failure;
            }

            last = i;
        }

        hh << '\n';

        hh << "// 1024 B,K,M,G...\n";
        hh << "// 1024 B,KiB,MiB,GiB...\n";

        hh << '\n';

        hh << "using fmt::table::byte_size::iec;\n";
        hh << "using fmt::table::byte_size::iec_short;\n";

        hh << '\n';
        hh << '\n';

        last = 0;
        for (const auto i : sizes)
        {
            if (i < last)
            {
                hh << '\n';
            }

            array<char, 6> buf;
            int ret =
                ::humanize_number(buf.begin(), buf.size(), i, "", HN_AUTOSCALE, HN_DECIMAL | HN_B);
            if (ret > 0)
            {
                const auto formatted_size = to_usize(ret);

                const cstrview value = buf.view(0, formatted_size - 1);
                const cstrview unit  = buf.view(formatted_size - 1, 1);

                // iec

                hh << "snn_require(fmt::byte_size<1024>(byte_size{" << as_num(i) << "}, iec) == \""
                   << value << unit;

                if (!hh.has_back('B'))
                {
                    hh << "iB";
                }

                hh << "\");\n";

                // iec_short

                hh << "snn_require(fmt::byte_size<1024>(byte_size{" << as_num(i)
                   << "}, iec_short) == \"" << value << unit << "\");\n";
            }
            else
            {
                fmt::print_error_line("Error: humanize_number() failed.");
                return constant::exit::failure;
            }

            last = i;
        }

        file::standard::out{} << hh;

        return constant::exit::success;
    }
}
