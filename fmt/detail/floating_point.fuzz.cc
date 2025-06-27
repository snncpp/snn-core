// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fmt/floating_point.hh"

#include "snn-core/fuzz.hh"
#include "snn-core/chr/common.hh"

namespace snn::app
{
    namespace
    {
        template <typename Str>
        void fuzz_format(const double d, const cstrview format_string)
        {
            formatter<double> f;
            fmt::context ctx;
            Str append_to;
            try
            {
                f.format(d, format_string, ctx, append_to, assume::no_overlap);
                snn_assert(append_to && append_to.size() <= 500); // Arbitrary
                snn_assert(append_to.all(chr::is_ascii_printable));
            }
            catch (const exception& e)
            {
                snn_assert(e.error_code() == fmt::error::invalid_printf_format_string);
            }
        }
    }
}

namespace snn
{
    int fuzz(cstrview input)
    {
        auto rng = input.range();

        double d{};
        if (rng.drop_front_read(d))
        {
            const cstrview format_string = rng.view();
            app::fuzz_format<str>(d, format_string);
            app::fuzz_format<strbuf>(d, format_string);
        }

        return constant::exit::success;
    }
}
