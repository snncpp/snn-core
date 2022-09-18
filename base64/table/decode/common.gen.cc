// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/main.hh"
#include "snn-core/strcore.hh"
#include "snn-core/base64/table/encode/common.hh"
#include "snn-core/file/standard/out.hh"
#include "snn-core/range/integral.hh"

namespace snn::app
{
    namespace
    {
        void generate_table(const cstrview name, const cstrview chars)
        {
            strbuf output;

            output << "inline constexpr array<u8, 256> " << name << "{";

            usize count = 0;
            for (const auto b : range::integral<byte>{})
            {
                const usize pos = chars.find(to_char(b)).value_or(255);
                output << as_num(pos) << ", ";
                ++count;
            }
            snn_assert(count == 256);

            output << "};\n\n";

            file::standard::out{} << output;
        }
    }
}

namespace snn
{
    int main(array_view<const env::argument>)
    {
        app::generate_table("standard", base64::table::encode::standard.view());
        app::generate_table("url", base64::table::encode::url.view());

        return constant::exit::success;
    }
}
